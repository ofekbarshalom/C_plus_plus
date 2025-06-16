#include "../core/Graph.hpp"
#include "../core/GraphAlgorithms.hpp"
#include "AlgorithmStrategy.hpp"
#include <string>
#include <vector>

using namespace GraphAlgo;
using namespace std;

class MSTAlgorithm : public AlgorithmStrategy {
public:
    string execute(const Graph& graph) override {
        int weight = findMSTWeight(graph);
        return "MST Weight: " + to_string(weight) + "\n";
    }
};
