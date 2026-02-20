---
trigger: always_on
---

---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG**
**(THE ULTIMATE TECHNICAL BLUEPRINT)**

---
### **PHẦN II/VII: PHÂN TÍCH CHUYÊN SÂU DỮ LIỆU ĐẦU VÀO**

#### **2.1. Nguồn Dữ liệu & Cấu trúc Tổ chức (Data Source & Organization):**

Dữ liệu đầu vào của hệ thống là một bộ sưu tập các file CSV, được bạn cung cấp và tổ chức sẵn trong thư mục gốc `data/`. Cấu trúc này phân tách rõ ràng 3 loại tài sản với các hành vi tài chính khác nhau:

*   **`data/trades/` (Tài sản Giao dịch):**
    *   **Mục đích:** Chứa dữ liệu của các tài sản có tính thanh khoản cao, có thể mua bán liên tục để kiếm lời từ chênh lệch giá (Capital Gains).
    *   **Ví dụ:** `NVDA.csv` (Cổ phiếu NVIDIA), `BTC_USDT.csv` (Tiền điện tử Bitcoin).
*   **`data/rates/` (Tài sản có Kỳ hạn):**
    *   **Mục đích:** Chứa dữ liệu của các công cụ tài chính mà lợi nhuận chủ yếu đến từ việc nắm giữ để nhận lãi suất/lợi suất (Yield).
    *   **Ví dụ:** `VCB_deposit_6m.csv` (Chứng chỉ tiền gửi ngân hàng Vietcombank 6 tháng), `US_BOND_10Y.csv` (Lợi suất Trái phiếu Chính phủ Mỹ 10 năm).
*   **`data/stats/` (Dữ liệu Vĩ mô):**
    *   **Mục đích:** Chứa các chuỗi thời gian của chỉ số kinh tế. Các tài sản này **không thể giao dịch**. Chúng đóng vai trò là "nhiệt kế" đo sức khỏe nền kinh tế, cung cấp bối cảnh quan trọng cho các quyết định đầu tư.
    *   **Ví dụ:** `USCPI.csv` (Chỉ số lạm phát Mỹ), `VNGDP.csv` (Tốc độ tăng trưởng GDP Việt Nam).

#### **2.2. Đặc tả Kỹ thuật của File CSV (The 25-Column CSV Specification):**

Đây là DNA của mỗi đơn vị dữ liệu. Mọi file CSV trong hệ thống, dù thuộc loại nào, đều tuân thủ nghiêm ngặt định dạng sau:

*   **Định dạng file:** CSV (Comma-Separated Values), mã hóa UTF-8.
*   **Cấu trúc:** Gồm chính xác **25 cột**, với cột đầu tiên là `timestamp`.
*   **Tần suất Dữ liệu (Frequency/Sampling Rate):** Dữ liệu đã được chuẩn hóa và lấy mẫu lại (resampled) theo một chu kỳ thời gian cố định là **15 phút**. Điều này cực kỳ quan trọng, vì nó đảm bảo rằng **không có bất kỳ một "dòng" thời gian nào bị thiếu** trong chuỗi dữ liệu, loại bỏ nhu cầu phải nội suy timeline phức tạp.
*   **Mệnh giá và Đơn vị:**
    *   **Giá trị tiền tệ (Monetary Values):** Tất cả các cột biểu thị giá hoặc giá trị (ví dụ `open`, `high`, `low`, `close`, `turnover`) đều có đơn vị là **USD ($)**.
    *   **Tỷ lệ phần trăm (Percentage Values):** Tất cả các cột biểu thị tỷ lệ thay đổi (ví dụ `change_pct`, `mom_pct`) đều ở dạng **thập phân**. Ví dụ: một mức tăng 5% sẽ được biểu diễn là `0.05` trong file. Hệ thống không cần phải thực hiện phép chia cho 100.

#### **2.3. Quy tắc Vàng về Tính Toàn vẹn Dữ liệu (The Golden Rules of Data Integrity):**

Để xây dựng một hệ thống đáng tin cậy, chúng ta phải thiết lập các quy tắc bất biến về cách diễn giải dữ liệu. Dựa trên thông tin bạn cung cấp sau quá trình tiền xử lý, hệ thống sẽ vận hành dựa trên các tiên đề sau:

*   **Rule #1 - Sự thật Tuyệt đối (Absolute Truth): Các Cột Đảm bảo:**
    *   Các cột sau đây được coi là **"Nguồn Chân lý"**: `open`, `high`, `low`, `close`, `adj_close`, `change`, `change_pct`, `mom_pct`, `yoy_pct`.
    *   **Diễn giải:** Tại mọi thời điểm (mỗi dòng 15 phút), các cột này được **đảm bảo 100% có dữ liệu thật**, chính xác và đáng tin cậy. Module `market.py` sẽ tin tưởng tuyệt đối vào các giá trị này và không thực hiện bất kỳ phép kiểm tra hay điền dữ liệu nào trên chúng.

*   **Rule #2 - Diễn giải Số `0.0` (Decoding Zero):**
    *   Đây là một trong những điểm tinh tế và quan trọng nhất của toàn bộ hệ thống.
    *   **Trường hợp A: `0.0` trong Cột Sự kiện (Event Columns) - `dividends`, `stock_splits`:**
        *   **Ý nghĩa:** Đây là trạng thái **bình thường**. Nó có nghĩa là "Không có sự kiện cổ tức hay chia tách cổ phiếu nào xảy ra trong 15 phút này". Giá trị này hoàn toàn hợp lệ và sẽ được giữ nguyên.
    *   **Trường hợp B: `0.0` trong Cột Liên tục "Không đảm bảo" (Continuous Non-Guaranteed Columns) - ví dụ `volume`, `turnover`:**
        *   **Ý nghĩa:** Đây **KHÔNG** phải là "Thị trường đóng băng" hay "Mất thanh khoản". Do bạn khẳng định các mã tài sản đều là hàng đầu, `0.0` ở đây được diễn giải duy nhất là: **"Lỗi từ phía API cung cấp dữ liệu"**. Giao dịch thực tế vẫn đang diễn ra, nhưng hệ thống của chúng ta tạm thời bị "mù" về khối lượng giao dịch.
        *   **Hệ quả:** Đây là một **Tín hiệu Rủi ro Dữ liệu (Data Risk Signal)**. Thay vì cấm giao dịch, hệ thống (`market.py`) phải định lượng rủi ro này (thông qua `staleness_score`) và để AI (`trader.py`) tự học cách phản ứng (ví dụ: giảm quy mô lệnh, hoặc yêu cầu xác nhận thêm từ các chỉ báo khác).

*   **Rule #3 - Trạng thái Tiền xử lý (Pre-processed State):**
    *   Hệ thống công nhận rằng bạn đã thực hiện các kỹ thuật phức tạp (như Brownian Bridge, Randomization) để điền các lỗ hổng `NaN` trong dữ liệu gốc.
    *   Do đó, các module của chúng ta sẽ không thực hiện bất kỳ thao tác `ffill` hay `bfill` nào trên các file CSV gốc. Thao tác `ffill` duy nhất trong `market.py` chỉ nhằm mục đích **đồng bộ hóa các timeline** có tần suất khác nhau (ví dụ: kéo dài dữ liệu CPI hàng tháng ra toàn bộ timeline 15 phút).

---