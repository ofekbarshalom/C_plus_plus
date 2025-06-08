#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <thread>
#include <mutex>
#include <algorithm>
#include <chrono>
#include "ConvexHullVector.hpp"

using namespace std;
using namespace vectorConvexHull;

// Shared geometry state
vector<Point> points;
std::mutex pointsMutex;

// Per-client parsing state
struct ClientState {
    int expectedPoints = 0;
    vector<string> pendingLines;
};

unordered_map<int, ClientState> clientStates;

// Parse a string like "3.5,4.6" into x and y
void parsePoint(const string& str, double& x, double& y) {
    string s = str;
    s.erase(remove(s.begin(), s.end(), '\n'), s.end());
    replace(s.begin(), s.end(), ',', ' ');
    stringstream ss(s);
    ss >> x >> y;
}

// Handle a full line from a client
void handleLine(int client_fd, const string& line) {
    ClientState& state = clientStates[client_fd];

    if (state.expectedPoints > 0) {
        state.pendingLines.push_back(line);
        if ((int)state.pendingLines.size() == state.expectedPoints) {
            std::lock_guard<std::mutex> lock(pointsMutex);
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

    if (line.rfind("Newgraph", 0) == 0) {
        int n = stoi(line.substr(9));
        state.expectedPoints = n;
        state.pendingLines.clear();
    }
    else if (line == "CH") {
        double area;
        {
            std::lock_guard<std::mutex> lock(pointsMutex);
            //std::this_thread::sleep_for(std::chrono::seconds(2));  to test the mutex

            vector<Point> hull = ConvexHull(points);
            area = polygonArea(hull);
        }
        string result = to_string(area) + "\n";
        write(client_fd, result.c_str(), result.length());
    }
    else if (line.rfind("Newpoint", 0) == 0) {
        double x, y;

        parsePoint(line.substr(9), x, y);
        std::lock_guard<std::mutex> lock(pointsMutex);
        //std::this_thread::sleep_for(std::chrono::seconds(2));   to test the mutex

        points.push_back({x, y});
    }
    else if (line.rfind("Removepoint", 0) == 0) {
        double x, y;
        parsePoint(line.substr(12), x, y);
        std::lock_guard<std::mutex> lock(pointsMutex);
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

// Handles a single client socket in its own thread
void clientHandler(int client_fd) {
    char buf[1024];

    while (true) {
        memset(buf, 0, sizeof(buf));
        int bytes = read(client_fd, buf, sizeof(buf));
        if (bytes <= 0) {
            close(client_fd);
            {
                std::lock_guard<std::mutex> lock(pointsMutex);
                clientStates.erase(client_fd);
            }
            cout << "Client " << client_fd << " disconnected." << endl;
            return;
        }

        string line(buf, bytes);
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(remove(line.begin(), line.end(), '\n'), line.end());

        handleLine(client_fd, line);
    }
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
    cout << "Server listening on port 9034..." << endl;

    while (true) {
        sockaddr_in client;
        socklen_t addrlen = sizeof(client);
        int client_fd = accept(listener, (sockaddr*)&client, &addrlen);
        if (client_fd >= 0) {
            cout << "New client connected: " << client_fd << endl;
            std::thread(clientHandler, client_fd).detach();  // Spawn new thread per client
        }
    }

    close(listener);
    return 0;
}
