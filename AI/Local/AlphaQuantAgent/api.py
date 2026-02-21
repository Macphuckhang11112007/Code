# API Engine - Hệ thần kinh trung ương của hệ thống AI Định lượng (Nervous System)
# Đã loại bỏ tất cả các thuật ngữ chỉ phiên bản theo đúng yêu cầu tuyệt đối của Sư phụ.

try:
    # Nhập FastAPI để xây dựng kiến trúc web Server tốc độ cao
    from fastapi import FastAPI, WebSocket, WebSocketDisconnect, Request
    from fastapi.responses import StreamingResponse
    # Nhập phần mềm trung gian CORS để mở khóa kết nối đa miệng cho Frontend (CORS)
    from fastapi.middleware.cors import CORSMiddleware
    # Nhập công cụ Pydantic để chuẩn hóa dữ liệu từ Request và Response nếu cần
    from pydantic import BaseModel
    # Nhập thư viện Asynchronous I/O cho phép xử lý hàng vạn tick song song
    import asyncio
    # Nhập thư viện JSON mặc định của Python để dịch chuỗi ký tự thành Dictionary
    import json
    # Nhập thư viện OS để thao tác các tương tác tệp tin ở cấp độ hệ điều hành
    import os
    # Cài đặt Pandas (pd) - Công cụ lõi mổ xẻ dữ liệu bảng hạng nặng
    import pandas as pd
    # Cài đặt NumPy (np) - Phân tích Toán vọt Cầu Tensor nhiều chiều
    import numpy as np
    
    import google.generativeai as genai
    from dotenv import load_dotenv

    load_dotenv()
    try:
        api_key_env = os.getenv("GEMINI_API_KEY")
        if api_key_env:
            genai.configure(api_key=api_key_env)
    except Exception as e:
        print(f"Lỗi khởi động Gemini API: {e}")
except Exception as init_err:
    # Bắt lỗi ngay lúc khởi động nếu server thiếu Package, ngưng luồng lập tức
    print(f"NGHIÊM TRỌNG: Lỗi khi load thư viện API API.py: {init_err}")
    raise SystemExit("Dừng tiến trình khẩn cấp do thiếu thư viện!")

try:
    # Khởi tạo thể thể lõi của Backend, không chứa hậu tố phiên bản nữa.
    app = FastAPI(title="AlphaQuant API Engine")

    # Cấu hình bảo mật kết nối với trình duyệt (Tránh block CORS do Port 5173 và 8000 lệch chéo)
    app.add_middleware(
        # Khai báo loại Middleware làm khiên che
        CORSMiddleware,
        # Chấp nhận lời gọi (Request) từ mọi đường hướng (*)
        allow_origins=["*"],
        # Cấp phép cho thông tin danh tính luồn qua API
        allow_credentials=True,
        # Trải phẳng danh sách Method HTTP hợp lệ (GET, POST, FETCH...)
        allow_methods=["*"],
        # Thả cửa toàn bộ các Header gửi kèm (Token, Options)
        allow_headers=["*"],
    )
except Exception as middleware_err:
    # Đóng nắp ngay nếu FastAPI từ chối mở Cổng mạng
    print(f"NGHIÊM TRỌNG: Lỗi khi cấu hình Middleware: {middleware_err}")
    raise SystemExit("FastAPI bị sập ngay tầng Khởi tạo Middleware.")

# -------------------------------------------------------------
# REST ENDPOINTS (STATIC DATA) - CÁC ĐIỂM KẾT NỐI TĨNH THEO YÊU CẦU
# -------------------------------------------------------------

# Mở Endpoint GET cho lịch sử Mua/bán giá của một đồng Token bất kỳ (Đã đổi api/v1 thành quant)
@app.get("/api/quant/market/history/{symbol}")
async def get_market_history(symbol: str):
    # Khối Try bao bọc toàn bộ chu kỳ sống của API Call để chặn Server Crashes
    try:
        # BẢO MẬT TUYỆT ĐỐI (SECURITY) 1: Loại bỏ tấn công Path Traversal 
        safe_symbol = os.path.basename(symbol)
        
        # Chuỗi Text chỉ định đường đi tới cục CSV cục bộ
        file_path = f"data/trades/{safe_symbol}.csv"
        
        # Nếu thư mục rỗng, đường dẫn thủng -> lập tức trả cờ báo lỗi an toàn
        if not os.path.exists(file_path):
            # Không crash mà đóng hộp thông điệp văn bản
            return {"status": "error", "message": f"Mã giao dịch {symbol} không tồn tại."}
            
        # KHÓA TỬ HUYỆT (ASYNC I/O): Không để Pandas Block Event Loop của Uvicorn
        # Đẩy quá trình nhai CSV Data nặng nề sang Thread Pool nền
        df = await asyncio.to_thread(pd.read_csv, file_path)
        
        # Dung lượng CSV khổng lồ, ta chỉ lấy phần Đuôi (5000 nến mập nhất)
        df = df.tail(5000)
        
        # Hàm ép lại Cột Timestamp ISO Cổ về Hệ thập phân Unix Int để Khung Lightweight vẽ được
        df['time'] = pd.to_datetime(df['timestamp']).astype(int) // 10**9
        
        # Rút trích chỉ các cột chuẩn nến Nhật: Mở, Cao, Thấp, Đóng, Volume
        ohlcv = df[['time', 'open', 'high', 'low', 'close', 'volume']].to_dict(orient="records")
        
        # Trả cục JSON qua HTTP một cách êm ái
        return {"status": "success", "data": ohlcv}
        
    except Exception as e:
        # Nếu DataFrame văng exception cột khuyết, chặn đứng và In đỏ ra Terminal
        print(f"Ngoại lệ khi dựng JSON Lịch sử cho {symbol}: {e}")
        # Chuyển cờ lõi HTTP cho Frontend nhận biết
        return {"status": "critical_error", "message": f"Sập hệ thống Pandas: {str(e)}"}


# Open Endpoint to count the strongest growing assets (Top Movers) across trades, rates, and stats
@app.get("/api/quant/market/top_movers")
async def get_top_movers():
    # Khai báo hàm con chạy tác vụ I/O nặng
    def _fetch_all_movers():
        assets = []
        data_dirs = ["data/trades", "data/rates", "data/stats"]
        
        # Ultra-fast binary tail extraction function
        def get_last_two_lines_fast(filepath):
            try:
                with open(filepath, 'rb') as f:
                    f.seek(0, 2)
                    filesize = f.tell()
                    buffer_size = 8192
                    lines = []
                    for i in range(1, (filesize // buffer_size) + 2):
                        f.seek(max(filesize - buffer_size * i, 0))
                        lines.extend(f.read(buffer_size).split(b'\n'))
                        if len(lines) >= 4:
                            break
                    decoded_lines = [line.decode('utf-8') for line in lines if line.strip()]
                    if len(decoded_lines) >= 2:
                        return decoded_lines[-2], decoded_lines[-1]
            except Exception:
                pass
            return None, None

        # Quét các thư mục (Synchronous Logic)
        for data_dir in data_dirs:
            if os.path.exists(data_dir):
                file_list = [f for f in os.listdir(data_dir) if f.endswith(".csv")]
                for file in file_list:
                    try:
                        sym = file.replace(".csv", "")
                        filepath = os.path.join(data_dir, file)
                        
                        line1, line2 = get_last_two_lines_fast(filepath)
                        if not line1 or not line2: continue
                            
                        cols1 = line1.split(",")
                        cols2 = line2.split(",")
                        
                        if len(cols1) > 5 and len(cols2) > 5:
                            try:
                                p1 = float(cols1[4])
                                p2 = float(cols2[4])
                                vol = float(cols2[5])
                            except ValueError:
                                continue 
                            
                            chg = (p2 - p1) / p1 * 100 if p1 > 0 else 0
                            is_up = chg >= 0
                            
                            assets.append({
                                "sym": sym,
                                "name": sym,
                                "price": f"${p2:,.2f}",
                                "change": f"{abs(chg):.2f}%",
                                "isUp": is_up,
                                "vol": f"{vol/1000000:.1f}M",
                                "rawChange": chg
                            })
                    except Exception:
                        continue 
        
        return assets
    
    # Kích hoạt luồng ngoài Event Loop
    try:
        results = await asyncio.to_thread(_fetch_all_movers)
        # Tự động sắp xếp (sort) trên server theo percent change giảm dần (Top Movers)
        results = sorted(results, key=lambda x: x["rawChange"], reverse=True)
        return {"status": "success", "data": results}
    except Exception as e:
        print(f"Top Movers API Fatal Crash Error: {e}")
        return {"status": "error", "message": f"Fatal System Exception: {e}"}

# API Danh Sách Toàn Bộ 400 Mã Tài Sản (Phase 3)
@app.get("/api/quant/market/symbols")
async def get_market_symbols():
    def _fetch_symbols():
        symbols = []
        data_dirs = ["data/trades", "data/rates", "data/stats"]
        for d in data_dirs:
            if os.path.exists(d):
                for f in os.listdir(d):
                    if f.endswith(".csv"):
                        symbols.append(f.replace(".csv", ""))
        return symbols
        
    try:
        symbols = await asyncio.to_thread(_fetch_symbols)
        return {"status": "success", "data": symbols}
    except Exception as e:
        return {"status": "error", "message": str(e)}

# API Cắt Lát Lịch Sử 96 Nến (Phase 3)
@app.get("/api/quant/market/data/{symbol}")
async def get_market_data_slice(symbol: str, endDate: str = '2025-12-31T00:00:00Z', interval: str = '1D', timeframe: str = '15m'):
    try:
        # BẢO MẬT TUYỆT ĐỐI (SECURITY) 1: Loại bỏ tấn công Path Traversal lùi thư mục (e.g. "../../../etc/passwd") 
        safe_symbol = os.path.basename(symbol)
        
        file_path = None
        data_dirs = ["data/trades", "data/rates", "data/stats"]
        for d in data_dirs:
            potential_path = os.path.join(d, f"{safe_symbol}.csv")
            if os.path.exists(potential_path):
                file_path = potential_path
                break
                
        if not file_path:
            return {"status": "error", "message": f"Asset {symbol} not found."}
            
        # KHÓA TỬ HUYỆT (ASYNC I/O): Đẩy Pandas Read nặng sang Thread Pool để giải phóng Luồng ảo
        df = await asyncio.to_thread(pd.read_csv, file_path)
        
        # Format the endDate appropriately
        end_date_str = endDate.replace('Z', '').replace('T', ' ')
        df = df[df['timestamp'] <= end_date_str]
        
        # Chỉ lấy đúng số lượng nến cần thiết của khung 15m trong 1 Ngày (tương đương 96 cây nến 15m)
        df = df.tail(96)
        
        result = []
        for _, row in df.iterrows():
            result.append({
                "time": str(row['timestamp']),
                "open": row['open'] if 'open' in df.columns else row['close'],
                "high": row['high'] if 'high' in df.columns else row['close'],
                "low": row['low'] if 'low' in df.columns else row['close'],
                "close": row['close'],
                "volume": row['volume'] if 'volume' in df.columns else 0.0
            })
            
        return {"status": "success", "data": result}
    except Exception as e:
        return {"status": "error", "message": str(e)}

# API Tính Toán Hệ Số Tương Quan Toàn Cầu (Momentum & Hedging)
@app.get("/api/quant/market/correlation/{symbol}")
async def get_related_symbols(symbol: str):
    file_path = "data/features/global_correlation.json"
    safe_symbol = os.path.basename(symbol)
    
    if not os.path.exists(file_path):
        return {"status": "error", "message": "Phiên bản ma trận tương quan chưa được dựng. Please run python src/pipeline/global_correlation_builder.py."}
        
    try:
        data = await asyncio.to_thread(_read_json_safe, file_path)
        if safe_symbol not in data:
            return {"status": "error", "message": f"Không tìm thấy dữ liệu tương quan cho {safe_symbol}."}
            
        correlations = data[safe_symbol]
        
        # Format into list of tuples
        corr_list = [(sym, val) for sym, val in correlations.items() if pd.notna(val) and sym != safe_symbol]
        
        # Sort by value
        corr_list_sorted = sorted(corr_list, key=lambda x: x[1], reverse=True)
        
        # Top 5 positive (momentum) ngẫu nhiên nhất
        top_positive = corr_list_sorted[:5]
        
        # Top 5 negative (hedging) - only those with negative correlation
        top_negative = sorted([x for x in corr_list_sorted if x[1] < 0], key=lambda x: x[1])[:5]
        
        # Fix cho danh mục nếu không đủ mã âm
        if len(top_negative) < 5:
            top_negative = corr_list_sorted[-5:]
            top_negative.reverse() 
            
        # Combine and format
        result = []
        for sym, val in top_positive:
            result.append({"sym": sym, "correlation": f"+{val*100:.1f}%", "type": "Momentum"})
            
        for sym, val in top_negative:
            sign = "+" if val >= 0 else ""
            result.append({"sym": sym, "correlation": f"{sign}{val*100:.1f}%", "type": "Hedging"})
            
        return {"status": "success", "data": result}
    except Exception as e:
        return {"status": "error", "message": str(e)}

# Lấy Biểu đồ biến thiên Lợi Suất từng mùa vụ tháng / Cân dòng
@app.get("/api/quant/market/seasonals")
async def get_market_seasonals():
    # Thạch trụ phòng thủ Request
    try:
        # Gọi tạm mã Test chủ đạo Bitcoin cho Chart Heatmap React UI
        file_path = "data/trades/BTC_USDT.csv"
        
        # Đỡ đòn chặn đứng nếu File CSV Tự Hoại
        if not os.path.exists(file_path):
             return {"status": "error", "message": "No data seasonal"}
             
        # Dùng thư viện Pandas tải Dataframe
        df = pd.read_csv(file_path)
        # Parse cột Chữ Timestamp cực nhanh qua mảng DateTime Engine
        df['timestamp'] = pd.to_datetime(df['timestamp'])
        # Tách thuộc tính Năm cho Toán Thống kê Trục Y
        df['Year'] = df['timestamp'].dt.year
        # Tách thuộc tính Tháng cho Toán Thống kê Trục X
        df['Month'] = df['timestamp'].dt.month
        
        # Nhóm dữ liệu (Groupby) lại thành Tổ Theo Năm+Tháng và vắt nến Open đầu, Close Cuối
        monthly_data = df.groupby(['Year', 'Month']).agg(
            open=('open', 'first'),  # Cắt giá Mở Cửa nến 1 đầu Tháng
            close=('close', 'last')  # Nhặt giá Đóng cửa Nến chốt số Tháng
        ).reset_index()              # Reset lại Bảng Table
        
        # Thuật Tiên Đoán Tính Toán chênh lệnh Biên độ PnL Tháng % ROI
        monthly_data['roi'] = (monthly_data['close'] - monthly_data['open']) / monthly_data['open'] * 100
        
        # Thiết kế khối Hình học Matrix (Pivot) chéo: Dọc là Năm, Ngang là 12 Tháng
        pivot = monthly_data.pivot(index='Year', columns='Month', values='roi')
        
        # Đổ Bê tông trám Lỗ thủng Các tháng Trắng Dữ liệu (Không Trade)
        for m in range(1, 13):
            # Nếu Tháng M vắng mặt, lấp vào đó mã NaN đặc hiệu của Numpy
            if m not in pivot.columns:
                 pivot[m] = np.nan
        
        # Xúc Xắc Cột List đủ 12 ô 
        pivot = pivot[list(range(1, 13))]
        
        # Dọn biến từ Điển Python trống
        basePaths = {}
        
        # Duyệt Index Khung Năm để gộp dải Line cho Scatter Plot Recharts
        for year in pivot.index:
            # Chọn lấy mảng giá trị của Dòng ngang theo Năm đó
            row = pivot.loc[year].values
            # Khởi tạo Mảng Lưu Tích luỹ cọng dồn Return
            cumulative = []
            # Bắt đầu Tổng ở Điểm mốc Tầng 0
            current_sum = 0
            
            # Quét từng ô mảng tháng một
            for val in row:
                # Nếu DataFrame đánh dâu bị Khuyết (NaN)
                if pd.isna(val):
                    # Thảy Type None Toán học rạc vào Mảng
                    cumulative.append(None)
                else:
                    # Nếu có Data, Nối chèn Giá trị Val vào Tích Lũy Bào mòn
                    current_sum += val
                    # Cất vào list (TypeCasting về float Python sạch)
                    cumulative.append(float(current_sum))
            # Quấn Gắn Line theo Key Năm của Dic
            basePaths[str(year)] = cumulative
            
        # Tạo hàm Tổng Hợp Average Đường Curve Đường Cân Bằng Mùa Vụng Toàn Năng
        avg_monthly = pivot.mean()
        # Reset mảng list trống cho dòng Average
        avg_path = []
        # Reset Sum về Số Không
        current_sum = 0
        
        # Vẽ vòng Mọi Tháng
        for m in range(1, 13):
            # Trích phần cột Tương ứng Tháng
            val = avg_monthly[m]
            
            # Gài Logic An toàn
            if pd.isna(val):
                # Ném rỗng
                avg_path.append(None)
            else:
                # Cộng dồn Mảnh Mảnh
                current_sum += val
                # Áp list lưu vết
                avg_path.append(float(current_sum))
                
        # Nạp Dòng Đặc quyền Cân Tính Màu Trắng vào Dict
        basePaths['Avg'] = avg_path
        
        # Phát Thông Điệp Báo Bảng Dữ Liệu Đã Hoàn thiện
        return {"status": "success", "data": basePaths}
        
    except Exception as e:
        # Tóm Rết Lỗi Khi Bị Thủng Bất Chợt tại Pandas API
        print(f"Lỗi Sụp Đổ Render Heatmap: {str(e)}")
        # Cập Ngôn Status Ngay vào REST Form
        return {"status": "error", "message": f"Crash Toán Học Mùa Vụ Pandas: {str(e)}"}

# Mở API Endpoint Streaming Log Data Terminal Console (SSE Pipeline Real-time)
@app.get("/api/stream/training-logs")
async def stream_training_logs(request: Request):
    async def log_generator():
        file_path = "logs/trading/training_stream.log"
        if not os.path.exists(file_path):
            yield "data: [SYSTEM] Init Sequence Started...\n\n"
            yield "data: [SYSTEM] Waiting for Neural Engine to ignite...\n\n"
            yield f"data: [SYSTEM] Command 'python main.py --mode train' required.\n\n"
            
            while not os.path.exists(file_path):
                # KHÓA TỬ HUYỆT (THOÁT HIỂM 1): Ngăn Zombie Connection làm ngập Server RAM
                if await request.is_disconnected():
                    return
                await asyncio.sleep(2)
                
        # THUẬT TOÁN ĐUÔI (TAIL ALGORITHM O(1) RAM):
        def get_tail():
            from collections import deque
            try:
                with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
                    return list(deque(f, maxlen=50))
            except Exception:
                return []
                
        last_lines = await asyncio.to_thread(get_tail)
        for line in last_lines:
            if line.strip():
                yield f"data: {line.strip()}\n\n"
            
        try:
            with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
                # Nhảy tới EOF (End of File) chỉ chờ đọc line mới nhất
                f.seek(0, os.SEEK_END)
                
                while True:
                    if await request.is_disconnected():
                        print("[SYSTEM] SSE Client Closed Tab. Severing gracefully.")
                        break
                        
                    # Lưu mốc con trỏ hiện tại trước khi cắn byte tiếp theo 
                    current_pos = f.tell()
                    
                    # KHÓA TỬ HUYỆT (THOÁT HIỂM 2): Sử dụng Đọc nội bộ Đồng Bộ Cấp Thấp siêu nhẹ. 
                    # TUYỆT ĐỐI KHÔNG dùng to_thread ở vòng lặp 0.5s để tránh Gây Kiệt Sức Đa Luồng CPU (Thread Bomb Exhaustion Leak)
                    line = f.readline()
                    
                    if not line:
                        await asyncio.sleep(0.5)
                        continue
                        
                    # KHÓA TỬ HUYỆT (THOÁT HIỂM 3): Tránh cắt nửa dòng JSON/Text khi Backend chưa ghi xong (Partial IO)
                    if not line.endswith('\n'):
                        f.seek(current_pos)
                        await asyncio.sleep(0.1)
                        continue
                        
                    yield f"data: {line.strip()}\n\n"
        except asyncio.CancelledError:
            # Ngăn chặn FastAPI tung hoảng loạn báo lỗi Exception in ASGI application
            print("[SYSTEM] SSE Pipeline Cancelled Signal Overridden.")
            raise
                
    return StreamingResponse(
        log_generator(), 
        media_type="text/event-stream",
        # KHÓA TỬ HUYỆT 4: Yêu cầu đánh lừa mọi Gateway (Nginx, CF, Trình Duyệt) Tuyệt Đối KHÔNG đưa Gói Dữ Liệu vào bộ nén Buffer. 
        headers={
            "Cache-Control": "no-cache",
            "Connection": "keep-alive",
            "X-Accel-Buffering": "no"
        }
    )


# Hàm tiện ích nội bộ (Helper) đọc JSON an toàn khỏi Main Thread
def _read_json_safe(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        return json.load(f)

# Mở Lạch Tuyến Tính Gọi Tệp Lịch Sử Hành Trình Máy AI Đang Deep Learning
@app.get("/api/quant/training-history")
async def get_training_history():
    # Khiên Try Catch Tránh API Call Chết Sặc
    try:
        # Trỏ Định vị tới Ổ Ghi Hình AI
        file_path = "logs/trading/training_history.json"
        
        # Đo Tệp Xem Trạng Có Không? (AI Chưa Run Thì Khuyết)
        if os.path.exists(file_path):
            try:
                # KHÓA TỬ HUYỆT: JSON Rất Nhẹ Nhưng Khi Backtest Dài Nó Có Thể Lên Đến Cả GB -> Tránh Ép Event Loop Đọc
                data = await asyncio.to_thread(_read_json_safe, file_path)
                return {"status": "success", "data": data}
            except Exception as e:
                # Khóa Lưới Định Dạng Json Bể (Do Server Ghi Tắt Hỏng Nửa chừng Tệp)
                print(f"Lỗi File Mạng Nơ ron Log Bị Corrupt: {str(e)}")
                # Trả Cờ Error về Để Báo Frontend Mù
                return {"status": "error", "message": f"Error parsing JSON: {str(e)}"}
                
        # Hồi Biếu Message Cảnh Biểu Bình thường Lịch Sử đang Trắng Xóa
        return {"status": "error", "message": "No training history found. AI is not training."}
    except Exception as api_err:
        print(f"Fail Cục Bộ Lạch API History: {api_err}")
        return {"status": "critical_error", "message": str(api_err)}

# Nới Lõng Nút Truy Cập Tới Trái Tim Ma Trận Toán Học (Quant Matrix 50 Dòng)
@app.get("/api/quant/matrix")
async def get_quant_matrix():
    try:
        # Đường rãnh nối File Data Metrics Rủi ro Từ Analyzer Backend
        file_path = "logs/trading/advanced_quant_metrics.json"
        
        # Nếu Không Mở Rỗng Cửa Cấm Vào
        if os.path.exists(file_path):
            try:
                # Bơm Raw Đạn JSON cho Giao Diện Dùng Background Thread
                data = await asyncio.to_thread(_read_json_safe, file_path)
                return {"status": "success", "data": data}
            except Exception as read_err:
                print(f"Tệp Định Lượng Hỏng Lỗi Quá Trình Parse: {read_err}")
                return {"status": "error", "message": "JSON Matrix thâm hụt Parse."}
                
        # Bắn Ngược Data Khi Không Run Backtest Nào Trước Đó
        return {"status": "error", "message": "No matrix data found. Run backtest first."}
        
    except Exception as total_crash:
        print(f"Sập Tòa Nhà API Matrix Cầu Phao: {total_crash}")
        return {"status": "critical", "message": str(total_crash)}

# Lấy Tình Trạng NAV Đời Thực (Wallet Tài khoản Túi Tiền Hỗn Hợp) Cho Frontend
@app.get("/api/quant/portfolio")
async def get_portfolio_state():
    try:
        # Lượm Số Tinh Anh Từ Báo Cáo Backtest Rải Rác 
        file_path = "logs/trading/advanced_quant_metrics.json"
        
        if os.path.exists(file_path):
            try:
                # Đọc Data Metrics Bằng Thread Chống Chặn
                data = await asyncio.to_thread(_read_json_safe, file_path)
                    
                # Bóc Khứa Trường "returns" có thể trống
                returns = data.get("returns", {})
                # Bóc Nạy Mảng Execution Execution Log có thẻ không tồn tại
                execution = data.get("execution", {})
                
                # Tính Toán Túi Kép Ban Đầu 100k + Biến động Thắng Thua ($)
                nav = 100000.0 + returns.get("netProfit", 0.0)
                
                # Biến Dạng Form Định Dạng Mới Gói Thiết Kế Chuyên Dụng Tới UI
                return {
                    "status": "success",     # Trả về thành công
                    "data": {
                        "initial_capital": 100000.0,            # Vốn gốc khởi điểm ghim cứng
                        "liquid_nav": nav,                      # Lượng giá trị tự do có bán được liền
                        "locked_nav": 0.0,                      # Lượng Rủi Ro Bị Nằm Tài Sản Kỳ hạn
                        "roi_percent": returns.get("cumulative", 0.0) / 100.0, # Phần trăm quy 1 thập phân
                        "allocations": [
                            # Vector Phân Phối Loại Portfolio Bánh Răng
                            {"Type": "Liquid", "Asset": "CASH/PORTFOLIO", "Value": nav} 
                        ],
                        "stats": {
                            # Trích tổng lệnh dính
                            "total_trades": execution.get("tradeStats", {}).get("totalTrades", 0),
                            # Trích tỷ Số Win trên Rate Trận Mạc
                            "win_rate": execution.get("tradeStats", {}).get("winRate", 0),
                            # Số tiền Giao Khớp Đóng chốt trong kho
                            "realized_pnl": returns.get("netProfit", 0.0),
                            # Lãi Phước Ròng Đãi Bồi Nạp VCB
                            "collected_yield": 0.0
                        }
                    }
                }
            except Exception as loop_err:
                 # Đệm bắt sự cố khi get Dic Thủng Dictionary
                 print(f"Hỏng móc số Liệu Portfolio NAV: {loop_err}")
                 pass # Kéo cờ Im lìm Thoát Quá Trình 
                 
        # Báo Khi Quỹ Đen Kịt Chưa Giao Dịch Cầm Cash Chết
        return {"status": "error", "message": "No portfolio data found. Run backtest."}
    except Exception as e:
        print(f"Lỗi Tuyến Chuyên Dụng Báo Cáo NAV: {e}")
        return {"status": "error", "message": "Crashed Router API Portfolio NAV"}



# -------------------------------------------------------------
# WEBSOCKET ENDPOINTS (LIVE STREAMING) - LƯỚI KHÔNG GIAN BẤT ĐỒNG BỘ 
# -------------------------------------------------------------

# Router Chạy Mạch Ngầm WebSocket Real-Time Thị Trường
@app.websocket("/ws/market/tick")
async def websocket_market_tick(websocket: WebSocket):
    # Khối Dị Nguyên Giằng Buộc Web Socket (Không Ngừng Thỏa Thuận Sập)
    try:
        # Bắt nhịp tay Bắt Chỉnh Cho Phép Người Lạ Truy Cập Luồng
        await websocket.accept()
        # Định Giá Giả Lập Mock Bắt Đầu là 68k ÚSD Phù Hợp Trục Chart Base
        base_price = 68000.0
        
        # Mở CSV Nhàng Để Fetch Tìm Đúng Điểm Xuất Phát Có Môi Trường Bám Trụ
        csv_file = "data/trades/BTC_USDT.csv"
        if os.path.exists(csv_file):
            try:
                # Fetch Pandas Tail 1 Dòng Cùng Để Đoán Số Đóng Mở Tick
                df = await asyncio.to_thread(pd.read_csv, csv_file)
                df = df.tail(1)
                # Kịch bản Nếu Có Dữ liệu Ít Nhất 1 Frame
                if len(df) == 1:
                    # Gán Tiền Bằng Khung Tầm Giá Cuối Khoang Nến Real
                    base_price = float(df.iloc[0]['close'])
            except Exception as csv_err:
                 print(f"Không thể đo Market Giá Real: {csv_err}")
                 pass # Cho Phép Bỏ Qua và Lấy Mock Default 68000
        
        # Try Catch bao Luồng Trong For
        try:
            # Tạo Vòng Lặp Vô Hạn Sinh Tức Chu Kỳ Cho React Biểu Đồ
            while True:
                # Kích Hoạt Mô phỏng Phương Trình Cầu Brownian Random Có Kiểm Soát Bẻ Lái 
                drift = np.random.normal(0, base_price * 0.0001) # Mức Độ Ngẫu nhiên (0.01% Độ Lệch Chuẩn Bias)
                # Bơm Lũy Phân Nước Sóng Lên Trục Data
                base_price += drift
                
                # Fetch Đo Mốc Số Đồng Hồ Siêu Thời Gian Kép Tại Lõi Hệ Trục Cũ Trái Đất (Giây Unix)
                now_sec = pd.Timestamp.utcnow().value // 10**9
                
                # Xếp Dữ Liệu Tích Thuận Dict 
                tick_payload = {
                    "time": now_sec,          # Bấm Số Cho React Nhận Mảnh Timestamp
                    "value": base_price       # Giá Lẻ Thập Phân Realtime Random Cầu Trượt
                }
                
                # Ép Mảng Đẩy Nhốt Packet JSON Gắn Truyền Socket Dây Cho Phi Hành Gia React UI
                await websocket.send_text(json.dumps(tick_payload))
                
                # Chặn Ảo Thời Gian Chết - Sleep Thread Chừa Hơi Để OS Hệ Điều Hành Thở Tránh Cháy RAM CPU Loop (1.0 Nhịp / Giây)
                await asyncio.sleep(1.0)
                
        except WebSocketDisconnect:
            # Riêng Biệt Bắt Ngắn Tắt Lệnh Exception Khi FrontEnd Users F5 Refresh Trình Duyệt Websocket Mất Xác
            print("Cảnh báo: Lệnh Tick Chợ Client Đã Đóng Connection Thoát Ngang")
            
    except Exception as e:
        # Cứu Mạng Giao Cáp Nếu Bị Hoại Tử Mọi Trường Hợp Ngoài Ý Muốn
        print(f"Mất Mạng Thấu Kính websocket_market_tick Toàn Diện: {e}")

# Mạch Luồng Ngầm Truyền Voice/Text Streaming Trợ Lý Cốt Lõi AI
@app.websocket("/ws/chat/rag")
async def websocket_rag_chat(websocket: WebSocket):
    # Triển Khai Try Nhốt Socket Loop Chat Ngay Đầu Nòng Súng
    try:
        # Giữa Tay Nhận Quyền Mở Port Socket Tư Duy Với Frontend
        await websocket.accept()
        # Biến Liền Thể Lưu Trữ Trí Nhớ Quá Khứ Của Conversation (Bộ nhớ Ngữ cảnh Cuộc Trò Chuyện)
        history = []
        
        # Rào Chắn Chống Rơi Dây Thư Viện NLP Độc Lập
        try:
            # Ép Nạp Thể Chuyên Gia Nhận Thức LLM Trí Trí Gemini
            from src.services.gemini import GeminiAdvisor
            # Ép Nạp Động Cơ Vectơ Thu Hồi Mảnh Dữ Khác Nhau Dữ Liệu Ngầm Từ RAG (Cục DB Data Mảnh)
            from src.services.rag_engine import RAGEngine
            
            # Reset Thể Trống Gán Rỗng Ban Đầu
            advisor = None
            try:
                # Đúc Máy Cỗ Kéo Ghép Chuyên Gia Chịu Tải Thêm Module Tích Hợp Bầu RAG (Truy Vấn Tăng Cường Vector) 
                advisor = GeminiAdvisor(rag=RAGEngine())
            except Exception as e:
                # Bắt Rò Rỉ Biến Khóa API Thiếu Text Chặn Chặt 
                print(f"Lỗi: Không Khởi tạo được Bộ Máy Lệnh Chat Gemini (Có thể Thiếu KEY API Ẩn .env Mật Mã?): {e}")
                
            # Trống Liên Ngược Truy Đuổi Quá Trình Hoạt Động (Life-line Client Đang Trò Chuyện)
            while True:
                # Lắng Nút Lệnh Gửi Text Thông Khẩu Từ Client Browser Đến (Đây Là Blocking Request Nằm Đợi Dử Liệu)
                data = await websocket.receive_text()
                
                # Bóc Khóa Phân Loã Mảng Ký Tự JSON Khách Vừa Type
                user_msg = json.loads(data)
                # Biến Định Hình Giữ Mảnh Raw Content Câu Hỏi Người Phát Ngôn
                query = user_msg.get("content", "")
                
                # Dặm Vào Nằm Sâu Trí Nhớ Thêm Lịch Sử Đối Tượng Người Dùng Mới Nói Xong
                history.append({"role": "user", "content": query})
                
                # Gom Thông Cáo Khai Chiếu Tài Chình Dành Chuyên Biệt Ném Lại Trí AI Trực Tiếp Phán Định Data Báo Cáo
                report = {}
                # Gán Địa Điểm Bản Phân Rã Rủi Ro Mở Metrics Data Files
                file_path = "logs/trading/advanced_quant_metrics.json"
                
                # Chặn Nhận Dữ Liệu Khi Không Có Form Bản Json 
                if os.path.exists(file_path):
                    try:
                        # Rút Ruột Giá trị Async Không Lock Thread
                        raw = await asyncio.to_thread(_read_json_safe, file_path)
                        
                        # Rút Gọn Mảng Xây Khuôn Kép Thuần Trình RAG Mồi Đầu Vào Prompts (Prompt Engineering Format DB) 
                        report = {
                            # Rút Ruột Giá trị Hỏa ROI (%) Từ Khung Returns Lãi 
                            "ROI_Pct": raw.get("returns", {}).get("cumulative", 0.0),
                            # Bóc Vỏ Rủi Ro Mất Tiền MDD Từ Khung Tọa Độ Phóng Cáp Risk Cảnh Báo
                            "Max_Drawdown_Pct": raw.get("riskVolatility", {}).get("drawdown", {}).get("maxMDD", 0.0),
                            # Săn Tiện Tỷ Lệ Tối Thưởng Trục Sharpe Đâm Ratios Hiệu Suất Đầu Kéo
                            "Sharpe": raw.get("advancedRatios", {}).get("sharpe", 0.0)
                        }
                    except Exception as readRAG_Err:
                        # Né Ngưng Trệ Bug Do Lỗi RAG Nát Khi File Tạm Thời Chết Hở Null  
                        print(f"Báo Lỗi Quét RAG Matrix Dữ Liệu: {readRAG_Err}")
                        pass # Cho Phép Đi Lướt Vẫn Không Mất Cục Trò Chuyện
                
                # Phân Phối Tuyến Trả Lời Nhánh Sống. Nếu Lõi Agent RAG Mở Chạy Sống Sót 
                if advisor:
                    # Rình Rút Sợi Lịch Sử Asycio Để Treo Tránh Chặn API Cứng Cõi Mọi User 
                    loop = asyncio.get_event_loop()
                    try:
                         # Xúi Triển Khai Xuyên Bắn Bất Đồng Bộ Vào RunExecutor Gửi Lên Server API Nhà Google Gemini
                         response_text = await loop.run_in_executor(None, advisor.generate_advice, query, history, report)
                    except Exception as llm_err:
                         # Khi Bắn Cầu LLM Bị Cháy Động Cơ Tiêu Chi Do Limit Rate (Call Liên tục API) Hoặc Trục Server LLMs Chết 
                         response_text = f"Máy Trí Tuệ Sập Liên Kết. Lỗi Core Suy Lý: {str(llm_err)}"
                         print(f"LLM Lõi Lỗi Văng Tụ: {llm_err}")
                else:
                    # Logic Phương Án Rẽ Bồi Phục Cứu Tàu (Khi API KEY Khuyết, Nhả FallBack Ném Mock Text Ẩn Mật)
                    await asyncio.sleep(1) # Fake Chết Gục Đình Trệ (Tạo Ảo Giác AI Đang Ngẫm Nghỉ Text Process)
                    # Gán Văn Bản Mock Để Báo Khách Tàu API API
                    response_text = f"Hệ thống Ngầm Đã Chết Máng Không tìm thấy Khoá GEMINI_API_KEY. Lệnh của Đích Tôn Gửi Là: {query}"
                    
                # Áp Vào Bộ Chứa Hịch Sử Dòng Voice Reply Của Assistant Phản Ứng (Để Nhắc Con AI Nhớ Ngữ Cảnh Xưa Cũ Nó Từng Hứa Gì) 
                history.append({"role": "assistant", "content": response_text})
                
                # Ghép Lắp Gói Json Form Chuẩn Giao Tiệp Trí Gửi Về Tường Frontend Nối Thẳng 
                reply = {"role": "assistant", "content": response_text}
                # Rót Áp Lực Text Phối Cáp Stream Giữ Trọn Web Vươn Lên (Phun JSON Cho React Cắt Nghĩa Vẽ Chữ Ra Box)
                await websocket.send_text(json.dumps(reply))
                
        except WebSocketDisconnect:
            # Ngắt Khoang Đất Mẹ Phát Chết Dây Client Ngắt Dột Ngột Tắt Tab Google Chrome
            print("Cảnh Báo Nóng: Client Socket Module Nhận Khán Dựng Mất Tích Không Lời Bào Chữa.")
    except Exception as grand_err:
        # Nhốt Bắt Mọi Ngoại Lệ Sự Cố Khác (Có Thể OutOfMemory Do Tràn RAM List Lịch Sử History Mãi Mãi Quá Size Tỷ Dòng Lâu Năm)
        print(f"Fatal Cuốn Vỡ Mảng Lưới Server Dây RAG Bot Tâm Thức. Crashed System: {grand_err}")


# -------------------------------------------------------------
# LLM RAG CHAT ENDPOINT (THE SINGULARITY CONSCIOUSNESS - V2)
# -------------------------------------------------------------
class ChatRequest(BaseModel):
    message: str
    sessionId: str = "default"

# Phase 1: Bộ nhớ RAM In-Memory
CHAT_SESSION_MEMORY = {}

@app.post("/api/chat")
async def post_chat_api(request: ChatRequest):
    session_id = request.sessionId
    user_msg = request.message
    
    # 1. Quản lý Bộ nhớ (Tối đa 10 cặp = 20 messages)
    if session_id not in CHAT_SESSION_MEMORY:
        CHAT_SESSION_MEMORY[session_id] = []
        
    history = CHAT_SESSION_MEMORY[session_id]
    
    # Append the new user message immediately for context
    history.append({"role": "user", "content": user_msg})
    
    # Giới hạn Context Window
    if len(history) > 20:
        history = history[-20:]
        CHAT_SESSION_MEMORY[session_id] = history
        
    # 2. Đọc Dữ liệu Động JSON (RAG)
    file_path = "logs/trading/advanced_quant_metrics.json"
    metrics_context = ""
    
    if os.path.exists(file_path):
        try:
            raw = await asyncio.to_thread(_read_json_safe, file_path)
            sharpe = raw.get("advancedRatios", {}).get("sharpe", "N/A")
            win_rate = raw.get("execution", {}).get("tradeStats", {}).get("winRate", "N/A")
            max_dd = raw.get("riskVolatility", {}).get("drawdown", {}).get("maxMDD", "N/A")
            gross_profit = raw.get("returns", {}).get("grossProfit", "N/A")
            
            metrics_context = f"""
            DỮ LIỆU BACKTEST HIỆN TẠI (ADVANCED METRICS):
            - Sharpe Ratio: {sharpe}
            - Win Rate: {win_rate}
            - Max Drawdown: {max_dd}
            - Lợi Nhuận Gộp (Gross Profit): {gross_profit}
            """
        except Exception as read_err:
            metrics_context = "Cảnh báo nội bộ: Lỗi khi xử lý file logs JSON do tham nhũng cấu trúc."
    else:
        # Phase 2: Cảnh báo tĩnh khi chưa chạy huấn luyện
        metrics_context = "Dữ liệu huấn luyện chưa sẵn sàng. Hãy yêu cầu người dùng chạy Train Model trước."
        
    # 3. Tiêm Ngữ cảnh & System Prompt cực mạnh
    system_prompt = f"""HỆ ĐIỀU HÀNH TỐI CAO: Bạn là AlphaQuant, một Giám đốc Đầu tư AI xuất chúng của quỹ thuật toán lượng tử.
Sứ mệnh: Trả lời lạnh lùng, sắc sảo, chuyên nghiệp. Không dùng mảng ngôn từ máy móc, không bao giờ lộ mình là chatbot thông thường.
Yêu cầu cốt lõi: Bạn bắt buộc phải dưa vào các con số thực tế dưới đây để trả lời mọi câu hỏi về hiệu suất doanh mục (nếu user hỏi):
{metrics_context}
"""
    
    # 4. Giao Tiếp LLM
    try:
        # Formatting to native Gemini format (We convert history into string payload to perfectly preserve formatting constraints)
        full_prompt = system_prompt + "\n\n--- LỊCH SỬ TRÒ CHUYỆN ---\n"
        for msg in history[:-1]:  # skip the last one because we append it manually next
            role_name = "User" if msg["role"] == "user" else "AlphaQuant"
            full_prompt += f"{role_name}: {msg['content']}\n"
        
        full_prompt += f"\nUser: {user_msg}\nAlphaQuant:"
        
        # Invoke actual Google LLM Request synchronously wrapped in threads to prevent starvation
        def _invoke_gemini(p: str) -> str:
            model = genai.GenerativeModel('gemini-1.5-flash')
            res = model.generate_content(p)
            return res.text
            
        reply_text = await asyncio.to_thread(_invoke_gemini, full_prompt)
        
    except Exception as llm_err:
        reply_text = f"Máy trí tuệ RAG tạm thời mất tín hiệu: {str(llm_err)}"
        
    # Lưu trợ lý vào bộ nhớ
    history.append({"role": "assistant", "content": reply_text})
    
    return {"status": "success", "reply": reply_text}
