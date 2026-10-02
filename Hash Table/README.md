### 📊 Bảng So Sánh Các Kỹ Thuật Hash Table

| Cấu trúc Hash Table | Nhóm | Đặc điểm nổi bật | Độ phức tạp Lookup (Worst Case) | Ứng dụng tiêu biểu |
| :--- | :---: | :--- | :---: | :--- |
| **Static Linear Probing** | CP | Mảng tĩnh phẳng, code ~15 dòng, không cấp phát động | $\mathcal{O}(N)$ | Thi đấu CP (Tối ưu tốc độ) |
| **Anti-Hack Custom Map** | CP | Trộn Time-based Seed với SplitMix64 | $\mathcal{O}(1)$ Avg | Khắc phục TLE bài test độc hại |
| **Dynamic Chaining** | Production | Tự động nhân đôi Capacity khi Load Factor > 0.75 | $\mathcal{O}(N)$ | C++ `std::unordered_map`, Java `HashMap` |
| **Robin Hood Hashing** | Production | Chớp vị trí của phần tử ít nghèo hơn (PSL nhỏ hơn) | $\mathcal{O}(\log N)$ | Rust `std::collections::HashMap` |
| **Cuckoo Hashing** | Production | Dùng 2 bảng băm, đá phần tử trùng văng sang bảng kia | $\mathcal{O}(1)$ Tuyệt đối | Cấu trúc dữ liệu phần cứng, Networking Router |
| **Concurrent Hash Map** | Production | Chia bảng thành nhiều Segment, Lock Striping từng vùng | $\mathcal{O}(1)$ Avg | Java `ConcurrentHashMap`, Hệ thống Đa luồng |