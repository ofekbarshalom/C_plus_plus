#include "Graph.hpp"

using namespace std;
using namespace GraphAlgo;  // Use your namespace

int main() {
    // Create undirected graph with 5 vertices
    Graph g(5, false);

    // Add some edges with weights
    g.addEdge(0, 1, 10);
    g.addEdge(0, 4, 20);
    g.addEdge(1, 2, 30);
    g.addEdge(1, 3, 40);
    g.addEdge(1, 4, 50);
    g.addEdge(2, 3, 60);
    g.addEdge(3, 4, 70);

    cout << "Original Graph:" << endl;
    g.printGraph();

    // Copy constructor demo
    Graph gCopy = g;
    cout << "\nCopied Graph (using copy constructor):" << endl;
    gCopy.printGraph();

    // Assignment operator demo
    Graph gAssigned(3, true);  // Create another graph with different size
    gAssigned = g;
    cout << "\nAssigned Graph (using assignment operator):" << endl;
    gAssigned.printGraph();

    // Accessing neighbors demo
    cout << "\nNeighbors of vertex 1: ";
    for (const Edge& e : g.getNeighbors(1)) {
        cout << "(" << e.dst << ", w=" << e.weight << ") ";
    }
    cout << endl;

    return 0;
}
