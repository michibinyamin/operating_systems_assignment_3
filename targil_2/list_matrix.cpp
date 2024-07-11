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
        for (auto it = adj[curr].begin(); it != adj[curr].end(); ++it) {
            int x = *it;
            if (!vis[x]) {
                if (dfs_Algo(x, des, adj, vis)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isPath(int src, int des, vector<vector<int>>& adjMatrix, vector<list<int>>& adjList) {
        vector<int> vis(adjMatrix.size() + 1, 0);
        return dfs_Algo(src, des, adjList, vis);
    }

    vector<vector<int>> findSCC(int n, vector<vector<int>>& edges) {
        vector<vector<int>> ans;
        vector<int> is_scc(n + 1, 0);
        vector<vector<int>> adjMatrix(n + 1, vector<int>(n + 1, 0));
        vector<list<int>> adjList(n + 1);

        for (int i = 0; i < edges.size(); i++) {
            int src = edges[i][0];
            int dest = edges[i][1];
            adjMatrix[src][dest] = 1;
            adjList[src].push_back(dest);
        }

        for (int i = 1; i <= n; i++) {
            if (!is_scc[i]) {
                vector<int> scc;
                scc.push_back(i);

                for (int j = i + 1; j <= n; j++) {
                    if (!is_scc[j] && isPath(i, j, adjMatrix, adjList) && isPath(j, i, adjMatrix, adjList)) {
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
