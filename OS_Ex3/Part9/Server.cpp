#include "Reactor.hpp"
#include "Proactor.hpp"
#include "ConvexHullVector.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <cstring>
#include <chrono>
#include <thread>

using namespace std;
using namespace vectorConvexHull;

reactor r;  // Reactor object for managing accept events
vector<Point> points;  // Shared geometry points
mutex pointsMutex;     // Mutex to protect access to `points`

// Per-client state
struct ClientState {
    int expectedPoints = 0;
    vector<string> pendingLines;
    string buffer;  // accumulates partial lines
};

unordered_map<int, ClientState> clientStates;

// Parse string into x and y
void parsePoint(const string& str, double& x, double& y) {
    string s = str;
    s.erase(remove(s.begin(), s.end(), '\n'), s.end());
    replace(s.begin(), s.end(), ',', ' ');
    stringstream ss(s);
    ss >> x >> y;
}

// Handle a full command from client
void handleLine(int client_fd, const string& line) {
    ClientState& state = clientStates[client_fd];

    if (state.expectedPoints > 0) {
        state.pendingLines.push_back(line);
        if ((int)state.pendingLines.size() == state.expectedPoints) {
            lock_guard<mutex> lock(pointsMutex);
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
            lock_guard<mutex> lock(pointsMutex);
            std::this_thread::sleep_for(std::chrono::seconds(2));  // simulate heavy calculation
            vector<Point> hull = ConvexHull(points);
            area = polygonArea(hull);
        }
        string result = to_string(area) + "\n";
        write(client_fd, result.c_str(), result.length());
    }
    else if (line.rfind("Newpoint", 0) == 0) {
        double x, y;
        parsePoint(line.substr(9), x, y);
        lock_guard<mutex> lock(pointsMutex);
        std::this_thread::sleep_for(std::chrono::seconds(2));  // simulate contention
        points.push_back({x, y});
    }
    else if (line.rfind("Removepoint", 0) == 0) {
        double x, y;
        parsePoint(line.substr(12), x, y);
        lock_guard<mutex> lock(pointsMutex);
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

// Thread function for each client
void* clientThreadFunc(int client_fd) {
    char buf[1024];
    clientStates[client_fd] = ClientState();

    while (true) {
        memset(buf, 0, sizeof(buf));
        int bytes = read(client_fd, buf, sizeof(buf));
        if (bytes <= 0) {
            close(client_fd);
            {
                lock_guard<mutex> lock(pointsMutex);
                clientStates.erase(client_fd);
            }
            cout << "Client " << client_fd << " disconnected.\n";
            return nullptr;
        }

        ClientState& state = clientStates[client_fd];
        state.buffer += string(buf, bytes);
        size_t pos;
        while ((pos = state.buffer.find('\n')) != string::npos) {
            string line = state.buffer.substr(0, pos);
            state.buffer.erase(0, pos + 1);
            handleLine(client_fd, line);
        }
    }

    return nullptr;
}

// Called by Reactor when new connection arrives
void listenerCallback(int listener_fd) {
    sockaddr_in client{};
    socklen_t len = sizeof(client);
    int client_fd = accept(listener_fd, (sockaddr*)&client, &len);
    if (client_fd >= 0) {
        cout << "Accepted client: " << client_fd << endl;
        startProactor(client_fd, clientThreadFunc);  // Hand over to Proactor (threaded)
    }
}

int main() {
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(9034);

    bind(listener, (sockaddr*)&server, sizeof(server));
    listen(listener, SOMAXCONN);
    fcntl(listener, F_SETFL, O_NONBLOCK);  // Needed for select()
    cout << "Hybrid Reactor + Proactor server on port 9034...\n";

    r = startReactor();
    addFdToReactor(r, listener, listenerCallback);  // Reactor waits for new connections
    runReactor(r);  // Starts the Reactor loop

    close(listener);
    return 0;
}
