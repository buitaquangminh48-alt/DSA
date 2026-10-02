---

## ⚡ Chuyên đề Nâng cao: Trọn bộ Sparse Table (CP & Production)

Sparse Table là cấu trúc dữ liệu dựa trên nguyên lý Quy hoạch động và Phân rã Nhị phân (Binary Lifting). Bằng cách tiền xử lý mảng $2D$ kích thước $\mathcal{O}(N \log N)$ trong thời gian $\mathcal{O}(N \log N)$, Sparse Table cho phép trả lời các truy vấn đoạn trên phép toán trùng lặp (Idempotent) chỉ trong **$\mathcal{O}(1)$ thời gian thực**.

### 📊 Bảng So Sánh Tất Cả Các Thuật Toán Sparse Table Kinh Điển

| Dạng Cài Đặt | Nhóm | Cơ Chế Thuật Toán | Độ Phức Tạp Precompute / Query | Ứng Dụng Tiêu Biểu |
| :--- | :---: | :--- | :---: | :--- |
| **RMQ Sparse Table** | CP | Bảng DP $2^k$ ô, phủ 2 cửa sổ giao nhau | $\mathcal{O}(N \log N)$ / $\mathcal{O}(1)$ | Tìm Min/Max đoạn trên mảng tĩnh |
| **Range GCD Query** | CP | Phép toán $\gcd(x, x) = x$ có tính chất Idempotent | $\mathcal{O}(N \log N)$ / $\mathcal{O}(1)$ | Tìm Ước chung lớn nhất trên khoảng tĩnh |
| **Range Sum Query** | CP | Phân rã độ dài $Len$ thành các Bit $1$ của $2^k$ | $\mathcal{O}(N \log N)$ / $\mathcal{O}(\log N)$ | Minh họa cơ chế phân rã nhị phân |
| **LCA via RMQ** | CP | Euler Tour biến cây thành mảng + Tìm Min Depth | $\mathcal{O}(N \log N)$ / $\mathcal{O}(1)$ | Tìm Tổ tiên chung thấp nhất $\mathcal{O}(1)$ |
| **Static Sensor Analytics** | Production | Bảng $2D$ lưu trữ số liệu lịch sử không thay đổi | $\mathcal{O}(N \log N)$ / $\mathcal{O}(1)$ | Hệ thống tra cứu IoT, thời tiết, chứng khoán |
| **Embedded Jump Table** | Production | Nhảy lũy thừa $2^k$ trên chuỗi khối Pointer | $\mathcal{O}(N \log K)$ / $\mathcal{O}(\log K)$ | Tra cứu khối dữ liệu ROM/Firmware tiết kiệm PIN |
| **LCP Search Indexer** | Production | Suffix Array + RMQ Sparse Table trên mảng LCP | $\mathcal{O}(N \log N)$ / $\mathcal{O}(1)$ | Bộ máy so khớp chuỗi DNA, Tìm kiếm văn bản lớn |

---

### 💡 So sánh Bộ Ba Cấu Trúc Dữ Liệu Đoạn (Range Queries)

| Tiêu Chí | Prefix Sum / Difference Array | Fenwick Tree (BIT) | Segment Tree | Sparse Table |
| :--- | :---: | :---: | :---: | :---: |
| **Thời gian Cập nhật** | $\mathcal{O}(1)$ (đoạn) / $\mathcal{O}(N)$ | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ | ❌ **Không hỗ trợ (Tĩnh)** |
| **Thời gian Truy vấn** | $\mathcal{O}(1)$ | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ | **$\mathcal{O}(1)$** (với Idempotent) |
| **Bộ nhớ (Space)** | $1 \times N$ | $1 \times N$ | $4 \times N$ | **$N \log N$** |
| **Trường hợp Tối ưu** | Cập nhật đoạn 1 lần, đọc cuối | Cập nhật điểm/đoạn động, code ngắn | Cập nhật & Truy vấn động phức tạp | Mảng cố định, đọc nhiều ($\mathcal{O}(1)$) |