#include "Graph.hpp"
#include "GraphAlgorithms.hpp"

#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <vector>
#include <random>
#include <set>

using namespace std;
using namespace GraphAlgo;

void print_usage() {
    cout << "Usage: ./main -v <vertices> -e <edges> -s <seed>" << endl;
}

int main(int argc, char* argv[]) {
    int vertices = -1, edges = -1, seed = -1;

    int opt;
    while ((opt = getopt(argc, argv, "v:e:s:")) != -1) {
        switch (opt) {
            case 'v':
                vertices = atoi(optarg);
                break;
            case 'e':
                edges = atoi(optarg);
                break;
            case 's':
                seed = atoi(optarg);
                break;
            default:
                print_usage();
                return 1;
        }
    }

    if (vertices <= 0 || edges < 0 || seed < 0) {
        print_usage();
        return 1;
    }

    if (edges > vertices * (vertices - 1) / 2) {
        cout << "Too many edges for undirected simple graph." << endl;
        return 1;
    }

    srand(seed);
    Graph g(vertices, false);

    set<pair<int, int>> used_edges;
    while (used_edges.size() < static_cast<size_t>(edges)) {
    int u = rand() % vertices;
    int v = rand() % vertices;
    if (u == v) continue;
    if (used_edges.count({min(u, v), max(u, v)})) continue;

    g.addEdge(u, v);
    used_edges.insert({min(u, v), max(u, v)});
    }


    cout << "Generated random graph:" << endl;
    g.printGraph();

    GraphAlgorithms algo(g);

    if (algo.isEulerian()) {
        cout << "The graph is Eulerian!" << endl;

        vector<int> circuit = algo.findEulerianCircuit();
        cout << "Eulerian Circuit: ";
        for (int v : circuit) cout << v << " ";
        cout << endl;
    } else {
        cout << "The graph is NOT Eulerian." << endl;
    }

    return 0;
}
