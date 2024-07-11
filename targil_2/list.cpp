#include <iostream>
#include <list>
#include <vector>

using namespace std;

class KOSARAJU {
public:
    bool dfs_Algo(int curr, int des, vector<list<int>>& adj, vector<int>& vis) {
        if (curr == des) {
            return true;
        }
        vis[curr] = 1;
        for (auto x : adj[curr]) {
            if (!vis[x]) {
                if (dfs_Algo(x, des, adj, vis)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isPath(int src, int des, vector<list<int>>& adj) {
        vector<int> vis(adj.size() + 1, 0);
        return dfs_Algo(src, des, adj, vis);
    }

    vector<vector<int>> findSCC(int n, vector<vector<int>>& a) {
        vector<vector<int>> ans;
        vector<int> is_scc(n + 1, 0);
        vector<list<int>> adj(n + 1);

        for (int i = 0; i < a.size(); i++) {
            adj[a[i][0]].push_back(a[i][1]);
        }

        for (int i = 1; i <= n; i++) {
            if (!is_scc[i]) {
                vector<int> scc;
                scc.push_back(i);

                for (int j = i + 1; j <= n; j++) {
                    if (!is_scc[j] && isPath(i, j, adj) && isPath(j, i, adj)) {
                        is_scc[j] = 1;
                        scc.push_back(j);
                    }
                }
                ans.push_back(scc);
            }
        }
        return ans;
    }
};

int main() {
    KOSARAJU kosaraju;
    int v = 0;
    int m = 0;

    cout << "Please enter numbers of <vertices> <edges>:\n";
    cin >> v >> m;

    if (v <= 0) {
        cout << "The number of vertices must be positive\n";
        return 1;
    }

    vector<vector<int>> edges;
    cout << "Please enter the edges: (for example- 1 2)\n";
    for (int i = 0; i < m; ++i) {
        int src, dest;
        cin >> src >> dest;

        if (dest < 1 || dest > v || src < 1 || src > v) {
            cout << "invalid vertices\n";
            return 0;
        }
        edges.push_back({ src, dest });
    }

    vector<vector<int>> ans = kosaraju.findSCC(v, edges);
    cout << "Strongly Connected Components are:\n";
    for (auto x : ans) {
        for (auto y : x) {
            cout << y << " ";
        }
        cout << "\n";
    }

    return 0;
}
