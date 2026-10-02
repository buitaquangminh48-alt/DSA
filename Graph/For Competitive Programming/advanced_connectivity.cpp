#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

// ============================================================================
// 1. THUẬT TOÁN TARJAN (TÌM THÀNH PHẦN LIÊN THÔNG MẠNH - SCC)
// Áp dụng: Đồ thị CÓ HƯỚNG
// Độ phức tạp: O(V + E)
// ============================================================================
int timer_scc = 0, scc_count = 0;
vector<int> num_scc, low_scc;
vector<bool> in_stack;
stack<int> st;

void tarjanDfs(int u, const vector<vector<int>>& adj) {
    num_scc[u] = low_scc[u] = ++timer_scc;
    st.push(u);
    in_stack[u] = true;

    cout << "  -> Thăm đỉnh " << u << " | num[" << u << "] = low[" << u << "] = " << num_scc[u] << "\n";

    for (int v : adj[u]) {
        if (num_scc[v] == 0) { // v chưa được thăm
            tarjanDfs(v, adj);
            low_scc[u] = min(low_scc[u], low_scc[v]);
            cout << "  Backtrack (" << u << " <- " << v << "): Cập nhật low[" << u << "] = " << low_scc[u] << "\n";
        } else if (in_stack[v]) { // v nằm trong Stack (Cạnh ngược / Back-edge)
            low_scc[u] = min(low_scc[u], num_scc[v]);
            cout << "  Cạnh ngược (" << u << " -> " << v << "): Cập nhật low[" << u << "] = " << low_scc[u] << "\n";
        }
    }

    // Nếu u là gốc của một SCC
    if (low_scc[u] == num_scc[u]) {
        scc_count++;
        cout << "\n  [TÌM THẤY SCC #" << scc_count << "] Các đỉnh: ";
        while (true) {
            int v = st.top();
            st.pop();
            in_stack[v] = false;
            cout << v << " ";
            if (u == v) break;
        }
        cout << "\n\n";
    }
}

void tarjanTrace(int V, const vector<vector<int>>& adj) {
    cout << "\n===================================================\n";
    cout << " 1. TARJAN ALGORITHM (TÌM THÀNH PHẦN LIÊN THÔNG MẠNH - SCC)\n";
    cout << "===================================================\n";

    timer_scc = scc_count = 0;
    num_scc.assign(V + 1, 0);
    low_scc.assign(V + 1, 0);
    in_stack.assign(V + 1, false);
    while (!st.empty()) st.pop();

    for (int i = 1; i <= V; i++) {
        if (num_scc[i] == 0) {
            tarjanDfs(i, adj);
        }
    }
    cout << "=> TỔNG SỐ THÀNH PHẦN LIÊN THÔNG MẠNH (SCC): " << scc_count << "\n";
}

// ============================================================================
// 2 & 3. TÌM CẦU (BRIDGES) VÀ TÌM KHỚP (ARTICULATION POINTS)
// Áp dụng: Đồ thị VÔ HƯỚNG
// Độ phức tạp: O(V + E)
// ============================================================================
int timer_bc = 0;
vector<int> num_bc, low_bc;
vector<bool> is_cut_vertex;
vector<pair<int, int>> bridges;

void findBridgesAndCutsDfs(int u, int p, const vector<vector<int>>& adj) {
    num_bc[u] = low_bc[u] = ++timer_bc;
    int child_count = 0; // Đếm số con trực tiếp trong cây DFS

    cout << "  -> DFS ghé đỉnh " << u << " | num[" << u << "] = low[" << u << "] = " << num_bc[u] << "\n";

    for (int v : adj[u]) {
        if (v == p) continue; // Bỏ qua cạnh đi ngược về cha trực tiếp

        if (num_bc[v] != 0) {
            // Cạnh ngược (Back-edge)
            low_bc[u] = min(low_bc[u], num_bc[v]);
            cout << "  Cạnh ngược (" << u << " -- " << v << "): low[" << u << "] = " << low_bc[u] << "\n";
        } else {
            // Cạnh cây DFS
            child_count++;
            findBridgesAndCutsDfs(v, u, adj);
            low_bc[u] = min(low_bc[u], low_bc[v]);

            cout << "  Backtrack (" << u << " <-- " << v << "): Cập nhật low[" << u << "] = " << low_bc[u] << "\n";

            // 1. ĐIỀU KIỆN TÌM CẦU (BRIDGE): low[v] > num[u]
            if (low_bc[v] > num_bc[u]) {
                bridges.push_back({u, v});
                cout << "    [TÌM THẤY CẦU]: (" << u << " -- " << v << ")\n";
            }

            // 2. ĐIỀU KIỆN TÌM KHỚP (CUT VERTEX) CHO ĐỈNH KHÔNG PHẢI GỐC: low[v] >= num[u]
            if (p != -1 && low_bc[v] >= num_bc[u]) {
                if (!is_cut_vertex[u]) {
                    is_cut_vertex[u] = true;
                    cout << "    [TÌM THẤY KHỚP]: Đỉnh " << u << "\n";
                }
            }
        }
    }

    // ĐIỀU KIỆN TÌM KHỚP CHO ĐỈNH GỐC CÂY DFS (p == -1): Có >= 2 con trực tiếp
    if (p == -1 && child_count > 1) {
        if (!is_cut_vertex[u]) {
            is_cut_vertex[u] = true;
            cout << "    [TÌM THẤY KHỚP (GỐC DFS)]: Đỉnh " << u << " (Có " << child_count << " con)\n";
        }
    }
}

void bridgesAndCutsTrace(int V, const vector<vector<int>>& adj) {
    cout << "\n===================================================\n";
    cout << " 2 & 3. TÌM CẦU (BRIDGES) & TÌM KHỚP (CUT VERTICES)\n";
    cout << "===================================================\n";

    timer_bc = 0;
    num_bc.assign(V + 1, 0);
    low_bc.assign(V + 1, 0);
    is_cut_vertex.assign(V + 1, false);
    bridges.clear();

    for (int i = 1; i <= V; i++) {
        if (num_bc[i] == 0) {
            findBridgesAndCutsDfs(i, -1, adj);
        }
    }

    cout << "\n---------------------------------------------------\n";
    cout << " KẾT QUẢ TỔNG HỢP:\n";
    cout << "---------------------------------------------------\n";
    
    // In danh sách Cầu
    cout << " Các CẦU (Bridges) trong đồ thị:\n";
    if (bridges.empty()) cout << "  (Không có)\n";
    for (auto b : bridges) {
        cout << "  - Cạnh (" << b.first << " -- " << b.second << ")\n";
    }

    // In danh sách Khớp
    cout << "\n Các KHỚP (Cut Vertices) trong đồ thị:\n  ";
    bool has_cut = false;
    for (int i = 1; i <= V; i++) {
        if (is_cut_vertex[i]) {
            cout << i << " ";
            has_cut = true;
        }
    }
    if (!has_cut) cout << "(Không có)";
    cout << "\n";
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ------------------------------------------------------------------------
    // Đồ thị 1: CÓ HƯỚNG thử nghiệm Tarjan SCC (V = 7, E = 9)
    // ------------------------------------------------------------------------
    int V1 = 7;
    vector<vector<int>> adj_directed(V1 + 1);

    adj_directed[1] = {2};
    adj_directed[2] = {3, 4};
    adj_directed[3] = {1};
    adj_directed[4] = {5};
    adj_directed[5] = {6};
    adj_directed[6] = {4, 7};
    adj_directed[7] = {};

    tarjanTrace(V1, adj_directed);

    // ------------------------------------------------------------------------
    // Đồ thị 2: VÔ HƯỚNG thử nghiệm Cầu & Khớp (V = 6, E = 7)
    // ------------------------------------------------------------------------
    int V2 = 6;
    vector<vector<int>> adj_undirected(V2 + 1);

    auto addUndirectedEdge = [&](int u, int v) {
        adj_undirected[u].push_back(v);
        adj_undirected[v].push_back(u);
    };

    addUndirectedEdge(1, 2);
    addUndirectedEdge(2, 3);
    addUndirectedEdge(3, 1);
    addUndirectedEdge(3, 4); // Cầu (3-4), Khớp (3 & 4)
    addUndirectedEdge(4, 5);
    addUndirectedEdge(5, 6);
    addUndirectedEdge(6, 4);

    bridgesAndCutsTrace(V2, adj_undirected);

    return 0;
}