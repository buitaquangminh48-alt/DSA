#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

const long long INF = 1e18;

// ============================================================================
// 1. THUẬT TOÁN DINIC (LUỒNG CỰC ĐẠI - MAX FLOW)
// Độ phức tạp: O(V^2 * E) chung, O(E * sqrt(V)) cho đồ thị đơn/unit network.
// Cấu trúc: Cạnh thặng dư (Residual Graph) + Cây BFS Level + DFS Tăng luồng
// ============================================================================
struct DinicEdge {
    int to;
    long long cap;
    long long flow;
    int rev; // Chỉ số của cạnh ngược lại trong adj[to]
};

struct Dinic {
    int n;
    vector<vector<DinicEdge>> adj;
    vector<int> level;
    vector<int> ptr; // Pointer optimization cho DFS

    Dinic(int n) : n(n), adj(n + 1), level(n + 1), ptr(n + 1) {}

    void addEdge(int from, int to, long long cap) {
        adj[from].push_back({to, cap, 0, (int)adj[to].size()});
        // Cạnh ngược có dung lượng ban đầu = 0
        adj[to].push_back({from, 0, 0, (int)adj[from].size() - 1});
    }

    // Bước 1: Duyệt BFS xây dựng Cây Level Graph
    bool bfsTrace(int s, int t, int phase) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);

        cout << "\n--- PHASE " << phase << " [BFS - Xây dựng Level Graph] ---\n";

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const auto& edge : adj[u]) {
                if (edge.cap - edge.flow > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                    cout << "  Gán Level[" << edge.to << "] = " << level[edge.to] 
                         << " (qua cạnh " << u << "->" << edge.to 
                         << " | Dung lượng còn lại = " << edge.cap - edge.flow << ")\n";
                }
            }
        }
        return level[t] != -1;
    }

    // Bước 2: Duyệt DFS tìm Luồng tắc nghẽn (Blocking Flow)
    long long dfsTrace(int u, int t, long long pushed, vector<int>& path) {
        if (pushed == 0) return 0;
        if (u == t) return pushed;

        for (int& cid = ptr[u]; cid < (int)adj[u].size(); ++cid) {
            auto& edge = adj[u][cid];
            int v = edge.to;

            if (level[u] + 1 != level[v] || edge.cap - edge.flow == 0) continue;

            path.push_back(v);
            long long tr = dfsTrace(v, t, min(pushed, edge.cap - edge.flow), path);
            
            if (tr == 0) {
                path.pop_back();
                continue;
            }

            // Cập nhật luồng trên cạnh xuôi và cạnh ngược
            edge.flow += tr;
            adj[v][edge.rev].flow -= tr;
            return tr;
        }
        return 0;
    }

    long long maxFlowTrace(int s, int t) {
        cout << "\n===================================================\n";
        cout << " 1. THUẬT TOÁN DINIC (MAX FLOW) - NGUỒN " << s << " HỐ " << t << "\n";
        cout << "===================================================\n";

        long long flow = 0;
        int phase = 1;

        while (bfsTrace(s, t, phase)) {
            fill(ptr.begin(), ptr.end(), 0);
            
            cout << "--- PHASE " << phase << " [DFS - Tìm đường tăng luồng] ---\n";
            while (true) {
                vector<int> path = {s};
                long long pushed = dfsTrace(s, t, INF, path);
                if (pushed == 0) break;

                flow += pushed;
                cout << "  -> Tìm thấy đường tăng luồng: ";
                for (size_t i = 0; i < path.size(); ++i) {
                    cout << path[i] << (i + 1 < path.size() ? " -> " : "");
                }
                cout << " | Luồng tăng thêm +=" << pushed << " (Tổng luồng: " << flow << ")\n";
            }
            phase++;
        }

        cout << "\n=> [KẾT QUẢ DINIC] LUỒNG CỰC ĐẠI (MAX FLOW) = " << flow << "\n";
        return flow;
    }
};

// ============================================================================
// 2. THUẬT TOÁN KUHN (MAXIMUM BIPARTITE MATCHING - GHÉP CẶP ĐỒ THỊ HAI PHÍA)
// Độ phức tạp: O(V * E)
// Áp dụng: Tìm tập các cạnh không chung đỉnh có số lượng lớn nhất trên Đồ thị 2 phía
// ============================================================================
struct KuhnMatching {
    int n_left, n_right;
    vector<vector<int>> adj;
    vector<int> match_right; // match_right[v] = u (Đỉnh v phía Phải ghép với u phía Trái)
    vector<bool> visited;

    KuhnMatching(int n_left, int n_right) 
        : n_left(n_left), n_right(n_right), adj(n_left + 1), match_right(n_right + 1, 0) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v); // Hướng từ tập Trái (u) sang tập Phải (v)
    }

    // DFS tìm đường tăng cặp (Augmenting Path)
    bool tryKuhnTrace(int u) {
        for (int v : adj[u]) {
            if (visited[v]) continue;
            visited[v] = true;

            // Nếu đỉnh v phía Phải chưa ghép, HOẶC đỉnh đang ghép với v có thể tìm đỉnh ghép mới
            if (match_right[v] == 0 || tryKuhnTrace(match_right[v])) {
                if (match_right[v] != 0) {
                    cout << "    * Đẩy đỉnh Trái " << match_right[v] << " tìm ghép mới thành công => ";
                }
                match_right[v] = u;
                cout << "Ghép Trái " << u << " <---> Phải " << v << "\n";
                return true;
            }
        }
        return false;
    }

    int maxMatchingTrace() {
        cout << "\n===================================================\n";
        cout << " 2. THUẬT TOÁN KUHN (GHÉP CẶP ĐỒ THỊ HAI PHÍA)\n";
        cout << "===================================================\n";

        int matching = 0;
        for (int u = 1; u <= n_left; ++u) {
            visited.assign(n_right + 1, false);
            cout << "Xét đỉnh Trái u = " << u << ":\n";
            if (tryKuhnTrace(u)) {
                matching++;
            } else {
                cout << "  -> Không tìm được đường tăng cặp cho Trái " << u << "\n";
            }
        }

        cout << "\n=> [KẾT QUẢ KUHN] SỐ CẶP GHÉP CỰC ĐẠI = " << matching << "\n";
        cout << "Danh sách các cặp ghép thành công:\n";
        for (int v = 1; v <= n_right; ++v) {
            if (match_right[v] != 0) {
                cout << "  Trái [" << match_right[v] << "] <---> Phải [" << v << "]\n";
            }
        }
        return matching;
    }
};

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ------------------------------------------------------------------------
    // Đồ thị 1: Mạng luồng cho Dinik Max Flow (Nguồn S=1, Hố T=6)
    // ------------------------------------------------------------------------
    int V_flow = 6;
    Dinic dinic(V_flow);

    dinic.addEdge(1, 2, 10);
    dinic.addEdge(1, 3, 10);
    dinic.addEdge(2, 3, 2);
    dinic.addEdge(2, 4, 4);
    dinic.addEdge(2, 5, 8);
    dinic.addEdge(3, 5, 9);
    dinic.addEdge(4, 6, 10);
    dinic.addEdge(5, 4, 6);
    dinic.addEdge(5, 6, 10);

    dinic.maxFlowTrace(1, 6);

    // ------------------------------------------------------------------------
    // Đồ thị 2: Đồ thị hai phía cho Kuhn Bipartite Matching
    // Tập Trái: {1, 2, 3, 4}, Tập Phải: {1, 2, 3, 4}
    // ------------------------------------------------------------------------
    int n_left = 4, n_right = 4;
    KuhnMatching kuhn(n_left, n_right);

    kuhn.addEdge(1, 1);
    kuhn.addEdge(1, 2);
    kuhn.addEdge(2, 2);
    kuhn.addEdge(2, 3);
    kuhn.addEdge(3, 1);
    kuhn.addEdge(3, 3);
    kuhn.addEdge(4, 3);

    kuhn.maxMatchingTrace();

    return 0;
}