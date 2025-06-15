#include "Graph.hpp"
#include "GraphAlgorithms.hpp"
#include "AlgorithmStrategy.hpp"
#include <string>
#include <vector>

using namespace GraphAlgo;
using namespace std;

class SCCAlgorithm : public AlgorithmStrategy {
public:
    string execute(const Graph& graph) override {
        vector<vector<int>> scc = findSCC(graph);
        string res = "SCC Components (" + to_string(scc.size()) + "):\n";
        for (const auto& comp : scc) {
            res += "{ ";
            for (int v : comp)
                res += to_string(v) + " ";
            res += "}\n";
        }
        return res;
    }
};
