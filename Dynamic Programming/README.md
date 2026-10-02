---

## 🧬 Chuyên đề Nâng cao: Trọn bộ Dynamic Programming (CP & Production)

Dynamic Programming (Quy hoạch động) là kỹ thuật tối ưu hóa bài toán bằng cách chia nhỏ thành các bài toán con overlapping (chồng chéo) và lưu trữ kết quả để tái sử dụng.

### 📊 Bảng So Sánh Tất Cả Các Dạng DP Kinh Điển

| Dạng Cài Đặt | Nhóm | Kỹ thuật Tối ưu | Độ phức tạp Thời gian | Ứng dụng Tiêu biểu |
| :--- | :---: | :--- | :---: | :--- |
| **LIS (Optimized)** | CP | Binary Search (`lower_bound`) | $\mathcal{O}(N \log N)$ | Tìm dãy con tăng dài nhất |
| **0-1 Knapsack** | CP | Mảng 1D cuộn ngược | $\mathcal{O}(N \cdot W)$ Time, $\mathcal{O}(W)$ Space | Bài toán cái túi chọn lựa tài nguyên |
| **Tree DP** | CP | DFS đệ quy trên đồ thị cây | $\mathcal{O}(N)$ | Max Independent Set / Rerooting DP |
| **Bitmask DP** | CP | Bitwise operations biểu diễn tập hợp | $\mathcal{O}(2^N \cdot N^2)$ | Bài toán Người du lịch (TSP) |
| **Digit DP** | CP | Duyệt trạng thái theo chữ số từ trái qua | $\mathcal{O}(\text{Digits} \cdot 10 \cdot \text{Mask})$ | Đếm số thỏa điều kiện trong $[L, R]$ |
| **Convex Hull Trick** | CP | Tối ưu bao lồi đường thẳng | $\mathcal{O}(N \log N)$ / $\mathcal{O}(N)$ | Giảm độ phức tạp từ $\mathcal{O}(N^2)$ xuống $\mathcal{O}(N)$ |
| **Spell Checker / Fuzzy Match**| Production | Levenshtein Distance + Mảng cuộn 1D | $\mathcal{O}(M \cdot N)$ Time, $\mathcal{O}(N)$ Space | Gợi ý từ gõ sai, Auto-complete Search |
| **Diff & Patch** | Production | LCS + Backtracking khôi phục path | $\mathcal{O}(M \cdot N)$ | Lệnh `git diff`, Lịch sử văn bản Docs |
| **Viterbi Algorithm** | Production | Max Probability Path trên HMM | $\mathcal{O}(T \cdot \vert{}S\vert{}^2)$ | Nhận dạng giọng nói, NLP, Giải mã tín hiệu |