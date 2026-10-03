# 🗝️ Masterclass Hash Map Data Structure — Từ CP Đỉnh Cao Đến System Production

> **Trọn bộ 7 dạng cài đặt Hash Map kinh điển nhất** được đóng gói hoàn chỉnh trong một file C++17 duy nhất (`full_hashmap_masterclass.cpp`), tích hợp hệ thống **Trace "Phép Thuật" in màu ANSI** giúp trực quan hóa cơ chế giải quyết đụng độ (Collision Resolution), Rehashing, Lock Striping, TTL Expiration, và hoán đổi Robin Hood trong thời gian thực!

---

## 📌 Tổng Quan Kiến Thức

**Hash Map (Bảng băm)** vận hành bằng cách ánh xạ các khóa (Keys) thành chỉ số mảng thông qua một **Hàm băm (Hash Function)** nhằm đạt tốc độ truy xuất $O(1)$.
- Trong **Competitive Programming (CP)**: Mảng tĩnh kết hợp **Linear Probing** tận dụng tối đa CPU Cache Locality. Để tránh các bộ test chống hash cố tình ép $O(1) \rightarrow O(N)$ gây TLE, kỹ thuật **Custom Hash (SplitMix64 + Time Seed)** và **`gp_hash_table` (PBDS)** là những công cụ tối thượng.
- Trong **Production / System**: Hash Map tập trung vào khả năng mở rộng **Dynamic Rehashing**, an toàn đa luồng hiệu năng cao (**Lock Striping Concurrent Map**), cơ chế tự hủy phiên làm việc (**TTL Map**), và phân bổ dữ liệu tối ưu worst-case (**Robin Hood Hashing**).

---

## 📊 Bảng So Sánh 7 Cấu Trúc Hash Map Kinh Điển

| STT | Tên Cấu Trúc / Thuật Toán | Nhóm | Cơ Chế Cài Đặt | Độ Phức Tạp (Avg / Worst) | Ứng Dụng Thực Tế / CP |
| :-: | :--- | :---: | :--- | :---: | :--- |
| **1** | **Static Linear Probing** | CP | Mảng tĩnh song song `keys[]`/`values[]` + Probing | $O(1) / O(N)$ | Tốc độ thuần túy, tối ưu Cache Locality tuyệt đối |
| **2** | **Custom Anti-Hack Hash** | CP | Hàm `SplitMix64` xáo trộn bit + Time-based Seed | $O(1) / O(1)$ | Kháng toàn bộ bộ test ép TLE trên Codeforces/LeetCode |
| **3** | **gp_hash_table** | CP | Mở rộng `__gnu_pbds` chuẩn của C++ GCC | $O(1) / O(1)$ | Nhanh gấp 3-5 lần `std::unordered_map` mặc định |
| **4** | **Dynamic Chaining Map** | Production | Chaining List + Automatic Rehashing (Load Factor 0.75) | $O(1) / O(N)$ | Mô phỏng cơ chế của Java `HashMap` & Python `dict` |
| **5** | **Concurrent Hash Map** | Production | Chia Map thành N Segments với Read-Write Locks độc lập | $O(1) / O(N)$ | Xử lý đa luồng đồng thời (Mô phỏng Java `ConcurrentHashMap`) |
| **6** | **TTL Hash Map** | Production | Đính kèm Timestamp hằng số + Tự hủy khi hết hạn | $O(1) / O(N)$ | Lưu Session Login, Cache tạm thời (Redis thu nhỏ) |
| **7** | **Robin Hood Hash Map** | Production | Open Addressing + Cướp chỗ dựa trên độ xa gốc (PSL) | $O(1) / O(\log N)$ | Giảm thiểu biến thiên độ dài tìm kiếm (Dùng trong Rust/C++23) |

---

## 🛠️ Hướng Dẫn Biên Dịch & Trải Nghiệm

### Lệnh biên dịch & chạy trên Terminal

```bash
# Biên dịch mã nguồn với chuẩn C++17 và tối ưu hóa O2
g++ -std=c++17 -O2 full_hashmap_masterclass.cpp -o hashmap_master

# Chạy chương trình và xem TRACE phép thuật!
./hashmap_master