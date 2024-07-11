#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

class Kosaraju {
private:
    void dfs1(int v, vector<vector<int>>& adjMatrix, vector<bool>& visited, deque<int>& order, int n) {
        visited[v] = true;
        for (int u = 1; u <= n; ++u) {
            if (adjMatrix[v][u] && !visited[u]) {
                dfs1(u, adjMatrix, visited, order, n);
            }
        }
        order.push_front(v); // Store nodes in reverse postorder
    }

    void dfs2(int v, vector<vector<int>>& adjMatrixTranspose, vector<bool>& visited, vector<int>& component, int n) {
        visited[v] = true;
        component.push_back(v);
        for (int u = 1; u <= n; ++u) {
            if (adjMatrixTranspose[v][u] && !visited[u]) {
                dfs2(u, adjMatrixTranspose, visited, component, n);
            }
        }
    }

public:
    vector<vector<int>> findSCC(int n, vector<vector<int>>& edges) {
        vector<vector<int>> adjMatrix(n + 1, vector<int>(n + 1, 0));
        vector<vector<int>> adjMatrixTranspose(n + 1, vector<int>(n + 1, 0));

        for (auto& edge : edges) {
            adjMatrix[edge[0]][edge[1]] = 1;
            adjMatrixTranspose[edge[1]][edge[0]] = 1;
        }

        vector<bool> visited(n + 1, false);
        deque<int> order; // Using deque to store nodes in reverse postorder

        // Step 1: Perform DFS on the original graph
        for (int i = 1; i <= n; ++i) {
            if (!visited[i]) {
                dfs1(i, adjMatrix, visited, order, n);
            }
        }

        // Step 2: Reset visited array
        fill(visited.begin(), visited.end(), false);
        vector<vector<int>> SCCs;

        // Step 3: Perform DFS on the transpose graph in reverse order of finishing times
        while (!order.empty()) {
            int v = order.front();
            order.pop_front();
            if (!visited[v]) {
                vector<int> component;
                dfs2(v, adjMatrixTranspose, visited, component, n);
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
