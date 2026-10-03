# 🔄 Masterclass Queue Data Structure — Từ CP Đỉnh Cao Đến System Production

> **Trọn bộ 8 dạng cài đặt Queue kinh điển nhất** được đóng gói hoàn chỉnh trong một file C++17 duy nhất (`full_queue_masterclass.cpp`), tích hợp hệ thống **Trace "Phép Thuật" in màu ANSI** giúp trực quan hóa luồng dữ liệu FIFO, di chuyển chỉ số vòng và cập nhật độ ưu tiên thời gian thực!

---

## 📌 Tổng Quan Kiến Thức

**Queue (Hàng đợi)** vận hành theo nguyên lý **FIFO (First-In, First-Out)** — phần tử vào trước sẽ được xử lý trước.
- Trong **Competitive Programming (CP)**: Queue phối hợp với các thuật toán duyệt đồ thị theo tầng (**BFS**, **0-1 BFS**) hoặc biến đổi thành **Monotonic Queue (Deque)** để xử lý bài toán cửa sổ trượt (Sliding Window) trong thời gian tuyến tính $\mathcal{O}(N)$.
- Trong **Production / System**: Queue là thành phần nền tảng của các kiến trúc hệ thống phân tán, xử lý bất đồng bộ (**Message Queue**, **LRU Cache**), tiết kiệm bộ nhớ cho phần cứng (**Ring Buffer**) và bộ điều phối tiến trình hệ điều hành (**Priority Task Scheduler**).

---

## 📊 Bảng So Sánh 8 Cấu Trúc Queue Kinh Điển

| STT | Tên Cấu Trúc / Thuật Toán | Nhóm | Cơ Chế Cài Đặt | Độ Phức Tạp | Ứng Dụng Thực Tế / CP |
| :-: | :--- | :---: | :--- | :---: | :--- |
| **1** | **Standard BFS** | CP | `std::queue` duyệt theo từng lớp (loang) | $\mathcal{O}(V + E)$ | Tìm đường đi ngắn nhất đồ thị không trọng số |
| **2** | **0-1 BFS** | CP | `std::deque` (Push Front nếu weight 0, Push Back nếu weight 1) | $\mathcal{O}(V + E)$ | Tối ưu đường đi ngắn nhất thay thế Dijkstra |
| **3** | **Monotonic Queue** | CP | `std::deque` duy trì dãy giá trị tăng/giảm đơn điệu | $\mathcal{O}(N)$ | Sliding Window Maximum/Minimum trong mảng |
| **4** | **Queue Using 2 Stacks** | CP | 2 Stack (`in_stack` & `out_stack`) hỗ trợ Amortized $\mathcal{O}(1)$ | $\mathcal{O}(1)^*$ | Bài toán tư duy biến đổi cấu trúc LIFO ➔ FIFO |
| **5** | **LRU Cache** | Production | Hash Map + Doubly Linked List / Deque | $\mathcal{O}(1)$ | Bộ nhớ đệm hiệu năng cao Redis, Memcached, Browser |
| **6** | **Message Queue** | Production | Thread-safe Queue mô hình Producer - Consumer | $\mathcal{O}(1)$ | Xử lý background job (BullMQ, Celery, Kafka) |
| **7** | **Ring Buffer (Circular)** | Production | Mảng cố định tận dụng phép chia lấy dư `%` | $\mathcal{O}(1)$ | Hệ thống nhúng (Embedded), Audio/Video Stream |
| **8** | **Priority Task Scheduler**| Production | Heap (`std::priority_queue`) sắp xếp theo Priority | $\mathcal{O}(\log N)$ | OS Process Scheduler, Email Queue khẩn cấp |

---

## 🛠️ Hướng Dẫn Biên Dịch & Trải Nghiệm

### Lệnh biên dịch & chạy trên Terminal

```bash
# Biên dịch mã nguồn với chuẩn C++17
g++ -std=c++17 -O2 full_queue_masterclass.cpp -o queue_master

# Chạy chương trình và xem TRACE phép thuật!
./queue_master