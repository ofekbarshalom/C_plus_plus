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
#include <atomic>
#include <csignal>
#include "BlockingQueue.hpp"
#include "PipelineData.hpp"
#include "../core/GraphAlgorithms.hpp"
#include "../core/Graph.hpp"

using namespace std;
using namespace GraphAlgo;

#define PORT 8080
#define MAX_BUFFER 4096

atomic<bool> running(true);

BlockingQueue<PipelineData> queueMST;
BlockingQueue<PipelineData> queueSCC;
BlockingQueue<PipelineData> queueClique;
BlockingQueue<PipelineData> queueMaxFlow;

void signal_handler(int signum) {
    cout << "\nReceived SIGINT, shutting down gracefully..." << endl;
    running = false;
    PipelineData dummy(Graph(0, false), -1);
    queueMST.push(dummy);
    queueSCC.push(dummy);
    queueClique.push(dummy);
    queueMaxFlow.push(dummy);
}

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
    try { return stoi(line); }
    catch (...) { return -1; }
}

Graph readGraphFromClient(int client_socket) {
    string firstLine = readLine(client_socket);
    istringstream firstStream(firstLine);
    int v, e, d;
    if (!(firstStream >> v >> e >> d)) throw runtime_error("Invalid graph header input.");
    Graph g(v, d == 1 ? false : true);
    for (int i = 0; i < e; ++i) {
        string edgeLine = readLine(client_socket);
        istringstream edgeStream(edgeLine);
        int u, w;
        if (!(edgeStream >> u >> w)) throw runtime_error("Invalid edge format");
        g.addEdge(u, w);
    }
    return g;
}

Graph generateRandomGraph(int vertices, int edges, int seed, bool directed) {
    Graph g(vertices, directed);
    set<pair<int, int>> used_edges;
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(0, vertices - 1);
    int maxEdges = directed ? vertices * (vertices - 1) : vertices * (vertices - 1) / 2;
    if (edges > maxEdges) throw runtime_error("Too many edges.");
    while (used_edges.size() < (size_t)edges) {
        int u = dist(rng), v = dist(rng);
        if (u == v) continue;
        auto edge = directed ? make_pair(u, v) : make_pair(min(u, v), max(u, v));
        if (used_edges.count(edge)) continue;
        g.addEdge(u, v);
        used_edges.insert(edge);
    }
    return g;
}

void mstWorker() {
    while (true) {
        PipelineData data = queueMST.pop();
        if (!running || data.client_socket == -1) break;
        int weight = findMSTWeight(data.graph);
        data.mstResult = "MST Weight: " + to_string(weight) + "\n";
        queueSCC.push(data);
    }
}

void sccWorker() {
    while (true) {
        PipelineData data = queueSCC.pop();
        if (!running || data.client_socket == -1) break;
        auto scc = findSCC(data.graph);
        data.sccResult = "SCC Components (" + to_string(scc.size()) + "):\n";
        for (const auto& comp : scc) {
            data.sccResult += "{ ";
            for (int v : comp) data.sccResult += to_string(v) + " ";
            data.sccResult += "}\n";
        }
        queueClique.push(data);
    }
}

void cliqueWorker() {
    while (true) {
        PipelineData data = queueClique.pop();
        if (!running || data.client_socket == -1) break;
        auto clique = findMaxClique(data.graph);
        data.cliqueResult = "Max Clique (" + to_string(clique.size()) + "): ";
        for (int v : clique) data.cliqueResult += to_string(v) + " ";
        data.cliqueResult += "\n";
        queueMaxFlow.push(data);
    }
}

void maxFlowWorker() {
    while (true) {
        PipelineData data = queueMaxFlow.pop();
        if (!running || data.client_socket == -1) break;
        int flow = findMaxFlow(data.graph);
        data.maxFlowResult = "Max Flow: " + to_string(flow) + "\n";
        string fullResponse = data.mstResult + data.sccResult + data.cliqueResult + data.maxFlowResult;
        send(data.client_socket, fullResponse.c_str(), fullResponse.size(), 0);
        close(data.client_socket);
        cout << "Finished client (fd = " << data.client_socket << ")" << endl;
    }
}

void handleClient(int client_socket) {
    try {
        string menu = "\nChoose input mode:\n1 - Enter graph manually\n2 - Generate random graph\n0 - Exit\n";
        send(client_socket, menu.c_str(), menu.size(), 0);
        int mode = readChoice(client_socket);
        if (mode == 0 || mode == -1) {
            close(client_socket);
            cout << "Client exited." << endl;
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
                send(client_socket, "Invalid input format.\n", 22, 0);
                close(client_socket);
                return;
            }
            g = generateRandomGraph(v, e, seed, d == 1 ? false : true);
        } else {
            send(client_socket, "Invalid choice.\n", 16, 0);
            close(client_socket);
            return;
        }
        PipelineData data(g, client_socket);
        queueMST.push(data);
    } catch (...) {
        cout << "Client disconnected.\n";
        close(client_socket);
    }
}

int main() {
    signal(SIGINT, signal_handler);
    thread t1(mstWorker), t2(sccWorker), t3(cliqueWorker), t4(maxFlowWorker);

    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    cout << "Pipeline server waiting for connections on port " << PORT << "...\n";

    while (running) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_socket < 0) {
            if (running) cerr << "Failed to accept connection.\n";
            continue;
        }
        cout << "Client connected (fd = " << client_socket << ")" << endl;
        thread(handleClient, client_socket).detach();
    }

    t1.join(); t2.join(); t3.join(); t4.join();
    close(server_fd);
    cout << "Server shutdown complete.\n";
    return 0;
}
