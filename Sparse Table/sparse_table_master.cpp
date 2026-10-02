#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include <iomanip>

using namespace std;

// ============================================================================
// NHÓM 1: BẢN CÀI ĐẶT KINH ĐIỂN CHO CP (COMPETITIVE PROGRAMMING)
// ============================================================================

namespace CP_SparseTable {

    // ------------------------------------------------------------------------
    // Bảng Tra Cứu Log2 Tiền Xử Lý (Tránh Dùng std::log2 để đảm bảo O(1) thực sự)
    // ------------------------------------------------------------------------
    vector<int> buildLogTable(int N) {
        vector<int> lg(N + 1, 0);
        for (int i = 2; i <= N; ++i) {
            lg[i] = lg[i / 2] + 1;
        }
        return lg;
    }

    // ------------------------------------------------------------------------
    // 1.1 Range Minimum / Maximum Query (RMQ - Phép toán Trùng lặp O(1))
    // ------------------------------------------------------------------------
    template <typename T>
    class SparseTableRMQ {
    private:
        int n, K;
        vector<vector<T>> st;
        vector<int> lg;
    public:
        SparseTableRMQ(const vector<T>& arr) {
            n = arr.size();
            lg = buildLogTable(n);
            K = lg[n] + 1;
            st.assign(K, vector<T>(n));

            // Khởi tạo tầng 0 (k = 0, độ dài 2^0 = 1)
            for (int i = 0; i < n; ++i) st[0][i] = arr[i];

            // Xây dựng các tầng k dựa trên quy hoạch động - O(N log N)
            for (int k = 1; k < K; ++k) {
                for (int i = 0; i + (1 << k) <= n; ++i) {
                    st[k][i] = min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
                }
            }
        }

        // Truy vấn Min trên đoạn [L .. R] - O(1)
        T queryMin(int L, int R) const {
            int len = R - L + 1;
            int k = lg[len];
            return min(st[k][L], st[k][R - (1 << k) + 1]);
        }
    };

    // ------------------------------------------------------------------------
    // 1.2 Range GCD Query (GCD có tính chất Idempotent - O(1))
    // ------------------------------------------------------------------------
    class SparseTableGCD {
    private:
        int n, K;
        vector<vector<long long>> st;
        vector<int> lg;
    public:
        SparseTableGCD(const vector<long long>& arr) {
            n = arr.size();
            lg = buildLogTable(n);
            K = lg[n] + 1;
            st.assign(K, vector<long long>(n));

            for (int i = 0; i < n; ++i) st[0][i] = arr[i];

            for (int k = 1; k < K; ++k) {
                for (int i = 0; i + (1 << k) <= n; ++i) {
                    st[k][i] = std::gcd(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
                }
            }
        }

        // Truy vấn GCD trên đoạn [L .. R] - O(1)
        long long queryGCD(int L, int R) const {
            int len = R - L + 1;
            int k = lg[len];
            return std::gcd(st[k][L], st[k][R - (1 << k) + 1]);
        }
    };

    // ------------------------------------------------------------------------
    // 1.3 Range Sum Query via Sparse Table (Phép toán Không Trùng Lặp - O(log N))
    // Minh họa cơ chế phân rã Bitwise / Binary Lifting
    // ------------------------------------------------------------------------
    class SparseTableSum {
    private:
        int n, K;
        vector<vector<long long>> st;
    public:
        SparseTableSum(const vector<long long>& arr) {
            n = arr.size();
            K = 32 - __builtin_clz(n);
            st.assign(K, vector<long long>(n, 0));

            for (int i = 0; i < n; ++i) st[0][i] = arr[i];

            for (int k = 1; k < K; ++k) {
                for (int i = 0; i + (1 << k) <= n; ++i) {
                    st[k][i] = st[k - 1][i] + st[k - 1][i + (1 << (k - 1))];
                }
            }
        }

        // Phân rã độ dài đoạn thành các lũy thừa của 2 - O(log N)
        long long querySum(int L, int R) const {
            long long sum = 0;
            int len = R - L + 1;
            for (int k = K - 1; k >= 0; --k) {
                if ((len >> k) & 1) {
                    sum += st[k][L];
                    L += (1 << k);
                }
            }
            return sum;
        }
    };

    // ------------------------------------------------------------------------
    // 1.4 LCA (Lowest Common Ancestor) via RMQ Euler Tour - O(1) Query
    // ------------------------------------------------------------------------
    class LCA_RMQ {
    private:
        int n;
        vector<vector<int>> adj;
        vector<int> euler_tour;
        vector<int> depth;
        vector<int> first_occ;
        vector<int> lg;
        vector<vector<int>> st; // Lưu index của Euler Tour có depth nhỏ nhất

        void dfs(int u, int p, int d) {
            depth[u] = d;
            first_occ[u] = euler_tour.size();
            euler_tour.push_back(u);

            for (int v : adj[u]) {
                if (v != p) {
                    dfs(v, u, d + 1);
                    euler_tour.push_back(u);
                }
            }
        }
    public:
        LCA_RMQ(int num_nodes) : n(num_nodes), adj(num_nodes + 1), depth(num_nodes + 1), first_occ(num_nodes + 1) {}

        void addEdge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        void build(int root = 1) {
            dfs(root, 0, 0);
            int m = euler_tour.size();
            lg = buildLogTable(m);
            int K = lg[m] + 1;
            st.assign(K, vector<int>(m));

            for (int i = 0; i < m; ++i) st[0][i] = i;

            for (int k = 1; k < K; ++k) {
                for (int i = 0; i + (1 << k) <= m; ++i) {
                    int idx1 = st[k - 1][i];
                    int idx2 = st[k - 1][i + (1 << (k - 1))];
                    st[k][i] = (depth[euler_tour[idx1]] < depth[euler_tour[idx2]]) ? idx1 : idx2;
                }
            }
        }

        // Tìm Tổ tiên chung thấp nhất - O(1)
        int getLCA(int u, int v) const {
            int L = first_occ[u];
            int R = first_occ[v];
            if (L > R) swap(L, R);

            int len = R - L + 1;
            int k = lg[len];
            int idx1 = st[k][L];
            int idx2 = st[k][R - (1 << k) + 1];

            int best_idx = (depth[euler_tour[idx1]] < depth[euler_tour[idx2]]) ? idx1 : idx2;
            return euler_tour[best_idx];
        }
    };
}

void demoCPGroup() {
    cout << "\n===================================================\n";
    cout << " 1. NHÓM THUẬT TOÁN CP (COMPETITIVE PROGRAMMING)\n";
    cout << "===================================================\n";

    // Demo RMQ
    vector<int> arr = {7, 2, 3, 0, 5, 10, 3, 12, 18};
    CP_SparseTable::SparseTableRMQ<int> rmq(arr);
    cout << "  [RMQ Sparse Table O(1)] Min trên [2..6] ({3,0,5,10,3}): " << rmq.queryMin(2, 6) << " (Expected: 0)\n";

    // Demo GCD
    vector<long long> gcd_arr = {12, 24, 18, 36, 48, 60};
    CP_SparseTable::SparseTableGCD st_gcd(gcd_arr);
    cout << "  [Range GCD O(1)] GCD trên [1..4] ({24,18,36,48}): " << st_gcd.queryGCD(1, 4) << " (Expected: 6)\n";

    // Demo Sum via Decomposing
    vector<long long> sum_arr = {1, 3, 5, 7, 9, 11};
    CP_SparseTable::SparseTableSum st_sum(sum_arr);
    cout << "  [Range Sum O(log N)] Sum trên [1..4] ({3+5+7+9}): " << st_sum.querySum(1, 4) << " (Expected: 24)\n";

    // Demo LCA via RMQ
    CP_SparseTable::LCA_RMQ lcaTree(7);
    lcaTree.addEdge(1, 2); lcaTree.addEdge(1, 3);
    lcaTree.addEdge(2, 4); lcaTree.addEdge(2, 5);
    lcaTree.addEdge(3, 6); lcaTree.addEdge(3, 7);
    lcaTree.build(1);
    cout << "  [LCA via RMQ Euler Tour O(1)] LCA của Node 4 và Node 5: " << lcaTree.getLCA(4, 5) << " (Expected: 2)\n";
    cout << "  [LCA via RMQ Euler Tour O(1)] LCA của Node 4 và Node 7: " << lcaTree.getLCA(4, 7) << " (Expected: 1)\n";
}

// ============================================================================
// NHÓM 2: BẢN CÀI ĐẶT KINH ĐIỂN CHO THỰC TẾ (PRODUCTION / SYSTEM)
// ============================================================================

namespace Production_SparseTable {

    // ------------------------------------------------------------------------
    // 2.1 Static Sensor Data Analytics (Phân tích Dữ liệu Cảm biến Lịch sử)
    // ------------------------------------------------------------------------
    class HistoricalSensorData {
    private:
        vector<double> minTemp;
        vector<double> maxTemp;
        CP_SparseTable::SparseTableRMQ<double>* stMin;
        // Triển khai phiên bản Max tùy chỉnh đơn giản
        int n, K;
        vector<vector<double>> stMax;
        vector<int> lg;
    public:
        HistoricalSensorData(const vector<double>& readouts) : minTemp(readouts), maxTemp(readouts) {
            stMin = new CP_SparseTable::SparseTableRMQ<double>(minTemp);

            n = readouts.size();
            lg = CP_SparseTable::buildLogTable(n);
            K = lg[n] + 1;
            stMax.assign(K, vector<double>(n));

            for (int i = 0; i < n; ++i) stMax[0][i] = readouts[i];

            for (int k = 1; k < K; ++k) {
                for (int i = 0; i + (1 << k) <= n; ++i) {
                    stMax[k][i] = max(stMax[k - 1][i], stMax[k - 1][i + (1 << (k - 1))]);
                }
            }
        }

        ~HistoricalSensorData() { delete stMin; }

        double getMaxTemperature(int startMin, int endMin) const {
            int len = endMin - startMin + 1;
            int k = lg[len];
            return max(stMax[k][startMin], stMax[k][endMin - (1 << k) + 1]);
        }

        double getMinTemperature(int startMin, int endMin) const {
            return stMin->queryMin(startMin, endMin);
        }
    };

    // ------------------------------------------------------------------------
    // 2.2 Fast File System Indexing / Binary Jump (Tra cứu Block/Quyền Firmware)
    // ------------------------------------------------------------------------
    class StaticFileSystemIndexer {
    private:
        int numBlocks;
        vector<vector<int>> jumpTable; // JumpTable[k][i] = Khối dữ liệu sau 2^k bước từ block i
    public:
        StaticFileSystemIndexer(const vector<int>& nextBlockMap) {
            numBlocks = nextBlockMap.size();
            int K = 20; // Hỗ trợ nhảy tới 2^20 bước (~1 triệu khối)
            jumpTable.assign(K, vector<int>(numBlocks, -1));

            for (int i = 0; i < numBlocks; ++i) jumpTable[0][i] = nextBlockMap[i];

            for (int k = 1; k < K; ++k) {
                for (int i = 0; i < numBlocks; ++i) {
                    if (jumpTable[k - 1][i] != -1) {
                        jumpTable[k][i] = jumpTable[k - 1][jumpTable[k - 1][i]];
                    }
                }
            }
        }

        // Tìm khối dữ liệu đích sau K bước chuyển đổi liên tiếp trong O(log K)
        int getBlockAfterSteps(int startBlock, int steps) const {
            int curr = startBlock;
            for (int k = 0; k < 20; ++k) {
                if ((steps >> k) & 1) {
                    curr = jumpTable[k][curr];
                    if (curr == -1) break;
                }
            }
            return curr;
        }
    };

    // ------------------------------------------------------------------------
    // 2.3 LCP (Longest Common Prefix) Query via Suffix Array & Sparse Table
    // ------------------------------------------------------------------------
    class LCP_SearchIndexer {
    private:
        string text;
        int n;
        vector<int> sa;   // Suffix Array
        vector<int> rank; // Inverse Suffix Array
        vector<int> lcp;  // LCP Array
        CP_SparseTable::SparseTableRMQ<int>* stLCP;

        void buildSuffixArray() {
            sa.resize(n);
            rank.resize(n);
            for (int i = 0; i < n; ++i) { sa[i] = i; rank[i] = text[i]; }

            for (int k = 1; k < n; k <<= 1) {
                auto cmp = [&](int i, int j) {
                    if (rank[i] != rank[j]) return rank[i] < rank[j];
                    int ri = (i + k < n) ? rank[i + k] : -1;
                    int rj = (j + k < n) ? rank[j + k] : -1;
                    return ri < rj;
                };
                sort(sa.begin(), sa.end(), cmp);

                vector<int> tmp_rank(n, 0);
                for (int i = 1; i < n; ++i) {
                    tmp_rank[i] = tmp_rank[i - 1] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
                }
                for (int i = 0; i < n; ++i) rank[sa[i]] = tmp_rank[i];
                if (rank[sa[n - 1]] == n - 1) break;
            }
        }

        void buildLCP() {
            lcp.assign(n, 0);
            int h = 0;
            for (int i = 0; i < n; ++i) {
                if (rank[i] > 0) {
                    int j = sa[rank[i] - 1];
                    while (i + h < n && j + h < n && text[i + h] == text[j + h]) h++;
                    lcp[rank[i]] = h;
                    if (h > 0) h--;
                }
            }
        }
    public:
        LCP_SearchIndexer(const string& str) : text(str), n(str.length()) {
            buildSuffixArray();
            buildLCP();
            stLCP = new CP_SparseTable::SparseTableRMQ<int>(lcp);
        }

        ~LCP_SearchIndexer() { delete stLCP; }

        // Tính Độ dài Tiền tố Chung Dài Nhất (LCP) giữa 2 chuỗi con xuất phát tại idx1 và idx2 - O(1)
        int queryLCP(int idx1, int idx2) const {
            if (idx1 == idx2) return n - idx1;
            int r1 = rank[idx1];
            int r2 = rank[idx2];
            if (r1 > r2) swap(r1, r2);
            return stLCP->queryMin(r1 + 1, r2);
        }
    };
}

void demoProductionGroup() {
    cout << "\n===================================================\n";
    cout << " 2. NHÓM THUẬT TOÁN PRODUCTION (THỰC TẾ & SYSTEM)\n";
    cout << "===================================================\n";

    // Demo Sensor Data
    vector<double> temps = {24.5, 25.0, 28.2, 31.5, 30.0, 27.8, 22.1, 21.0};
    Production_SparseTable::HistoricalSensorData sensor(temps);
    cout << "  [Static Sensor Analytics O(1)] Max Temp từ phút 1..5: " 
         << sensor.getMaxTemperature(1, 5) << "°C (Expected: 31.5°C)\n";

    // Demo File System Binary Jump
    vector<int> blockPointers = {1, 2, 3, 4, 5, 6, -1}; // Chuỗi Khối liên kết: 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6
    Production_SparseTable::StaticFileSystemIndexer fsIndexer(blockPointers);
    cout << "  [Embedded FS Jump Table O(log K)] Từ Block 0 nhảy 5 bước: Block " 
         << fsIndexer.getBlockAfterSteps(0, 5) << " (Expected: 5)\n";

    // Demo LCP Engine
    string text = "banana";
    Production_SparseTable::LCP_SearchIndexer searchIndexer(text);
    cout << "  [LCP Search Engine O(1)] LCP của \"ana\" (idx 1) và \"ana\" (idx 3) trong \"banana\": " 
         << searchIndexer.queryLCP(1, 3) << " (Expected: 3)\n";
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "===================================================\n";
    cout << " 🚀 TRỌN BỘ CÁC BẢN CÀI ĐẶT SPARSE TABLE KINH ĐIỂN\n";
    cout << "===================================================\n";

    demoCPGroup();
    demoProductionGroup();

    return 0;
}