#ifndef PIPELINE_DATA_HPP
#define PIPELINE_DATA_HPP

#include "../core/Graph.hpp"
#include <string>

using namespace GraphAlgo;
using namespace std;

struct PipelineData {
    Graph graph;
    string mstResult;
    string sccResult;
    string cliqueResult;
    string maxFlowResult;
    int client_socket;

    PipelineData(Graph g, int sock) : graph(g), client_socket(sock) {}
};

#endif
