#include "GraphAlgorithms.hpp" 
#include "Graph.hpp"
#include <vector>
#include <stack>
#include <set>
#include <tuple>      
#include <numeric>    
#include <functional> 
#include <algorithm>
#include <iostream>
#include <climits>
#include <queue>

using namespace std;

// Helper struct for Union-Find (UF)
struct UF {
    vector<int> parent;

    UF(int n) : parent(n) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);  // Path compression
        return parent[x];
    }

    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;  // Already connected
        parent[py] = px;
        return true;
    }
};

namespace GraphAlgo {

// =========== 4 Algorithms ===========

int findMSTWeight(const Graph& graph) {
    int n = graph.getVertexCount();
    vector<tuple<int, int, int>> edges;  // tuple = (wieght, src, dst)

    // Collect all edges 
    for (int u = 0; u < n; ++u) {
        for (const Edge& e : graph.getNeighbors(u)) {
            if (u < e.dst) {
                edges.push_back({e.weight, u, e.dst});
            }
        }
    }

    sort(edges.begin(), edges.end());

    UF uf(n);
    int mstWeight = 0;

    for (auto [weight, u, v] : edges) {
        if (uf.unite(u, v)) {
            mstWeight += weight;
        }
    }

    return mstWeight;
}
//-----------------------------------------------------------------

// Helper function: transpose the graph
Graph transposeGraph(const Graph& graph) {
    int n = graph.getVertexCount();
    Graph transposed(n, true); // Directed

    for (int u = 0; u < n; ++u) {
        for (const Edge& e : graph.getNeighbors(u)) {
            transposed.addEdge(e.dst, u, e.weight);  // Reverse edge
        }
    }
    return transposed;
}

// First DFS pass
void dfsOrder(const Graph& graph, int v, vector<bool>& visited, stack<int>& finishOrder) {
    visited[v] = true;
    for (const Edge& e : graph.getNeighbors(v)) {
        if (!visited[e.dst]) {
            dfsOrder(graph, e.dst, visited, finishOrder);
        }
    }
    finishOrder.push(v);
}

// Second DFS pass to collect SCCs
void dfsCollect(const Graph& graph, int v, vector<bool>& visited, vector<int>& component) {
    visited[v] = true;
    component.push_back(v);
    for (const Edge& e : graph.getNeighbors(v)) {
        if (!visited[e.dst]) {
            dfsCollect(graph, e.dst, visited, component);
        }
    }
}

vector<vector<int>> findSCC(const Graph& graph) {
    int n = graph.getVertexCount();
    vector<bool> visited(n, false);
    stack<int> finishOrder;

    // First DFS to fill finish order
    for (int v = 0; v < n; ++v) {
        if (!visited[v])
            dfsOrder(graph, v, visited, finishOrder);
    }

    // Transpose the graph
    Graph transposed = transposeGraph(graph);

    // Second DFS using reversed graph
    visited.assign(n, false);
    vector<vector<int>> components;

    while (!finishOrder.empty()) {
        int v = finishOrder.top();
        finishOrder.pop();
        if (!visited[v]) {
            vector<int> component;
            dfsCollect(transposed, v, visited, component);
            components.push_back(component);
        }
    }
    return components;
}
//---------------------------------------------------------

// Build adjacency set for fast lookup
vector<set<int>> buildAdjacencySet(const Graph& graph) {
    int n = graph.getVertexCount();
    vector<set<int>> adjSet(n);

    for (int u = 0; u < n; ++u) {
        for (const Edge& e : graph.getNeighbors(u)) {
            adjSet[u].insert(e.dst);
        }
    }
    return adjSet;
}

// Bron Kerbosch recursive function
void bronKerbosch(const vector<set<int>>& adjSet, 
                  vector<int>& R, 
                  set<int>& P, 
                  set<int>& X, 
                  vector<int>& maxClique) {
    
    if (P.empty() && X.empty()) {
        if (R.size() > maxClique.size())
            maxClique = R;
        return;
    }

    int pivot = -1;
    if (!P.empty()) pivot = *P.begin();

    set<int> candidates;
    for (int v : P) {
        if (pivot == -1 || adjSet[pivot].count(v) == 0) {
            candidates.insert(v);
        }
    }

    for (int v : candidates) {
        R.push_back(v);

        set<int> newP;
        for (int u : P) {
            if (adjSet[v].count(u)) newP.insert(u);
        }

        set<int> newX;
        for (int u : X) {
            if (adjSet[v].count(u)) newX.insert(u);
        }

        bronKerbosch(adjSet, R, newP, newX, maxClique);

        R.pop_back();
        P.erase(v);
        X.insert(v);
    }
}

vector<int> findMaxClique(const Graph& graph) {
    int n = graph.getVertexCount();
    vector<set<int>> adjSet = buildAdjacencySet(graph);

    vector<int> R;
    set<int> P, X;

    for (int i = 0; i < n; ++i) {
        P.insert(i);
    }

    vector<int> maxClique;
    bronKerbosch(adjSet, R, P, X, maxClique);
    return maxClique;
}

//------------------------------------------------------------

// BFS to find augmenting path (Edmonds-Karp)
bool bfs(const vector<vector<int>>& residual, int s, int t, vector<int>& parent) {
    int n = residual.size();
    fill(parent.begin(), parent.end(), -1);
    parent[s] = s;

    queue<int> q;
    q.push(s);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < n; v++) {
            if (parent[v] == -1 && residual[u][v] > 0) {
                parent[v] = u;
                if (v == t)
                    return true;
                q.push(v);
            }
        }
    }
    return false;
}

int findMaxFlow(const Graph& graph) {
    int n = graph.getVertexCount();
    vector<vector<int>> residual(n, vector<int>(n, 0));

    // Build residual graph from your existing adjList
    for (int u = 0; u < n; ++u) {
        for (const Edge& e : graph.getNeighbors(u)) {
            residual[u][e.dst] += e.weight; 
        }
    }

    int maxFlow = 0;
    vector<int> parent(n);

    // Keep finding augmenting paths using BFS
    while (bfs(residual, 0, n - 1, parent)) {
        // Find minimum residual capacity along the path
        int pathFlow = INT_MAX;
        int v = n - 1;
        while (v != 0) {
            int u = parent[v];
            pathFlow = min(pathFlow, residual[u][v]);
            v = u;
        }

        // Update residual capacities along the path
        v = n - 1;
        while (v != 0) {
            int u = parent[v];
            residual[u][v] -= pathFlow;
            residual[v][u] += pathFlow;
            v = u;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}


} // namespace GraphAlgo