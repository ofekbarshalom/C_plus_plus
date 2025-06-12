#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <iostream>

using namespace std;

namespace GraphAlgo {

struct Edge {
    int dst;
    int weight;
    Edge(int destination, int w) : dst(destination), weight(w) {}
};

class Graph {
private:
    int numVertices;
    bool directed;
    vector<vector<Edge>> adjList;

public:
    // Constructor
    Graph(int vertices, bool isDirected);

    // Copy Constructor 
    Graph(const Graph& other);

    // Copy Assignment Operator 
    Graph& operator=(const Graph& other);

    // Destructor 
    ~Graph() = default;

    // Methods
    void addEdge(int src, int dst, int weight = 1);
    void printGraph() const;
    int getVertexCount() const;
    const vector<Edge>& getNeighbors(int v) const;
    const vector<vector<Edge>>& getAdjList() const { return adjList; }

};

} // namespace GraphAlgo

#endif // GRAPH_HPP
