# 🌳 Masterclass Binary Tree Data Structure — Từ Thuật Toán CP Đến Kiến Trúc Production

> **Trọn bộ 9 cấu trúc & thuật toán Cây Nhị Phân kinh điển** được đóng gói hoàn chỉnh trong một file C++17 duy nhất (`full_binary_tree_masterclass.cpp`), tích hợp hệ thống **Trace "Phép Thuật" in màu ANSI** giúp trực quan hóa cơ chế chuyển con trỏ, phép quay cây (Rotations), biến đổi Euler Tour, phân tích AST, và nén bit Huffman thời gian thực.

---

## 📌 Tổng Quan Kiến Thức

Cây Nhị Phân (Binary Tree) là cấu trúc dữ liệu phân cấp trong đó mỗi nút có tối đa 2 nút con.
- Trong **Competitive Programming (CP)**: Trọng tâm nằm ở khả năng tối ưu bộ nhớ phụ (**Morris Traversal $O(1)$ memory**), cân bằng động bằng xác suất ngẫu nhiên (**Treap**), tối ưu hóa tính locality (**Splay Tree**), và kỹ thuật trải phẳng cây (**Euler Tour**) để biến truy vấn cây con thành truy vấn đoạn trên Segment Tree/Fenwick Tree.
- Trong **Production / System**: Cây nhị phân đóng vai trò làm khung xương cho trình biên dịch (**Abstract Syntax Tree - AST**), thuật toán nén file tối ưu (**Huffman Coding**), indexing chỉ mục cơ sở dữ liệu (**B+ Tree**), và cấu trúc lưu trữ dữ liệu an toàn đa luồng (**Thread-Safe Concurrent BST**).

---

## 📊 Bảng So Sánh 9 Cấu Trúc & Thuật Toán Cây Nhị Phân Kinh Điển

| STT | Cấu Trúc / Thuật Toán | Nhóm | Cơ Chế Cài Đặt / Đặc Điểm | Độ Phức Tạp (Avg / Worst) | Ứng Dụng Thực Tế / CP |
| :-: | :--- | :---: | :--- | :---: | :--- |
| **1** | **Iterative Traversals** | CP / Algo | Dùng Stack thủ công thay thế Call Stack đệ quy | $O(N) / O(N)$ | Tránh lỗi Stack Overflow khi cây có độ sâu lớn |
| **2** | **Morris Traversal** | CP / Algo | Dùng Thread Pointer nối tạm về In-order Predecessor | $O(N) / O(N)$ [Memory $O(1)$] | Duyệt cây In-order với bộ nhớ phụ $O(1)$ tuyệt đối |
| **3** | **Treap** | CP | Kết hợp thuộc tính BST (Key) & Heap (Priority ngẫu nhiên) | $O(\log N) / O(\log N)$ | Cấu trúc dữ liệu động cực mạnh giải bài toán CP |
| **4** | **Splay Tree** | CP | Dùng phép quay Zig/Zag đẩy nút vừa truy cập lên Gốc | $O(\log N)$ Amortized | Tối ưu tuyệt vời cho dữ liệu có tính chất truy xuất lặp |
| **5** | **Euler Tour Flattening** | CP | Lưu `in_time` và `out_time` khi DFS qua cây | $O(N)$ tạo mảng | Chuyển Subtree Query thành Range Query trên Segment Tree |
| **6** | **Binary Expression Tree** | Production | Nút lá là Toán hạng, Nút cha là Toán tử | $O(N)$ đánh giá | Abstract Syntax Tree (AST) trong Compilers & Interpreters |
| **7** | **Huffman Coding Tree** | Production | Cấu trúc cây dựa trên Tần suất xuất hiện ký tự | $O(N \log N)$ | Thuật toán nén file nền tảng (ZIP, JPEG, MP3) |
| **8** | **B+ Tree Indexing** | Production | Cây tìm kiếm nhiều nhánh (Bậc M) trỏ đến Leaf Pages | $O(\log_M N)$ | Cơ chế Indexing trong MySQL (InnoDB) & PostgreSQL |
| **9** | **Thread-Safe BST** | Production | Khóa Cục bộ (Fine-Grained Mutex/Locking) tại nút | $O(\log N) / O(N)$ | Hệ thống ghi/đọc dữ liệu cây đa luồng đồng thời |

---

## 🛠️ Hướng Dẫn Biên Dịch & Trải Nghiệm

```bash
# Biên dịch mã nguồn với chuẩn C++17 và tối ưu hóa O2
g++ -std=c++17 -O2 full_binary_tree_masterclass.cpp -o tree_master

# Chạy chương trình và xem TRACE phép thuật!
./tree_master