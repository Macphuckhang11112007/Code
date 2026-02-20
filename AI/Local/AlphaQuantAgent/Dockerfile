# Sử dụng Python 3.12
FROM python:3.12

# Thiết lập biến môi trường để Python không tạo file .pyc và log mượt hơn
ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

# Cài đặt các gói hệ thống cần thiết (gcc, libpq để kết nối DB)
RUN apt-get update && apt-get install -y \
    build-essential \
    libpq-dev \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Thiết lập thư mục làm việc trong container
WORKDIR /app

# Copy file requirements và cài đặt thư viện
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy toàn bộ code vào container
COPY . .

# Mở port (dự kiến dùng FastAPI sau này)
EXPOSE 8000

# Lệnh mặc định: Giữ container luôn chạy để ta vào code (Dev Mode)
# Khi lên Prod sẽ đổi thành lệnh chạy Server
CMD ["tail", "-f", "/dev/null"]
