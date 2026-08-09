#include <iostream>
#include <vector>

using namespace std;

const int MAXN = 100005;
vector<int> adj[MAXN];
bool visited[MAXN];

void dfs(int u, int depth = 0) {
    visited[u] = true;

    auto indent = string(depth * 2, ' ');

    cout << indent << "[ENTER] u = " << u << '\n';

    for (int v : adj[u]) {
        cout << indent << "  Check v = " << v;

        if (!visited[v]) {
            cout << " -> GO\n";
            dfs(v, depth + 1);
        }
        else {
            cout << " -> SKIP (visited)\n";
        }
    }

    cout << indent << "[EXIT] u = " << u << '\n';
}

int main() {
    // Tối ưu I/O cho C++
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m; // n: số đỉnh, m: số cạnh
    cin >> n >> m;

    for (int i = 0; i < m; i++) {
        int u, v;
    
        cin >> u >> v;
    
        cout << "\nEdge: " << u << " - " << v << '\n';
    
        adj[u].push_back(v);
        adj[v].push_back(u);
    
        cout << "adj[u=" << u << "]: ";
        for (int x : adj[u])
            cout << x << " ";
        cout << '\n';
    
        cout << "adj[v=" << v << "]: ";
        for (int x : adj[v])
            cout << x << " ";
        cout << "\n\n";
    }
    
    // Duyệt từ đỉnh 1
    dfs(1);
    // bfs(1);
    
    return 0;
}
/*
try testcase:
4 6
1 2
1 3
1 4
2 3
2 4
3 4
*/
