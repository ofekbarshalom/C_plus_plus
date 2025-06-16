#ifndef ALGORITHM_STRATEGY_HPP
#define ALGORITHM_STRATEGY_HPP

#include "../core/Graph.hpp"
#include <string>

using namespace GraphAlgo;
using namespace std;

class AlgorithmStrategy {
public:
    virtual string execute(const Graph& graph) = 0;
    virtual ~AlgorithmStrategy() = default;
};

#endif
