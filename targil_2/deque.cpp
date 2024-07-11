#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

class Kosaraju {
private:
    void dfs1(int v, vector<vector<int>>& adj, vector<bool>& visited, deque<int>& order) {
        visited[v] = true;
        for (int u : adj[v]) {
            if (!visited[u]) {
                dfs1(u, adj, visited, order);
            }
        }
        order.push_front(v); // Store nodes in reverse postorder
    }

    void dfs2(int v, vector<vector<int>>& adjTranspose, vector<bool>& visited, vector<int>& component) {
        visited[v] = true;
        component.push_back(v);
        for (int u : adjTranspose[v]) {
            if (!visited[u]) {
                dfs2(u, adjTranspose, visited, component);
            }
        }
    }

public:
    vector<vector<int>> findSCC(int n, vector<vector<int>>& edges) {
        vector<vector<int>> adj(n + 1), adjTranspose(n + 1);
        for (auto& edge : edges) {
            adj[edge[0]].push_back(edge[1]);
            adjTranspose[edge[1]].push_back(edge[0]);
        }

        vector<bool> visited(n + 1, false);
        deque<int> order; // Using deque to store nodes in reverse postorder

        for (int i = 1; i <= n; ++i) {
            if (!visited[i]) {
                dfs1(i, adj, visited, order);
            }
        }

        fill(visited.begin(), visited.end(), false);
        vector<vector<int>> SCCs;

        for (int v : order) {
            if (!visited[v]) {
                vector<int> component;
                dfs2(v, adjTranspose, visited, component);
                SCCs.push_back(component);
            }
        }

        return SCCs;
    }
};

int main() {
    int v, m;
    cout << "Please enter number of vertices and edges: ";
    cin >> v >> m;

    if (v <= 0) {
        cout << "Number of vertices must be positive.\n";
        return 1;
    }

    vector<vector<int>> edges(m, vector<int>(2));
    cout << "Please enter the edges (src dest):\n";
    for (int i = 0; i < m; ++i) {
        cin >> edges[i][0] >> edges[i][1];
    }

    Kosaraju kosaraju;
    vector<vector<int>> SCCs = kosaraju.findSCC(v, edges);

    cout << "Strongly Connected Components:\n";
    for (auto& component : SCCs) {
        for (int node : component) {
            cout << node << " ";
        }
        cout << "\n";
    }

    return 0;
}
