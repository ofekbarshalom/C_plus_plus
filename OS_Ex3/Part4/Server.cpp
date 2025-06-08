#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <algorithm>
#include "ConvexHullVector.hpp"

using namespace std;
using namespace vectorConvexHull;

vector<Point> points;  // Shared list of points from all clients

// Per-client state for Newgraph parsing
struct ClientState {
    int expectedPoints = 0;
    vector<string> pendingLines;
};

unordered_map<int, ClientState> clientStates;

// Parse "x,y" string into doubles
void parsePoint(const string& str, double& x, double& y) {
    string s = str;
    s.erase(remove(s.begin(), s.end(), '\n'), s.end());
    replace(s.begin(), s.end(), ',', ' ');
    stringstream ss(s);
    ss >> x >> y;
}

// Handle one complete command from a client
void handleLine(int client_fd, const string& line) {
    ClientState& state = clientStates[client_fd];

    // Accumulate expected points from Newgraph
    if (state.expectedPoints > 0) {

        state.pendingLines.push_back(line);
        if ((int)state.pendingLines.size() == state.expectedPoints) {
            points.clear();
            for (const string& l : state.pendingLines) {
                double x, y;
                parsePoint(l, x, y);
                points.push_back({x, y});
            }
            state.expectedPoints = 0;
            state.pendingLines.clear();
        }

        return;
    }

    // Parse and execute commands
    if (line.rfind("Newgraph", 0) == 0) {
        int n = stoi(line.substr(9));
        state.expectedPoints = n;
        state.pendingLines.clear();
    } 

    else if (line == "CH") {
        vector<Point> hull = ConvexHull(points);
        double area = polygonArea(hull);
        string result = to_string(area) + "\n";
        write(client_fd, result.c_str(), result.length());
    }

    else if (line.rfind("Newpoint", 0) == 0) {
        double x, y;
        parsePoint(line.substr(9), x, y);
        points.push_back({x, y});
    }

    else if (line.rfind("Removepoint", 0) == 0) {
        double x, y;
        parsePoint(line.substr(12), x, y);
        auto it = remove_if(points.begin(), points.end(), [&](const Point& p) {
            return p.x == x && p.y == y;
        });
        points.erase(it, points.end());
    } 

    else {
        string err = "Unknown command\n";
        write(client_fd, err.c_str(), err.length());
    }
}

// Set socket to non-blocking mode
int setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(9034);

    if (bind(listener, (sockaddr*)&server, sizeof(server)) < 0) {
        perror("bind");
        return 1;
    }

    listen(listener, SOMAXCONN);
    cout << "Server listening on port 9034...\n";

    fd_set master_set, read_fds;
    FD_ZERO(&master_set);
    FD_SET(listener, &master_set);
    int fdmax = listener;

    while (true) {
        read_fds = master_set;
        if (select(fdmax + 1, &read_fds, nullptr, nullptr, nullptr) < 0) {
            perror("select");
            break;
        }

        // Check all file descriptors
        for (int fd = 0; fd <= fdmax; ++fd) {
            if (!FD_ISSET(fd, &read_fds)) continue;

            if (fd == listener) {
                // New client connection
                sockaddr_in client;
                socklen_t addrlen = sizeof(client);
                int newfd = accept(listener, (sockaddr*)&client, &addrlen);
                if (newfd >= 0) {
                    setNonBlocking(newfd);
                    FD_SET(newfd, &master_set);
                    if (newfd > fdmax) fdmax = newfd;
                    cout << "New client connected: " << newfd << endl;
                }
            }
            
            else {
                // Handle client message
                char buf[1024] = {0};
                int bytes = read(fd, buf, sizeof(buf));

                if (bytes <= 0) {
                    // Client disconnected
                    close(fd);
                    FD_CLR(fd, &master_set);
                    clientStates.erase(fd);
                    cout << "Client " << fd << " disconnected.\n";
                
                } 
                else {
                    // Clean and process input
                    string line(buf, bytes);
                    line.erase(remove(line.begin(), line.end(), '\r'), line.end());
                    line.erase(remove(line.begin(), line.end(), '\n'), line.end());
                    handleLine(fd, line);
                }
            }
        }
    }

    close(listener);
    return 0;
}
