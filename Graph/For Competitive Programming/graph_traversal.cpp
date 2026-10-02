#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>

using namespace std;

// Số đỉnh đồ thị demo
const int MAXN = 100;

// ============================================================================
// 1. DUYỆT THEO CHIỀU SÂU (DFS - DEPTH-FIRST SEARCH)
// Độ phức tạp: O(V + E)
// ----------------------------------------------------------------------------
void dfsHelper(int u, const vector<vector<int>>& adj, vector<bool>& visited) {
    visited[u] = true;
    cout << u << " ";

    for (int v : adj[u]) {
        if (!visited[v]) {
            dfsHelper(v, adj, visited);
        }
    }
}

void dfsTrace(int start, int V, const vector<vector<int>>& adj) {
    cout << "\n===================================================\n";
    cout << " 1. DFS (DEPTH-FIRST SEARCH) - BAT DAU TU DINH " << start << "\n";
    cout << "===================================================\n";
    
    vector<bool> visited(V + 1, false);
    cout << "Thu tu duyet DFS: ";
    dfsHelper(start, adj, visited);
    cout << "\n";
}

// ============================================================================
// 2. DUYỆT THEO CHIỀU RỘNG (BFS - BREADTH-FIRST SEARCH)
// Độ phức tạp: O(V + E) - Tìm đường đi ngắn nhất đồ thị KHÔNG TRỌNG SỐ
// ----------------------------------------------------------------------------
void bfsTrace(int start, int V, const vector<vector<int>>& adj) {
    cout << "\n===================================================\n";
    cout << " 2. BFS (BREADTH-FIRST SEARCH) - BAT DAU TU DINH " << start << "\n";
    cout << "===================================================\n";

    vector<bool> visited(V + 1, false);
    vector<int> dist(V + 1, -1); // Khoảng cách từ start
    queue<int> q;

    q.push(start);
    visited[start] = true;
    dist[start] = 0;

    int step = 1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        cout << "Step " << step++ << ": Lay dinh " << u 
             << " (Dist = " << dist[u] << ") -> Duyet cac dinh ke: ";

        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                q.push(v);
                cout << v << "(Dist=" << dist[v] << ") ";
            }
        }
        cout << "\n";
    }

    cout << "\n-> Bang khoang cach ngan nhat tu dinh " << start << ":\n";
    for (int i = 1; i <= V; i++) {
        cout << "  Dinh " << i << ": " << dist[i] << "\n";
    }
}

// ============================================================================
// 3. 0-1 BFS (BIẾN THỂ DÙNG STD::DEQUE CHO ĐỒ THỊ TRỌNG SỐ 0 VÀ 1)
// Độ phức tạp: O(V + E) - Tối ưu hơn Dijkstra O((V + E) log V)
// ----------------------------------------------------------------------------
struct Edge {
    int to;
    int weight; // Trọng số chỉ nhận giá trị 0 hoặc 1
};

void zeroOneBfsTrace(int start, int V, const vector<vector<Edge>>& adj01) {
    cout << "\n===================================================\n";
    cout << " 3. 0-1 BFS (STD::DEQUE) - BAT DAU TU DINH " << start << "\n";
    cout << "===================================================\n";

    const int INF = 1e9;
    vector<int> dist(V + 1, INF);
    deque<int> dq;

    dist[start] = 0;
    dq.push_back(start);

    int step = 1;
    while (!dq.empty()) {
        int u = dq.front();
        dq.pop_front();

        cout << "Step " << step++ << ": Lay dinh " << u << " (Dist = " << dist[u] << ")\n";

        for (auto edge : adj01[u]) {
            int v = edge.to;
            int w = edge.weight;

            // Kỹ thuật Relax (Thư giãn cạnh)
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                cout << "  -> Cua ngat dist[" << v << "] = " << dist[v] << " (w = " << w << ") => ";

                // Trọng số 0 -> Đẩy lên ĐẦU hàng đợi (Ưu tiên cao hơn)
                // Trọng số 1 -> Đẩy vào CUỐI hàng đợi
                if (w == 0) {
                    dq.push_front(v);
                    cout << "Push FRONT " << v << "\n";
                } else {
                    dq.push_back(v);
                    cout << "Push BACK " << v << "\n";
                }
            }
        }
    }

    cout << "\n-> Bang khoang cach 0-1 BFS tu dinh " << start << ":\n";
    for (int i = 1; i <= V; i++) {
        cout << "  Dinh " << i << ": " << dist[i] << "\n";
    }
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ------------------------------------------------------------------------
    // Đồ thị 1: Đồ thị không trọng số cho BFS & DFS (V = 6, E = 7)
    // Đỉnh: 1..6
    // ------------------------------------------------------------------------
    int V1 = 6;
    vector<vector<int>> adj1(V1 + 1);
    
    // Biểu diễn danh sách kề
    adj1[1] = {2, 3};
    adj1[2] = {1, 4, 5};
    adj1[3] = {1, 6};
    adj1[4] = {2};
    adj1[5] = {2, 6};
    adj1[6] = {3, 5};

    dfsTrace(1, V1, adj1);
    bfsTrace(1, V1, adj1);

    // ------------------------------------------------------------------------
    // Đồ thị 2: Đồ thị có trọng số 0 và 1 cho 0-1 BFS (V = 5)
    // ------------------------------------------------------------------------
    int V2 = 5;
    vector<vector<Edge>> adj01(V2 + 1);

    adj01[1] = {{2, 1}, {3, 0}};
    adj01[2] = {{4, 0}};
    adj01[3] = {{2, 0}, {5, 1}};
    adj01[4] = {{5, 0}};
    adj01[5] = {};

    zeroOneBfsTrace(1, V2, adj01);

    return 0;
}