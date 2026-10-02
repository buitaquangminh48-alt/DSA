---

## 🪟 Chuyên đề Nâng cao: Trọn bộ Sliding Window (CP & Production)

Sliding Window (Cửa sổ trượt) là kỹ thuật chuyển đổi các bài toán xử lý trên mảng/chuỗi từ độ phức tạp $\mathcal{O}(N^2)$ hoặc $\mathcal{O}(N \cdot K)$ về $\mathcal{O}(N)$ tuyến tính bằng cách tái sử dụng kết quả tính toán của các phần tử trùng lặp trong khoảng trượt.

### 📊 Bảng So Sánh Tất Cả Các Dạng Sliding Window Kinh Điển

| Dạng Cài Đặt | Nhóm | Cấu trúc Dữ liệu | Độ phức tạp Thời gian | Ứng dụng Tiêu biểu |
| :--- | :---: | :--- | :---: | :--- |
| **Fixed-size Window** | CP | Mảng tĩnh / 2 Pointers | $\mathcal{O}(N)$ | Tìm tổng/trung bình lớn nhất của K phần tử |
| **Variable-size Window** | CP | Hash Map / 2 Pointers | $\mathcal{O}(N)$ | Minimum Window Substring, Longest Substring |
| **Monotonic Deque (RMQ)** | CP | `std::deque` đơn điệu | $\mathcal{O}(N)$ | Sliding Window Maximum/Minimum (LeetCode 239) |
| **Rate Limiter Log** | Production | `std::queue` + Timestamps | $\mathcal{O}(1)$ Avg / Req | API Gateway, Chống Tấn công DDoS (HTTP 429) |
| **Stream Aggregator** | Production | `std::deque` + Metrics | $\mathcal{O}(1)$ / Event | Prometheus Metrics, Apache Flink Streaming |
| **TCP Sliding Window** | Production | Sequence Buffer | $\mathcal{O}(N)$ | Kiểm soát lưu lượng (Flow Control) mạng Internet |
| **Convolution 2D Matrix** | Production | Ma trận cửa sổ 2D | $\mathcal{O}(R \cdot C \cdot K^2)$ | AI CNN Layer, Bộ lọc làm mờ ảnh (Box Blur) |