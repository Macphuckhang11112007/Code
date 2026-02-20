"""
/**
 * MODULE: System UI - Portfolio Splitter
 * VAI TRÒ: Vẽ biểu đồ Cơ cấu phân rã Dòng Tiền. Đảm bảo triệt để hiển thị được tỷ lệ CỨNG (Sự tách lìa giữa Liquid NAV và Locked NAV).
 * TẠI SAO QUAN TRỌNG: User phải nhận thức được họ không thể Rút (Withdraw) cục tiền đang bị khóa trong Bank (Maturity Locked) mà hệ thống đang vận hành ngầm.
 */
"""
import streamlit as st

def render_portfolio():
    """Dựng hình khối Metric hiển thị."""
    
    if "wallet" not in st.session_state:
        st.warning("Khung ví tiền đang ở trạng thái ngủ (Ngắt kết nối với Model Local).")
        return
        
    wallet = st.session_state.wallet
    
    # Kích hoạt bảng điều khiển NẠP TIỀN
    with st.expander("💳 Fiat Deposit (Bơm Vốn Thanh Khoản)"):
        dep_amount = st.number_input("Deposit Amount (USD)", min_value=1.0, value=1000.0, step=100.0)
        if st.button("Execute Deposit"):
            wallet.deposit_cash(dep_amount)
            st.success(f"Successfully deposited {dep_amount:.2f} USD into Liquid Cash Reserve!")

    nav, unrealized, alloc = wallet.mark_to_market({})
    
    liquid = wallet.cash
    locked = 0.0
    
    for sym, total_v in alloc.items():
        if sym == 'CASH': continue
        if sym.startswith("VCB") or sym.startswith("US"): locked += total_v
        else: liquid += total_v
        
    roi = (nav - wallet.initial_capital) / wallet.initial_capital * 100

    col1, col2, col3 = st.columns(3)
    with col1:
        st.markdown(
            f'''<div class="metric-card">
            <h3>Liquid NAV (Khả Dụng)</h3>
            <h2>${liquid:,.2f}</h2>
            </div>''', unsafe_allow_html=True
        )
    with col2:
        st.markdown(
            f'''<div class="metric-card" style="border-left-color: #F0B90B;">
            <h3>Locked NAV (Cấm Rút)</h3>
            <h2>${locked:,.2f}</h2>
            </div>''', unsafe_allow_html=True
        )
    with col3:
        st.markdown(
            f'''<div class="metric-card" style="border-left-color: #0ECB81;">
            <h3>Return on Investment</h3>
            <h2>{roi:,.2f} %</h2>
            </div>''', unsafe_allow_html=True
        )
        
    st.markdown("---")
    st.markdown("### 🍩 Allocation Sunburst")
    
    # Portfolio Sunburst Data construction
    import pandas as pd
    import plotly.express as px
    
    portfolio_data = [{"Type": "Liquid", "Asset": "CASH", "Value_USD": wallet.cash}]
    for sym, val in alloc.items():
        if sym == 'CASH': continue
        ptype = "Locked" if sym.startswith("VCB") or sym.startswith("US") else "Liquid"
        portfolio_data.append({"Type": ptype, "Asset": sym, "Value_USD": max(val, 0.001)})
    
    df_portfolio = pd.DataFrame(portfolio_data)
    
    if df_portfolio['Value_USD'].sum() > 0:
        fig_sunburst = px.sunburst(
            df_portfolio,
            path=['Type', 'Asset'],
            values='Value_USD',
            color='Type',
            color_discrete_map={'Liquid': '#0ECB81', 'Locked': '#F0B90B'}
        )
        fig_sunburst.update_layout(
            paper_bgcolor="rgba(0,0,0,0)",
            plot_bgcolor="rgba(0,0,0,0)",
            margin=dict(t=0, l=0, r=0, b=0),
            font=dict(color="#D1D4DC")
        )
        st.plotly_chart(fig_sunburst, use_container_width=True, config={'displayModeBar': False})
        
    st.markdown("### 📊 Live Analytics & Trade Statistics")
    # Tự động tính các chỉ số Quant nếu có Transaction
    win_rate = (wallet.win_trades / wallet.total_trades * 100) if wallet.total_trades > 0 else 0.0
    c1, c2, c3, c4 = st.columns(4)
    c1.metric("Total Executed Trades", f"{wallet.total_trades}")
    c2.metric("Win Rate", f"{win_rate:.1f} %")
    c3.metric("Realized PnL", f"${wallet.cum_pnl_closed:,.2f}")
    c4.metric("Collected Yield", f"${wallet.cum_yield:,.2f}")
    
    st.markdown("---")
    st.markdown("### 📉 Tail Risk & Correlation Matrices")
    
    # Load Transactions to calculate MDD and Advanced metrics via Analyzer
    import os
    import pandas as pd
    from src.engine.analyzer import AnalyticsEngine
    
    file_path = "logs/trading/transactions.csv"
    if os.path.exists(file_path):
        try:
             # Fast patch: Build a nominal NAV history to feed into Analytics Engine
             df_tx = pd.read_csv(file_path)
             if not df_tx.empty and len(df_tx) > 5:
                  # Giả lập lịch sử NAV tạm thời từ lợi nhuận đã đóng để tính MDD
                  # Đây là một giải pháp xấp xỉ hiển thị nhanh
                  df_tx['pnl_cumsum'] = df_tx['pnl'].cumsum() if 'pnl' in df_tx.columns else 0
                  df_tx['nav_nominal'] = wallet.initial_capital + df_tx['pnl_cumsum']
                  df_tx['timestamp'] = pd.to_datetime(df_tx['ts'])
                  
                  analyzer = AnalyticsEngine(df_tx.to_dict('records'))
                  report = analyzer.generate_comprehensive_report()
                  
                  r1, r2, r3, r4 = st.columns(4)
                  r1.metric("Max Drawdown (MDD)", f"{report['risk_profile']['max_drawdown']*100:.2f} %")
                  r2.metric("Value at Risk (95%)", f"{report['risk_profile']['value_at_risk_95']*100:.2f} %")
                  r3.metric("Sharpe Ratio", f"{report['efficiency']['sharpe_ratio']:.2f}")
                  
                  alpha_color = "normal" if report['opportunity_cost']['is_winning'] else "inverse"
                  r4.metric("Alpha (vs Bank Rate)", f"${report['opportunity_cost']['alpha_abs']:,.2f}", delta="Beating Market!" if report['opportunity_cost']['is_winning'] else "Underperforming")
                  
             else:
                  st.info("Cần ít nhất 5 giao dịch (Transactions) để kích hoạt Bộ tính toán Khả năng rủi ro (Risk Engine).")
        except Exception as e:
             st.error(f"Failed to generate Analytics Matrix: {e}")
    else:
        st.info("Chưa có Dữ liệu Sổ cái Kế toán để kết tinh Báo cáo Ma Trận (Matrix Analytics). Hãy chạy thuật toán giả lập ngay!")

    # Nút bấm Lượng tử (Monte Carlo Stress Test) Nhúng Phẳng vào Cấu Trúc UI
    st.markdown("---")
    st.subheader("⚛️ Monte Carlo Stress Test (Lượng Tử Hóa Rủi Ro)")
    st.write("Simulate thousands of future random walk scenarios via Geometric Brownian Motion (GBM) to identify Portfolio Failure Probability and Tail Risk.")
    
    if st.button("🚀 Kích Hoạt Mô Phỏng Áp Lực Đa Vũ Trụ", use_container_width=True):
        with st.spinner("Đang xé rách không-thời gian để tính toán (Running Parallel Universes SDE)..."):
            try:
                from src.engine.market import Market
                from src.engine.monte_carlo import MonteCarloSimulator
                import numpy as np
                import pandas as pd
                
                # Nạp nhanh data để lấy Seed
                mkt = Market(asset_list=["BTC_USDT"], context_list=["US10Y_10y", "US_CPI"], data_path="data/")
                mkt.load()
                close_idx = mkt.feature_map.get('close')
                
                hist_prices = mkt.data[:, 0, close_idx]
                hist_prices = hist_prices[hist_prices > 0]
                returns = np.diff(hist_prices) / hist_prices[:-1]
                
                # Bắt đầu chạy
                mc = MonteCarloSimulator(n_paths=1000, horizon_steps=252*4)
                mu, sigma = mc.estimate_drift_and_vol(returns)
                report = mc.run_stress_test(agent=None, initial_capital=wallet.initial_capital, S0=hist_prices[-1], mu=mu, sigma=sigma)
                
                # Trình bày Báo Cáo
                c1, c2, c3 = st.columns(3)
                c1.metric("Kỳ Vọng R.O.I Tương Lai", f"{report['Expected_ROI']*100:.2f} %")
                c2.metric("Xác Suất Cháy Mầm (Probability of Ruin)", f"{report['Probability_of_Ruin']*100:.2f} %")
                c3.metric("Rủi Ro Đuôi (CVaR 95%)", f"{report['CVaR_95']*100:.2f} %")
                
                st.success("Mô phỏng 1000 vũ trụ hoàn tất rực rỡ! Đây là tham chiếu giúp AI Agent tự bảo vệ danh mục ở các kịch bản tồi tệ nhất.")
            except Exception as e:
                st.error(f"Thất bại khi xé rách đa vũ trụ: {e}")
