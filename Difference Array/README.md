---

## 📉 Chuyên đề Nâng cao: Trọn bộ Difference Array (CP & Production)

Difference Array (Mảng hiệu) là kỹ thuật đối nghịch với Prefix Sum. Nó biến thao tác cập nhật trên một khoảng bối cảnh $\mathcal{O}(N)$ thành thao tác ghi nhận điểm đầu/điểm kết thúc trong $\mathcal{O}(1)$, sau đó quét một lượt Prefix Sum để thu về trạng thái cuối cùng.

### 📊 Bảng So Sánh Tất Cả Các Thuật Toán Difference Array Kinh Điển

| Dạng Cài Đặt | Nhóm | Cơ Chế Thuật Toán | Độ Phức Tạp Update / Reconstruct | Ứng Dụng Tiêu Biểu |
| :--- | :---: | :--- | :---: | :--- |
| **1D / 2D Difference Array** | CP | Bao hàm - loại trừ đảo ngược ($4$ điểm góc cho 2D) | $\mathcal{O}(1)$ Update, $\mathcal{O}(R \cdot C)$ Recon | Cập nhật hình chữ nhật/vùng lưới đồ họa |
| **Arithmetic Range Update** | CP | Mảng hiệu bậc hai (Double Difference Array $D_2$) | $\mathcal{O}(1)$ Update, $\mathcal{O}(N)$ Recon | Cộng cấp số cộng (lực/gia tốc tăng dần theo khoảng cách) |
| **Sparse Difference Array** | CP | Mảng hiệu kết hợp Nén tọa độ (`std::map`) | $\mathcal{O}(\log Q)$ Update | Xử lý cập nhật trên dải tọa độ siêu lớn ($10^9$) |
| **Booking / Capacity Check** | Production | $Start \leftarrow +Req, End + 1 \leftarrow -Req$ | $\mathcal{O}(1)$ Book, $\mathcal{O}(T)$ Scan | Hệ thống kiểm tra tải phòng (Agoda, Airbnb, Flight) |
| **CPU Load Profiler** | Production | Đo tích lũy CPU theo Timeline ($0..T$) | $\mathcal{O}(1)$ Task, $\mathcal{O}(T)$ Scan | Kích hoạt Auto-scaling hạ tầng Cloud (AWS / GCP) |
| **Subtitle Overlap QA** | Production | Đánh dấu $+1 / -1$ tại timestamp milliseconds | $\mathcal{O}(1)$ Sub, $\mathcal{O}(K)$ Scan | Công cụ QA kiểm tra trùng lặp phụ đề video |
| **Delta Encoding** | Production | $Delta[i] = Frame[i] - Frame[i-1]$ | $\mathcal{O}(N)$ Stream | Nén luồng khung hình Video (H.264 / MPEG) |

---

### 💡 So sánh Tổng quan: Prefix Sum vs. Difference Array

* **Prefix Sum (Cộng dồn):** Tập trung vào **ĐỌC (Read-heavy)**. Tiền xử lý $\mathcal{O}(N)$ để trả lời vô số câu hỏi truy vấn tổng đoạn $[L, R]$ trong $\mathcal{O}(1)$.
* **Difference Array (Mảng hiệu):** Tập trung vào **GHI (Write-heavy)**. Cập nhật biến động trên đoạn $[L, R]$ trong $\mathcal{O}(1)$, khôi phục kết quả cuối cùng 1 lần duy nhất trong $\mathcal{O}(N)$.