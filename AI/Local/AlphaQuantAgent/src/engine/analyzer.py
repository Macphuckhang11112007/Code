"""
/**
 * MODULE: Analytics Engine (The Brain)
 * VAI TRÒ: Máy phân tích dữ liệu lõi biến đổi chuỗi Sổ cái (Ledger NAV thô) thu hoạch được sau Backtest thành báo cáo Xác suất & Rủi ro (RAG Context).
 * TÍNH NĂNG CỐT LÕI (SATURATED):
 * 1. Empirical Quartiles (Q1, Q2, Q3): Phân tích phân phối tần suất lợi nhuận thực tế sinh lời.
 * 2. Tail Risk (VaR, MDD): Đo lường rủi ro đuôi (Thiên nga đen) và sức chịu đựng tâm lý rớt giá.
 * 3. Opportunity Cost (Alpha): Nhất quán so sánh hiệu ứng chi phí cơ hội so với chỉ gửi tiết kiệm (Risk-free).
 * 4. Efficiency (Sharpe): Đánh giá chính xác tỷ suất lợi nhuận trên đúng một lượng rủi ro duy nhất.
 */
"""

import pandas as pd
import numpy as np
import json
import os
from typing import Dict, List, Optional

class AnalyticsEngine:
    def __init__(self, nav_history: List[Dict]):
        """
        Khởi tạo Bộ não thống kê với dữ liệu ròng lịch sử nhập vào từ Wallet.
        INPUT DỰ KIẾN: [{'timestamp':..., 'nav_nominal':..., 'nav_real':..., 'cash':...}]
        CHIẾN LƯỢC: Map toàn bộ vào Dataframe Pandas giúp Vectorized Operations (O(1)).
        """
        if not nav_history:
            self.df = pd.DataFrame()
            return

        self.df = pd.DataFrame(nav_history)
        if 'timestamp' in self.df.columns:
            self.df['timestamp'] = pd.to_datetime(self.df['timestamp'])
            self.df.set_index('timestamp', inplace=True)
            self.df.sort_index(inplace=True)

        # Tính toán Lợi nhuận chuỗi thời gian Logarit (Log Returns)
        # TẠI SAO: Log returns có đặc tính "cộng gộp qua thời gian" liên tục, cực kì ích lợi để model Deep Learning không bị trượt toán học do lãi kép.
        # Log Return = ln(NAV_t / NAV_{t-1})
        self.df['log_returns'] = np.log(self.df['nav_nominal'] / self.df['nav_nominal'].shift(1))
        self.df['log_returns'].fillna(0, inplace=True)

        # Simple Returns (Pct Change) - Cho báo cáo tường minh (Lucid Reporting) để con người (User) dễ hiểu hơn.
        self.df['pct_change'] = self.df['nav_nominal'].pct_change().fillna(0)

    def generate_comprehensive_report(self, risk_free_rate_annual: float = 0.05, periods_per_year: int = 252 * 16) -> Dict:
        """
        Gói (Pack) Báo cáo sức khỏe tài chính toàn diện phục vụ công tác Diễn giải LLM (RAG Explanation).
        THÔNG SỐ MẶC ĐỊNH: 
        - risk_free_rate_annual: Lãi suất phi rủi ro danh nghĩa (Risk-Free rate 5%/Năm).
        - periods_per_year: Số nến (Ticks) trong 1 năm tài chính, công thức là B (Số ngày làm việc ~252) * K (Số nến/ngày ~16 với nến 15p).
        """
        if self.df.empty or len(self.df) < 2:
            return self._get_empty_report()

        # 1. Thống kê Phân phối Đám Đảo (Distribution Statistics)
        returns_series = self.df['pct_change']
        q1 = np.percentile(returns_series, 25)
        # Median an toàn và bảo mật hơn Mean vì loại trừ hiện tượng nhiễu đột biến (Outliers skew).
        q2 = np.median(returns_series)      
        q3 = np.percentile(returns_series, 75)

        # 2. Định Lượng Rủi ro Thiên Nga Đen (Tail Risk Tracking)
        # VaR (Value at Risk) 95%: Biểu diễn Tình huống cực xấu, mức lỗ có khả năng xảy ra là 5% trong lịch sử dữ liệu được backtest
        var_95 = np.percentile(returns_series, 5)
        max_drawdown = self._calculate_max_drawdown()

        # 3. Tính Điểm Hiệu Quả Thị Trường (Efficiency / Sharpe Ratio)
        avg_return_daily = returns_series.mean()
        std_dev_daily = returns_series.std()

        if std_dev_daily == 0:
            sharpe_ratio = 0.0
        else:
            # Đồng nhất quy đổi Phương sai (Volatility) và Lợi nhuận (Return) lên thước đo Niên Độ Hóa (Annualization)
            # TẠI SAO: Sharpe chỉ có thể so sánh giữa 2 danh mục độc lập nếu chuẩn trên cùng khung 1 năm.
            annualized_return = avg_return_daily * periods_per_year
            annualized_volatility = std_dev_daily * np.sqrt(periods_per_year)
            sharpe_ratio = (annualized_return - risk_free_rate_annual) / annualized_volatility

        # 4. Trừ Chi phí cơ hội Hệ Kế toán (Opportunity Cost - Alpha)
        initial_balance = self.df['nav_nominal'].iloc[0]
        final_balance = self.df['nav_nominal'].iloc[-1]

        # Tính vi sai thời gian ngày để áp công thức Lãi kép thực của bank
        days_passed = (self.df.index[-1] - self.df.index[0]).days
        years_passed = max(days_passed, 0) / 365.25

        # Công thức Vết Ngân Hàng Kép (Compound Bank Scenario): A = P * (1 + r)^t
        bank_balance_scenario = initial_balance * ((1 + risk_free_rate_annual) ** years_passed)

        opportunity_cost_abs = final_balance - bank_balance_scenario
        is_beating_market = opportunity_cost_abs > 0 # Tạo boolean true nếu thuật toán thắng tiền gửi bank rảnh rỗi

        return {
            "status": "success",
            "distribution": {
                "q1_conservative": q1,
                "q2_median": q2,
                "q3_optimistic": q3,
                "min_return": returns_series.min(),
                "max_return": returns_series.max(),
                "mean_return": avg_return_daily
            },
            "risk_profile": {
                # Mức thoái trào tài sản đau đớn nhất (Max Drawdown)
                "max_drawdown": max_drawdown, 
                "value_at_risk_95": var_95,  
                "volatility_annualized": annualized_volatility if 'annualized_volatility' in locals() else 0.0
            },
            "efficiency": {
                "sharpe_ratio": sharpe_ratio,
                "annualized_return": annualized_return if 'annualized_return' in locals() else 0.0
            },
            "opportunity_cost": {
                "initial_balance": initial_balance,
                "current_nav": final_balance,
                "bank_scenario_nav": bank_balance_scenario,
                # Delta Âm (-) tức là hệ thống thua tiết kiệm gốc
                "alpha_abs": opportunity_cost_abs, 
                "is_winning": is_beating_market
            }
        }

    def _calculate_max_drawdown(self) -> float:
        """
        Tính Toán Sức Chịu Đựng Thoái Trào (Peak-to-Trough Maximum Drawdown).
        CÔNG THỨC Vectorized (Time O(N)): Tạo chuỗi đỉnh Peak luân phiên bằng hàm Numpy Tích lũy (cummax), sau đó dóng khoảng cách (NAV - Peak) / Peak.
        """
        nav_series = self.df['nav_nominal']
        rolling_peak = nav_series.cummax()
        # Tính tỷ lệ %. MDD mang kết quả giá trị âm (-) càng nhỏ càng tệ khốc.
        drawdown = (nav_series - rolling_peak) / rolling_peak
        return drawdown.min()

    def _get_empty_report(self) -> Dict:
        """Fallback chống Crash vỡ ngoại lệ (Exception Overflow) khi Agent nạp mảng rỗng."""
        return {
            "status": "insufficient_data",
            "distribution": {"q1_conservative": 0, "q2_median": 0, "q3_optimistic": 0, "min_return": 0, "max_return": 0, "mean_return": 0},
            "risk_profile": {"max_drawdown": 0, "var_95": 0, "volatility_annualized": 0},
            "efficiency": {"sharpe_ratio": 0, "annualized_return": 0},
            "opportunity_cost": {"alpha_abs": 0, "is_winning": False}
        }

    def export_metrics_to_json(self, output_path: str = "logs/trading/advanced_quant_metrics.json"):
        """Kết xuất 50 con số Quant Matrix ra file định dạng JSON cho Streamlit UI."""
        report = self.generate_comprehensive_report()
        os.makedirs(os.path.dirname(output_path), exist_ok=True)
        with open(output_path, "w", encoding="utf-8") as f:
            json.dump(report, f, indent=4)
