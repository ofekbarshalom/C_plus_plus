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

// ========= Find Euler circle =========

// Internal DFS used by isEulerian
static void dfsUtil(const Graph& graph, int v, vector<bool>& visited) {
    visited[v] = true;

    for (const Edge& e : graph.getNeighbors(v)) {
        if (!visited[e.dst]) {
            dfsUtil(graph, e.dst, visited);
        }
    }
}

bool isEulerian(const Graph& graph) {
    for (int i = 0; i < graph.getVertexCount(); ++i) {
        if (graph.getNeighbors(i).size() % 2 != 0)
            return false;
    }

    vector<bool> visited(graph.getVertexCount(), false);
    int start = -1;

    for (int i = 0; i < graph.getVertexCount(); ++i) {
        if (!graph.getNeighbors(i).empty()) {
            start = i;
            break;
        }
    }

    if (start == -1)
        return true;

    dfsUtil(graph, start, visited);

    for (int i = 0; i < graph.getVertexCount(); ++i) {
        if (!graph.getNeighbors(i).empty() && !visited[i])
            return false;
    }

    return true;
}

vector<int> findEulerianCircuit(const Graph& graph) {
    vector<int> circuit;

    if (!isEulerian(graph)) {
        cout << "Eulerian circuit does not exist.\n";
        return circuit;
    }

    vector<vector<Edge>> tempAdj = graph.getAdjList();
    stack<int> s;

    int start = 0;
    for (size_t i = 0; i < tempAdj.size(); ++i) {
        if (!tempAdj[i].empty()) {
            start = i;
            break;
        }
    }

    s.push(start);

    while (!s.empty()) {
        int u = s.top();

        if (tempAdj[u].empty()) {
            circuit.push_back(u);
            s.pop();
        } else {
            Edge e = tempAdj[u].back();
            tempAdj[u].pop_back();

            int v = e.dst;
            auto& adjV = tempAdj[v];
            for (auto it = adjV.begin(); it != adjV.end(); ++it) {
                if (it->dst == u && it->src == v) {
                    adjV.erase(it);
                    break;
                }
            }

            s.push(v);
        }
    }

    reverse(circuit.begin(), circuit.end());
    return circuit;
}


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
    vector<std::vector<int>> components;

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
std::vector<std::set<int>> buildAdjacencySet(const Graph& graph) {
    int n = graph.getVertexCount();
    std::vector<std::set<int>> adjSet(n);

    for (int u = 0; u < n; ++u) {
        for (const Edge& e : graph.getNeighbors(u)) {
            adjSet[u].insert(e.dst);
        }
    }
    return adjSet;
}

// Bron Kerbosch recursive function
void bronKerbosch(const std::vector<std::set<int>>& adjSet, 
                  std::vector<int>& R, 
                  std::set<int>& P, 
                  std::set<int>& X, 
                  std::vector<int>& maxClique) {
    
    if (P.empty() && X.empty()) {
        if (R.size() > maxClique.size())
            maxClique = R;
        return;
    }

    int pivot = -1;
    if (!P.empty()) pivot = *P.begin();

    std::set<int> candidates;
    for (int v : P) {
        if (pivot == -1 || adjSet[pivot].count(v) == 0) {
            candidates.insert(v);
        }
    }

    for (int v : candidates) {
        R.push_back(v);

        std::set<int> newP;
        for (int u : P) {
            if (adjSet[v].count(u)) newP.insert(u);
        }

        std::set<int> newX;
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
    vector<std::set<int>> adjSet = buildAdjacencySet(graph);

    std::vector<int> R;
    std::set<int> P, X;

    for (int i = 0; i < n; ++i) {
        P.insert(i);
    }

    std::vector<int> maxClique;
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