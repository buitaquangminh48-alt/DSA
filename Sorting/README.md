# ⚡ Classic & Advanced Sorting Algorithms (C++)

Bộ sưu tập mã nguồn C++ minh họa chi tiết **7 Thuật toán Sắp xếp (Sorting Algorithms)** từ các thuật toán cơ bản $O(N^2)$ đến các thuật toán nâng cao $O(N \log N)$ và thuật toán phi so sánh $O(N)$. 

Toàn bộ thuật toán được cài đặt trong một file duy nhất kèm **Trace từng bước** chuyển đổi của mảng, giúp trực quan hóa luồng chạy của từng thuật toán.

---

## 📊 Bảng So Sánh Chi Tiết 7 Thuật Toán

| # | Thuật toán | Best Case | Average Case | Worst Case | Space Complexity | Stability (Ổn định) | Tưởng chủ đạo / Thiết kế |
|---|---|---|---|---|---|---|---|
| **1** | **Bubble Sort** | $O(N)$ | $O(N^2)$ | $O(N^2)$ | $O(1)$ | **Stable** | So sánh & tráo đổi hai phần tử kế tiếp |
| **2** | **Selection Sort** | $O(N^2)$ | $O(N^2)$ | $O(N^2)$ | $O(1)$ | **Unstable** | Chọn phần tử Min đưa về đầu mảng |
| **3** | **Insertion Sort** | $O(N)$ | $O(N^2)$ | $O(N^2)$ | $O(1)$ | **Stable** | Chèn phần tử vào đoạn đã sắp xếp |
| **4** | **Quick Sort** | $O(N \log N)$ | $O(N \log N)$ | $O(N^2)$ | $O(\log N)$ *(Stack)* | **Unstable** | Chia để trị (Phân hoạch theo Pivot) |
| **5** | **Merge Sort** | $O(N \log N)$ | $O(N \log N)$ | $O(N \log N)$ | $O(N)$ | **Stable** | Chia để trị (Chia đôi & Trộn mảng con) |
| **6** | **Heap Sort** | $O(N \log N)$ | $O(N \log N)$ | $O(N \log N)$ | $O(1)$ | **Unstable** | Sử dụng cấu trúc Max-Heap / Min-Heap |
| **7** | **Radix Sort** | $O(d \cdot (N + K))$ | $O(d \cdot (N + K))$ | $O(d \cdot (N + K))$ | $O(N + K)$ | **Stable** | Không so sánh (Phân loại theo chữ số) |

> **Ghi chú:**
> - **Stable (Ổn định):** Giữ nguyên thứ tự tương đối giữa các phần tử có cùng giá trị trong mảng ban đầu.
> - $d$: Số lượng chữ số của phần tử lớn nhất ($10^d$).
> - $K$: Cơ số đếm (mặc định cơ số 10 $\rightarrow K = 10$).

---

## 📁 Cấu Trúc Mã Nguồn

```text
.
├── all_in_one_sorting.cpp    # File tổng hợp trọn bộ 7 thuật toán kèm Trace
└── README.md                 # Tài liệu tổng hợp & Bảng so sánh