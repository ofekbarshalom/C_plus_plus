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

// Read graph structure from client (vertex count, edge count, directed/undirected, and edges)
Graph readGraphFromClient(int client_socket) {
    char buffer[MAX_BUFFER];
    string graphInput;

    // Read first line: number of vertices, edges, and direction flag
    memset(buffer, 0, MAX_BUFFER);
    int bytesRead = read(client_socket, buffer, MAX_BUFFER);
    if (bytesRead <= 0) throw runtime_error("Connection lost while reading graph");

    graphInput += buffer;
    istringstream firstLine(graphInput);
    int v, e, d;
    firstLine >> v >> e >> d;

    Graph g(v, d == 1 ? false : true);  // Create graph object

    // Read edges
    int edgesRead = 0;
    while (edgesRead < e) {
        memset(buffer, 0, MAX_BUFFER);
        bytesRead = read(client_socket, buffer, MAX_BUFFER);
        if (bytesRead <= 0) throw runtime_error("Connection lost while reading edges");

        istringstream edgeLines(buffer);
        while (edgesRead < e) {
            int u, w;
            if (!(edgeLines >> u >> w)) break;
            g.addEdge(u, w);
            edgesRead++;
        }
    }

    return g;
}

// Send menu to client
void sendMenu(int client_socket) {
    string menu = "\nChoose algorithm:\n";
    menu += "1 - Eulerian Circuit\n";
    menu += "2 - MST Weight\n";
    menu += "3 - SCC (Directed)\n";
    menu += "4 - Max Clique\n";
    menu += "5 - Max Flow (Directed)\n";
    menu += "0 - Exit\n";
    send(client_socket, menu.c_str(), menu.size(), 0);
}

// Read algorithm choice from client
int readChoice(int client_socket) {
    char buffer[MAX_BUFFER];
    memset(buffer, 0, MAX_BUFFER);
    int bytesRead = read(client_socket, buffer, MAX_BUFFER);
    if (bytesRead <= 0) return -1;
    return stoi(string(buffer));
}

// Handle one client session 
void handleClient(int client_socket) {
    try {
        Graph g = readGraphFromClient(client_socket);

        // Infinite loop for menu interaction until client exits
        while (true) {
            sendMenu(client_socket);
            int choice = readChoice(client_socket);
            if (choice == 0 || choice == -1) break;

            string response;

            if (choice == 1) {  // Eulerian Circuit
                if (isEulerian(g)) {
                    vector<int> circuit = findEulerianCircuit(g);
                    response = "EULERIAN CIRCUIT: ";
                    for (int v : circuit)
                        response += to_string(v) + " -> ";
                    if (!circuit.empty())
                        response.erase(response.size() - 4);
                    response += "\n";
                } else {
                    response = "NOT EULERIAN\n";
                }
            }

            else if (choice == 2) {  // MST Weight
                int weight = findMSTWeight(g);
                response = "MST Weight: " + to_string(weight) + "\n";
            }

            else if (choice == 3) {  // SCC
                vector<vector<int>> scc = findSCC(g);
                response = "SCC Components (" + to_string(scc.size()) + "):\n";
                for (const auto& comp : scc) {
                    response += "{ ";
                    for (int v : comp)
                        response += to_string(v) + " ";
                    response += "}\n";
                }
            }

            else if (choice == 4) {  // Max Clique
                vector<int> clique = findMaxClique(g);
                response = "Max Clique (" + to_string(clique.size()) + "): ";
                for (int v : clique)
                    response += to_string(v) + " ";
                response += "\n";
            }

            else if (choice == 5) {  // Max Flow
                int flow = findMaxFlow(g);
                response = "Max Flow: " + to_string(flow) + "\n";
            }

            else {  // Invalid option
                response = "Invalid choice.\n";
            }

            send(client_socket, response.c_str(), response.size(), 0);
        }

    } catch (...) {
        cout << "Client disconnected or error occurred." << endl;
    }
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[MAX_BUFFER];

    // Create socket and bind to port
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    listen(server_fd, 3);
    cout << "Waiting for connections on port " << PORT << "...\n";

    // Main loop: accept new client, handle interaction
    while (true) {
        memset(buffer, 0, MAX_BUFFER);
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        handleClient(client_socket);
        close(client_socket);
    }

    close(server_fd);
    return 0;
}
