#include "Graph.hpp"
#include "GraphAlgorithms.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace GraphAlgo;

int main() {
    // Create undirected graph with 5 vertices
    Graph g(5, false);  // false = undirected

    // Add edges (this graph has an Eulerian circuit)
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 0);
    g.addEdge(1, 3);
    g.addEdge(0, 2);
    g.addEdge(2, 4);
    g.addEdge(0, 3);
    g.addEdge(1, 4);

    cout << "Graph structure:" << endl;
    g.printGraph();

    // Create algorithms object
    GraphAlgorithms algo(g);

    // Check if Eulerian
    if (algo.isEulerian()) {
        cout << "The graph is Eulerian!" << endl;

        // Find Eulerian circuit
        vector<int> circuit = algo.findEulerianCircuit();
        cout << "Eulerian Circuit: ";
        for (size_t i = 0; i < circuit.size(); ++i) {
            cout << circuit[i];
            if (i != circuit.size() - 1)
                cout << " -> ";
        }
        cout << endl;
    } else {
        cout << "The graph is NOT Eulerian." << endl;
    }

    return 0;
}
