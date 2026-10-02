---

## 🌳 Chuyên đề Nâng cao: Trọn bộ Fenwick Tree / Binary Indexed Tree (CP & Production)

Fenwick Tree (BIT) là cấu trúc dữ liệu biểu diễn cây ẩn dưới dạng mảng 1D thông qua phép toán Bitwise `i & (-i)` (Least Significant Bit - LSB). Nó cho phép cập nhật phần tử và tính tổng tiền tố trong $\mathcal{O}(\log N)$ thời gian thực với hằng số ẩn (hidden constant) cực kỳ nhỏ.

### 📊 Bảng So Sánh Tất Cả Các Thuật Toán Fenwick Tree Kinh Điển

| Dạng Cài Đặt | Nhóm | Cơ Chế Thuật Toán | Độ Phức Tạp Update / Query | Ứng Dụng Tiêu Biểu |
| :--- | :---: | :--- | :---: | :--- |
| **PURQ Fenwick** | CP | Thao tác trên cây bit LSB `i += i & (-i)` | $\mathcal{O}(\log N)$ / $\mathcal{O}(\log N)$ | Truy vấn tổng đoạn linh hoạt trên mảng động |
| **RUPQ Fenwick** | CP | Kết hợp Mảng hiệu (Difference Array) | $\mathcal{O}(\log N)$ / $\mathcal{O}(\log N)$ | Cập nhật đoạn, truy vấn giá trị tại 1 điểm |
| **RURQ Fenwick** | CP | 2 Cây Fenwick song song ($i \cdot B_1[i] - B_2[i]$) | $\mathcal{O}(\log N)$ / $\mathcal{O}(\log N)$ | Cập nhật đoạn VÀ Truy vấn tổng đoạn (Thay thế Lazy SegTree đơn giản) |
| **2D Fenwick Tree** | CP | Vòng lặp Bit lồng nhau ($i$ và $j$) | $\mathcal{O}(\log R \cdot \log C)$ | Truy vấn tổng vùng chữ nhật trên ma trận động |
| **Inversion Count** | CP | Nén tọa độ + Điểm danh tần suất ngược | $\mathcal{O}(N \log N)$ | Đếm số cặp nghịch thế, tính số bước Swap ít nhất |
| **Binary Lifting BIT** | CP | Nhảy bit trên các lũy thừa của 2 | $\mathcal{O}(\log N)$ Search | Tìm vị trí có tổng tích lũy $\ge K$ trong $\mathcal{O}(\log N)$ thay vì $\mathcal{O}(\log^2 N)$ |
| **Dynamic Leaderboard** | Production | BIT đếm tần suất điểm số (Frequency BIT) | $\mathcal{O}(\log S)$ | Xếp hạng Real-time & Bách phân vị (Percentile) trong Game/HR |
| **Running CDF Tracker** | Production | Tích lũy tần suất mẫu vào Bucket BIT | $\mathcal{O}(\log B)$ | Phân tích phân phối xác suất luồng dữ liệu tài chính/mạng |
| **Columnar Frequency Index** | Production | In-memory Dynamic Indexing trên RAM | $\mathcal{O}(\log D)$ | Tối ưu hóa đếm bản ghi trong Columnar DB (ClickHouse, DuckDB) |

---

### 💡 So sánh Kiến trúc: Fenwick Tree vs. Segment Tree

* **Bộ nhớ:** Fenwick Tree chỉ tốn $1 \times N$ bộ nhớ (mảng tĩnh), trong khi Segment Tree cần $4 \times N$ bộ nhớ và cấu trúc con trỏ/đệ quy.
* **Tốc độ:** Fenwick Tree nhanh hơn Segment Tree từ **2 - 4 lần** nhờ tận dụng tốt CPU Cache (Cache Locality) và không tốn chi phí gọi hàm đệ quy.
* **Phạm vi ứng dụng:** 
  * dùng **Fenwick Tree** khi bài toán chỉ liên quan đến các phép toán có tính chất nhóm nghịch đảo (như phép cộng `+`, XOR `^`).
  * dùng **Segment Tree** khi bài toán yêu cầu các phép toán không có tính chất nghịch đảo (như `Min`, `Max`, `GCD`, hoặc các cập nhật Lazy phức tạp).