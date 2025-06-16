#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <netinet/in.h>
#include <thread>
#include <unistd.h>
#include "../core/Graph.hpp"
#include "../core/GraphAlgorithms.hpp"
#include "../algorithms/AlgorithmFactory.hpp"

using namespace std;
using namespace GraphAlgo;

#define PORT 8080
#define MAX_BUFFER 4096

// Helper: read a single full line from socket
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

Graph readGraphFromClient(int client_socket) {
    // Read first line: v e d
    string firstLine = readLine(client_socket);
    istringstream firstStream(firstLine);
    int v, e, d;
    firstStream >> v >> e >> d;

    Graph g(v, d == 1 ? false : true);

    // Read exactly e edge lines
    for (int i = 0; i < e; ++i) {
        string edgeLine = readLine(client_socket);
        istringstream edgeStream(edgeLine);
        int u, w;
        edgeStream >> u >> w;
        g.addEdge(u, w);
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

            auto algorithm = AlgorithmFactory::create(choice);
            string response;

            if (!algorithm) {
                response = "Invalid choice.\n";
            } else {
                response = algorithm->execute(g);
            }

            send(client_socket, response.c_str(), response.size(), 0);
        }

    } catch (...) {
        cout << "Client disconnected or error occurred." << endl;
    }

    close(client_socket); 
    cout << "Client disconnected (fd = " << client_socket  << ")" << endl;
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
        if (client_socket < 0) {
            cerr << "Failed to accept connection.\n";
            continue;
        }

        cout << "Client connected (fd = " << client_socket << ")" <<endl;

        thread t(handleClient, client_socket);
        t.detach();
    }

    close(server_fd);
    return 0;
}
