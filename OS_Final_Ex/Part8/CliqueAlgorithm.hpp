#include "Graph.hpp"
#include "GraphAlgorithms.hpp"
#include "AlgorithmStrategy.hpp"
#include <string>
#include <vector>

using namespace GraphAlgo;
using namespace std;

class CliqueAlgorithm : public AlgorithmStrategy {
public:
    string execute(const Graph& graph) override {
        vector<int> clique = findMaxClique(graph);
        string res = "Max Clique (" + to_string(clique.size()) + "): ";
        for (int v : clique)
            res += to_string(v) + " ";
        return res + "\n";
    }
};
