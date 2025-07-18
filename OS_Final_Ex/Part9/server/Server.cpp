#include <iostream>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <random>
#include <set>
#include <vector>               
#include <utility>              
#include "BlockingQueue.hpp"
#include "PipelineData.hpp"
#include "../core/GraphAlgorithms.hpp"
#include "../core/Graph.hpp"
#include "../algorithms/AlgorithmFactory.hpp"

using namespace std;
using namespace GraphAlgo;

#define PORT 8080
#define MAX_BUFFER 4096

// Create blocking queues
BlockingQueue<PipelineData> queueMST;
BlockingQueue<PipelineData> queueSCC;
BlockingQueue<PipelineData> queueClique;
BlockingQueue<PipelineData> queueMaxFlow;

string readLine(int client_socket) {
    string line;
    char c;
    while (true) {
        ssize_t bytesRead = read(client_socket, &c, 1);
        if (bytesRead <= 0) throw runtime_error("Connection lost while reading");
        if (c == '\n') break;
        line += c;
    }
    return line;
}

int readChoice(int client_socket) {
    string line = readLine(client_socket);
    try {
        return stoi(line);
    } catch (...) {
        return -1;  // Invalid input
    }
}

// Read full graph
Graph readGraphFromClient(int client_socket) {
    string firstLine = readLine(client_socket);
    istringstream firstStream(firstLine);
    int v, e, d;

    if (!(firstStream >> v >> e >> d)) {
        throw runtime_error("Invalid graph header input.");
    }

    Graph g(v, d == 1 ? false : true);

    for (int i = 0; i < e; ++i) {
        string edgeLine = readLine(client_socket);
        istringstream edgeStream(edgeLine);
        int u, w;

        if (!(edgeStream >> u >> w)) {
            throw runtime_error("Invalid edge format");
        }

        g.addEdge(u, w);
    }

    return g;
}

// Random graph generator
Graph generateRandomGraph(int vertices, int edges, int seed, bool directed) {
    Graph g(vertices, directed);
    set<pair<int, int>> used_edges;

    mt19937 rng(seed);
    uniform_int_distribution<int> dist(0, vertices - 1);

    int maxEdges = directed ? vertices * (vertices - 1) : vertices * (vertices - 1) / 2;
    if (edges > maxEdges) {
        throw runtime_error("Too many edges for the given number of vertices.");
    }

    while (used_edges.size() < static_cast<size_t>(edges)) {
        int u = dist(rng);
        int v = dist(rng);
        if (u == v) continue;

        auto edge = directed ? make_pair(u, v): make_pair(min(u, v), max(u, v));

        if (used_edges.count(edge)) continue;

        g.addEdge(u, v);
        used_edges.insert(edge);
    }

    return g;
}

void mstWorker() {
    while (true) {
        PipelineData data = queueMST.pop();
        auto algorithm = AlgorithmFactory::create(2);
        data.mstResult = algorithm->execute(data.graph);
        queueSCC.push(data);
    }
}

void sccWorker() {
    while (true) {
        PipelineData data = queueSCC.pop();
        auto algorithm = AlgorithmFactory::create(3);
        data.sccResult = algorithm->execute(data.graph);
        queueClique.push(data);
    }
}

void cliqueWorker() {
    while (true) {
        PipelineData data = queueClique.pop();
        auto algorithm = AlgorithmFactory::create(4);
        data.cliqueResult = algorithm->execute(data.graph);
        queueMaxFlow.push(data);
    }
}

void maxFlowWorker() {
    while (true) {
        PipelineData data = queueMaxFlow.pop();
        auto algorithm = AlgorithmFactory::create(5);
        data.maxFlowResult = algorithm->execute(data.graph);

        // Combine result
        string fullResponse = data.mstResult + data.sccResult + data.cliqueResult + data.maxFlowResult;
        send(data.client_socket, fullResponse.c_str(), fullResponse.size(), 0);
        close(data.client_socket);
        cout << "Finished client (fd = " << data.client_socket << ")" << endl;
    }
}

void handleClient(int client_socket) {
    try {
        // Send input mode menu
        string menu = "\nChoose input mode:\n1 - Enter graph manually\n2 - Generate random graph\n0 - Exit\n";
        send(client_socket, menu.c_str(), menu.size(), 0);

        int mode = readChoice(client_socket);
        if (mode == 0 || mode == -1) {
            close(client_socket);
            cout << "Client exited or invalid input." << endl;
            return;
        }

        Graph g(0, false);

        if (mode == 1) {
            g = readGraphFromClient(client_socket);
        } else if (mode == 2) {
            string prompt = "Enter: vertices edges seed directed(0=dir,1=undir):\n";
            send(client_socket, prompt.c_str(), prompt.size(), 0);

            string input = readLine(client_socket);
            istringstream ss(input);
            int v, e, seed, d;
            if (!(ss >> v >> e >> seed >> d)) {
                string err = "Invalid input format. Expected 4 integers.\n";
                send(client_socket, err.c_str(), err.size(), 0);
                close(client_socket);
                return;
            }

            try {
                g = generateRandomGraph(v, e, seed, d == 1 ? false : true);
            } catch (const exception& ex) {
                string err = string("Error generating graph: ") + ex.what() + "\n";
                send(client_socket, err.c_str(), err.size(), 0);
                close(client_socket);
                return;
            }
        } else {
            string err = "Invalid input.\n";
            send(client_socket, err.c_str(), err.size(), 0);
            close(client_socket);
            return;
        }

        // Pass to pipeline
        PipelineData data(g, client_socket);
        queueMST.push(data);
    } catch (...) {
        cout << "Client disconnected or error occurred." << endl;
        close(client_socket);
    }
}


int main() {
    // Launch worker threads
    thread(mstWorker).detach();
    thread(sccWorker).detach();
    thread(cliqueWorker).detach();
    thread(maxFlowWorker).detach();

    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[MAX_BUFFER];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    cout << "Pipeline server waiting for connections on port " << PORT << "...\n";

    while (true) {
        memset(buffer, 0, MAX_BUFFER);
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_socket < 0) {
            cerr << "Failed to accept connection.\n";
            continue;
        }
        cout << "Client connected (fd = " << client_socket << ")" << endl;
        thread t(handleClient, client_socket);
        t.detach();
    }

    close(server_fd);
    return 0;
}
