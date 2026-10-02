#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

// ============================================================================
// NHÓM 1: BẢN CÀI ĐẶT KINH ĐIỂN CHO CP (COMPETITIVE PROGRAMMING)
// ============================================================================

namespace CP_Fenwick {

    // ------------------------------------------------------------------------
    // 1.1 Model 1: Point Update - Range Query (Mô hình gốc 1-indexed)
    // ------------------------------------------------------------------------
    class FenwickTree1D {
    protected:
        int n;
        vector<long long> bit;
    public:
        FenwickTree1D(int size) : n(size), bit(size + 1, 0) {}

        void update(int idx, long long val) {
            for (; idx <= n; idx += idx & (-idx)) {
                bit[idx] += val;
            }
        }

        // Truy vấn tổng tiền tố [1 .. idx]
        long long query(int idx) const {
            long long sum = 0;
            for (; idx > 0; idx -= idx & (-idx)) {
                sum += bit[idx];
            }
            return sum;
        }

        // Truy vấn tổng đoạn [L .. R] - O(log N)
        long long queryRange(int L, int R) const {
            if (L > R) return 0;
            return query(R) - query(L - 1);
        }
    };

    // ------------------------------------------------------------------------
    // 1.2 Model 2: Range Update - Point Query (Dùng Difference Array concept)
    // ------------------------------------------------------------------------
    class RUPQ_Fenwick : private FenwickTree1D {
    public:
        RUPQ_Fenwick(int size) : FenwickTree1D(size) {}

        // Cộng val vào đoạn [L .. R] - O(log N)
        void updateRange(int L, int R, long long val) {
            update(L, val);
            update(R + 1, -val);
        }

        // Xem giá trị tại điểm i - O(log N)
        long long queryPoint(int idx) const {
            return query(idx);
        }
    };

    // ------------------------------------------------------------------------
    // 1.3 Model 3: Range Update - Range Query (Sử dụng 2 cây Fenwick song song)
    // Công thức: Sum(1..i) = i * B1[i] - B2[i]
    // ------------------------------------------------------------------------
    class RURQ_Fenwick {
    private:
        int n;
        FenwickTree1D B1, B2;

        long long prefixSum(int idx) const {
            return B1.query(idx) * idx - B2.query(idx);
        }
    public:
        RURQ_Fenwick(int size) : n(size), B1(size), B2(size) {}

        // Cộng val vào đoạn [L .. R] - O(log N)
        void updateRange(int L, int R, long long val) {
            B1.update(L, val);
            B1.update(R + 1, -val);
            B2.update(L, val * (L - 1));
            B2.update(R + 1, -val * R);
        }

        // Truy vấn tổng đoạn [L .. R] - O(log N)
        long long queryRange(int L, int R) const {
            return prefixSum(R) - prefixSum(L - 1);
        }
    };

    // ------------------------------------------------------------------------
    // 1.4 Model 4: 2D Fenwick Tree (Đa chiều)
    // ------------------------------------------------------------------------
    class FenwickTree2D {
    private:
        int R, C;
        vector<vector<long long>> bit;
    public:
        FenwickTree2D(int r, int c) : R(r), C(c) {
            bit.assign(R + 1, vector<long long>(C + 1, 0));
        }

        void update(int r, int c, long long val) {
            for (int i = r; i <= R; i += i & (-i)) {
                for (int j = c; j <= C; j += j & (-j)) {
                    bit[i][j] += val;
                }
            }
        }

        long long query(int r, int c) const {
            long long sum = 0;
            for (int i = r; i > 0; i -= i & (-i)) {
                for (int j = c; j > 0; j -= j & (-j)) {
                    sum += bit[i][j];
                }
            }
            return sum;
        }

        // Truy vấn hình chữ nhật (r1, c1) -> (r2, c2) - O(log R * log C)
        long long queryRegion(int r1, int c1, int r2, int c2) const {
            return query(r2, c2) - query(r1 - 1, c2) - query(r2, c1 - 1) + query(r1 - 1, c1 - 1);
        }
    };

    // ------------------------------------------------------------------------
    // 1.5 Model 5: Inversion Count (Đếm cặp nghịch thế + Nén tọa độ)
    // ------------------------------------------------------------------------
    long long countInversions(vector<int> arr) {
        int n = arr.size();
        // Nén tọa độ
        vector<int> sorted_arr = arr;
        sort(sorted_arr.begin(), sorted_arr.end());
        sorted_arr.erase(unique(sorted_arr.begin(), sorted_arr.end()), sorted_arr.end());

        FenwickTree1D bit(sorted_arr.size());
        long long inv_count = 0;

        for (int i = n - 1; i >= 0; --i) {
            int rank = lower_bound(sorted_arr.begin(), sorted_arr.end(), arr[i]) - sorted_arr.begin() + 1;
            inv_count += bit.query(rank - 1); // Đếm các số đã duyệt nhỏ hơn arr[i]
            bit.update(rank, 1);
        }
        return inv_count;
    }

    // ------------------------------------------------------------------------
    // 1.6 Model 6: Binary Lifting / Binary Search trên Fenwick Tree O(log N)
    // Tìm vị trí nhỏ nhất k sao cho PrefixSum(k) >= target_sum
    // ------------------------------------------------------------------------
    class BinarySearchFenwick : public FenwickTree1D {
    public:
        BinarySearchFenwick(int size) : FenwickTree1D(size) {}

        int lower_bound(long long target_sum) const {
            int idx = 0;
            long long accumulated = 0;
            // Tìm lũy thừa lớn nhất của 2 <= n
            int max_pow = 1;
            while ((max_pow << 1) <= n) max_pow <<= 1;

            for (int sz = max_pow; sz > 0; sz >>= 1) {
                if (idx + sz <= n && accumulated + bit[idx + sz] < target_sum) {
                    idx += sz;
                    accumulated += bit[idx];
                }
            }
            return idx + 1; // 1-indexed position
        }
    };
}

void demoCPGroup() {
    cout << "\n===================================================\n";
    cout << " 1. NHÓM THUẬT TOÁN CP (COMPETITIVE PROGRAMMING)\n";
    cout << "===================================================\n";

    // Demo PURQ
    CP_Fenwick::FenwickTree1D purq(5);
    purq.update(1, 3); purq.update(3, 5); purq.update(5, 2);
    cout << "  [PURQ Fenwick] Sum[2..5]: " << purq.queryRange(2, 5) << " (Expected: 7)\n";

    // Demo RUPQ
    CP_Fenwick::RUPQ_Fenwick rupq(5);
    rupq.updateRange(2, 4, 10);
    cout << "  [RUPQ Fenwick] Val tại idx 3: " << rupq.queryPoint(3) << " (Expected: 10)\n";

    // Demo RURQ
    CP_Fenwick::RURQ_Fenwick rurq(5);
    rurq.updateRange(1, 3, 5);  // [5, 5, 5, 0, 0]
    rurq.updateRange(2, 4, 10); // [5, 15, 15, 10, 0]
    cout << "  [RURQ Fenwick] Sum[2..4]: " << rurq.queryRange(2, 4) << " (Expected: 40)\n";

    // Demo 2D
    CP_Fenwick::FenwickTree2D bit2d(3, 3);
    bit2d.update(2, 2, 9);
    cout << "  [2D Fenwick] Query Region (1,1)->(3,3): " << bit2d.queryRegion(1, 1, 3, 3) << "\n";

    // Demo Inversion Count
    vector<int> arr = {8, 4, 2, 1};
    cout << "  [Inversion Count O(N log N)] Cặp nghịch thế {8,4,2,1}: " 
         << CP_Fenwick::countInversions(arr) << " (Expected: 6)\n";

    // Demo Binary Lifting on BIT
    CP_Fenwick::BinarySearchFenwick bsBit(8);
    // Mảng gốc: [2, 1, 4, 3, 2, 1, 5, 1] -> PrefixSum: [2, 3, 7, 10, 12, 13, 18, 19]
    vector<int> vals = {2, 1, 4, 3, 2, 1, 5, 1};
    for (int i = 0; i < 8; ++i) bsBit.update(i + 1, vals[i]);
    cout << "  [Binary Lifting BIT O(log N)] Vị trí đầu tiên có PrefixSum >= 10: Index " 
         << bsBit.lower_bound(10) << " (Expected: 4)\n";
}

// ============================================================================
// NHÓM 2: BẢN CÀI ĐẶT KINH ĐIỂN CHO THỰC TẾ (PRODUCTION / SYSTEM)
// ============================================================================

namespace Production_Fenwick {

    // ------------------------------------------------------------------------
    // 2.1 Dynamic Leaderboard & Percentile Calculator (Xếp hạng Game Real-time)
    // ------------------------------------------------------------------------
    class DynamicLeaderboard {
    private:
        int maxScore;
        CP_Fenwick::BinarySearchFenwick scoreFreqBIT;
        long long totalPlayers;
    public:
        DynamicLeaderboard(int max_score_limit) 
            : maxScore(max_score_limit), scoreFreqBIT(max_score_limit + 1), totalPlayers(0) {}

        void addPlayerScore(int score) {
            scoreFreqBIT.update(score + 1, 1); // Shift 1-index
            totalPlayers++;
        }

        void updatePlayerScore(int oldScore, int newScore) {
            scoreFreqBIT.update(oldScore + 1, -1);
            scoreFreqBIT.update(newScore + 1, 1);
        }

        // Tính thứ hạng (Rank 1 là điểm cao nhất)
        long long getRank(int score) const {
            long long playersWithLowerOrEqual = scoreFreqBIT.query(score + 1);
            return totalPlayers - playersWithLowerOrEqual + 1;
        }

        // Tính Bách phân vị (Percentile)
        double getPercentile(int score) const {
            if (totalPlayers == 0) return 0.0;
            long long playersStrictlyLower = scoreFreqBIT.query(score);
            return (double)playersStrictlyLower / totalPlayers * 100.0;
        }
    };

    // ------------------------------------------------------------------------
    // 2.2 Running Cumulative Distribution Function - CDF (Thống kê luồng)
    // ------------------------------------------------------------------------
    class StreamCDFTracker {
    private:
        int numBuckets;
        CP_Fenwick::FenwickTree1D bucketBIT;
        long long totalSamples;
    public:
        StreamCDFTracker(int buckets) : numBuckets(buckets), bucketBIT(buckets), totalSamples(0) {}

        void addSample(int bucketIdx) {
            if (bucketIdx < 1 || bucketIdx > numBuckets) return;
            bucketBIT.update(bucketIdx, 1);
            totalSamples++;
        }

        // Tính xác suất tích lũy F(X) = P(Sample <= bucketIdx)
        double getCDF(int bucketIdx) const {
            if (totalSamples == 0) return 0.0;
            return (double)bucketBIT.query(bucketIdx) / totalSamples;
        }
    };

    // ------------------------------------------------------------------------
    // 2.3 Database Frequency Indexer / Compressed Counting (Chỉ mục tần suất)
    // ------------------------------------------------------------------------
    class ColumnarFrequencyIndexer {
    private:
        int domainSize;
        CP_Fenwick::FenwickTree1D freqBIT;
    public:
        ColumnarFrequencyIndexer(int domain) : domainSize(domain), freqBIT(domain) {}

        void insertRecord(int value) {
            freqBIT.update(value, 1);
        }

        void deleteRecord(int value) {
            freqBIT.update(value, -1);
        }

        // Đếm số lượng phần tử có giá trị nhỏ hơn value
        long long countLessThan(int value) const {
            if (value <= 1) return 0;
            return freqBIT.query(value - 1);
        }

        // Đếm số lượng phần tử rơi vào khoảng [valL, valR]
        long long countInRange(int valL, int valR) const {
            return freqBIT.queryRange(valL, valR);
        }
    };
}

void demoProductionGroup() {
    cout << "\n===================================================\n";
    cout << " 2. NHÓM THUẬT TOÁN PRODUCTION (THỰC TẾ & SYSTEM)\n";
    cout << "===================================================\n";

    // Demo Game Leaderboard
    Production_Fenwick::DynamicLeaderboard leaderboard(100); // Score 0-100
    leaderboard.addPlayerScore(50);
    leaderboard.addPlayerScore(70);
    leaderboard.addPlayerScore(90);
    leaderboard.addPlayerScore(70);

    cout << "  [Real-time Game Leaderboard]:\n";
    cout << "    - Rank của người chơi 70 điểm: TOP " << leaderboard.getRank(70) << "\n";
    cout << "    - Percentile của người chơi 70 điểm: " 
         << fixed << setprecision(1) << leaderboard.getPercentile(70) << "th Percentile\n";

    // Demo Stream CDF
    Production_Fenwick::StreamCDFTracker cdfTracker(10);
    for (int b : {1, 2, 2, 3, 5, 5, 5, 8}) cdfTracker.addSample(b);
    cout << "  [Running Stream CDF]:\n";
    cout << "    - P(Sample <= 5): " << cdfTracker.getCDF(5) * 100.0 << "%\n";

    // Demo Columnar DB Indexer
    Production_Fenwick::ColumnarFrequencyIndexer dbIndex(1000);
    dbIndex.insertRecord(100);
    dbIndex.insertRecord(250);
    dbIndex.insertRecord(500);
    dbIndex.insertRecord(250);

    cout << "  [DB Columnar Frequency Indexer]:\n";
    cout << "    - Số bản ghi có giá trị < 300: " << dbIndex.countLessThan(300) << "\n";
    cout << "    - Số bản ghi thuộc khoảng [200, 600]: " << dbIndex.countInRange(200, 600) << "\n";
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "===================================================\n";
    cout << " 🚀 TRỌN BỘ CÁC BẢN CÀI ĐẶT FENWICK TREE (BIT) KINH ĐIỂN\n";
    cout << "===================================================\n";

    demoCPGroup();
    demoProductionGroup();

    return 0;
}