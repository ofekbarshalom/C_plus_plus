// ofekbarshalom@gmail.com

#include "Graph.hpp"
#include "Algorithms.hpp"
#include <iostream>

using namespace std;
using namespace graph;
using namespace algorithms;

int main() {
    Graph g(6);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 4);
    g.addEdge(3, 4, 2);
    g.addEdge(4, 5, 6);

    cout << "Original Graph:" << endl;
    g.print_graph();

    cout << "\nBFS Tree (from vertex 0):" << endl;
    Graph bfsTree = Algorithms::bfs(g, 0);
    bfsTree.print_graph();

    cout << "\nDFS Tree (from vertex 0):" << endl;
    Graph dfsTree = Algorithms::dfs(g, 0);
    dfsTree.print_graph();

    cout << "\nDijkstra Shortest-Path Tree (from vertex 0):" << endl;
    Graph dijkstraTree = Algorithms::dijkstra(g, 0);
    dijkstraTree.print_graph();

    cout << "\nMinimum Spanning Tree (Prim):" << endl;
    Graph primTree = Algorithms::prim(g);
    primTree.print_graph();

    cout << "\nMinimum Spanning Tree (Kruskal):" << endl;
    Graph kruskalTree = Algorithms::kruskal(g);
    kruskalTree.print_graph();

    return 0;
}
