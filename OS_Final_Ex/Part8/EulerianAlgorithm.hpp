#include "Graph.hpp"
#include "GraphAlgorithms.hpp"
#include "AlgorithmStrategy.hpp"
#include <string>
#include <vector>

using namespace GraphAlgo;
using namespace std;

class EulerianAlgorithm : public AlgorithmStrategy {
public:
    string execute(const Graph& graph) override {
        if (isEulerian(graph)) {
            vector<int> circuit = findEulerianCircuit(graph);
            string res = "EULERIAN CIRCUIT: ";
            for (int v : circuit)
                res += to_string(v) + " -> ";
            if (!circuit.empty())
                res.erase(res.size() - 4);
            return res + "\n";
        }
        return "NOT EULERIAN\n";
    }
};
