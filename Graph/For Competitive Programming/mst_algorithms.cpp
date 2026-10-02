#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// ============================================================================
// 1. CẤU TRÚC DỮ LIỆU DSU (DISJOINT SET UNION - TAP HỢP RỜI RẠC)
// Dùng cho thuật toán Kruskal (Tối ưu bằng Path Compression & Union by Rank/Size)
// ----------------------------------------------------------------------------
struct DSU {
    vector<int> parent;
    vector<int> sz;

    DSU(int n) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        for (int i = 1; i <= n; i++) parent[i] = i;
    }

    // Nén đường đi (Path Compression) -> O(alpha(N))
    int find(int u) {
        if (u == parent[u]) return u;
        return parent[u] = find(parent[u]);
    }

    // Hợp nhất 2 tập hợp (Union by Size)
    bool unite(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) return false; // u và v đã thuộc cùng một thành phần liên thông
        if (sz[u] < sz[v]) swap(u, v);
        parent[v] = u;
        sz[u] += sz[v];
        return true;
    }
};

// Cấu trúc cạnh dành cho Kruskal
struct Edge {
    int u, v;
    int weight;

    // Sắp xếp cạnh theo trọng số tăng dần
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// ============================================================================
// 1. THUẬT TOÁN KRUSKAL (DỰA TRÊN CẠNH + DSU)
// Độ phức tạp: O(E log E) hoặc O(E log V)
// Áp dụng: Đồ thị thưa, dễ cài đặt, quốc dân cho CP.
// ----------------------------------------------------------------------------
void kruskalTrace(int V, vector<Edge> edges) {
    cout << "\n===================================================\n";
    cout << " 1. THUẬT TOÁN KRUSKAL (KẾT HỢP DSU)\n";
    cout << "===================================================\n";

    // Bước 1: Sắp xếp các cạnh theo trọng số tăng dần
    sort(edges.begin(), edges.end());

    DSU dsu(V);
    int mst_weight = 0;
    int edges_count = 0;

    cout << "Cac canh sau khi sap xep theo trong so:\n";
    for (const auto& e : edges) {
        cout << "  (" << e.u << " - " << e.v << ", w = " << e.weight << ")\n";
    }
    cout << "\n--- Bat dau ket noi bang DSU ---\n";

    int step = 1;
    for (const auto& e : edges) {
        // Nếu u và v thuộc 2 tập hợp khác nhau -> Thêm cạnh vào MST
        if (dsu.unite(e.u, e.v)) {
            mst_weight += e.weight;
            edges_count++;
            cout << "Step " << step++ << ": CHON canh (" << e.u << " - " << e.v 
                 << ", w = " << e.weight << ") | Tong Trong So hien tai = " << mst_weight << "\n";

            // MST luôn có đúng (V - 1) cạnh
            if (edges_count == V - 1) break;
        } else {
            cout << "  -> BO QUA canh (" << e.u << " - " << e.v << ", w = " << e.weight 
                 << ") vi tao thanh chu trinh!\n";
        }
    }

    if (edges_count == V - 1) {
        cout << "\n=> [THANH CONG] Tong trong so MST (Kruskal) = " << mst_weight << "\n";
    } else {
        cout << "\n=> [THAT BAI] Do thi KHONG LIEN THONG! Khong the tao MST.\n";
    }
}

// ============================================================================
// 2. THUẬT TOÁN PRIM (DỰA TRÊN ĐỈNH + MIN-HEAP)
// Độ phức tạp: O((V + E) log V)
// Áp dụng: Đồ thị dày đặc cạnh (Dense Graph).
// ----------------------------------------------------------------------------
struct PrimEdge {
    int to;
    int weight;
};

void primTrace(int start, int V, const vector<vector<PrimEdge>>& adj) {
    cout << "\n===================================================\n";
    cout << " 2. THUẬT TOÁN PRIM (BẮT ĐẦU TỪ ĐỈNH " << start << ")\n";
    cout << "===================================================\n";

    vector<bool> visited(V + 1, false);
    // Min-Heap lưu cặp {trọng số, đỉnh v}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    int mst_weight = 0;
    int edges_count = 0;

    // Bắt đầu từ đỉnh 'start'
    pq.push({0, start});

    int step = 1;
    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();

        // Bỏ qua nếu đỉnh u đã thuộc MST
        if (visited[u]) continue;

        visited[u] = true;
        mst_weight += w;
        
        if (u != start) {
            edges_count++;
            cout << "Step " << step++ << ": KET NOI dinh " << u << " vao MST qua canh w = " 
                 << w << " | Tong Trong So = " << mst_weight << "\n";
        } else {
            cout << "Start: Dua dinh ban dau " << start << " vao MST.\n";
        }

        // Đẩy tất cả các cạnh nối từ u tới đỉnh chưa thuộc MST vào Min-Heap
        for (const auto& edge : adj[u]) {
            if (!visited[edge.to]) {
                pq.push({edge.weight, edge.to});
                cout << "  -> Day canh (" << u << " - " << edge.to << ", w = " 
                     << edge.weight << ") vao Priority Queue\n";
            }
        }
    }

    if (edges_count == V - 1) {
        cout << "\n=> [THANH CONG] Tong trong so MST (Prim) = " << mst_weight << "\n";
    } else {
        cout << "\n=> [THAT BAI] Do thi KHONG LIEN THONG! Khong the tao MST.\n";
    }
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ------------------------------------------------------------------------
    // Đồ thị vô hướng thử nghiệm (V = 5, E = 7)
    // ------------------------------------------------------------------------
    int V = 5;
    
    // Danh sách cạnh cho Kruskal
    vector<Edge> edges = {
        {1, 2, 2},
        {1, 3, 3},
        {2, 3, 1},
        {2, 4, 4},
        {3, 4, 5},
        {3, 5, 6},
        {4, 5, 7}
    };

    // Danh sách kề cho Prim
    vector<vector<PrimEdge>> adj(V + 1);
    for (const auto& e : edges) {
        adj[e.u].push_back({e.v, e.weight});
        adj[e.v].push_back({e.u, e.weight});
    }

    // 1. Chạy Kruskal
    kruskalTrace(V, edges);

    // 2. Chạy Prim xuất phát từ đỉnh 1
    primTrace(1, V, adj);

    return 0;
}