#include "GraphAlgorithms.hpp" 
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

vector<int> GraphAlgorithms::findEulerianCircuit() const {
    vector<int> circuit;

    // First check if the graph is Eulerian
    if (!isEulerian()) {
        cout << "Eulerian circuit does not exist.\n";
        return circuit;
    }

    // Create a copy of adjacency list to modify
    vector<multiset<int>> tempAdj(graph.getVertexCount());
    for (int u = 0; u < graph.getVertexCount(); ++u) {
        for (const Edge& e : graph.getNeighbors(u)) {
            tempAdj[u].insert(e.dst);
        }
    }

    stack<int> stack;
    stack.push(0);  // Start from any vertex

    while (!stack.empty()) {
        int u = stack.top();
        if (tempAdj[u].empty()) {
            circuit.push_back(u);
            stack.pop();
        } else {
            int v = *tempAdj[u].begin();
            tempAdj[u].erase(tempAdj[u].begin());
            tempAdj[v].erase(tempAdj[v].find(u));
            stack.push(v);
        }
    }

    reverse(circuit.begin(), circuit.end());
    return circuit;
}


} // namespace GraphAlgo