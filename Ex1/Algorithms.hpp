// ofekbarshalom@gmail.com

#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "Graph.hpp"

namespace algorithms {

    class Algorithms {
        public:
            // Breadth-First Search (BFS)
            static graph::Graph bfs(const graph::Graph& graph, int start);

            // Depth-First Search (DFS)
            static graph::Graph dfs(const graph::Graph& graph, int start);

            // Dijkstra's Algorithm for shortest paths
            static graph::Graph dijkstra(const graph::Graph& graph, int start);

            // Prim's Algorithm for Minimum Spanning Tree (MST)
            static graph::Graph prim(const graph::Graph& graph);

            // Kruskal's Algorithm for MST
            static graph::Graph kruskal(const graph::Graph& graph);
    };

}


#endif //ALGORITHMS_HPP
