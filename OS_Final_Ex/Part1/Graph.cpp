#include "Graph.hpp"

using namespace std;

namespace GraphAlgo {

// Constructor
Graph::Graph(int vertices, bool isDirected)
    : numVertices(vertices), directed(isDirected), adjList(vertices) {}

// Copy Constructor
Graph::Graph(const Graph& other)
    : numVertices(other.numVertices), directed(other.directed), adjList(other.adjList) {}

// Copy Assignment Operator
Graph& Graph::operator=(const Graph& other) {
    if (this != &other) {
        numVertices = other.numVertices;
        directed = other.directed;
        adjList = other.adjList;
    }
    return *this;
}

//=============== Methods ===============
void Graph::addEdge(int src, int dst, int weight) {
    adjList[src].emplace_back(dst, weight);
    if (!directed) {
        adjList[dst].emplace_back(src, weight);
    }
}

void Graph::printGraph() const {
    for (int i = 0; i < numVertices; ++i) {
        cout << "Vertex " << i << ": ";
        for (const Edge& e : adjList[i]) {
            cout << "(" << e.dst << ", w=" << e.weight << ") ";
        }
        cout << '\n';
    }
}

int Graph::getVertexCount() const {
    return numVertices;
}

const vector<Edge>& Graph::getNeighbors(int v) const {
    return adjList[v];
}

} // namespace GraphAlgo
