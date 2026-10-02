#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <cmath>
#include <tuple>

using namespace std;

// ============================================================================
// NHÓM 1: BẢN CÀI ĐẶT KINH ĐIỂN CHO CP (COMPETITIVE PROGRAMMING)
// ============================================================================

namespace CP_DP {

    // ------------------------------------------------------------------------
    // 1.1 LIS - Longest Increasing Subsequence O(N log N) dùng Binary Search
    // ------------------------------------------------------------------------
    int LIS_Optimized(const vector<int>& nums) {
        vector<int> tails;
        for (int x : nums) {
            auto it = lower_bound(tails.begin(), tails.end(), x);
            if (it == tails.end()) {
                tails.push_back(x);
            } else {
                *it = x;
            }
        }
        return tails.size();
    }

    // ------------------------------------------------------------------------
    // 1.2 Knapsack DP - Cái túi 0-1 (Tối ưu mảng 1D cuộn từ O(N*W) -> O(W) Space)
    // ------------------------------------------------------------------------
    int knapsack01(int W, const vector<int>& weights, const vector<int>& values) {
        int n = weights.size();
        vector<int> dp(W + 1, 0);

        for (int i = 0; i < n; ++i) {
            // Duyệt ngược từ W về weights[i] để tránh dùng lặp lại cùng 1 phần tử
            for (int w = W; w >= weights[i]; --w) {
                dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
            }
        }
        return dp[W];
    }

    // ------------------------------------------------------------------------
    // 1.3 Tree DP - Tìm Tập độc lập có trọng số lớn nhất trên Cây (Maximum Weight Independent Set)
    // ------------------------------------------------------------------------
    void dfsTreeDP(int u, int p, const vector<vector<int>>& adj, const vector<int>& weight, 
                   vector<int>& dp0, vector<int>& dp1) {
        dp0[u] = 0;         // Không chọn đỉnh u
        dp1[u] = weight[u]; // Chọn đỉnh u

        for (int v : adj[u]) {
            if (v == p) continue;
            dfsTreeDP(v, u, adj, weight, dp0, dp1);
            
            // Nếu không chọn u -> con v chọn hay không chọn đều được (lấy max)
            dp0[u] += max(dp0[v], dp1[v]);
            // Nếu chọn u -> bắt buộc không được chọn con v
            dp1[u] += dp0[v];
        }
    }

    // ------------------------------------------------------------------------
    // 1.4 Bitmask DP - Travelling Salesperson Problem (TSP) O(2^N * N^2)
    // ------------------------------------------------------------------------
    const int INF = 1e9;
    int tsp(int mask, int pos, int n, const vector<vector<int>>& dist, vector<vector<int>>& memo) {
        if (mask == (1 << n) - 1) return dist[pos][0];
        if (memo[mask][pos] != -1) return memo[mask][pos];

        int ans = INF;
        for (int nxt = 0; nxt < n; ++nxt) {
            if (!(mask & (1 << nxt))) {
                int newCost = dist[pos][nxt] + tsp(mask | (1 << nxt), nxt, n, dist, memo);
                ans = min(ans, newCost);
            }
        }
        return memo[mask][pos] = ans;
    }

    // ------------------------------------------------------------------------
    // 1.5 Digit DP - Đếm số lượng số trong [0, N] có chữ số không lặp lại
    // ------------------------------------------------------------------------
    long long memoDigit[20][2][1 << 10];
    long long solveDigitDP(const string& S, int idx, bool tight, int mask) {
        if (idx == S.length()) return mask > 0 ? 1 : 0;
        if (memoDigit[idx][tight][mask] != -1) return memoDigit[idx][tight][mask];

        int limit = tight ? (S[idx] - '0') : 9;
        long long ans = 0;

        for (int d = 0; d <= limit; ++d) {
            bool newTight = tight && (d == limit);
            if (mask & (1 << d)) continue;
            
            int newMask = (mask == 0 && d == 0) ? 0 : (mask | (1 << d));
            ans += solveDigitDP(S, idx + 1, newTight, newMask);
        }
        return memoDigit[idx][tight][mask] = ans;
    }

    // ------------------------------------------------------------------------
    // 1.6 Convex Hull Trick (CHT) - Tối ưu DP O(N^2) -> O(N log N) / O(N)
    // ------------------------------------------------------------------------
    struct Line {
        long long m, c;
        long long eval(long long x) const { return m * x + c; }
        double intersect(const Line& l) const { return (double)(l.c - c) / (m - l.m); }
    };

    class ConvexHullTrick {
    private:
        vector<Line> hull;
    public:
        void addLine(long long m, long long c) {
            Line newLine = {m, c};
            while (hull.size() >= 2) {
                Line l1 = hull[hull.size() - 2];
                Line l2 = hull.back();
                if (newLine.intersect(l2) <= l2.intersect(l1)) {
                    hull.pop_back();
                } else break;
            }
            hull.push_back(newLine);
        }

        long long query(long long x) {
            int l = 0, r = hull.size() - 1;
            long long res = hull[0].eval(x);
            while (l <= r) {
                int mid = l + (r - l) / 2;
                long long val1 = hull[mid].eval(x);
                res = min(res, val1);
                if (mid + 1 < hull.size() && hull[mid + 1].eval(x) < val1) {
                    l = mid + 1;
                } else {
                    r = mid - 1;
                }
            }
            return res;
        }
    };
}

void demoCPGroup() {
    cout << "\n===================================================\n";
    cout << " 1. NHÓM THUẬT TOÁN CP (COMPETITIVE PROGRAMMING)\n";
    cout << "===================================================\n";

    // Demo LIS
    vector<int> arr = {10, 9, 2, 5, 3, 7, 101, 18};
    cout << "  [LIS O(N log N)] Độ dài LIS: " << CP_DP::LIS_Optimized(arr) << "\n";

    // Demo Knapsack 0-1
    vector<int> weights = {2, 3, 4, 5};
    vector<int> values = {3, 4, 5, 6};
    int W = 5;
    cout << "  [0-1 Knapsack O(W) Space] Giá trị tối đa thu được: " << CP_DP::knapsack01(W, weights, values) << "\n";

    // Demo Tree DP
    int n = 5;
    vector<vector<int>> adj(n);
    vector<int> node_weight = {10, 20, 15, 25, 30};
    // Dựng cây 0 - 1, 0 - 2, 1 - 3, 1 - 4
    adj[0] = {1, 2}; adj[1] = {0, 3, 4}; adj[2] = {0}; adj[3] = {1}; adj[4] = {1};
    vector<int> dp0(n, 0), dp1(n, 0);
    CP_DP::dfsTreeDP(0, -1, adj, node_weight, dp0, dp1);
    cout << "  [Tree DP] Maximum Weight Independent Set: " << max(dp0[0], dp1[0]) << "\n";

    // Demo TSP
    vector<vector<int>> dist = {
        {0, 20, 42, 25}, {20, 0, 30, 34},
        {42, 30, 0, 10}, {25, 34, 10, 0}
    };
    vector<vector<int>> memo(1 << 4, vector<int>(4, -1));
    cout << "  [Bitmask DP] TSP Chi phí nhỏ nhất: " << CP_DP::tsp(1, 0, 4, dist, memo) << "\n";

    // Demo Digit DP
    string N = "100";
    for(int i=0; i<20; ++i) for(int j=0; j<2; ++j) for(int k=0; k<(1<<10); ++k) CP_DP::memoDigit[i][j][k] = -1;
    cout << "  [Digit DP] Số các số <= 100 không trùng chữ số: " << CP_DP::solveDigitDP(N, 0, true, 0) << "\n";
}

// ============================================================================
// NHÓM 2: BẢN CÀI ĐẶT KINH ĐIỂN CHO THỰC TẾ (PRODUCTION)
// ============================================================================

namespace Production_DP {

    // ------------------------------------------------------------------------
    // 2.1 Space-Optimized Levenshtein Distance (Dùng cho Spell Checker / Fuzzy String Matching)
    // Tối ưu bộ nhớ cuộn từ O(M*N) xuống O(N) bằng 2 hàng mảng phẳng
    // ------------------------------------------------------------------------
    int editDistance(const string& s1, const string& s2) {
        int m = s1.length(), n = s2.length();
        vector<int> prev(n + 1, 0), curr(n + 1, 0);

        for (int j = 0; j <= n; ++j) prev[j] = j;

        for (int i = 1; i <= m; ++i) {
            curr[0] = i;
            for (int j = 1; j <= n; ++j) {
                if (s1[i - 1] == s2[j - 1]) {
                    curr[j] = prev[j - 1];
                } else {
                    curr[j] = 1 + min({prev[j],      // Delete
                                      curr[j - 1],   // Insert
                                      prev[j - 1]}); // Replace
                }
            }
            prev = curr;
        }
        return prev[n];
    }

    // ------------------------------------------------------------------------
    // 2.2 Spell Checker & Fuzzy String Matching (Gợi ý từ gõ sai dựa trên Từ điển)
    // ------------------------------------------------------------------------
    class SpellChecker {
    private:
        vector<string> dictionary;
    public:
        SpellChecker(const vector<string>& dict) : dictionary(dict) {}

        vector<string> suggest(const string& query, int maxDistance = 2) {
            vector<string> suggestions;
            for (const auto& word : dictionary) {
                if (abs((int)word.length() - (int)query.length()) > maxDistance) continue;
                
                int dist = editDistance(query, word);
                if (dist <= maxDistance) {
                    suggestions.push_back(word);
                }
            }
            return suggestions;
        }
    };

    // ------------------------------------------------------------------------
    // 2.3 Diff Algorithm - Tìm LCS và khôi phục mảng khác biệt (Git Diff)
    // ------------------------------------------------------------------------
    void gitDiff(const vector<string>& fileA, const vector<string>& fileB) {
        int m = fileA.size(), n = fileB.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (fileA[i - 1] == fileB[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        // Khôi phục đường đi Diff
        int i = m, j = n;
        vector<string> diffOutput;
        while (i > 0 || j > 0) {
            if (i > 0 && j > 0 && fileA[i - 1] == fileB[j - 1]) {
                diffOutput.push_back("  " + fileA[i - 1]);
                i--; j--;
            } else if (j > 0 && (i == 0 || dp[i][j - 1] >= dp[i - 1][j])) {
                diffOutput.push_back("+ " + fileB[j - 1]);
                j--;
            } else if (i > 0 && (j == 0 || dp[i][j - 1] < dp[i - 1][j])) {
                diffOutput.push_back("- " + fileA[i - 1]);
                i--;
            }
        }

        reverse(diffOutput.begin(), diffOutput.end());
        cout << "  --- GIT DIFF OUTPUT ---\n";
        for (const auto& line : diffOutput) {
            cout << "  " << line << "\n";
        }
    }

    // ------------------------------------------------------------------------
    // 2.4 Viterbi Algorithm - Tìm chuỗi trạng thái ẩn có xác suất cao nhất (NLP)
    // ------------------------------------------------------------------------
    void viterbiDemo() {
        vector<string> states = {"Healthy", "Fever"};
        vector<int> seq = {0, 1, 2}; // Normal -> Cold -> Dizzy
        int T = seq.size();

        double start_p[] = {0.6, 0.4};
        double trans_p[2][2] = {{0.7, 0.3}, {0.4, 0.6}};
        double emit_p[2][3] = {{0.5, 0.4, 0.1}, {0.1, 0.3, 0.6}};

        vector<vector<double>> V(T, vector<double>(2, 0.0));
        vector<vector<int>> path(T, vector<int>(2, 0));

        for (int s = 0; s < 2; ++s) {
            V[0][s] = start_p[s] * emit_p[s][seq[0]];
            path[0][s] = s;
        }

        for (int t = 1; t < T; ++t) {
            for (int s = 0; s < 2; ++s) {
                double max_prob = -1.0;
                int best_prev = -1;
                for (int prev = 0; prev < 2; ++prev) {
                    double prob = V[t - 1][prev] * trans_p[prev][s] * emit_p[s][seq[t]];
                    if (prob > max_prob) {
                        max_prob = prob;
                        best_prev = prev;
                    }
                }
                V[t][s] = max_prob;
                path[t][s] = best_prev;
            }
        }

        int best_last = (V[T - 1][0] > V[T - 1][1]) ? 0 : 1;
        vector<int> result(T);
        result[T - 1] = best_last;
        for (int t = T - 1; t > 0; --t) {
            result[t - 1] = path[t][result[t]];
        }

        cout << "  [Viterbi] Chuỗi triệu chứng: Normal -> Cold -> Dizzy\n";
        cout << "  [Viterbi] Chẩn đoán trạng thái ẩn tối ưu: ";
        for (int st : result) cout << states[st] << " -> ";
        cout << "END\n";
    }
}

void demoProductionGroup() {
    cout << "\n===================================================\n";
    cout << " 2. NHÓM THUẬT TOÁN PRODUCTION (THỰC TẾ & SYSTEM)\n";
    cout << "===================================================\n";

    // Demo Spell Checker & Fuzzy String Matching
    vector<string> dict = {"apple", "apply", "banana", "cat", "algorithm"};
    Production_DP::SpellChecker checker(dict);
    string query = "aple"; // Gõ sai từ "apple"
    cout << "  [Spell Checker] Từ gốc gõ sai: '" << query << "' -> Gợi ý từ điển: ";
    for (const auto& w : checker.suggest(query)) {
        cout << "'" << w << "' ";
    }
    cout << "\n";

    // Demo Git Diff
    vector<string> fileV1 = {"const int x = 10;", "cout << x;", "return 0;"};
    vector<string> fileV2 = {"const int x = 20;", "cout << x << endl;", "return 0;"};
    Production_DP::gitDiff(fileV1, fileV2);

    // Demo Viterbi
    Production_DP::viterbiDemo();
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "===================================================\n";
    cout << " 🚀 TRỌN BỘ CÁC CẤU TRÚC VÀ DẠNG BÀI DP KINH ĐIỂN\n";
    cout << "===================================================\n";

    demoCPGroup();
    demoProductionGroup();

    return 0;
}