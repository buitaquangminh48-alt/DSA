#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;

// ============================================================================
// NHÓM 1: BẢN CÀI ĐẶT KINH ĐIỂN CHO CP (COMPETITIVE PROGRAMMING)
// ============================================================================

namespace CP_DifferenceArray {

    // ------------------------------------------------------------------------
    // 1.1 2D Difference Array (Ma trận hiệu 2 chiều)
    // ------------------------------------------------------------------------
    class DifferenceMatrix2D {
    private:
        int R, C;
        vector<vector<int>> diff;
    public:
        DifferenceMatrix2D(int r, int c) : R(r), C(c) {
            diff.assign(R + 2, vector<int>(C + 2, 0));
        }

        // Cập nhật tăng val cho hình chữ nhật từ (r1, c1) đến (r2, c2) - O(1)
        void updateRectangle(int r1, int c1, int r2, int c2, int val) {
            diff[r1][c1] += val;
            diff[r1][c2 + 1] -= val;
            diff[r2 + 1][c1] -= val;
            diff[r2 + 1][c2 + 1] += val;
        }

        // Khôi phục lại ma trận ban đầu dùng 2D Prefix Sum - O(R * C)
        vector<vector<int>> getFinalMatrix() {
            vector<vector<int>> result(R, vector<int>(C, 0));
            vector<vector<int>> pref(R + 1, vector<int>(C + 1, 0));

            for (int r = 0; r < R; ++r) {
                for (int c = 0; c < C; ++c) {
                    pref[r + 1][c + 1] = diff[r][c] 
                                       + pref[r][c + 1] 
                                       + pref[r + 1][c] 
                                       - pref[r][c];
                    result[r][c] = pref[r + 1][c + 1];
                }
            }
            return result;
        }
    };

    // ------------------------------------------------------------------------
    // 1.2 Arithmetic Progression Range Update (Cấp số cộng - Second-Order Difference Array)
    // Cập nhật đoạn [L, R] với dãy: A, A+D, A+2D, ..., A+(R-L)*D trong O(1)
    // ------------------------------------------------------------------------
    class ArithmeticDifferenceArray {
    private:
        int n;
        vector<long long> D2; // Mảng hiệu bậc 2 (Double Difference Array)
    public:
        ArithmeticDifferenceArray(int size) : n(size), D2(size + 3, 0) {}

        void addArithmeticProgression(int L, int R, long long A, long long D) {
            long long Last = A + (R - L) * D;

            D2[L] += A;
            D2[L + 1] += (D - A);
            D2[R + 1] -= (Last + D);
            D2[R + 2] += Last;
        }

        // Quét 2 lượt Prefix Sum để khôi phục mảng ban đầu - O(N)
        vector<long long> getFinalArray() {
            vector<long long> D1(n + 1, 0);
            vector<long long> A(n, 0);

            // Lượt Prefix Sum 1: Khôi phục D1
            long long cur_d2 = 0;
            for (int i = 0; i <= n; ++i) {
                cur_d2 += D2[i];
                D1[i] = cur_d2;
            }

            // Lượt Prefix Sum 2: Khôi phục Mảng A
            long long cur_d1 = 0;
            for (int i = 0; i < n; ++i) {
                cur_d1 += D1[i];
                A[i] = cur_d1;
            }
            return A;
        }
    };

    // ------------------------------------------------------------------------
    // 1.3 Sparse Difference Array (Mảng hiệu thưa kết hợp Map / Nén tọa độ)
    // Dùng khi tọa độ L, R cực lớn (lên tới 10^9)
    // ------------------------------------------------------------------------
    class SparseDifferenceArray {
    private:
        map<long long, long long> diff_map;
    public:
        void updateRange(long long L, long long R, long long val) {
            diff_map[L] += val;
            diff_map[R + 1] -= val;
        }

        // Trả về danh sách các khoảng giá trị bị thay đổi
        void printProfile() {
            long long current_val = 0;
            long long prev_pos = -1;

            cout << "    [Sparse Segments Profile]:\n";
            for (auto const& [pos, delta] : diff_map) {
                if (prev_pos != -1 && current_val > 0) {
                    cout << "      Đoạn [" << prev_pos << " .. " << pos - 1 
                         << "] có giá trị tích lũy = " << current_val << "\n";
                }
                current_val += delta;
                prev_pos = pos;
            }
        }
    };
}

void demoCPGroup() {
    cout << "\n===================================================\n";
    cout << " 1. NHÓM THUẬT TOÁN CP (COMPETITIVE PROGRAMMING)\n";
    cout << "===================================================\n";

    // Demo 2D Difference Array
    CP_DifferenceArray::DifferenceMatrix2D diff2d(3, 3);
    diff2d.updateRectangle(0, 0, 1, 1, 5); // Tăng 5 cho vùng top-left 2x2
    auto mat = diff2d.getFinalMatrix();
    cout << "  [2D Difference Matrix] Ma trận sau khi cập nhật vùng (0,0)->(1,1) +5:\n";
    for (int r = 0; r < 3; ++r) {
        cout << "    ";
        for (int c = 0; c < 3; ++c) cout << mat[r][c] << " ";
        cout << "\n";
    }

    // Demo Arithmetic Progression Range Update
    CP_DifferenceArray::ArithmeticDifferenceArray arithDiff(6);
    // Cộng cấp số cộng A=2, D=3 vào đoạn [1, 4] -> Thêm {2, 5, 8, 11} tại index 1, 2, 3, 4
    arithDiff.addArithmeticProgression(1, 4, 2, 3);
    auto arithArr = arithDiff.getFinalArray();
    cout << "  [Arithmetic Range Update O(1)] Mảng sau khi cộng CSC [1..4] (A=2, D=3): ";
    for (long long x : arithArr) cout << x << " ";
    cout << "\n";

    // Demo Sparse Difference Array
    CP_DifferenceArray::SparseDifferenceArray sparseDiff;
    sparseDiff.updateRange(100, 1000000000LL, 10); // Cập nhật trên tọa độ khổng lồ 10^9
    sparseDiff.updateRange(500, 2000000000LL, 20);
    cout << "  [Sparse Difference Array (10^9 Coord)]:\n";
    sparseDiff.printProfile();
}

// ============================================================================
// NHÓM 2: BẢN CÀI ĐẶT KINH ĐIỂN CHO THỰC TẾ (PRODUCTION / SYSTEM)
// ============================================================================

namespace Production_DifferenceArray {

    // ------------------------------------------------------------------------
    // 2.1 Hotel/Flight Booking Availability Check (Agoda/Airbnb Capacity)
    // ------------------------------------------------------------------------
    struct BookingRequest {
        int startDay;
        int endDay;
        int numRooms;
    };

    void checkHotelOverbooking(int maxCapacity, int totalDays, const vector<BookingRequest>& requests) {
        vector<int> diff(totalDays + 2, 0);

        for (const auto& req : requests) {
            diff[req.startDay] += req.numRooms;
            diff[req.endDay + 1] -= req.numRooms; // Khách trả phòng vào ngày endDay + 1
        }

        int currentRoomsOccupied = 0;
        bool overbooked = false;

        cout << "  --- HOTEL CAPACITY MONITORING (Max: " << maxCapacity << " rooms) ---\n";
        for (int day = 1; day <= totalDays; ++day) {
            currentRoomsOccupied += diff[day];
            if (currentRoomsOccupied > maxCapacity) {
                cout << "    [OVERBOOKED WARNING] Ngày " << day << ": Cần " 
                     << currentRoomsOccupied << " phòng (Vượt quá " 
                     << currentRoomsOccupied - maxCapacity << " phòng!)\n";
                overbooked = true;
            }
        }
        if (!overbooked) cout << "    [OK] Tất cả các ngày đều đủ phòng đáp ứng!\n";
    }

    // ------------------------------------------------------------------------
    // 2.2 CPU Task Scheduler & Load Profiler (Cloud Auto-scaling)
    // ------------------------------------------------------------------------
    struct CPUTask {
        string name;
        int startSec;
        int endSec;
        int cpuUsagePercent;
    };

    void profileCPULoadAndAutoscale(int timeWindowSec, const vector<CPUTask>& tasks) {
        vector<int> cpuDiff(timeWindowSec + 2, 0);

        for (const auto& t : tasks) {
            cpuDiff[t.startSec] += t.cpuUsagePercent;
            cpuDiff[t.endSec + 1] -= t.cpuUsagePercent;
        }

        int currentCPU = 0;
        int maxPeakCPU = 0;
        int peakTime = 0;

        for (int sec = 0; sec <= timeWindowSec; ++sec) {
            currentCPU += cpuDiff[sec];
            if (currentCPU > maxPeakCPU) {
                maxPeakCPU = currentCPU;
                peakTime = sec;
            }
        }

        cout << "  [Cloud CPU Load Profiler]: Peak Load = " << maxPeakCPU 
             << "% tại giây thứ " << peakTime << "s.";
        if (maxPeakCPU > 100) {
            int extraInstances = ceil((maxPeakCPU - 100) / 100.0);
            cout << " => [AUTO-SCALING TRIGGERED] Kích hoạt thêm " 
                 << extraInstances << " Server Instance!\n";
        } else {
            cout << " => Hệ thống hoạt động an toàn.\n";
        }
    }

    // ------------------------------------------------------------------------
    // 2.3 Video Subtitle Overlap Detector (QA Tool cho Media Processing)
    // ------------------------------------------------------------------------
    struct Subtitle {
        int id;
        int startMs;
        int endMs;
        string text;
    };

    void detectSubtitleOverlap(const vector<Subtitle>& subs) {
        map<int, int> timelineDiff;

        for (const auto& sub : subs) {
            timelineDiff[sub.startMs] += 1;
            timelineDiff[sub.endMs + 1] -= 1;
        }

        int activeSubs = 0;
        bool hasOverlap = false;

        cout << "  [Subtitle Overlap Checker]:\n";
        for (auto const& [timeMs, delta] : timelineDiff) {
            activeSubs += delta;
            if (activeSubs > 1) {
                cout << "    [OVERLAP DETECTED] Tại thời điểm " << timeMs 
                     << "ms: Có " << activeSubs << " phụ đề hiển thị cùng lúc!\n";
                hasOverlap = true;
            }
        }
        if (!hasOverlap) cout << "    [OK] File phụ đề chuẩn, không có chồng lấn!\n";
    }

    // ------------------------------------------------------------------------
    // 2.4 Delta Encoding / Video Frame Compression (Nén khung hình Video)
    // ------------------------------------------------------------------------
    void demoDeltaEncodingVideoFrames(const vector<int>& rawKeyFrames) {
        vector<int> deltaFrame(rawKeyFrames.size());

        // Encoding Step: Lưu sự khác biệt (Difference Array Concept)
        deltaFrame[0] = rawKeyFrames[0];
        for (size_t i = 1; i < rawKeyFrames.size(); ++i) {
            deltaFrame[i] = rawKeyFrames[i] - rawKeyFrames[i - 1];
        }

        // Decoding Step (Video Playback): Khôi phục qua Prefix Sum
        vector<int> decodedFrames(rawKeyFrames.size());
        int accumulatedPixelVal = 0;
        for (size_t i = 0; i < deltaFrame.size(); ++i) {
            accumulatedPixelVal += deltaFrame[i];
            decodedFrames[i] = accumulatedPixelVal;
        }

        cout << "  [Delta Encoding / Video Stream Compression]:\n";
        cout << "    - Raw Frames Data:     ";
        for (int x : rawKeyFrames) cout << x << " ";
        cout << "\n    - Encoded Delta Stream:";
        for (int x : deltaFrame) cout << x << " ";
        cout << " (Tiết kiệm dung lượng nhờ chuỗi Delta nhỏ!)\n";
        cout << "    - Decrypted Playback:  ";
        for (int x : decodedFrames) cout << x << " ";
        cout << "\n";
    }
}

void demoProductionGroup() {
    cout << "\n===================================================\n";
    cout << " 2. NHÓM THUẬT TOÁN PRODUCTION (THỰC TẾ & SYSTEM)\n";
    cout << "===================================================\n";

    // Demo Hotel Booking
    vector<Production_DifferenceArray::BookingRequest> bookings = {
        {1, 5, 10}, {3, 7, 15}, {5, 8, 10}
    };
    Production_DifferenceArray::checkHotelOverbooking(30, 10, bookings);

    // Demo Cloud Auto-scaling
    vector<Production_DifferenceArray::CPUTask> cpuTasks = {
        {"Task 1", 2, 10, 40}, {"Task 2", 5, 12, 50}, {"Task 3", 8, 15, 30}
    };
    Production_DifferenceArray::profileCPULoadAndAutoscale(20, cpuTasks);

    // Demo Subtitle QA Tool
    vector<Production_DifferenceArray::Subtitle> subs = {
        {1, 1000, 3000, "Hello World"},
        {2, 2500, 4500, "Welcome to the show"}, // Chồng lấn tại [2500, 3000]
        {3, 5000, 7000, "Goodbye"}
    };
    Production_DifferenceArray::detectSubtitleOverlap(subs);

    // Demo Video Compression
    vector<int> framePixels = {100, 102, 105, 103, 108, 110};
    Production_DifferenceArray::demoDeltaEncodingVideoFrames(framePixels);
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "===================================================\n";
    cout << " 🚀 TRỌN BỘ CÁC BẢN CÀI ĐẶT DIFFERENCE ARRAY KINH ĐIỂN\n";
    cout << "===================================================\n";

    demoCPGroup();
    demoProductionGroup();

    return 0;
}