---

## ➕ Chuyên đề Nâng cao: Trọn bộ Prefix Sum & Scan Algorithms (CP & System)

### 📊 Bảng So Sánh Tất Cả Các Thuật Toán Prefix Sum Kinh Điển

| Dạng Cài Đặt | Nhóm | Cơ Chế Thuật Toán | Độ Phức Tạp Truy Vấn / Xử Lý | Ứng Dụng Tiêu Biểu |
| :--- | :---: | :--- | :---: | :--- |
| **2D Prefix Sum** | CP | Bao hàm - Loại trừ (Inclusion-Exclusion) | $\mathcal{O}(1)$ Query | Truy vấn tổng vùng ma trận, tìm hình chữ nhật con tối ưu |
| **Difference Array** | CP | Đảo ngược Prefix Sum ($L \leftarrow +V, R+1 \leftarrow -V$) | $\mathcal{O}(1)$ Update, $\mathcal{O}(N)$ Reconstruct | Thực hiện hàng loạt Range Update trước khi query |
| **Prefix XOR** | CP | Tính chất $X \oplus X = 0$ ($A[L..R] = pref[R] \oplus pref[L-1]$) | $\mathcal{O}(1)$ Query, $\mathcal{O}(N)$ với Map | Đếm đoạn con có tổng XOR bằng $K$ hoặc bằng $0$ |
| **Integral Image** | Production | 2D Prefix Sum áp dụng trên Pixel Image | $\mathcal{O}(1)$ per Pixel | Bộ lọc Box Blur, thuật toán Viola-Jones nhận diện mặt |
| **Radix Sort** | Production | Prefix Sum trên Counting Array để xác định Index | $\mathcal{O}(d \cdot (N + K))$ | Sắp xếp số nguyên/chuỗi siêu tốc không cần so sánh |
| **Parallel Scan (Blelloch)** | Production | Cây nhị phân Up-sweep & Down-sweep trên GPU | $\mathcal{O}(\log N)$ Depth / Step | Tối ưu hóa tính toán song song CUDA, đồ họa 3D Engine |
| **Stream Cipher** | Production | Tích lũy Prefix XOR giữa Key Stream & Ciphertext | $\mathcal{O}(N)$ | Mã hóa dòng dữ liệu nhẹ, bảo mật giao thức thiết bị IoT |