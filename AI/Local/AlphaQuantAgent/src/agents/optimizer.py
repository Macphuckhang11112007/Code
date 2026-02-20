"""
/**
 * MODULE: The Optimizer (HRP Core)
 * VAI TRÒ: Chuyên gia Khai phóng & Phân mảnh vốn Không Giám Sát (Unsupervised). Thay vì ngốn GPU như PPO, 
 * Module này sử dụng thuật toán Hierarchical Risk Parity (HRP) của hệ máy học Lopez de Prado.
 * CƠ CHẾ CỐT LÕI (TRÍCH DẪN KHÓA): HRP tự động gom nhóm (Clustering) các tài sản có chuỗi biến động giá 
 * đồng pha thành các cụm (Baskets) liên kết. Sau đó nó thực hiện chia đôi Đệ quy (Recursive Bisection) 
 * gọt rẽ phân bổ vốn (Risk Parity) TỶ LỆ NGHỊCH (Inverse Ratio) với biến động Phương sai của cụm. Đóng đinh 
 * nguyên lý Rủi ro Cân bằng (Risk Parity). 
 * TẠI SAO: Giải quyết triệt để Hội chứng "Rác vào Rác ra" (Garbage In - Garbage Out) của thuật toán Markowitz kinh điển, 
 * giúp AlphaQuant xây dưng được bộ đệm chấn động mạnh nhất trước Black SWAN.
 * ĐỘ PHỨC TẠP: Time $O(N^3)$ (Của thuât toán Clustering Linkage), Space $O(N^2)$ (Matrix Dist/Cov).
 */
"""

import numpy as np
import pandas as pd
from scipy.cluster.hierarchy import linkage
from scipy.spatial.distance import squareform
from typing import List, Dict

class HRPOptimizer:
    def __init__(self):
        """Khởi Tạo Hệ Thống Linkage-Cluster (Không cần Fit Weight nên không lưu trạng thái đĩa cứng)."""
        pass

    def get_correl(self, returns: pd.DataFrame) -> pd.DataFrame:
        """Tính ma trận hệ số tương quan ngang (Pearson Correlation Matrix)."""
        return returns.corr()

    def get_covar(self, returns: pd.DataFrame) -> pd.DataFrame:
        """Khai sinh Ma trận Hiệp Phương Sai (Covariance Matrix / Rủi ro Biên)."""
        return returns.cov()

    def get_distance_matrix(self, corr: pd.DataFrame) -> pd.DataFrame:
        """
        BƯỚC 1: Biến đổi Metric (Distance Translation).
        CÔNG THỨC: $D = \\sqrt{0.5 \\cdot (1 - \\rho)}$
        TẠI SAO: Nhóm thuật toán Clustering vẽ Cây đệ quy Dendrogram chỉ ăn số đo Khoảng cách Hình học (Distance >= 0). 
        Do đó, khi 2 tài sản đồng pha (Corr = 1), Khoảng cách sẽ ép về $D = 0$ (Đồng thể). 
        Nếu ngược pha (Corr = -1), khoảng cách ép ra Max $D = 1$ (Cách ly).
        """
        return np.sqrt(0.5 * (1 - corr))

    def get_quasi_diag(self, link: np.ndarray) -> List[int]:
        """
        BƯỚC 2: Xoáy Trục Đường Chéo Cục Bộ (Quasi-Diagonalization).
        TÓM LƯỢC: Chạy thuật toán đồ thị, thay vì để BTC và VCB nằm tán loạn (Index tự nhiên 0,1,2,3), 
        Hàm sắp khít tụi nó lại với nhau sao cho BTC đặt sát ETH (vì khoảng cách D=0 cực ngắn của Cây phân nhánh Linkage).
        """
        link = link.astype(int)
        sortIx = pd.Series([link[-1, 0], link[-1, 1]])
        numItems = link[-1, 3] # Node gốc chứa tổng cộng bao nhiêu Leaves Element
        
        while sortIx.max() >= numItems:
            sortIx.index = range(0, sortIx.shape[0] * 2, 2) 
            df0 = sortIx[sortIx >= numItems]
            i = df0.index
            j = df0.values - numItems
            sortIx[i] = link[j, 0] # Expand Node Nhánh 1
            df2 = pd.Series(link[j, 1], index=i + 1) # Điền kẹp kẽ Node Nhánh 2
            sortIx = pd.concat([sortIx, df2])
            sortIx = sortIx.sort_index()
            sortIx.index = range(sortIx.shape[0])
            
        return sortIx.tolist()
        
    def get_cluster_var(self, cov: pd.DataFrame, cItems: List[int]) -> float:
        """
        Thuật toán Tính Năng Hàm Phương Sai Hỗn Chỉ Số (Cluster Variance Weighted Pool).
        Đo lường sự rung lắc của một túi tài sản con khi mix trộn lẫn lộn lại với nhau dựa trên Tỷ Trọng Nghịch Phương. (Inverse-Variance Weighting).
        """
        cov_slice = cov.iloc[cItems, cItems]
        # Inverse Variance = 1/Var
        ivp = 1.0 / np.diag(cov_slice)
        ivp /= ivp.sum()
        w = ivp.reshape(-1, 1)
        # cVar = W^T * Cov_Matrix * W
        cVar = np.dot(np.dot(w.T, cov_slice), w)[0, 0]
        return float(cVar)

    def get_rec_bipart(self, cov: pd.DataFrame, sortIx: List[int]) -> pd.Series:
        """
        BƯỚC 3: Dập Môi Phân Rã Khối Đệ Quy (Recursive Bisection Alpha Weighting).
        NGUYÊN LÝ HOẠT ĐỘNG:
        1. Xem toàn bộ Rổ tài sản là 1 cục to $W = 100\\%$.
        2. Chặt đôi nó ra thành Left_Basket và Right_Basket.
        3. Tính rủi ro cVar0 (Left) và cVar1 (Right).
        4. Chia Tỷ lệ Tiền $\\alpha$ (Alpha): Ví dụ Left Risk x2 lần Right Risk -> Cấp cho Left số Tiền bằng 1/2 Right.
        5. Đệ quy tiếp tục chẻ nhỏ cho đến khi chỉ còn rác tài sản độc lập (End node).
        """
        w = pd.Series(1.0, index=sortIx)
        cItems = [sortIx] # Lớp danh sách Root
        
        while len(cItems) > 0:
            # Thuật toán sinh sản Bisection: Điểm phân cách (Split ratio) cứng tại Median của List
            cItems = [i[j:k] for i in cItems for j, k in ((0, len(i)//2), (len(i)//2, len(i))) if len(i) > 1]
            
            for i in range(0, len(cItems), 2): 
                cItems0 = cItems[i] 
                cItems1 = cItems[i + 1]
                
                cVar0 = self.get_cluster_var(cov, cItems0)
                cVar1 = self.get_cluster_var(cov, cItems1)
                
                # Alpha HRP Rule: Risk parity allocation (Inverse scaling proportional to variance)
                # Alpha càng thấp (Khi cvar0 khổng lồ) thì cụm đó được cấp vốn càng ít.
                alpha = 1 - cVar0 / (cVar0 + cVar1)
                
                # Cập nhật Vector Trọng lượng Đầu tư Thực tiễn (Investment Target Weights)
                w[cItems0] *= alpha
                w[cItems1] *= 1 - alpha

        return w

    def optimize(self, price_matrix: np.ndarray, asset_names: List[str]) -> Dict[str, float]:
        """
        ĐIỂM GIAO THU THẬP KỸ NĂNG (The Entry Gate to Unsupervised Engine).
        THÔNG SỐ INPUT: Ma trận cửa sổ giá nội tại (Timesteps x Assets). Bóc tách ra từ 3D State Tensor Cổ Điển của Market.
        """
        if price_matrix.shape[0] < 2:
            return {sym: 1.0/len(asset_names) for sym in asset_names}

        df = pd.DataFrame(price_matrix, columns=asset_names)
        # Trực tiếp quy đổi Tỷ suất sinh lợi ngầm định (Pct_change returns array)
        returns = df.pct_change().fillna(0)

        # Fallback Bypass: Tránh lỗi chia 0 (Zero division/ Singularity matrix collapse) do giá trượt dài ko thay đổi
        if (returns == 0).all().all():
             return {sym: 1.0/len(asset_names) for sym in asset_names}

        corr = self.get_correl(returns)
        cov = self.get_covar(returns)
        
        # Biến hóa Metric Không Gian Đo Lường
        dist = self.get_distance_matrix(corr)
        
        # Scipy Hierarchy Core Algo: Sử dụng Linkage (kết nối láng giềng gần nhất - Single Linkage distance method) 
        link = linkage(squareform(dist.values), 'single')
        
        # Dịch pha chuỗi (Ordering the sequence block-by-block correlated bounds via Quasi)
        sortIx = self.get_quasi_diag(link)
        
        # Alpha Cutting Edge Fragmentation logic (Chẻ nhỏ Vốn)
        weights = self.get_rec_bipart(cov, sortIx)
        
        # Index trả về đã bị sortIx xáo trộn Index integer, nên dùng weights.index để match chuẩn xác.
        return {asset_names[idx]: float(weights[idx]) for idx in weights.index}
