#ifndef GRAPH_ALGORITHMS_HPP
#define GRAPH_ALGORITHMS_HPP

using namespace std;

#include "Graph.hpp"
#include <vector>

namespace GraphAlgo {

    bool isEulerian(const Graph& g);
    vector<int> findEulerianCircuit(const Graph& g);
    int findMSTWeight(const Graph& g);

} // namespace GraphAlgo

#endif
