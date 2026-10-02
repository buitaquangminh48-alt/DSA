# ⚡ Masterclass Greedy Algorithms — Từ CP Đỉnh Cao Đến System Production

> **Trọn bộ 8 dạng cài đặt Thuật toán Tham Lam (Greedy Algorithms) kinh điển nhất** được đóng gói hoàn chỉnh trong một file C++17 duy nhất (`full_greedy_masterclass.cpp`), tích hợp hệ thống **Trace "Phép Thuật" in màu ANSI** giúp trực quan hóa từng bước chọn lựa phần tử cục bộ thời gian thực!

---

## 📌 Tổng Quan Kiến Thức

**Thuật toán Tham Lam (Greedy Algorithm)** đưa ra quyết định dựa trên lựa chọn tối ưu nhất ở từng bước hiện tại mà không bao giờ quay xe (backtrack).
- Trong **Competitive Programming (CP)**: Greedy yêu cầu chứng minh toán học khắt khe (Exchange Argument, Structural Property) để đảm bảo nghiệm tìm được là **Tối ưu Tuyệt đối (Global Optimal)**.
- Trong **Production / System**: Greedy đóng vai trò là giải pháp **Xấp xỉ (Heuristic)** giúp giải quyết các bài toán NP-Hard trong thời gian cực ngắn với kết quả "đủ tốt" phục vụ vận hành thực tế.

---

## 📊 Bảng So Sánh 8 Cấu Trúc Greedy Kinh Điển

| STT | Tên Cấu Trúc / Bài Toán | Nhóm | Chiến Lược Tham Lam (Greedy Criterion) | Độ Phức Tạp | Ứng Dụng Thực Tế / CP |
| :-: | :--- | :---: | :--- | :---: | :--- |
| **1** | **Interval Scheduling** | CP | Chọn công việc có **Finish Time sớm nhất** | $\mathcal{O}(N \log N)$ | Lập lịch cuộc họp, tối ưu số lượng task thực thi |
| **2** | **Interval Partitioning** | CP | Sắp xếp theo Start Time + Heap lưu Finish Time | $\mathcal{O}(N \log N)$ | Quản lý tài nguyên, xếp phòng học tối thiểu |
| **3** | **Fractional Knapsack** | CP | Chọn vật phẩm có tỷ lệ **Value / Weight cao nhất** | $\mathcal{O}(N \log N)$ | Phân bổ ngân sách linh hoạt có thể chia nhỏ |
| **4** | **Kruskal MST** | CP | Sắp xếp tất cả các cạnh theo **Trọng số tăng dần** (DSU) | $\mathcal{O}(E \log E)$ | Thiết kế mạng lưới điện, mạng cáp quang tối thiểu |
| **5** | **Huffman Engine** | Production | Gom **2 nút có tần suất nhỏ nhất** bằng Priority Queue | $\mathcal{O}(N \log N)$ | Nén dữ liệu nền tảng cho ZIP, GZIP, PNG, JPEG |
| **6** | **Packet Scheduling** | Production | Ưu tiên gói tin **Kích thước nhỏ nhất** (Shortest Job First) | $\mathcal{O}(N \log N)$ | Giảm thiểu Latency trung bình trên Router/Switch |
| **7** | **Cloud VM Placement** | Production | First-Fit Decreasing (Xếp VM **RAM lớn trước**) | $\mathcal{O}(N \log N)$ | Tối ưu đóng gói VM trên Server Cloud (AWS/Azure) |
| **8** | **Cash Register / ATM** | Production | Luôn rút tờ tiền **Mệnh giá lớn nhất có thể** | $\mathcal{O}(N)$ | Hệ thống thối tiền tự động tại cây ATM, POS |

---

## 🛠️ Hướng Dẫn Biên Dịch & Trải Nghiệm

### Lệnh biên dịch & chạy trên Terminal

```bash
# Biên dịch mã nguồn với C++17
g++ -std=c++17 -O2 full_greedy_masterclass.cpp -o greedy_master

# Chạy chương trình và xem TRACE phép thuật!
./greedy_master