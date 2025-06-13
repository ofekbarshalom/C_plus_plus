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

using namespace std;

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


// ========= Find MST Weight =========
int findMSTWeight(const Graph& g) {
    int n = g.getVertexCount();
    vector<vector<Edge>> adj = g.getAdjList();
    vector<tuple<int, int, int>> edges;

    for (int u = 0; u < n; ++u) {
        for (const Edge& e : adj[u]) {
            if (u < e.dst)  // avoid duplicates in undirected graph
                edges.emplace_back(e.weight, u, e.dst);
        }
    }

    sort(edges.begin(), edges.end());

    vector<int> parent(n);
    iota(parent.begin(), parent.end(), 0);

    function<int(int)> find = [&](int u) {
        return parent[u] == u ? u : parent[u] = find(parent[u]);
    };

    int mstWeight = 0;
    for (auto [w, u, v] : edges) {
        int pu = find(u), pv = find(v);
        if (pu != pv) {
            parent[pu] = pv;
            mstWeight += w;
        }
    }

    return mstWeight;
}


} // namespace GraphAlgo