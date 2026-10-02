# 🚀 Masterclass Trie (Prefix Tree) — Từ CP đỉnh cao đến System Production

> **Trọn bộ 7 dạng cài đặt Trie kinh điển nhất** được đóng gói hoàn chỉnh trong một file C++17 duy nhất (`full_trie_masterclass.cpp`), tích hợp hệ thống **Trace "Phép Thuật" in màu ANSI** giúp trực quan hóa từng bước di chuyển trên cây thời gian thực!

---

## 📌 Tổng Quan Kiến Thức

**Trie (Cây tiền tố)** là cấu trúc dữ liệu dạng cây đặc biệt dùng để quản lý chuỗi hoặc tập hợp các bit. Khi **Hash Table** đạt giới hạn ở các bài toán tìm kiếm theo tiền tố hoặc thao tác bitwise, **Trie** xuất hiện như một "vũ khí tối thượng" với độ phức tạp phụ thuộc vào độ dài chuỗi $L$ thay vì số lượng phần tử $N$.

### 📊 Bảng So Sánh 7 Cấu Trúc Trie Kinh Điển

| STT | Tên Cấu Trúc | Nhóm | Cơ Chế Cài Đặt | Độ Phức Tạp | Ứng Dụng Thực Tế / CP |
| :-: | :--- | :---: | :--- | :---: | :--- |
| **1** | **Alphabet String Trie** | CP | Mảng 2D phẳng `trie[MAX_NODES][26]` | $\mathcal{O}(L)$ | Đếm từ, kiểm tra tiền tố tốc độ cao trong CP |
| **2** | **Binary Trie** | CP | Mảng 2D 2 nhánh (`0` & `1`), duyệt bit tham lam | $\mathcal{O}(\text{Bits})$ | Tìm Maximum XOR Pair / Maximum XOR Subarray |
| **3** | **Aho-Corasick Automaton** | CP | Trie + KMP (Failure Links bằng BFS) | $\mathcal{O}(N + M)$ | Tìm kiếm đồng thời hàng nghìn mẫu chuỗi |
| **4** | **Autocomplete Engine** | Production | Cây con trỏ + `unordered_map` + Điểm số Weight | $\mathcal{O}(L + K \log K)$ | Search Suggestion (Google, Shopee, E-commerce) |
| **5** | **Profanity Filter** | Production | Trie từ cấm + Con trỏ trượt quét văn bản | $\mathcal{O}(N \times L)$ | Kiểm duyệt bình luận, Chat filter trong Game |
| **6** | **IP Router (Patricia)** | Production | Binary Trie định tuyến bit tiền tố dài nhất | $\mathcal{O}(\text{Prefix Length})$ | Longest Prefix Matching trong Hardware Router |
| **7** | **Radix Tree (Compressed)** | Production | Nén các nút đơn lẻ thành 1 chuỗi (Split Edge) | $\mathcal{O}(L)$ | Tối ưu RAM trong Redis, File System, Web Router |

---

## 🌟 Tính Năng Nổi Bật File Code Masterclass

- **Single-File Architecture:** Tất cả 7 thuật toán được mô-đun hóa sạch sẽ trong các `namespace` riêng biệt, không xung đột bộ nhớ.
- **Magic Trace System:** Tích hợp bộ ghi log in màu ANSI (`\033[...]`) sinh động trong terminal, giúp bạn thấy rõ:
  - Từng bước tạo nút / duyệt qua các nhánh.
  - Quá trình nhảy bit ngược để tìm Maximum XOR.
  - Quá trình dựng đường liên kết lỗi (**Failure Link**) trong Aho-Corasick.
  - Thao tác **Tách Cạnh (Split Edge)** khi nén chuỗi trong Radix Tree.
  - Quá trình khớp tiền tố dài nhất (**Longest Prefix Match**) khi định tuyến IP.

---

## 🏗️ Chi Tiết 7 Cài Đặt Trong Repo

### 1️⃣ CP: Alphabet String Trie (Flat Array)
Sử dụng mảng phẳng `trie[MAX_NODES][ALPHABET_SIZE]` giúp tối ưu bộ nhớ đệm (Cache Locality) và tránh overhead cấp phát động (`new/delete`), chống TLE/MLE tuyệt đối trong Competitive Programming.

### 2️⃣ CP: Binary Trie (Maximum XOR Pair)
Biến các số nguyên thành dãy bit nhị phân (31-bit / 60-bit). Khi cần tìm số có XOR lớn nhất với $X$, thuật toán duyệt tham lam từ bit cao nhất xuống bit thấp nhất, ưu tiên đi theo nhánh có **bit ngược lại** với bit của $X$.

### 3️⃣ CP: Aho-Corasick Automaton
Kỹ thuật "khủng" nhất khi xử lý chuỗi trong CP. Bằng cách nối các **Failure Link** giữa các nút (tương tự bảng KMP), thuật toán cho phép quét văn bản độ dài $N$ và tìm ra tất cả các mẫu trong tập $M$ chuỗi cho trước chỉ trong 1 lần duyệt duy nhất.

### 4️⃣ Production: Autocomplete & Search Suggestion Engine
Sử dụng `unordered_map` hỗ trợ bảng chữ cái mở rộng (Unicode/UTF-8). Đính kèm độ phổ biến (`weight`/lượt click) tại mỗi từ để lọc ra Top $K$ từ khóa gợi ý hot nhất cho người dùng.

### 5️⃣ Production: Profanity Filter & Content Censorship
Lưu danh sách từ cấm vào Trie. Quét qua câu văn và che đi các từ độc hại bằng ký tự `***`. Xử lý chính xác các từ cấm có độ dài khác nhau đè lên nhau.

### 6️⃣ Production: IP Router (Longest Prefix Matching)
Gói tin IP được duyệt dưới dạng chuỗi Bitstream. Router sẽ tìm đường đi trùng khớp với chuỗi tiền tố dài nhất hiện có trên cây Trie để quyết định đẩy gói tin ra đúng cổng mạng (Port).

### 7️⃣ Production: Compressed Trie / Radix Tree
Giải quyết nhược điểm ngốn RAM của Trie thông thường. Khi một chuỗi các nút chỉ có 1 con duy nhất, Radix Tree sẽ gộp chúng lại thành 1 cạnh duy nhất. Nếu có chuỗi mới chèn vào giữa, cây sẽ tự động thực hiện thao tác **Split Edge (Tách cạnh)**.

---

## 🛠️ Hướng Dẫn Biên Dịch & Chạy Code

### Yêu cầu hệ thống
- Biên dịch C++ hỗ trợ chuẩn **C++17** trở lên (GCC, Clang, MSVC).
- Terminal hỗ trợ **ANSI Color Codes** (Linux Terminal, macOS Terminal, Windows Terminal/PowerShell).

### Lệnh biên dịch & chạy

```bash
# Biên dịch mã nguồn với C++17
g++ -std=c++17 -O2 full_trie_masterclass.cpp -o masterclass

# Chạy chương trình và trải nghiệm Magic Trace!
./masterclass