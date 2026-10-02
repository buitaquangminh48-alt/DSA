# 🚀 Advanced Segment Tree Collections (C++)

Một bộ sưu tập toàn diện về **Segment Tree (Cây phân đoạn)** được cài đặt bằng C++, từ các thuật toán Lập trình thi đấu (Competitive Programming) tối ưu đến các thiết kế Chuẩn phần mềm (Software Engineering).

---

## 📊 Bảng So Sánh Chi Tiết

| # | Loại Segment Tree | Trường hợp sử dụng tiêu biểu | Độ phức tạp Thời gian (Update / Query) | Độ phức tạp Bộ nhớ | Đặc điểm nổi bật |
|---|---|---|---|---|---|
| **01** | **Basic Segment Tree** | Cập nhật 1 điểm, truy vấn đoạn | $O(\log N) / O(\log N)$ | $O(4N)$ | Đơn giản, đệ quy trực quan |
| **02** | **Lazy Propagation** | Cập nhật đoạn, truy vấn đoạn | $O(\log N) / O(\log N)$ | $O(4N)$ | Trì hoãn cập nhật bằng mảng `lazy` |
| **03** | **Iterative Segment Tree** | Tốc độ tối đa, tránh tràn stack | $O(\log N) / O(\log N)$ | $O(2N)$ | Cài đặt vòng lặp Bottom-Up,Cache-friendly |
| **04** | **Persistent Segment Tree** | Lưu lịch sử phiên bản, Undo/Redo | $O(\log N) / O(\log N)$ | $O(N + Q \log N)$ | Path Copying, chia sẻ nút cây cũ |
| **05** | **Dynamic Segment Tree** | Tọa độ cực lớn ($0 \le x \le 10^9$) | $O(\log R) / O(\log R)$ | $O(Q \log R)$ | Khởi tạo nút động (Lazy Allocation) |
| **06** | **Generic Segment Tree** | Tái sử dụng code cho mọi kiểu/phép toán | $O(\log N) / O(\log N)$ | $O(4N)$ | C++ Template + OOP + Lambda Merge |

> **Ghi chú:**
> - $N$: Số lượng phần tử mảng đầu vào.
> - $Q$: Số lượng truy vấn/thao tác cập nhật.
> - $R$: Phạm vi tọa độ (ví dụ: $R = 10^9$).

---

## 📁 Cấu Trúc Mã Nguồn

```text
.
├── 01_segment_tree_basic.cpp          # Point Update & Range Query
├── 02_segment_tree_lazy_propagation.cpp# Range Update & Range Query
├── 03_iterative_segment_tree.cpp      # Non-recursive / Bottom-Up
├── 04_persistent_segment_tree.cpp     # Version History / Path Copying
├── 05_dynamic_segment_tree.cpp        # Node-based Pointer / [0, 10^9]
└── 06_generic_segment_tree.cpp        # C++ Template & Lambda Function