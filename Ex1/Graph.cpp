// ofekbarshalom@gmail.com

#include "Graph.hpp"
#include <iostream>

using namespace std;

namespace graph {

    // NeighborList Implementation

    // constractor
    NeighborList::NeighborList() {
        size = 0;
        capacity = 1;
        neighbors = new Neighbor[capacity];
    }

    // Destructor
    NeighborList::~NeighborList() {
        delete[] neighbors;
    }

    // helper function to resize the dynamic array of neighbors
    void NeighborList::resize() {
        capacity *= 2;
        Neighbor* newArray = new Neighbor[capacity];
        for (int i = 0; i < size; ++i) {
            newArray[i] = neighbors[i];
        }
        delete[] neighbors;
        neighbors = newArray;
    }

    // Add a neighbor
    void NeighborList::addNeighbor(int id, int weight) {
        if (size == capacity) resize();
        neighbors[size++] = {id, weight};
    }

    // Remove a neighbor
    bool NeighborList::removeNeighbor(int id) {
        for (int i = 0; i < size; ++i) {
            if (neighbors[i].id == id) {
                neighbors[i] = neighbors[size - 1];
                --size;
                return true;
            }
        }
        return false;
    }

    // Print the Neighbor List
    void NeighborList::print() const {
        for (int i = 0; i < size; ++i) {
            cout << "->" << neighbors[i].id << "(w=" << neighbors[i].weight << ")";
        }
    }

    // Graph Implementation

    // Constructor
    Graph::Graph(int numVertices): numVertices(numVertices) {
        adjacencyList = new NeighborList[numVertices];
    }

    // Destructor
    Graph::~Graph() {
        delete[] adjacencyList;
    }

    // Return the number of vertices
    bool Graph::hasEdge(int src, int dest) const {
        if (src < 0 || src >= numVertices || dest < 0 || dest >= numVertices) return false;

        const NeighborList& neighbors = adjacencyList[src];
        for (int i = 0; i < neighbors.size; ++i) {
            if (neighbors.neighbors[i].id == dest) return true;
        }
        return false;
    }

    // Get the number of vertex
    int Graph::getNumVertices() const{
        return numVertices;
    }

    // Get the vertex neighbors
    const NeighborList& Graph::getNeighbors(int vertex) const {
        if (vertex < 0 || vertex >= numVertices) throw invalid_argument("Invalid vertex index");
        return adjacencyList[vertex];
    }

    // Add an undirected edge between source and dest
    void Graph::addEdge(int src, int dest, int weight) {
        if (src < 0 || src >= numVertices || dest < 0 || dest >= numVertices) {
            cout << "Invalid vertex index in addEdge";
            return;
        }

        if (hasEdge(src, dest)) {
            cout << "Edge between " << src << " and " << dest << " already exists." << endl;
            return;
        }

        adjacencyList[src].addNeighbor(dest, weight);
        adjacencyList[dest].addNeighbor(src, weight);
    }

    // Add a directed edge from src to dest
    void Graph::addDirectedEdge(int src, int dest, int weight) {
        if (src < 0 || src >= numVertices || dest < 0 || dest >= numVertices) {
            cout << "Invalid vertex index in addDirectedEdge" << endl;
            return;
        }

        if (hasEdge(src, dest)) {
            cout << "Directed edge from " << src << " to " << dest << " already exists." << endl;
            return;
        }

        adjacencyList[src].addNeighbor(dest, weight);
    }

    // Remove an undirected edge between source and dest
    void Graph::removeEdge(int src, int dest) {
        if (src < 0 || src >= numVertices || dest < 0 || dest >= numVertices) {
            throw out_of_range("Invalid vertex index in removeEdge");
        }

        bool removedSrc = adjacencyList[src].removeNeighbor(dest);
        bool removedDest = adjacencyList[dest].removeNeighbor(src);

        if (!removedSrc || !removedDest) {
            throw runtime_error("Problem removing edge");
        }
    }

    void Graph::print_graph() const {
        for (int i = 0; i < numVertices; ++i) {
            cout << "Vertex " << i << ": ";
            adjacencyList[i].print();
            cout << endl;
        }
    }
}
