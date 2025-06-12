#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include "Graph.hpp"
#include "GraphAlgorithms.hpp"

using namespace std;
using namespace GraphAlgo;

#define PORT 8080
#define MAX_BUFFER 4096

Graph parseGraphFromStream(istringstream& ss) {
    int v, e;
    ss >> v >> e;
    Graph g(v, false); // Undirected graph

    for (int i = 0; i < e; ++i) {
        int u, w;
        ss >> u >> w;
        g.addEdge(u, w);  // Undirected
    }

    return g;
}

int main() {
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
    cout << "Waiting for connections on port " << PORT << "...\n";

    while (true) {
        memset(buffer, 0, MAX_BUFFER);  // Clear buffer
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        read(client_socket, buffer, MAX_BUFFER);

        istringstream ss(buffer);
        Graph g = parseGraphFromStream(ss);

        string response;
        if (isEulerian(g)) {
            vector<int> circuit = findEulerianCircuit(g);
            response = "EULERIAN CIRCUIT: ";
            for (int v : circuit)
                response += to_string(v) + " -> ";
            if (!circuit.empty())
                response.erase(response.size() - 4);  // Remove last " -> "
        } else {
            response = "NOT EULERIAN";
        }

        send(client_socket, response.c_str(), response.size(), 0);
        close(client_socket);
    }

    close(server_fd);
    return 0;
}
