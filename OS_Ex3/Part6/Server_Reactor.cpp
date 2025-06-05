#include "Reactor.hpp"
#include "ConvexHullVector.hpp"
#include "Point.hpp"

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;
using namespace vectorConvexHull;

#define PORT 9034                           // Server port

reactor r;      
int listener;                               // Server socket for listening
vector<Point> points;                       // Global point collection

// Parses a string into two doubles
void parsePoint(const string& str, double& x, double& y) {
    string s = str;
    replace(s.begin(), s.end(), ',', ' ');
    stringstream ss(s);
    ss >> x >> y;
}

void handleLine(int client_fd, const string& line) {
    static unordered_map<int, vector<string>> pendingLines;
    static unordered_map<int, int> expectedPoints;

    // Trim trailing newline and carriage return
    string cleanLine = line;
    while (!cleanLine.empty() && (cleanLine.back() == '\n' || cleanLine.back() == '\r')) {
        cleanLine.pop_back();
    }

    // If waiting for point data after "Newgraph"
    if (expectedPoints[client_fd] > 0) {
        pendingLines[client_fd].push_back(cleanLine);

        if ((int)pendingLines[client_fd].size() == expectedPoints[client_fd]) {
            points.clear();

            for (const string& l : pendingLines[client_fd]) {
                double x, y;
                parsePoint(l, x, y);
                points.push_back({x, y});
            }

            expectedPoints[client_fd] = 0;
            pendingLines[client_fd].clear();
        }

        return;
    }

    // Newgraph N
    if (cleanLine.rfind("Newgraph", 0) == 0) {
        int n = stoi(cleanLine.substr(9));
        expectedPoints[client_fd] = n;
        pendingLines[client_fd].clear();
    }

    // CH: Compute convex hull and send area
    else if (cleanLine == "CH") {
        vector<Point> hull = ConvexHull(points);
        double area = polygonArea(hull);
        string result = to_string(area) + "\n";
        write(client_fd, result.c_str(), result.length());
    }

    // Newpoint x,y
    else if (cleanLine.rfind("Newpoint", 0) == 0) {
        double x, y;
        parsePoint(cleanLine.substr(9), x, y);
        points.push_back({x, y});
    }

    // Removepoint x,y
    else if (cleanLine.rfind("Removepoint", 0) == 0) {
        double x, y;
        parsePoint(cleanLine.substr(12), x, y);
        auto it = remove_if(points.begin(), points.end(), [&](const Point& p) {
            return p.x == x && p.y == y;
        });
        points.erase(it, points.end());
    }

    // Unknown command
    else {
        string err = "Unknown command\n";
        write(client_fd, err.c_str(), err.length());
    }
}


// Callback when a client socket has data
void handleClient(int fd) {
    char buf[1024];
    int bytes = read(fd, buf, sizeof(buf));

    // Client disconnected or error
    if (bytes <= 0) {
        close(fd);
        removeFdFromReactor(r, fd);
        cout << "Client " << fd << " disconnected.\n";
        return;
    }

    string line(buf, bytes);
    handleLine(fd, line);
}

// Called when a new client connects
void handleNewConnection(int listener_fd) {
    sockaddr_in clientAddr;
    socklen_t addrlen = sizeof(clientAddr);
    int client_fd = accept(listener_fd, (sockaddr*)&clientAddr, &addrlen);

    if (client_fd >= 0) {
        addFdToReactor(r, client_fd, handleClient);
        cout << "New client connected: " << client_fd << endl;
    }
}


int main() {
    // Create socket
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("socket");
        return 1;
    }

    // Bind to port 9034 on all interfaces
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(listener, (sockaddr*)&server, sizeof(server)) < 0) {
        perror("bind");
        return 1;
    }

    // Listen for incoming connections
    listen(listener, SOMAXCONN);
    cout << "Server listening on port " << PORT << "...\n";

    // Start reactor loop
    r = startReactor();
    addFdToReactor(r, listener, handleNewConnection);
    runReactor(r);

    // Cleanup
    close(listener);
    return 0;
}
