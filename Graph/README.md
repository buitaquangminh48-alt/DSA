# 🚀 Ultimate Graph Algorithms Library (C++17)

Một thư viện mã nguồn C++17 hoàn chỉnh, tổng hợp toàn bộ các thuật toán đồ thị từ **Cơ bản (Nhóm 1)** đến **Nâng cao & Ứng dụng thực tế (Nhóm 2)**. Code được thiết kế sạch vẽ, tối ưu và đi kèm **Log Trace từng bước trực quan** giúp dễ dàng theo dõi và học tập.

---

## 📌 Danh mục các thuật toán

### 🔹 NHÓM 1: Thuật toán Cơ bản & Kinh điển (Fundamental Graph Algorithms)
Nhóm các thuật toán nền tảng về duyệt đồ thị, tìm đường đi ngắn nhất cơ bản, cây khung tối thiểu và luồng cực đại.

| STT | Thuật toán | Mục đích / Ứng dụng | Độ phức tạp |
| :---: | :--- | :--- | :---: |
| **1** | **BFS & DFS** | Duyệt đồ thị, tìm thành phần liên thông, kiểm tra đường đi | $\mathcal{O}(V + E)$ |
| **2** | **Dijkstra** | Đường đi ngắn nhất từ 1 đỉnh (trọng số không âm) | $\mathcal{O}((V + E) \log V)$ |
| **3** | **Bellman-Ford** | Đường đi ngắn nhất (chấp nhận trọng số âm, phát hiện chu trình âm) | $\mathcal{O}(V \cdot E)$ |
| **4** | **Floyd-Warshall** | Đường đi ngắn nhất giữa mọi cặp đỉnh | $\mathcal{O}(V^3)$ |
| **5** | **Kruskal's Algorithm** | Cây khung nhỏ nhất (MST) dựa trên Tập hợp rời rạc (DSU) | $\mathcal{O}(E \log E)$ |
| **6** | **Prim's Algorithm** | Cây khung nhỏ nhất (MST) phát triển từ 1 đỉnh | $\mathcal{O}(E \log V)$ |
| **7** | **Ford-Fulkerson (Edmonds-Karp)** | Tìm luồng cực đại (Max Flow) trên mạng luồng | $\mathcal{O}(V \cdot E^2)$ |

---

### 🔸 NHÓM 2: Thuật toán Nâng cao & Ứng dụng Thực tế (Advanced & Industry-Level Algorithms)
Nhóm các thuật toán dùng trong phân tích Web, định tuyến GPS, phân tích Mạng xã hội và Lập lịch phụ thuộc.

| STT | Thuật toán | Mục đích / Ứng dụng | Độ phức tạp |
| :---: | :--- | :--- | :---: |
| **8** | **PageRank** | Đánh giá độ uy tín trang web / Hệ thống gợi ý (Recommendation) | $\mathcal{O}(K \cdot (V + E))$ |
| **9** | **HITS** (Hubs & Authorities) | Phân tích liên kết Web theo vai trò Điều hướng (Hub) & Nội dung (Authority) | $\mathcal{O}(K \cdot (V + E))$ |
| **10** | **A\* Search** | Định tuyến GPS / AI Pathfinding tối ưu bằng hàm Heuristic | $\mathcal{O}(E \log V)$ |
| **11** | **Kahn's Topological Sort** | Sắp xếp thứ tự công việc, giải quyết phụ thuộc vòng (Circular Dependency) | $\mathcal{O}(V + E)$ |
| **12** | **Tarjan's SCC** | Tìm các Thành phần liên thông mạnh trong đồ thị có hướng | $\mathcal{O}(V + E)$ |
| **13** | **Louvain Method** | Phân cụm đồ thị mạng xã hội dựa trên tối ưu Modularity ($Q$) | $\mathcal{O}(V \log V)$ |
| **14** | **Contraction Hierarchies (CH)** | Kỹ thuật co đỉnh tạo đường tắt (Shortcut Edges) siêu tốc cho bản đồ | Preprocess: $\mathcal{O}(V \log V + E)$ |

---

## 🛠️ Yêu cầu hệ thống & Biên dịch

### Yêu cầu:
* **Trình biên dịch:** `g++` hoặc `clang++` hỗ trợ chuẩn **C++17** trở lên.

### Lệnh biên dịch & Chạy mã nguồn:

```bash
# 1. Clone repository về máy
git clone [https://github.com/USERNAME/REPO_NAME.git](https://github.com/USERNAME/REPO_NAME.git)
cd REPO_NAME

# 2. Biên dịch mã nguồn
g++ -std=c++17 -O2 main.cpp -o graph_library

# 3. Chạy chương trình demo
./graph_library