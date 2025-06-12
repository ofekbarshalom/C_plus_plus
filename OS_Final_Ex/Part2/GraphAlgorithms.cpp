#include "GraphAlgorithms.hpp" 
#include "Graph.hpp"
#include <vector>
#include <stack>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

namespace GraphAlgo {

// Constructor: store reference to input graph
GraphAlgorithms::GraphAlgorithms(const Graph& g) : graph(g) {}

void GraphAlgorithms::dfsUtil(int v, vector<bool>& visited) const {
    visited[v] = true;

    // Visit all neighbors
    for (const Edge& e : graph.getNeighbors(v)) {
        if (!visited[e.dst]) {
            dfsUtil(e.dst, visited);
        }
    }
}

bool GraphAlgorithms::isEulerian() const {
    // Check if all degrees are even
    for (int i = 0; i < graph.getVertexCount(); i++) {
        if (graph.getNeighbors(i).size() % 2 != 0)
            return false;
    }

    // Connectivity check (ignore isolated vertices)
    vector<bool> visited(graph.getVertexCount(), false);
    int start = -1;

    // Find any vertex with degree > 0 to start DFS
    for (int i = 0; i < graph.getVertexCount(); i++) {
        if (!graph.getNeighbors(i).empty()) {
            start = i;
            break;
        }
    }

    // If no edges exist (all isolated nodes), it's Eulerian
    if (start == -1)
        return true;

    dfsUtil(start, visited);

    // Verify all non-isolated vertices were visited
    for (int i = 0; i < graph.getVertexCount(); i++) {
        if (!graph.getNeighbors(i).empty() && !visited[i])
            return false;
    }

    return true;
}

//  Uses Hierholzer's algorithm
vector<int> GraphAlgorithms::findEulerianCircuit() const {
    vector<int> circuit;

    if (!isEulerian()) {
        cout << "Eulerian circuit does not exist.\n";
        return circuit;
    }

    // Create copy of adjacency list
    vector<vector<Edge>> tempAdj = graph.getAdjList();

    stack<int> s;
    int start = 0;
    for (int i = 0; i < tempAdj.size(); i++) {
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
            // Get next neighbor
            Edge e = tempAdj[u].back();
            tempAdj[u].pop_back();

            // Remove reverse edge manually
            int v = e.dst;
            auto& adjV = tempAdj[v];
            for (auto it = adjV.begin(); it != adjV.end(); ++it) {
                if (it->dst == u) {
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



} // namespace GraphAlgo