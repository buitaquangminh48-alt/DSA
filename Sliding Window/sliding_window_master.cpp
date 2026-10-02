#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <queue>
#include <cmath>
#include <numeric>

using namespace std;

// ============================================================================
// NHÓM 1: BẢN CÀI ĐẶT KINH ĐIỂN CHO CP (COMPETITIVE PROGRAMMING)
// ============================================================================

namespace CP_SlidingWindow {

    // ------------------------------------------------------------------------
    // 1.1 Fixed-size Window: Tìm tổng lớn nhất của K phần tử liên tiếp - O(N)
    // ------------------------------------------------------------------------
    int maxSubarraySumK(const vector<int>& arr, int k) {
        int n = arr.size();
        if (n < k) return -1;

        int window_sum = 0;
        for (int i = 0; i < k; ++i) window_sum += arr[i];

        int max_sum = window_sum;
        for (int i = k; i < n; ++i) {
            window_sum += arr[i] - arr[i - k]; // Thêm phần tử mới, bớt phần tử cũ
            max_sum = max(max_sum, window_sum);
        }
        return max_sum;
    }

    // ------------------------------------------------------------------------
    // 1.2 Variable-size Window: Chuỗi con ngắn nhất chứa đủ ký tự của t - O(N)
    // (Minimum Window Substring - LeetCode 76)
    // ------------------------------------------------------------------------
    string minWindow(string s, string t) {
        unordered_map<char, int> target_count, window_count;
        for (char c : t) target_count[c]++;

        int required = target_count.size();
        int formed = 0;
        int left = 0, right = 0;
        int min_len = 1e9, start_idx = 0;

        while (right < s.length()) {
            char c = s[right];
            window_count[c]++;

            if (target_count.count(c) && window_count[c] == target_count[c]) {
                formed++;
            }

            // Co hẹp cạnh trái khi đã thỏa mãn điều kiện
            while (left <= right && formed == required) {
                c = s[left];
                if (right - left + 1 < min_len) {
                    min_len = right - left + 1;
                    start_idx = left;
                }

                window_count[c]--;
                if (target_count.count(c) && window_count[c] < target_count[c]) {
                    formed--;
                }
                left++;
            }
            right++;
        }
        return min_len == 1e9 ? "" : s.substr(start_idx, min_len);
    }

    // ------------------------------------------------------------------------
    // 1.3 Monotonic Queue Sliding Window: Sliding Window Maximum - O(N)
    // (LeetCode 239 - Dùng std::deque duy trì thứ tự giảm dần)
    // ------------------------------------------------------------------------
    vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
        deque<int> dq; // Lưu CHỈ SỐ (indices) của các phần tử
        vector<int> result;

        for (int i = 0; i < nums.size(); ++i) {
            // 1. Loại bỏ các phần tử nằm ngoài cửa sổ hiện tại (bên trái)
            if (!dq.empty() && dq.front() == i - k) {
                dq.pop_front();
            }

            // 2. Loại bỏ các phần tử nhỏ hơn phần tử sắp thêm (duy trì tính đơn điệu)
            while (!dq.empty() && nums[dq.back()] <= nums[i]) {
                dq.pop_back();
            }

            dq.push_back(i);

            // 3. Ghi nhận kết quả khi cửa sổ đủ kích thước K
            if (i >= k - 1) {
                result.push_back(nums[dq.front()]);
            }
        }
        return result;
    }
}

void demoCPGroup() {
    cout << "\n===================================================\n";
    cout << " 1. NHÓM THUẬT TOÁN CP (COMPETITIVE PROGRAMMING)\n";
    cout << "===================================================\n";

    // Demo Fixed-size
    vector<int> arr = {2, 1, 5, 1, 3, 2};
    cout << "  [Fixed Window K=3] Tổng max 3 phần tử liên tiếp: " 
         << CP_SlidingWindow::maxSubarraySumK(arr, 3) << "\n";

    // Demo Variable-size
    string s = "ADOBECODEBANC", t = "ABC";
    cout << "  [Min Window Substring] Chuỗi nhỏ nhất chứa '" << t << "' trong '" << s << "': " 
         << CP_SlidingWindow::minWindow(s, t) << "\n";

    // Demo Monotonic Deque
    vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    vector<int> max_win = CP_SlidingWindow::maxSlidingWindow(nums, 3);
    cout << "  [Monotonic Deque O(N)] Sliding Window Max (K=3): ";
    for (int x : max_win) cout << x << " ";
    cout << "\n";
}

// ============================================================================
// NHÓM 2: BẢN CÀI ĐẶT KINH ĐIỂN CHO THỰC TẾ (PRODUCTION / SYSTEM)
// ============================================================================

namespace Production_SlidingWindow {

    // ------------------------------------------------------------------------
    // 2.1 Sliding Window Log Rate Limiter (Chống DDoS / API Gateway)
    // ------------------------------------------------------------------------
    class SlidingWindowRateLimiter {
    private:
        int max_requests;
        chrono::milliseconds window_size;
        queue<chrono::steady_clock::time_point> request_timestamps;

    public:
        SlidingWindowRateLimiter(int requests, int window_seconds)
            : max_requests(requests), window_size(window_seconds * 1000) {}

        bool allowRequest() {
            auto now = chrono::steady_clock::now();

            // Loại bỏ các request đã hết hạn (nằm ngoài cửa sổ thời gian)
            while (!request_timestamps.empty() && 
                   (now - request_timestamps.front()) > window_size) {
                request_timestamps.pop();
            }

            if (request_timestamps.size() < max_requests) {
                request_timestamps.push(now);
                return true; // Cho phép request
            }
            return false; // HTTP 429 Too Many Requests
        }
    };

    // ------------------------------------------------------------------------
    // 2.2 Sliding Window Aggregator (Thống kê luồng dữ liệu thời gian thực)
    // Thường dùng trong Monitoring (Prometheus) & Stream Processing (Flink, Spark)
    // ------------------------------------------------------------------------
    struct MetricEvent {
        chrono::steady_clock::time_point timestamp;
        double value;
    };

    class SlidingWindowAggregator {
    private:
        chrono::milliseconds window_size;
        deque<MetricEvent> event_window;
        double running_sum = 0.0;

    public:
        SlidingWindowAggregator(int window_seconds)
            : window_size(window_seconds * 1000) {}

        void record(double value) {
            auto now = chrono::steady_clock::now();
            
            // Xóa các event cũ quá khoảng thời gian cửa sổ
            while (!event_window.empty() && (now - event_window.front().timestamp) > window_size) {
                running_sum -= event_window.front().value;
                event_window.pop_front();
            }

            event_window.push_back({now, value});
            running_sum += value;
        }

        double getAverage() {
            if (event_window.empty()) return 0.0;
            return running_sum / event_window.size();
        }

        int getEventCount() {
            return event_window.size();
        }
    };

    // ------------------------------------------------------------------------
    // 2.3 TCP Sliding Window Protocol (Mô phỏng Flow Control - Sender Side)
    // ------------------------------------------------------------------------
    void simulateTCPSlidingWindow(int total_packets, int window_size) {
        cout << "  [TCP Flow Control] Bắt đầu truyền " << total_packets << " gói tin với Cửa sổ = " << window_size << "\n";
        int send_base = 0;
        int next_seq_num = 0;

        while (send_base < total_packets) {
            // Gửi tất cả các gói tin trong cửa sổ hiện tại
            while (next_seq_num < send_base + window_size && next_seq_num < total_packets) {
                cout << "    -> [SEND] Gói tin " << next_seq_num << "\n";
                next_seq_num++;
            }

            // Giả lập nhận ACK cho gói tin đầu tiên trong cửa sổ
            cout << "    <- [ACK] Nhận ACK cho gói tin " << send_base << " -> Cửa sổ trượt sang phải!\n";
            send_base++; // Cửa sổ trượt tiến lên
        }
    }

    // ------------------------------------------------------------------------
    // 2.4 Image Processing: Box Blur Filter (Ma trận tích chập Convolution 3x3)
    // ------------------------------------------------------------------------
    vector<vector<int>> applyBoxBlur(const vector<vector<int>>& image) {
        int rows = image.size();
        int cols = image[0].size();
        vector<vector<int>> blurred(rows, vector<int>(cols, 0));

        // Trượt cửa sổ 3x3 qua từng pixel (bỏ qua biên)
        for (int r = 1; r < rows - 1; ++r) {
            for (int c = 1; c < cols - 1; ++c) {
                int sum = 0;
                // Tính tổng 9 pixel trong cửa sổ 3x3
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        sum += image[r + dr][c + dc];
                    }
                }
                blurred[r][c] = sum / 9; // Lấy giá trị trung bình
            }
        }
        return blurred;
    }
}

void demoProductionGroup() {
    cout << "\n===================================================\n";
    cout << " 2. NHÓM THUẬT TOÁN PRODUCTION (THỰC TẾ & SYSTEM)\n";
    cout << "===================================================\n";

    // Demo Rate Limiter
    cout << "  [Rate Limiter] Giới hạn: Max 3 requests / 1 giây\n";
    Production_SlidingWindow::SlidingWindowRateLimiter limiter(3, 1);
    for (int i = 1; i <= 5; ++i) {
        bool allowed = limiter.allowRequest();
        cout << "    Request " << i << ": " << (allowed ? "ACCEPT (200 OK)" : "REJECT (429 Too Many Requests)") << "\n";
    }

    // Demo Stream Aggregator
    cout << "  [Stream Aggregator] Thống kê Response Time trong cửa sổ 5 giây:\n";
    Production_SlidingWindow::SlidingWindowAggregator aggregator(5);
    aggregator.record(120.0); // 120 ms
    aggregator.record(250.0); // 250 ms
    aggregator.record(80.0);  // 80 ms
    cout << "    Tổng số sự kiện thu thập: " << aggregator.getEventCount() << "\n";
    cout << "    Response Time trung bình: " << aggregator.getAverage() << " ms\n";

    // Demo TCP Sliding Window
    Production_SlidingWindow::simulateTCPSlidingWindow(5, 3);

    // Demo Image Processing
    vector<vector<int>> image = {
        {10, 20, 30, 40},
        {50, 60, 70, 80},
        {90, 100, 110, 120},
        {130, 140, 150, 160}
    };
    vector<vector<int>> blurred = Production_SlidingWindow::applyBoxBlur(image);
    cout << "  [Image Processing] Giá trị Pixel trung tâm sau khi Blur 3x3: " << blurred[1][1] << "\n";
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "===================================================\n";
    cout << " 🚀 TRỌN BỘ CÁC BẢN CÀI ĐẶT SLIDING WINDOW KINH ĐIỂN\n";
    cout << "===================================================\n";

    demoCPGroup();
    demoProductionGroup();

    return 0;
}