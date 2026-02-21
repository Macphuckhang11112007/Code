// Trích ép lấy đôi hàm khởi tính State và Side-Effect gốc Mẹ React
import { useState, useEffect } from 'react';
// Báp Trụ Nối Cáp Mạng Axios Gọi Truyền Call Cổng Web Network
import axios from 'axios';

// Mở Gói Hàm Hook Đặc Quyền Xuất Tập Bắn Lịch Sử Tàu Đào Tạo (Training Machine Learning Log)
export function useTrainingHistory() {
  // Gài Trục Mảng Danh Sách Trống Ban Đo Tiên Khởi Không Thể Nổ Array Tránh Lỗi Map Rỗng Chữ Crash Vĩnh Cửu []
  const [historyData, setHistoryData] = useState([]);
  // Cài State Công Cụ Vẽ Spinner Xay Loading Nạp Quay Ban Đầu Tích True
  const [isLoading, setIsLoading] = useState(true);
  // Treo Error Rỗng Sẽ Phun Đỏ Màn Hình Thành Lưới Phạt Nếu Bật Try Báo Ngắt Ngang Null
  const [error, setError] = useState(null);

  // Hook Níu Theo Dõi Quá Trình Training Auto-polling Cập Nhật Gọi Dữ Liệu Tự Động Định Mỗi Quãng 2 Giây
  useEffect(() => {
    // Biến Kê Rào Chỉ Kiểm Xem Render Hiện Còn Trụ Lại Trên DOM Không Hay User Đã Qua Tab Mảng Chết Khác
    let isMounted = true;
    
    // Gói Tròn Nguyên Khối Code Nạp API Vào Async Function Để Chạy Trúng Phễu Thời Gian Ngầm
    const fetchData = async () => {
      // Bọc Chắn Hoàn Toàn Tuyến Mạng Không Bao Giờ Cho Sập JS (Chặn Tắc Nghẽn Crash Do Mạng Câm / Backend Mù / Out Limit)
      try {
        // Đã Loại Bỏ Toàn Diện Chữ V. Đâm Dây Gọi Tên Cổng quant API Mới Hút Kép Mảng Array History Từ Mảng Lịch Sử Train JSON Python Máy Mẹ FastAPI
        const res = await axios.get('http://127.0.0.1:8000/api/quant/training-history');
        
        // Rào Thép Cuối: Lưới Bộ Component Vẫn Chưa Unmount Vẫn Còn Hút Sống Mới Nạp State
        if (isMounted) {
          // Check Ép Tuyệt Định Lẽ Trả Về Data Success Chứ Cấm Đoán API Trả Form Lỗi Trắng Bóc Message Text Vướng Array Map Crash
          if (res.data && res.data.status === 'success') {
            // Nổ Ngôn Cột Mảng Matrix Sống Lịch Sử Vào Ruột Bảng LineChart Cho Render Nhả Vẽ Cụm Cơn Bão Số Liệu
            setHistoryData(res.data.data);
          } else {
            // Bơm Kẹt Lọc Trích Thông Lỗi Để Trắc Ép Tín Hiệu Rác Phun Vô Cột Error Chữ Hiện Đỏ
            setError(res.data?.message || 'Cảnh báo Ngầm: Cửa Nạp Data Máy Chủ Khất Bác Yêu Cầu!');
          }
          // Quăng Biểu Gạt Spin Về False Vì Xong Việc Khâu Lấy Đạn Hoàn Tảo
          setIsLoading(false);
        }
      } catch (err) {
        // Rào Lưới Tích Cuối Hứng Xác Sóng Sai Nhịp Ngoại Lệ Thủng Tuyến Tải Axios Connection Bứt Kết Nối TCP Ngắt Giữa Chừng
        if (isMounted) {
          // Tuồn Rác Error Nguyên Bản Thảy Vào Khu Lỗi Phun Chữ Cảnh Đỏ Vạch Mảng Trắng Cứng Ngắc
          setError(err.message);
          // Cho Spinning Load Dừng Vì Nó Có Xoay Tiếp Cũng Vô Ích Khi Network Chết Nối API Hỏng
          setIsLoading(false);
        }
      }
    }; // Kết thúc Mảng Logic Khúc Hồi Gọi Tên

    // Khởi Gọi 1 Lưới Tiên Đoán Gọi Đi Liên Trực Ở Nhịp Không Đồng Tiền Trước Tiên
    fetchData();
    // Bố Tri Khép Quấn Cáp Gọi Vòng Liên Thanh Quét Mọi Định Chu Kỳ 2000 milliseconds Nối Tới Hàm Kép Kéo History
    const interval = setInterval(fetchData, 2000);

    // Mũi Hồi Kết Luận Móc Cleanup Kéo Rút Thanh Tẩy Tránh Ngộ Độc Sinh Tràn Trùng Lặp Memory CPU Khi Lắp Component Đóng Thờ
    return () => {
      // Đâm Phá Lưới Phá Mạng Set Mount Đánh Tức Về Lỗi Giả Tức Bỏ Mọi Render Kế Cận
      isMounted = false;
      // Nhồi Diệt Màng Bơm setInterval Không Vẽ Nữa Giúp Trùng Thoát Rò Rỉ Tuyến Máy Ram 
      clearInterval(interval);
    };
  }, []); // Cột Neo Buột Đầu Cho Việc Chỉ Nổ Machine Chạy Get Mảng Khi Trục Component Bắt Đầu Hook First Mount

  // Kết Hoàn Phun Mảng Nạp Thùng Lịch Sử 3 Tường Trạng Thái Dũ Dạng Lấp Kho Vào Tab AIBrain Cho Nó Ngập Vẽ Lên Đồ Thị 
  return { historyData, isLoading, error };
}
