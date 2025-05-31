#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int minDist(vector<int> &dist, vector<bool> &sptSet) {
    int min = INT_MAX, idx = -1;
    for (int i = 0; i < dist.size(); i++) {
        if (!sptSet[i] && dist[i] < min) {
            min = dist[i];
            idx = i;
        }
    }
    return idx;
}

void dijkstra(vector<vector<int>> &graph, int src) {
    int V = graph.size();
    vector<int> dist(V, INT_MAX);
    vector<bool> sptSet(V, false);

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDist(dist, sptSet);
        if (u == -1)
            break;

        sptSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] > 0 &&
                dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    cout << "Vertex \t Distance from Source\n";
    for (int i = 0; i < V; i++)
        cout << i << " \t\t " << dist[i] << '\n';
}

int main() {
    for (;;) {
        int V;
        cout << "Enter number of vertices (or -1 to exit): ";
        if (!(cin >> V) || V <= 0) break;

        vector<vector<int>> graph(V, vector<int>(V));

        cout << "Enter the adjacency matrix (use 0 for no edge):\n";
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                cout << "[" << i << "][" << j << "]: ";
                if (!(cin >> graph[i][j])) {
                    cerr << "Invalid input at [" << i << "][" << j << "]\n";
                    return 1;
                }
                if (graph[i][j] < 0) {
                    cerr << "Error: Negative weights are not allowed in Dijkstra.\n";
                    return 1;
                }
            }
        }

        int src;
        cout << "Enter source vertex (0 to " << V - 1 << "): ";
        if (!(cin >> src) || src < 0 || src >= V) {
            cerr << "Invalid source vertex.\n";
            return 1;
        }

        dijkstra(graph, src);
        cout << "----- Done -----\n\n";
    }

    return 0;
}
