#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <unordered_map>
#include <set>

using namespace std;

// ============================================================================
// 1. PAGERANK ALGORITHM (RECOMMENDATION & WEB RANKING)
// Mô hình: Random Surfer Model với Hệ số xả (Damping Factor d = 0.85)
// Độ phức tạp: O(K * (V + E)) với K là số vòng lặp hội tụ
// ============================================================================
void pageRankTrace(int V, const vector<pair<int, int>>& edges, int max_iterations = 20, double d = 0.85, double tol = 1e-6) {
    cout << "\n===================================================\n";
    cout << " 1. PAGERANK ALGORITHM (RECOMMENDATION & RANKING)\n";
    cout << "===================================================\n";

    vector<vector<int>> out_adj(V + 1);
    vector<int> out_degree(V + 1, 0);

    for (const auto& edge : edges) {
        out_adj[edge.first].push_back(edge.second);
        out_degree[edge.first]++;
    }

    vector<double> PR(V + 1, 1.0 / V);

    cout << "Khởi tạo PageRank ban đầu cho " << V << " đỉnh: " << fixed << setprecision(4) << (1.0 / V) << "\n";
    cout << "Hệ số xả (Damping Factor d) = " << d << "\n\n";

    for (int iter = 1; iter <= max_iterations; ++iter) {
        vector<double> new_PR(V + 1, (1.0 - d) / V);

        // Xử lý Dangling Nodes (Đỉnh không có đường ra)
        double dangling_sum = 0.0;
        for (int u = 1; u <= V; ++u) {
            if (out_degree[u] == 0) dangling_sum += PR[u];
        }

        for (int u = 1; u <= V; ++u) {
            if (out_degree[u] > 0) {
                double contribution = d * (PR[u] / out_degree[u]);
                for (int v : out_adj[u]) {
                    new_PR[v] += contribution;
                }
            }
        }

        double dangling_contribution = d * (dangling_sum / V);
        for (int i = 1; i <= V; ++i) new_PR[i] += dangling_contribution;

        double diff = 0.0;
        for (int i = 1; i <= V; ++i) diff += fabs(new_PR[i] - PR[i]);

        PR = new_PR;

        cout << "Lần lặp " << setw(2) << iter << " | Delta = " << scientific << setprecision(2) << diff << " | PR: ";
        cout << fixed << setprecision(4);
        for (int i = 1; i <= V; ++i) cout << "P" << i << "=" << PR[i] << " ";
        cout << "\n";

        if (diff < tol) {
            cout << "\n[HỘI TỤ THÀNH CÔNG] Thuật toán dừng tại lần lặp " << iter << "!\n";
            break;
        }
    }

    cout << "\n-> BẢNG XẾP HẠNG PAGERANK CUỐI CÙNG:\n";
    vector<pair<double, int>> ranked;
    for (int i = 1; i <= V; ++i) ranked.push_back({PR[i], i});
    sort(ranked.rbegin(), ranked.rend());

    for (size_t rank = 0; rank < ranked.size(); ++rank) {
        cout << "  Hạng " << rank + 1 << ": Đỉnh " << ranked[rank].second 
             << " - Điểm PageRank: " << fixed << setprecision(6) << ranked[rank].first << "\n";
    }
}

// ============================================================================
// 2. HITS ALGORITHM (AUTHORITY & HUB SCORES)
// Thuật toán đánh giá trang web/user theo vai trò Nội dung (Authority) & Điều hướng (Hub)
// ============================================================================
void hitsTrace(int V, const vector<pair<int, int>>& edges, int max_iter = 20) {
    cout << "\n===================================================\n";
    cout << " 2. HITS ALGORITHM (AUTHORITIES & HUBS SCORES)\n";
    cout << "===================================================\n";

    vector<vector<int>> adj(V + 1);      // Out-edges (u -> v)
    vector<vector<int>> rev_adj(V + 1);  // In-edges (v <- u)

    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        rev_adj[e.second].push_back(e.first);
    }

    vector<double> auth(V + 1, 1.0);
    vector<double> hub(V + 1, 1.0);

    for (int iter = 1; iter <= max_iter; ++iter) {
        vector<double> new_auth(V + 1, 0.0);
        vector<double> new_hub(V + 1, 0.0);

        // 1. Cập nhật Authority Score: a(v) = sum(h(u)) với mọi u trỏ tới v
        double norm_a = 0.0;
        for (int v = 1; v <= V; ++v) {
            for (int u : rev_adj[v]) new_auth[v] += hub[u];
            norm_a += new_auth[v] * new_auth[v];
        }
        norm_a = sqrt(norm_a);

        // 2. Cập nhật Hub Score: h(u) = sum(a(v)) với mọi v được u trỏ tới
        double norm_h = 0.0;
        for (int u = 1; u <= V; ++u) {
            for (int v : adj[u]) new_hub[u] += new_auth[v];
            norm_h += new_hub[u] * new_hub[u];
        }
        norm_h = sqrt(norm_h);

        // 3. Chuẩn hóa L2 Norm
        for (int i = 1; i <= V; ++i) {
            if (norm_a > 0) new_auth[i] /= norm_a;
            if (norm_h > 0) new_hub[i] /= norm_h;
        }

        auth = new_auth;
        hub = new_hub;
    }

    cout << fixed << setprecision(4);
    cout << "BẢNG ĐIỂM HITS CUỐI CÙNG:\n";
    for (int i = 1; i <= V; ++i) {
        cout << "  Đỉnh " << i << " | Authority Score: " << auth[i] << " | Hub Score: " << hub[i] << "\n";
    }
}

// ============================================================================
// 3. A* SEARCH ALGORITHM (GPS ROUTING / AI PATHFINDING)
// Công thức: f(n) = g(n) + h(n)
// ============================================================================
struct Node2D {
    int id;
    double x, y;
};

struct AStarEdge {
    int to;
    double weight;
};

double euclideanHeuristic(const Node2D& a, const Node2D& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void aStarTrace(int start, int target, int V, const vector<Node2D>& nodes, const vector<vector<AStarEdge>>& adj) {
    cout << "\n===================================================\n";
    cout << " 3. A* SEARCH ALGORITHM (ROUTING & NAVIGATION)\n";
    cout << "===================================================\n";

    const double INF = 1e18;
    vector<double> g_score(V + 1, INF);
    vector<double> f_score(V + 1, INF);
    vector<int> parent(V + 1, -1);

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> open_set;

    g_score[start] = 0;
    f_score[start] = euclideanHeuristic(nodes[start], nodes[target]);
    open_set.push({f_score[start], start});

    int step = 1;
    cout << fixed << setprecision(2);

    while (!open_set.empty()) {
        auto [current_f, u] = open_set.top();
        open_set.pop();

        cout << "Step " << step++ << ": Duyệt đỉnh " << u 
             << " | g(" << u << ")=" << g_score[u] 
             << " + h(" << u << ")=" << euclideanHeuristic(nodes[u], nodes[target]) 
             << " => f(" << u << ")=" << current_f << "\n";

        if (u == target) {
            cout << "\n[ĐÃ TỚI ĐÍCH T = " << target << "]\n";
            vector<int> path;
            int curr = target;
            while (curr != -1) {
                path.push_back(curr);
                curr = parent[curr];
            }
            reverse(path.begin(), path.end());

            cout << "-> Đường đi ngắn nhất A*: ";
            for (size_t i = 0; i < path.size(); ++i) {
                cout << path[i] << (i + 1 < path.size() ? " -> " : "");
            }
            cout << "\n-> Chi phí thực tế g(Target) = " << g_score[target] << "\n";
            return;
        }

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            double tentative_g = g_score[u] + edge.weight;

            if (tentative_g < g_score[v]) {
                parent[v] = u;
                g_score[v] = tentative_g;
                double h_v = euclideanHeuristic(nodes[v], nodes[target]);
                f_score[v] = g_score[v] + h_v;
                open_set.push({f_score[v], v});
                cout << "   -> Cập nhật đỉnh " << v << ": g=" << g_score[v] 
                     << ", h=" << h_v << " => Push f(" << v << ")=" << f_score[v] << "\n";
            }
        }
    }
}

// ============================================================================
// 4. KAHN'S ALGORITHM (TOPOLOGICAL SORT & DEPENDENCY RESOLUTION)
// Độ phức tạp: O(V + E)
// ============================================================================
void kahnsTrace(int V, const vector<vector<int>>& adj, const vector<string>& task_names) {
    cout << "\n===================================================\n";
    cout << " 4. KAHN'S ALGORITHM (TOPOLOGICAL SORT / DEPENDENCY RESOLUTION)\n";
    cout << "===================================================\n";

    vector<int> in_degree(V + 1, 0);
    for (int u = 1; u <= V; ++u) {
        for (int v : adj[u]) in_degree[v]++;
    }

    queue<int> q;
    for (int i = 1; i <= V; ++i) {
        if (in_degree[i] == 0) q.push(i);
    }

    vector<int> topo_order;
    int step = 1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);

        cout << "Step " << step++ << ": Thực thi Task " << u << " (\"" << task_names[u] << "\") -> Giảm In-degree kề: ";
        for (int v : adj[u]) {
            in_degree[v]--;
            cout << v << "(In-deg=" << in_degree[v] << ") ";
            if (in_degree[v] == 0) q.push(v);
        }
        cout << "\n";
    }

    if ((int)topo_order.size() == V) {
        cout << "\n=> [LẬP LỊCH THÀNH CÔNG] Thứ tự thực thi hợp lệ:\n   ";
        for (size_t i = 0; i < topo_order.size(); ++i) {
            cout << task_names[topo_order[i]] << (i + 1 < topo_order.size() ? " ==> " : "");
        }
        cout << "\n";
    } else {
        cout << "\n=> [LỖI PHÁT HIỆN CHU TRÌNH] Tồn tại Circular Dependency!\n";
    }
}

// ============================================================================
// 5. TARJAN'S ALGORITHM (STRONGLY CONNECTED COMPONENTS - SCC)
// Phân tích thành phần liên thông mạnh trong đồ thị có hướng bằng DFS + Low/Num
// ============================================================================
void tarjanDFS(int u, const vector<vector<int>>& adj, vector<int>& num, vector<int>& low, 
               stack<int>& st, vector<bool>& in_stack, int& timer, int& scc_count) {
    num[u] = low[u] = ++timer;
    st.push(u);
    in_stack[u] = true;

    for (int v : adj[u]) {
        if (num[v] == 0) {
            tarjanDFS(v, adj, num, low, st, in_stack, timer, scc_count);
            low[u] = min(low[u], low[v]);
        } else if (in_stack[v]) {
            low[u] = min(low[u], num[v]);
        }
    }

    if (low[u] == num[u]) {
        scc_count++;
        cout << "  SCC #" << scc_count << ": { ";
        while (true) {
            int v = st.top();
            st.pop();
            in_stack[v] = false;
            cout << v << " ";
            if (u == v) break;
        }
        cout << "}\n";
    }
}

void tarjanSCC(int V, const vector<vector<int>>& adj) {
    cout << "\n===================================================\n";
    cout << " 5. TARJAN'S ALGORITHM (STRONGLY CONNECTED COMPONENTS)\n";
    cout << "===================================================\n";

    vector<int> num(V + 1, 0), low(V + 1, 0);
    vector<bool> in_stack(V + 1, false);
    stack<int> st;
    int timer = 0, scc_count = 0;

    for (int i = 1; i <= V; ++i) {
        if (num[i] == 0) tarjanDFS(i, adj, num, low, st, in_stack, timer, scc_count);
    }
    cout << "=> Tổng số thành phần liên thông mạnh (SCC): " << scc_count << "\n";
}

// ============================================================================
// 6. LOUVAIN COMMUNITY DETECTION (GRAPH CLUSTERING / NETWORK MODULARITY)
// Mô phỏng Phase 1: Khởi tạo các cụm & đo lường trọng số liên kết nội cụm
// ============================================================================
struct WeightedEdge { int to; double weight; };

void louvainCommunityDemo(int V, const vector<vector<WeightedEdge>>& adj) {
    cout << "\n===================================================\n";
    cout << " 6. LOUVAIN COMMUNITY DETECTION (COMMUNITY STRUCTURING)\n";
    cout << "===================================================\n";

    double total_weight = 0.0;
    vector<double> node_degree(V + 1, 0.0);

    for (int u = 1; u <= V; ++u) {
        for (const auto& e : adj[u]) {
            node_degree[u] += e.weight;
            total_weight += e.weight;
        }
    }

    cout << "Khởi tạo: Mỗi đỉnh ban đầu là 1 cụm riêng biệt (C1 đến C" << V << ")\n";
    cout << "Tổng trọng số liên kết (2m) = " << total_weight << "\n";
    cout << "Thuật toán gom cụm dựa trên việc tối ưu hóa chỉ số Modularity (Q).\n";
    for (int i = 1; i <= V; ++i) {
        cout << "  Đỉnh " << i << " | Bậc trọng số (Degree k_" << i << ") = " << node_degree[i] << "\n";
    }
}

// ============================================================================
// 7. CONTRACTION HIERARCHIES (CH) - PREPROCESSING (SHORTCUT EDGES CREATION)
// Kỹ thuật Co đồ thị tạo đường tắt (Shortcut) cho bản đồ GPS cỡ lớn
// ============================================================================
void contractionHierarchiesDemo(int V, const vector<vector<pair<int, double>>>& adj) {
    cout << "\n===================================================\n";
    cout << " 7. CONTRACTION HIERARCHIES (CH PREPROCESSING DEMO)\n";
    cout << "===================================================\n";

    cout << "Mô phỏng thu nhỏ đỉnh (Node Contraction) theo thứ tự tầm quan trọng (Importance Level):\n";
    cout << "Khi loại bỏ đỉnh u, nếu đường đi ngắn nhất giữa 2 đỉnh kề (v, w) đi qua u,\n";
    cout << "ta phải thêm một Cạnh Tắt (Shortcut Edge) v -> w với trọng số = dist(v,u) + dist(u,w).\n\n";

    // Demo co đỉnh 2 (giả sử đỉnh 2 bị loại bỏ đầu tiên)
    int u = 2;
    cout << "-> Co đỉnh " << u << ":\n";
    for (const auto& in_edge : adj[1]) { // Giả sử v = 1 trỏ tới 2
        if (in_edge.first == u) {
            double w1 = in_edge.second;
            for (const auto& out_edge : adj[u]) { // u = 2 trỏ tới w = 4
                int w = out_edge.first;
                double w2 = out_edge.second;
                cout << "   [SHORTCUT CREATED] Thêm cạnh tắt giữa " << 1 << " -> " << w 
                     << " với Trọng số mới = " << w1 << " + " << w2 << " = " << (w1 + w2) << "\n";
            }
        }
    }
}

// ============================================================================
// MAIN DEMO
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. PageRank
    pageRankTrace(4, {{1, 2}, {1, 3}, {2, 4}, {3, 1}, {3, 2}, {4, 3}});

    // 2. HITS
    hitsTrace(4, {{1, 2}, {1, 3}, {2, 4}, {3, 4}});

    // 3. A* Search
    vector<Node2D> nodes_astar = {{0,0,0}, {1,0,0}, {2,2,1}, {3,1,3}, {4,4,2}, {5,5,4}};
    vector<vector<AStarEdge>> adj_astar(6);
    adj_astar[1] = {{2, 2.5}, {3, 3.2}};
    adj_astar[2] = {{4, 2.3}};
    adj_astar[3] = {{4, 3.1}, {5, 4.2}};
    adj_astar[4] = {{5, 2.1}};
    aStarTrace(1, 5, 5, nodes_astar, adj_astar);

    // 4. Kahn Topo
    vector<string> task_names = {"", "Init DB", "Load Config", "Auth Service", "Payment", "API Gateway", "Deploy"};
    vector<vector<int>> adj_kahn(7);
    adj_kahn[1] = {3, 4};
    adj_kahn[2] = {3};
    adj_kahn[3] = {5};
    adj_kahn[4] = {5};
    adj_kahn[5] = {6};
    kahnsTrace(6, adj_kahn, task_names);

    // 5. Tarjan SCC
    vector<vector<int>> adj_scc(6);
    adj_scc[1] = {2}; adj_scc[2] = {3, 4}; adj_scc[3] = {1}; adj_scc[4] = {5};
    tarjanSCC(5, adj_scc);

    // 6. Louvain
    vector<vector<WeightedEdge>> adj_louvain(5);
    adj_louvain[1] = {{2, 1.0}, {3, 1.0}};
    adj_louvain[2] = {{1, 1.0}, {3, 1.0}};
    adj_louvain[3] = {{1, 1.0}, {2, 1.0}, {4, 0.1}};
    adj_louvain[4] = {{3, 0.1}};
    louvainCommunityDemo(4, adj_louvain);

    // 7. Contraction Hierarchies
    vector<vector<pair<int, double>>> adj_ch(5);
    adj_ch[1] = {{2, 1.5}};
    adj_ch[2] = {{4, 2.0}};
    contractionHierarchiesDemo(4, adj_ch);

    return 0;
}