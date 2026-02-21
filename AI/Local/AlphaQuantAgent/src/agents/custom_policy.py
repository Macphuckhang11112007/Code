"""
/**
 * MODULE: AlphaQuant Custom Policy Extractor
 * VAI TRÒ: Trái tim chống học vẹt (Anti-Overfitting) của mô hình RL PPO.
 * CƠ CHẾ: Mạng nơ-ron 2 nhánh (Two-Headed Network).
 *   - Nhánh 1: LSTM xử lý Time-Series (ts_data) để nắm bắt tuần tự thời gian 15 phút rưỡi.
 *   - Nhánh 2: MLP xử lý Static Data (29 biến số của Persona) để nhận diện môi trường.
 *   - Sau đó: Concat 2 luồng đặc trưng, đi qua Dropout layer để chống học vẹt.
 */
"""
import torch
import torch.nn as nn
from stable_baselines3.common.torch_layers import BaseFeaturesExtractor
from gymnasium import spaces

class AlphaQuantCustomExtractor(BaseFeaturesExtractor):
    """
    Feature Extractor tùy chỉnh cho Dict observation space.
    Gồm ts_data (Time Series) và static_data (Persona metadata).
    """
    def __init__(self, observation_space: spaces.Dict, features_dim: int = 256):
        super(AlphaQuantCustomExtractor, self).__init__(observation_space, features_dim)
        
        # Lấy định dạng kích thước từ Gymnasium Space
        ts_shape = observation_space.spaces['ts_data'].shape
        self.window_size = ts_shape[0]
        self.num_assets = ts_shape[1]
        self.num_features = ts_shape[2]
        
        # 1. Nhánh LSTM cho chuỗi thời gian (ts_data)
        # Trải phẳng 2 chiều cuối cho LSTM gặm (features của mọi tài sản tại 1 tick)
        lstm_input_size = self.num_assets * self.num_features
        self.lstm_hidden_size = 128
        
        self.lstm = nn.LSTM(
            input_size=lstm_input_size,
            hidden_size=self.lstm_hidden_size,
            num_layers=2,
            batch_first=True,
            dropout=0.2 # Trị Overfitting
        )
        
        # 2. Nhánh MLP cho Dữ liệu Tĩnh (static_data - Persona)
        static_shape = observation_space.spaces['static_data'].shape[0]
        self.static_net = nn.Sequential(
            nn.Linear(static_shape, 64),
            nn.LeakyReLU(),
            nn.Dropout(p=0.2), # Trị Overfitting
            nn.Linear(64, 64),
            nn.LeakyReLU()
        )
        
        # 3. Mạng Fusion (Gộp đa phương thức)
        concat_dim = self.lstm_hidden_size + 64
        self.fusion_net = nn.Sequential(
            nn.Linear(concat_dim, features_dim),
            nn.LeakyReLU(),
            nn.Dropout(p=0.3) # Phạt nặng để mô hình học các đại diện feature mạnh nhất
        )

    def forward(self, observations: dict) -> torch.Tensor:
        # A. Nhánh Time-Series
        ts_data = observations['ts_data']
        batch_size = ts_data.shape[0]
        
        # Reshape cho dễ ép vào LSTM: (batch, window, assets * features)
        ts_reshaped = ts_data.reshape(batch_size, self.window_size, self.num_assets * self.num_features)
        
        lstm_out, (h_n, c_n) = self.lstm(ts_reshaped)
        # Lấy hidden state của layer cuối cùng (num_layers=2 -> index -1 là layer cao nhất)
        ts_features = h_n[-1] # Shape: (batch_size, lstm_hidden_size)
        
        # B. Nhánh Static
        static_data = observations['static_data']
        static_features = self.static_net(static_data)
        
        # C. Fusion
        fused = torch.cat((ts_features, static_features), dim=1)
        output = self.fusion_net(fused)
        
        return output
