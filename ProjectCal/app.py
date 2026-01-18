import numpy as np
import pandas as pd
import streamlit as st
import plotly.graph_objects as go
import plotly.express as px
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier, GradientBoostingClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import confusion_matrix, accuracy_score, log_loss
import torch
import torch.nn as nn
import torch.optim as optim
import warnings

# ==============================================================================
# 1. SYSTEM CONFIGURATION
# ==============================================================================

# Suppress warnings for clean output
warnings.filterwarnings("ignore")

st.set_page_config(
    layout = "wide",
    page_title = "AI Credit Risk System",
    page_icon = "🏦",
    initial_sidebar_state = "expanded",
)

# Constants
# Exchange rate to normalize VND inputs to USD scale for numerical stability
VND_USD_RATE = 26340.0
SEED_VALUE = 11

# Feature definitions (Must match training data order)
FEATURES = [
    "dependents",
    "education",
    "self_employed",
    "income",
    "loan_amt",
    "loan_term",
    "cibil",
    "res_asset",
    "com_asset",
    "lux_asset",
    "bank_asset",
]

# Check theme for UI styling
if "theme" not in st.session_state:
    st.session_state.theme = "dark"
is_dark_mode = st.session_state.theme == "dark"

# Custom CSS
st.markdown(
    f"""
<style>
    .stApp {{ background-color: {"#0e1117" if is_dark_mode else "#f8fafc"}; color: {"#e6edf3" if is_dark_mode else "#1f2937"}; }}
    .kpi-card {{
        background: {"#161b22" if is_dark_mode else "#ffffff"}; 
        border: 1px solid {"#30363d" if is_dark_mode else "#e5e7eb"};
        border-radius: 8px; 
        padding: 20px; 
        box-shadow: 0 4px 6px rgba(0,0,0,0.1);
        margin-bottom: 10px;
    }}
    .kpi-val {{ font-size: 1.8rem; font-weight: 700; color: {"#f0f6fc" if is_dark_mode else "#111827"}; }}
    .kpi-lbl {{ font-size: 0.8rem; color: #8b949e; text-transform: uppercase; font-weight: 600; }}
</style>
""",
    unsafe_allow_html = True,
)


# ==============================================================================
# 2. FINANCIAL MATH & LOGIC
# ==============================================================================

# Format large numbers with commas
def format_currency(val):
    return f"{val:,.0f}"


# Apply Sigmoid function to scale asset value based on credit score
# Logic: Higher scores get a higher asset multiplier (up to 2.0x)

def calculate_asset_multiplier(score):
    # Applies Sigmoid function to scale asset value based on credit score.
    # Logic: 300->0.5x (Risky), 660->1.25x (Average), 900->2.0x (Prime).
    # With x is the value of the asset - 660 is the average number of industry-standard
    return 0.5 + 1.5 / (1 + np.exp(-(score - 660) / 50))


# Calculate monthly payment (EMI) using standard formula
def calculate_emi(principal, rate, years):
    try:
        monthly_rate = rate / 100 / 12
        months = years * 12
        # No interest
        if monthly_rate <= 1e-9:
            return principal / months
        # The formular of money paid monthly
        numerator = principal * monthly_rate * ((1 + monthly_rate) ** months)
        denominator = ((1 + monthly_rate) ** months) - 1
        return numerator / denominator
    except:
        return 0.0


# Rule-based logic for baseline comparison
# Determines maximum loan based on Income (Unsecured) or Assets (Secured)
def check_rules(score, inc_usd, ast_usd):
    # Policy 1: Unsecured Limit (Max 15x Monthly Income)
    limit_unsecured = inc_usd / 12.0 * 15.0

    # Policy 2: Secured Limit (Assets * Risk Multiplier)
    risk_multiplier = calculate_asset_multiplier(score)
    limit_secured = ast_usd * risk_multiplier

    # Return the maximum eligibility
    return max(limit_unsecured, limit_secured), risk_multiplier


# ==============================================================================
# 3. MANUAL ALGORITHM IMPLEMENTATION
# ==============================================================================

# Custom Logistic Regression with L2 Regularization and Label Smoothing
# Demonstrates advanced optimization techniques from scratch
class ManualLogisticRegression:
    def __init__(self, lr = 0.01, ep = 1000, lambda_reg = 0.1):
        self.lr = lr
        self.ep = ep
        self.lambda_reg = lambda_reg
        self.w = None
        self.b = None
        self.losses = []

    def _sigmoid(self, z):
        return 1 / (1 + np.exp(-z))

    def fit(self, X, y):
        # n is the number of feature,
        #   each feature have one weight to that n feature->n weight
        #   m is the number of data sample
        m, n = X.shape
        self.w = np.zeros(n)
        self.b = 0
        # Convert array y to 1D array
        y = y.ravel()

        # Apply Label Smoothing: [0, 1] -> [0.025, 0.975]
        # Prevents the model from becoming overconfident
        # If label range [0,1] -> the weight could be too large-> too sensitive with features
        y_smooth = y * 0.95 + 0.025

        for _ in range(self.ep):
            # Forward pass
            z = np.dot(X, self.w) + self.b
            y_pred = self._sigmoid(z)

            # Compute Cost with L2 Penalty (Ridge Regression)
            # L2 is sum of lambda/(2*the number of sample)* sum(w1^2,w2^2,...)
            # L2 penalty increase the loss so that when derivative the slope will decrease quicker
            # Slope is less steep mean that the model is less sensitive with new sample x  
            # np.mean calculate average
            eps = 1e-15
            # Avoid error when ln(0)
            loss = -np.mean(
                y_smooth * np.log(y_pred + eps)
                + (1 - y_smooth) * np.log(1 - y_pred + eps)
            ) + (self.lambda_reg / (2 * m)) * np.sum(self.w**2)
            self.losses.append(loss)

            # Backpropagation
            # Derivative
            dw = (1 / m) * np.dot(X.T, (y_pred - y_smooth)) + (
                self.lambda_reg / m
            ) * self.w
            db = (1 / m) * np.sum(y_pred - y_smooth)

            # Update weights
            self.w -= self.lr * dw
            self.b -= self.lr * db

    def predict_proba(self, X):
        return self._sigmoid(np.dot(X, self.w) + self.b)

    def get_weights(self):
        return np.abs(self.w)


# ==============================================================================
# 4. DATA PIPELINE & TRAINING
# ==============================================================================


@st.cache_resource
def train_models():
    np.random.seed(SEED_VALUE)
    data_buffer = []

    # --- Part A: Synthetic Data Generation ---
    # Function to generate diverse borrower profiles
    def generate_data(n_samples, profile_type = "secured", noise = 0.05):
        df = pd.DataFrame()
        df["dependents"] = np.random.randint(0, 6, n_samples)
        # 1: Grad, 0: Undergrad
        df["education"] = np.random.randint(0, 2, n_samples) 
        df["self_employed"] = np.random.randint(0, 2, n_samples)
        df["cibil"] = np.random.randint(300, 900, n_samples)
        df["loan_term"] = np.random.randint(12, 180, n_samples) / 12

        if profile_type == "secured":
            # Secured Profile: Moderate Income, Tangible Assets
            df["income"] = np.random.uniform(5000, 50000, n_samples) * VND_USD_RATE
            df["res_asset"] = np.random.uniform(10000, 500000, n_samples) * VND_USD_RATE
            df["com_asset"] = np.random.uniform(0, 100000, n_samples) * VND_USD_RATE
            df["lux_asset"] = np.random.uniform(0, 50000, n_samples) * VND_USD_RATE
            df["bank_asset"] = np.random.uniform(0, 50000, n_samples) * VND_USD_RATE

            total_assets = (
                df["res_asset"] + df["com_asset"] + df["lux_asset"] + df["bank_asset"]
            )
            # Apply Sigmoid Logic to calculate the risk (if risk is high: the limit = 0.5 * total_assets, if risk is average, the limit = 1.25 * total_assets, 
            # if risk is low: the limit = 1.5 * total_assets)
            # Logic: Loan Approved if Amount <= Adjusted Asset Limit
            
            mult = 0.5 + 1.5 / (1 + np.exp(-(df["cibil"] - 660) / 50))
            limit = total_assets * mult
            df["loan_amt"] = limit * np.random.uniform(0.5, 1.5, n_samples)
            df["status"] = (df["loan_amt"] <= limit).astype(int)

        else:
            # Unsecured Profile: High Cashflow, No Collateral
            df["income"] = np.random.uniform(20000000, 150000000, n_samples) * 12
            df["res_asset"] = 0
            df["com_asset"] = 0
            df["lux_asset"] = 0
            df["bank_asset"] = 0

            monthly_inc = df["income"] / 12
            df["loan_amt"] = monthly_inc * np.random.uniform(2.0, 15.0, n_samples)

            # Logic: Loan Approved based on DTI and Credit Score
            # DTI=monthly debt/monthly income, account for 70% of decision
            # clip bound the value between 0 and 1 
            rate = 0.12 / 12
            term = df["loan_term"] * 12
            emi = (df["loan_amt"] * rate * (1 + rate) ** term) / (
                (1 + rate) ** term - 1
            )
            dti = emi / monthly_inc

            score = (1 - np.clip(dti, 0, 1)) * 0.7 + ((df["cibil"] - 300) / 600) * 0.3
            df["status"] = (score > 0.50).astype(int)

        # Inject noise to test robustness
        # Calculate the number of sample be flip (noise is the rate of noise we want to create)
        n_flip = int(n_samples * noise)
        # Chose which n_flip, replace=false to avoid coincide
        idx = np.random.choice(df.index, n_flip, replace = False)
        # flip the result approve -> disapprove
        df.loc[idx, "status"] = 1 - df.loc[idx, "status"]

        return df[FEATURES + ["status"]]

    # Generate Training Batches
    data_buffer.append(generate_data(5000, "secured", 0.08))
    data_buffer.append(generate_data(5000, "unsecured", 0.05))

    # --- Part B: Real Data Loading ---
    try:
        df_real = pd.read_csv("Loan.csv")
        df_real.columns = [c.strip() for c in df_real.columns]
        df_real.rename(
            columns = {
                "no_of_dependents": "dependents",
                "income_annum": "income",
                "loan_amount": "loan_amt",
                "cibil_score": "cibil",
                "loan_term": "loan_term",
                "residential_assets_value": "res_asset",
                "commercial_assets_value": "com_asset",
                "luxury_assets_value": "lux_asset",
                "bank_asset_value": "bank_asset",
                "loan_status": "status",
            },
            inplace = True,
        )
        # If the data have string "Grad", we assume that the customer is graduation
        df_real["education"] = df_real["education"].apply(
            lambda x: 1 if "Grad" in str(x) else 0
        )
        # If the data have string "Yes", we assume that the customer is self_employed
        df_real["self_employed"] = df_real["self_employed"].apply(
            lambda x: 1 if "Yes" in str(x) else 0
        )
        # If the data have string "App", we assume that the bank has approved 
        df_real["status"] = df_real["status"].apply(
            lambda x: 1 if "App" in str(x) else 0
        )
        data_buffer.append(df_real[FEATURES + ["status"]])
    except:
        pass

    # Data Pipeline
    # Concat = merge 2D array into 1D array, ignore=true: reset the index 0-N-1
    df_final = pd.concat(data_buffer, ignore_index = True).fillna(0)
    X = df_final[FEATURES].values
    y = df_final["status"].values.reshape(-1, 1)

    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)
    X_train, X_test, y_train, y_test = train_test_split(
        X_scaled, y, test_size = 0.2, random_state = SEED_VALUE
    )

    # --- Part C: Model Training ---

    # 1. Neural Network
    m_nn = nn.Sequential(
        nn.Linear(11, 64),
        nn.SiLU(),
        nn.Dropout(0.3),
        nn.Linear(64, 32),
        nn.SiLU(),
        nn.Linear(32, 1),
        nn.Sigmoid(),
    )
    opt = optim.AdamW(m_nn.parameters(), lr = 0.005, weight_decay = 1e-4)
    loss_hist_nn = []
    y_smooth = torch.tensor(y_train * 0.95 + 0.025, dtype = torch.float32)

    for _ in range(300):
        opt.zero_grad()
        preds = m_nn(torch.tensor(X_train, dtype = torch.float32))
        loss = nn.BCELoss()(preds, y_smooth)
        loss.backward()
        opt.step()
        loss_hist_nn.append(loss.item())

    # 2. Random Forest
    m_rf = RandomForestClassifier(
        n_estimators = 300,
        warm_start = True,
        max_depth = 10,
        min_samples_leaf = 10,
        random_state = SEED_VALUE,
    )
    loss_hist_rf = []
    for i in range(1, 301):
        m_rf.n_estimators = i
        m_rf.fit(X_train, y_train.ravel())
        loss_hist_rf.append(log_loss(y_train, m_rf.predict_proba(X_train)))

    # 3. Gradient Boosting
    m_gb = GradientBoostingClassifier(
        n_estimators = 300,
        learning_rate = 0.005,
        max_depth = 8,
        subsample = 0.8,
        min_samples_leaf = 20,
        random_state = SEED_VALUE,
    )
    m_gb.fit(X_train, y_train.ravel())
    loss_hist_gb = m_gb.train_score_.tolist()

    # 4. Manual Logistic Regression
    # lr: learning rate, ep: the number of loop, lambda_reg: Penalize large weights by adding
    m_lr = ManualLogisticRegression(lr = 0.005, ep = 300, lambda_reg = 0.5)
    m_lr.fit(X_train, y_train)
    loss_hist_lr = m_lr.losses

    # --- Part D: Metrics Aggregation ---
    def interpolate(arr):
        return np.interp(np.linspace(0, len(arr) - 1, 300), np.arange(len(arr)), arr)

    loss_total = (
        interpolate(loss_hist_nn)
        + interpolate(loss_hist_rf)
        + interpolate(loss_hist_gb)
        + interpolate(loss_hist_lr)
    ) / 4

    feat_imp = (
        m_rf.feature_importances_
        + m_gb.feature_importances_
        + (m_lr.get_weights() / np.sum(m_lr.get_weights()))
    ) / 3

    return (
        scaler,
        m_nn,
        m_rf,
        m_gb,
        m_lr,
        X_test,
        y_test,
        feat_imp,
        loss_hist_nn,
        loss_hist_rf,
        loss_hist_gb,
        loss_hist_lr,
        loss_total,
    )


(
    SCALER,
    M_NN,
    M_RF,
    M_GB,
    M_LR,
    X_TEST,
    Y_TEST,
    FEAT_IMP,
    H_NN,
    H_RF,
    H_GB,
    H_LR,
    H_TOT,
) = train_models()


# ==============================================================================
# 5. USER INTERFACE
# ==============================================================================

with st.sidebar:
    st.header("Applicant Information")

    with st.expander("Personal Details", expanded = True):
        u_dep = st.selectbox("Dependents", [0, 1, 2, 3, 4, 5], index = 2)
        u_self = st.selectbox("Employment", ["Salaried", "Self-Employed"], index = 0)
        u_edu = st.selectbox("Education", ["Graduate", "Not Graduate"], index = 0)

    val_self = 1 if u_self == "Self-Employed" else 0
    val_edu = 1 if u_edu == "Graduate" else 0

    with st.expander("Financial Data (VNĐ)", expanded=True):
        u_cibil = st.slider("Credit Score (CIBIL)", 300, 900, 750)
        u_inc = st.number_input(
            "Monthly Income", 0.0, step = 1e7, value = 5e7, format = "%.0f"
        )
        st.caption("Asset Breakdown:")
        u_res = st.number_input("Residential", 0.0, step = 1e9, format = "%.0f")
        u_com = st.number_input("Commercial", 0.0, step = 1e9, format = "%.0f")
        u_lux = st.number_input("Luxury", 0.0, step = 1e8, format = "%.0f")
        u_bank = st.number_input("Bank Deposits", 0.0, step = 1e8, format = "%.0f")
    total_assets = u_res + u_com + u_lux + u_bank

    with st.expander("Loan Request (VNĐ)", expanded = True):
        u_amt = st.number_input("Loan Amount", 0.0, step = 1e8, value = 2e8, format = "%.0f")
        u_term = st.number_input("Term (Months)", 1, 480, 36)
        u_rate = st.number_input("Interest (% / Month)", 0.0, 25.0, 10.0)

    btn_run = st.button("RUN PREDICTION", type = "primary", width = "stretch")


# ==============================================================================
# 6. LOGIC & INFERENCE
# ==============================================================================

if u_amt <= 0:
    u_amt = 1.0

# 1. Normalize Inputs
inc_usd = (u_inc * 12) / VND_USD_RATE
ast_usd = total_assets / VND_USD_RATE
amt_usd = u_amt / VND_USD_RATE
term_yr = u_term / 12

# 2. Rule Validation
limit_usd, risk_mult = check_rules(u_cibil, inc_usd, ast_usd)
limit_vnd = limit_usd * VND_USD_RATE
emi_val = calculate_emi(u_amt, u_rate, term_yr)
dti_val = emi_val / (u_inc + 1)

# 3. AI Inference
# Construct input vector
raw_vec = [
    [
        u_dep,
        val_edu,
        val_self,
        inc_usd,
        amt_usd,
        term_yr,
        u_cibil,
        u_res / VND_USD_RATE,
        u_com / VND_USD_RATE,
        u_lux / VND_USD_RATE,
        u_bank / VND_USD_RATE,
    ]
]
scaled_vec = SCALER.transform(raw_vec)

# Get Predictions
with torch.no_grad():
    p_nn = M_NN(torch.tensor(scaled_vec, dtype = torch.float32)).item()
p_rf = M_RF.predict_proba(scaled_vec)[0, 1]
p_gb = M_GB.predict_proba(scaled_vec)[0, 1]
p_lr = M_LR.predict_proba(scaled_vec)[0]

# Ensemble Score
final_score = (p_nn + p_rf + p_gb + p_lr) / 4

# Approval Logic
is_approved = True
warnings_lst = []

# Capacity check (Unsecured vs Secured)
max_cap = u_inc * 15 if total_assets == 0 else limit_vnd * 1.5
if u_amt > max_cap:
    is_approved = False
    final_score *= 0.4
    warnings_lst.append(f"Exceeds Risk Limit (Max: {format_currency(max_cap)})")

if dti_val > 0.65:
    is_approved = False
    final_score *= 0.5
    warnings_lst.append(f"High DTI Ratio ({dti_val * 100:.1f}%)")

if final_score < 0.5:
    is_approved = False
    warnings_lst.append(f"Low Trust Score ({final_score * 100:.1f}%)")

color = "#238636" if is_approved else "#da3633"
status_txt = "APPROVED" if is_approved else "REJECTED"


# ==============================================================================
# 7. DASHBOARD & VISUALIZATION
# ==============================================================================

if btn_run and is_approved:
    st.balloons()

st.title("Credit Underwriting Report")
st.markdown(
    f"<h2 style = 'text-align: center; color: {color}; border: 2px solid {color}; padding: 15px; border-radius: 10px;'>{status_txt} <span style = 'font-size: 20px; color: gray'>(Confidence: {final_score * 100:.1f}%)</span></h2>",
    unsafe_allow_html = True,
)

# KPI Section
k1, k2, k3, k4 = st.columns(4)


def show_kpi(col, lbl, val, sub, b_col):
    col.markdown(
        f"<div class = 'kpi-card' style = 'border-left-color: {b_col}'><div class = 'kpi-lbl'>{lbl}</div><div class = 'kpi-val'>{val}</div><small style = 'color: #6b7280; font-weight: bold;'>{sub}</small></div>",
        unsafe_allow_html = True,
    )


loan_type = "UNSECURED" if total_assets == 0 else "SECURED"
show_kpi(k1, "LOAN AMOUNT", format_currency(u_amt), "VNĐ", "#3b82f6")
show_kpi(k2, "RISK TYPE", loan_type, f"Multiplier: x{risk_mult:.2f}", "#8b5cf6")
show_kpi(
    k3, "EST. EMI / MONTH", format_currency(emi_val), f"DTI: {dti_val * 100:.1f}%", color
)
show_kpi(k4, "TRUST SCORE", f"{final_score * 100:.1f}%", "Ensemble Core", color)

st.write("")
tab1, tab2 = st.tabs(["📉 MODEL LEARNING CURVES", "⚙️ SYSTEM VALIDATION"])

with tab1:
    st.subheader("Model Convergence (Training Loss)")
    st.info("Tracking error reduction across all 4 core models.")

    r1c1, r1c2, r1c3 = st.columns(3)
    with r1c1:
        st.plotly_chart(
            px.line(
                y = H_NN,
                title = "1. Neural Network",
                labels = {"y": "BCE Loss"},
                color_discrete_sequence = ["#ff7b72"],
            ),
            width = "stretch",
        )
    with r1c2:
        st.plotly_chart(
            px.line(
                y = H_GB,
                title = "2. Gradient Boosting",
                labels = {"y": "Deviance"},
                color_discrete_sequence = ["#79c0ff"],
            ),
            width = "stretch",
        )
    with r1c3:
        st.plotly_chart(
            px.line(
                y = H_RF,
                title = "3. Random Forest",
                labels = {"y": "Log Loss"},
                color_discrete_sequence = ["#d2a8ff"],
            ),
            width = "stretch",
        )

    r2c1, r2c2 = st.columns(2)
    with r2c1:
        st.plotly_chart(
            px.line(
                y = H_LR,
                title = "4. Logistic Regression",
                labels = {"y": "Cost J"},
                color_discrete_sequence = ["#56d364"],
            ),
            width = "stretch",
        )
    with r2c2:
        st.plotly_chart(
            px.line(
                y = H_TOT,
                title = "5. TOTAL ENSEMBLE LOSS",
                color_discrete_sequence=["#ffffff"],
            ),
            width = "stretch",
        )

with tab2:
    # Prediction Breakdown
    c_vote, c_mat = st.columns([1, 2])
    with c_vote:
        st.subheader("Model Voting")
        st.metric("Neural Network", f"{p_nn * 100:.1f}%")
        st.metric("Random Forest", f"{p_rf * 100:.1f}%")
        st.metric("Gradient Boosting", f"{p_gb * 100:.1f}%")
        st.metric("Logistic Reg.", f"{p_lr * 100:.1f}%")

    with c_mat:
        # Evaluation on Test Set
        xt_t = torch.tensor(X_TEST, dtype = torch.float32)
        with torch.no_grad():
            yt_nn = M_NN(xt_t).numpy().flatten()
        yt_rf = M_RF.predict_proba(X_TEST)[:, 1]
        yt_gb = M_GB.predict_proba(X_TEST)[:, 1]
        yt_lr = M_LR.predict_proba(X_TEST)

        yt_final = (yt_nn + yt_rf + yt_gb + yt_lr) / 4
        pred_class = (yt_final > 0.5).astype(int)
        cm = confusion_matrix(Y_TEST, pred_class)
        acc = accuracy_score(Y_TEST, pred_class)

        c_cm, c_fi = st.columns(2)
        with c_cm:
            st.subheader("Confusion Matrix")
            st.caption(f"Test Accuracy: **{acc * 100:.2f}%**")
            st.plotly_chart(
                px.imshow(
                    cm,
                    text_auto = True,
                    color_continuous_scale = "Mint",
                    x = ["Reject", "Approve"],
                    y = ["Reject", "Approve"],
                ),
                width = "stretch",
            )
        with c_fi:
            st.subheader("Feature Importance")
            fi_df = pd.DataFrame(
                {"Feature": FEATURES, "Importance": FEAT_IMP}
            ).sort_values(by = "Importance", ascending = True)
            st.plotly_chart(
                px.bar(
                    fi_df,
                    x = "Importance",
                    y = "Feature",
                    orientation = "h",
                    color = "Importance",
                    color_continuous_scale = "Blues",
                ),
                width = "stretch",
            )

# Final Result Dialog
if btn_run:

    @st.dialog("Decision Details")
    def show_dialog():
        st.header(status_txt)
        if is_approved:
            st.success("Application Approved.")
            st.write(f"**Profile:** {u_edu}, {u_self}")
            st.write(f"**Loan Type:** {loan_type}")
        else:
            st.error("Application Declined.")
            for m in warnings_lst:
                st.write(f"❌ {m}")
        st.divider()
        st.caption(f"Authors: MPK - MeoBeoSama - MDuy | Score: {final_score * 100:.1f}%")

    show_dialog()
