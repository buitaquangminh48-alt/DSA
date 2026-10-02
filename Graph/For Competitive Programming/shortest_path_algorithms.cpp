#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <iomanip>

using namespace std;

const long long INF = 1e18;

// Cấu trúc cạnh cho đồ thị có trọng số
struct Edge {
    int to;
    long long weight;
};

// ============================================================================
// 1. DIJKSTRA ALGORITHM (Sử dụng priority_queue / Min-Heap)
// Độ phức tạp: O((V + E) log V)
// Áp dụng: Đồ thị trọng số KHÔNG ÂM từ 1 đỉnh nguồn
// ============================================================================
void dijkstraTrace(int start, int V, const vector<vector<Edge>>& adj) {
    cout << "\n===================================================\n";
    cout << " 1. DIJKSTRA ALGORITHM (MIN-HEAP) - BẮT ĐẦU TỪ ĐỈNH " << start << "\n";
    cout << "===================================================\n";

    vector<long long> dist(V + 1, INF);
    // Hàng đợi ưu tiên lưu cặp {khoảng cách, đỉnh}, sắp xếp từ nhỏ đến lớn
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    int step = 1;
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // Bỏ qua nếu đã tìm được đường đi ngắn hơn trước đó
        if (d > dist[u]) continue;

        cout << "Step " << step++ << ": Lấy đỉnh " << u << " (Dist = " << d << ")\n";

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long w = edge.weight;

            // Kỹ thuật Relax (Thư giãn cạnh)
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
                cout << "  -> Relax thành công cạnh (" << u << " -> " << v 
                     << ", w=" << w << ") => dist[" << v << "] = " << dist[v] << "\n";
            }
        }
    }

    cout << "\n-> Bảng khoảng cách ngắn nhất từ đỉnh " << start << ":\n";
    for (int i = 1; i <= V; i++) {
        if (dist[i] == INF) cout << "  Đỉnh " << i << ": INF\n";
        else cout << "  Đỉnh " << i << ": " << dist[i] << "\n";
    }
}

// ============================================================================
// 2. SPFA - SHORTEST PATH FASTER ALGORITHM (Bellman-Ford tối ưu bằng Queue)
// Độ phức tạp: Trung bình O(E), Worst-case O(V * E)
// Áp dụng: Chấp nhận trọng số ÂM & PHÁT HIỆN CHU TRÌNH ÂM
// ============================================================================
bool spfaTrace(int start, int V, const vector<vector<Edge>>& adj) {
    cout << "\n===================================================\n";
    cout << " 2. SPFA ALGORITHM (PHÁT HIỆN CHU TRÌNH ÂM) - NGUỒN " << start << "\n";
    cout << "===================================================\n";

    vector<long long> dist(V + 1, INF);
    vector<int> count(V + 1, 0);     // Đếm số lần 1 đỉnh được Push vào Queue
    vector<bool> inqueue(V + 1, false);
    queue<int> q;

    dist[start] = 0;
    q.push(start);
    inqueue[start] = true;
    count[start] = 1;

    int step = 1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        inqueue[u] = false;

        cout << "Step " << step++ << ": Pops " << u << " khỏi Queue (dist[" << u << "] = " << dist[u] << ")\n";

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            long long w = edge.weight;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                cout << "  -> Cập nhật dist[" << v << "] = " << dist[v];

                if (!inqueue[v]) {
                    q.push(v);
                    inqueue[v] = true;
                    count[v]++;
                    cout << " | Push " << v << " vào Queue (Lần thứ " << count[v] << ")";

                    // Nếu 1 đỉnh được cập nhật/push vào queue >= V lần -> Có CHU TRÌNH ÂM!
                    if (count[v] >= V) {
                        cout << "\n\n [CẢNH BÁO] PHÁT HIỆN CHU TRÌNH ÂM chứa/dẫn tới đỉnh " << v << "!\n";
                        return false;
                    }
                }
                cout << "\n";
            }
        }
    }

    cout << "\n-> Bảng khoảng cách SPFA từ đỉnh " << start << ":\n";
    for (int i = 1; i <= V; i++) {
        if (dist[i] == INF) cout << "  Đỉnh " << i << ": INF\n";
        else cout << "  Đỉnh " << i << ": " << dist[i] << "\n";
    }
    return true;
}

// ============================================================================
// 3. FLOYD-WARSHALL ALGORITHM (Quy hoạch động All-Pairs Shortest Path)
// Độ phức tạp: O(V^3)
// Áp dụng: Tìm đường đi ngắn nhất giữa MỌI CẶP ĐỈNH (V <= 400)
// ============================================================================
void floydWarshallTrace(int V, vector<vector<long long>> dist) {
    cout << "\n===================================================\n";
    cout << " 3. FLOYD-WARSHALL ALGORITHM (ALL-PAIRS SHORTEST PATH)\n";
    cout << "===================================================\n";

    // Khởi tạo đường chéo chính = 0
    for (int i = 1; i <= V; i++) dist[i][i] = 0;

    // 3 vòng lặp DP kinh điển: k (đỉnh trung gian), i (đỉnh bắt đầu), j (đỉnh kết thúc)
    for (int k = 1; k <= V; k++) {
        cout << "-- Thử dùng đỉnh trung gian k = " << k << " --\n";
        bool updated = false;
        for (int i = 1; i <= V; i++) {
            for (int j = 1; j <= V; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        cout << "  Tối ưu dist[" << i << "][" << j << "]: " 
                             << dist[i][j] << " -> " << dist[i][k] + dist[k][j] 
                             << " (qua " << k << ")\n";
                        dist[i][j] = dist[i][k] + dist[k][j];
                        updated = true;
                    }
                }
            }
        }
        if (!updated) cout << "  (Không có đường đi nào được tối ưu qua k = " << k << ")\n";
    }

    cout << "\n-> Ma trận đường đi ngắn nhất giữa mọi cặp đỉnh (Floyd-Warshall):\n    ";
    for (int j = 1; j <= V; j++) cout << setw(6) << j;
    cout << "\n  " << string(6 * (V + 1), '-') << "\n";

    for (int i = 1; i <= V; i++) {
        cout << setw(3) << i << " |";
        for (int j = 1; j <= V; j++) {
            if (dist[i][j] == INF) cout << setw(6) << "INF";
            else cout << setw(6) << dist[i][j];
        }
        cout << "\n";
    }
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // ------------------------------------------------------------------------
    // Đồ thị 1: Trọng số DƯƠNG thử nghiệm Dijkstra & Floyd-Warshall (V = 5)
    // ------------------------------------------------------------------------
    int V1 = 5;
    vector<vector<Edge>> adj1(V1 + 1);
    vector<vector<long long>> matrix1(V1 + 1, vector<long long>(V1 + 1, INF));

    auto addEdge = [&](int u, int v, long long w) {
        adj1[u].push_back({v, w});
        matrix1[u][v] = w;
    };

    addEdge(1, 2, 4);
    addEdge(1, 3, 2);
    addEdge(2, 3, 1);
    addEdge(2, 4, 5);
    addEdge(3, 4, 8);
    addEdge(3, 5, 10);
    addEdge(4, 5, 2);

    // 1. Chạy Dijkstra từ đỉnh 1
    dijkstraTrace(1, V1, adj1);

    // 2. Chạy Floyd-Warshall trên đồ thị 1
    floydWarshallTrace(V1, matrix1);

    // ------------------------------------------------------------------------
    // Đồ thị 2: Đồ thị có CHU TRÌNH ÂM thử nghiệm SPFA (V = 4)
    // ------------------------------------------------------------------------
    int V2 = 4;
    vector<vector<Edge>> adj2(V2 + 1);

    adj2[1].push_back({2, 1});
    adj2[2].push_back({3, -2});
    adj2[3].push_back({4, -3});
    adj2[4].push_back({2, 1}); // Tạo chu trình âm: 2 -> 3 -> 4 -> 2 (Tổng trọng số: -2 - 3 + 1 = -4)

    // 3. Chạy SPFA kiểm tra phát hiện chu trình âm
    spfaTrace(1, V2, adj2);

    return 0;
}