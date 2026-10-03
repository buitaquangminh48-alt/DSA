# 📚 Masterclass Stack Data Structure — Từ CP Đỉnh Cao Đến System Production

> **Trọn bộ 7 dạng cài đặt Stack kinh điển nhất** được đóng gói hoàn chỉnh trong một file C++17 duy nhất (`full_stack_masterclass.cpp`), tích hợp hệ thống **Trace "Phép Thuật" in màu ANSI** giúp trực quan hóa từng thao tác Push/Pop, biến đổi trạng thái và xử lý con trỏ thời gian thực!

---

## 📌 Tổng Quan Kiến Thức

**Stack (Ngăn xếp)** tuân theo nguyên lý **LIFO (Last-In, First-Out)** — phần tử vào sau cùng sẽ là phần tử được lấy ra đầu tiên.
- Trong **Competitive Programming (CP)**: Stack biến hóa thành **Monotonic Stack** giúp triệt tiêu các vòng lặp lồng nhau, tối ưu độ phức tạp từ $\mathcal{O}(N^2)$ xuống $\mathcal{O}(N)$, hoặc dùng làm bộ đệm khử đệ quy chống tràn bộ nhớ (**Stack Overflow**).
- Trong **Production / System**: Stack là xương sống xử lý các luồng hoàn tác (**Undo/Redo**), phân tích cú pháp mã nguồn (**Linter/Parser**), quản lý điều hướng trình duyệt và quản lý luồng thực thi hàm (**Call Stack Frame**).

---

## 📊 Bảng So Sánh 7 Cấu Trúc Stack Kinh Điển

| STT | Tên Cấu Trúc / Thuật Toán | Nhóm | Cơ Chế Cài Đặt | Độ Phức Tạp | Ứng Dụng Thực Tế / CP |
| :-: | :--- | :---: | :--- | :---: | :--- |
| **1** | **Monotonic Stack** | CP | Duy trì Stack tăng/giảm đơn điệu bằng cách Pop vi phạm | $\mathcal{O}(N)$ | Next Greater Element, Largest Rectangle Histogram |
| **2** | **Shunting-Yard (Dijkstra)** | CP | Stack lưu toán tử + Queue/String lưu Hậu Tố (Postfix) | $\mathcal{O}(N)$ | Expression Evaluator, Trình biên dịch toán học |
| **3** | **Iterative DFS** | CP | Thay đệ quy bằng Stack thủ công trên Heap | $\mathcal{O}(V + E)$ | Duyệt cây/đồ thị độ sâu $10^6$ tránh Stack Overflow |
| **4** | **Undo / Redo Manager** | Production | Dual-Stack (`undo_stack` & `redo_stack`) | $\mathcal{O}(1)$ | Ctrl+Z / Ctrl+Y trong VS Code, Word, Photoshop |
| **5** | **Syntax Validator** | Production | Push thẻ mở, Pop & Match khi gặp thẻ đóng | $\mathcal{O}(N)$ | Linter, Web Browser DOM Parser, JSON Validator |
| **6** | **Browser History** | Production | Dual-Stack (`back_stack` & `forward_stack`) | $\mathcal{O}(1)$ | Điều hướng Back/Forward trên Chrome, Firefox |
| **7** | **Call Stack Tracker** | Production | Stack chứa các `Frame` (Tên hàm, Dòng lệnh, Biến) | $\mathcal{O}(1)$ | Debugger, Runtime Interpreter, Exception Handling |

---

## 🛠️ Hướng Dẫn Biên Dịch & Trải Nghiệm

### Lệnh biên dịch & chạy trên Terminal

```bash
# Biên dịch mã nguồn với chuẩn C++17
g++ -std=c++17 -O2 full_stack_masterclass.cpp -o stack_master

# Chạy chương trình và xem TRACE phép thuật!
./stack_master