#include "Graph.hpp"
#include "GraphAlgorithms.hpp"
#include "AlgorithmStrategy.hpp"
#include <string>
#include <vector>

using namespace GraphAlgo;
using namespace std;

class MaxFlowAlgorithm : public AlgorithmStrategy {
public:
    string execute(const Graph& graph) override {
        int flow = findMaxFlow(graph);
        return "Max Flow: " + to_string(flow) + "\n";
    }
};
