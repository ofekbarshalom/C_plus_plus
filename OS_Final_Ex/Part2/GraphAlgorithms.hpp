#ifndef GRAPH_ALGORITHMS_HPP
#define GRAPH_ALGORITHMS_HPP

#include "Graph.hpp"
#include <vector>

namespace GraphAlgo {

class GraphAlgorithms {
public:
    GraphAlgorithms(const Graph& g);
    bool isEulerian() const;
    std::vector<int> findEulerianCircuit() const;


private:
    const Graph& graph;
    void dfsUtil(int v, std::vector<bool>& visited) const;
};

} // namespace GraphAlgo

#endif
