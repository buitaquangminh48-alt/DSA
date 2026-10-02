# 🚀 Advanced Hash Algorithms Template (CP & System Design)

Bộ sưu tập mã nguồn C++17 hoàn chỉnh, tối ưu và chuẩn mực cho các thuật toán băm (**Hash Algorithms**) kinh điển. Dự án phục vụ hai mục đích chính: làm **Template thi đấu Lập trình Thi đấu (Competitive Programming)** và làm **Tài liệu trực quan cho System Design / Production Systems**.

---

## 📌 Danh Mục Thuật Toán

### 1. Nhóm Kinh Điển cho CP (Competitive Programming)

Thế giới của Hash trong CP giúp giảm độ phức tạp thời gian xử lý xuống $\mathcal{O}(1)$ bằng cách chấp nhận tỷ lệ va chạm cực kỳ nhỏ.

*   **`CustomHash` (SplitMix64):** 
    *   *Mục đích:* Tùy biến hàm băm cho `std::unordered_map` / `std::unordered_set`.
    *   *Ứng dụng:* Né hoàn toàn các bài test phá hoại (Anti-Hash Test / Hack Test) khiến độ phức tạp bị đẩy từ $\mathcal{O}(1)$ lên $\mathcal{O}(N)$ (gây lỗi Time Limit Exceeded).
*   **`DoubleHashTable` (Băm đôi xử lý va chạm):**
    *   *Mục đích:* Cấu trúc Bảng băm tùy chỉnh sử dụng chiến lược Open Addressing với 2 hàm băm độc lập $h_1(k)$ và $h_2(k)$.
    *   *Công thức dò:* $\text{Index}_i = (h_1(k) + i \cdot h_2(k)) \bmod \text{TABLE\_SIZE}$
    *   *Ứng dụng:* Xử lý va chạm tối ưu, loại bỏ hiện tượng tích tụ cụm (Primary & Secondary Clustering).
*   **`DoubleRollingHash` (Polynomial Rolling Hash đôi):**
    *   *Mục đích:* Băm chuỗi sử dụng 2 cặp $(Base_1, Mod_1)$ và $(Base_2, Mod_2)$ song song.
    *   *Ứng dụng:* So sánh hai chuỗi con bất kỳ trong $\mathcal{O}(1)$ sau khi tiền xử lý $\mathcal{O}(N)$ (Rabin-Karp), triệt tiêu $99.9999\%$ nguy cơ trùng mã băm (WA).
*   **`Tree Hashing` (Băm cây):**
    *   *Mục đích:* Tính toán giá trị băm đại diện cho cấu trúc đồ thị cây độc lập với thứ tự các đỉnh con.
    *   *Ứng dụng:* Kiểm tra hai cây có đồng cấu với nhau hay không (Tree Isomorphism) trong $\mathcal{O}(V \log V)$.

---

### 2. Nhóm Kinh Điển cho Hệ Thống & Thực Tế (Production / System Design)

Hash là "xương sống" cho các hệ thống phân tán, cơ sở dữ liệu lớn và an toàn thông tin.

*   **`Consistent Hashing` (Băm nhất quán):**
    *   *Mục đích:* Định vị dữ liệu/request trên một Vòng tròn Băm (Hash Ring) kèm khái niệm Virtual Nodes.
    *   *Ứng dụng:* Load Balancers, Distributed Caching (Redis Cluster, Memcached), CDNs. Tối thiểu hóa số lượng key phải re-map khi thêm/bớt Server.
*   **`Bloom Filter` (Bộ lọc Bloom):**
    *   *Mục đích:* Cấu trúc dữ liệu xác suất (Probabilistic Data Structure) cực kỳ tiết kiệm bộ nhớ.
    *   *Ứng dụng:* Kiểm tra nhanh sự tồn tại của phần tử. Bảo đảm không bị False Negative (Nói KHÔNG là chắc chắn KHÔNG), giúp tránh các truy vấn đắt đỏ xuống DB hoặc Disk API.
*   **`Merkle Tree` (Cây Hash):**
    *   *Mục đích:* Cấu trúc cây băm phân cấp xác thực tính toàn vẹn của tập dữ liệu lớn.
    *   *Ứng dụng:* Blockchain (Bitcoin, Ethereum), Git Version Control, Peer-to-Peer Networks (BitTorrent).
*   **`Locality-Sensitive Hashing` (LSH):**
    *   *Mục đích:* Hàm băm bảo toàn khoảng cách/đặc trưng tương đồng (ngược lại với Hash thông thường).
    *   *Ứng dụng:* Reverse Image Search, phát hiện trùng lặp văn bản/đạo văn, Shazam (Music Matching).

---

## 🛠️ Hướng Dẫn Biên Dịch & Chạy thử

Dự án viết bằng C++17 tiêu chuẩn, không phụ thuộc vào thư viện ngoài.

### 1. Biên dịch
Sử dụng `g++` hoặc bất kỳ trình biên dịch C++ nào hỗ trợ C++17 trở lên:

```bash
g++ -std=c++17 -O2 hash_advanced_all.cpp -o hash_demo