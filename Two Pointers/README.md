---

## 👈👉 Chuyên đề Nâng cao: Trọn bộ Two Pointers (CP & Production)

Two Pointers (Hai con trỏ) là kỹ thuật sử dụng hai con trỏ trượt hoặc duyệt song song để duyệt qua tập dữ liệu nhằm loại bỏ các bước duyệt thừa, giảm độ phức tạp từ $\mathcal{O}(N^2)$ xuống $\mathcal{O}(N)$ hoặc xử lý In-place với $\mathcal{O}(1)$ phụ trợ bộ nhớ.

### 📊 Bảng So Sánh Tất Cả Các Dạng Two Pointers Kinh Điển

| Dạng Cài Đặt | Nhóm | Mô Hình / Cơ Chế | Độ phức tạp Thời gian | Ứng dụng Tiêu biểu |
| :--- | :---: | :--- | :---: | :--- |
| **Two Sum / 3Sum** | CP | Hai đầu hội tụ (Opposite) | $\mathcal{O}(N^2)$ (cho 3Sum) | Tìm bộ số có tổng bằng $K$ trên mảng sorted |
| **Floyd's Cycle Detection** | CP | Rùa & Thỏ (Same Direction) | $\mathcal{O}(N)$ Time, $\mathcal{O}(1)$ Space | Phát hiện chu trình & điểm bắt đầu trên Linked List |
| **Merge Sorted Arrays** | CP | Two Arrays Merge | $\mathcal{O}(N + M)$ | Hợp nhất mảng, cốt lõi thuật toán Merge Sort |
| **In-place Deduplication** | Read & Write Pointers | $\mathcal{O}(N)$ Time, $\mathcal{O}(1)$ Space | Dọn dẹp ký tự rác, lọc trùng log file trực tiếp trên RAM |
| **Sort-Merge Join** | Production | Two Arrays / Database Join | $\mathcal{O}(N \log N + M \log M)$ | Tối ưu hóa truy vấn JOIN giữa 2 bảng lớn trong SQL Engine |
| **Run-Length Encoding (RLE)**| Production | Fast & Slow Block Pointer | $\mathcal{O}(N)$ | Nén dữ liệu ảnh BMP, giảm băng thông mạng tốc độ cao |
| **Container With Most Water**| Production | Two-way Shrinking | $\mathcal{O}(N)$ | Tối ưu diện tích mặt phẳng, phân tích dữ liệu Histogram |

---

### 💡 So sánh Tư duy: Two Pointers vs. Sliding Window

* **Sliding Window:** Hai con trỏ tạo thành một **khoảng liên tục (Window)** để duy trì tổng, max/min, hoặc đếm tần suất của một subsegment trên chuỗi/mảng.
* **Two Pointers:** Hai con trỏ hoạt động **độc lập hơn** (chạy ngược chiều để thu hẹp khoảng tìm kiếm, hoặc nhảy tốc độ khác nhau để tìm cycle, hoặc làm nhiệm vụ Đọc/Ghi dữ liệu).