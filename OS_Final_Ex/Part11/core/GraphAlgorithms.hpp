#ifndef GRAPH_ALGORITHMS_HPP
#define GRAPH_ALGORITHMS_HPP

using namespace std;

#include "Graph.hpp"
#include <vector>

namespace GraphAlgo {

    // ====== 4 Algorithms ======
    int findMSTWeight(const Graph& graph);              // Finding MST weight
    vector<vector<int>> findSCC(const Graph& graph);    // Finding SCC on the graph
    vector<int> findMaxClique(const Graph& graph);      // Finding max clique
    int findMaxFlow(const Graph& graph);                // Finding max flow between node 0 (source) and node (n-1)

} // namespace GraphAlgo

#endif
