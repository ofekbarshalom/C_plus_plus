#include "atomWarehouse.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <vector>
#include <climits>

#define BUFFER_SIZE 1024

using namespace std;

namespace warehouse {

const unordered_map<string, unordered_map<string, int>> AtomWarehouse::moleculeRequirements = {
    {"WATER",           {{"HYDROGEN", 2}, {"OXYGEN", 1}}},
    {"CARBON DIOXIDE",  {{"CARBON", 1}, {"OXYGEN", 2}}},
    {"ALCOHOL",         {{"CARBON", 2}, {"HYDROGEN", 6}, {"OXYGEN", 1}}},
    {"GLUCOSE",         {{"CARBON", 6}, {"HYDROGEN", 12}, {"OXYGEN", 6}}}
};

AtomWarehouse::AtomWarehouse() {
    inventory = {
        {"CARBON", 0},
        {"OXYGEN", 0},
        {"HYDROGEN", 0}
    };
}

bool AtomWarehouse::isValidAtom(const string& type) const {
    return inventory.find(type) != inventory.end();
}

void AtomWarehouse::addAtom(const string& type, unsigned int count) {
    inventory[type] += count;
}

void AtomWarehouse::printInventory() const {
    cout << "Inventory:\n";
    for (const auto& pair : inventory) {
        cout << pair.first << ": " << pair.second << endl;
    }
}

bool AtomWarehouse::canDeliver(const string& molecule, unsigned int count) const {
    const auto& req = moleculeRequirements.at(molecule);
    for (const auto& pair : req) {
        const string& atom = pair.first;
        unsigned int needed = pair.second * count;
        if (inventory.at(atom) < needed)
            return false;
    }
    return true;
}

void AtomWarehouse::deliverMolecule(const string& molecule, int count) {
    const auto& req = moleculeRequirements.at(molecule);
    for (const auto& pair : req) {
        const string& atom = pair.first;
        int toSubtract = pair.second * count;
        inventory[atom] -= toSubtract;
    }
}

void AtomWarehouse::handleConsoleInput() {
    string line;
    getline(cin, line);
    istringstream iss(line);
    string cmd, drink;
    iss >> cmd >> ws;
    getline(iss, drink);

    if (cmd != "GEN") {
        cout << "Error: Invalid command -> " << line << endl;
        return;
    }

    unordered_map<string, int> totalAtoms;

    if (drink == "SOFT DRINK") {
        totalAtoms = {
            {"HYDROGEN", 14},  // 2 (WATER) + 12 (GLUCOSE)
            {"OXYGEN", 9},     // 1 (WATER) + 2 (CO2) + 6 (GLUCOSE)
            {"CARBON", 7}      // 1 (CO2) + 6 (GLUCOSE)
        };
    } else if (drink == "VODKA") {
        totalAtoms = {
            {"HYDROGEN", 14},  // 2 (WATER) + 12 (GLUCOSE)
            {"OXYGEN", 9},     // 1 (WATER) + 2 (CO2) + 6 (GLUCOSE)
            {"CARBON", 7}      // 1 (CO2) + 6 (GLUCOSE)
        };
    } else if (drink == "CHAMPAGNE") {
         totalAtoms = {
            {"HYDROGEN", 8},   // 2 (WATER) + 6 (ALCOHOL)
            {"OXYGEN", 4},     // 1 (WATER) + 2 (CO2) + 1 (ALCOHOL)
            {"CARBON", 3}      // 1 (CO2) + 2 (ALCOHOL)
        };
    } else {
        cout << "Error: Unknown drink -> " << drink << endl;
        return;
    }

    int maxDrinks = INT_MAX;
    for (const auto& [atom, required] : totalAtoms) {
        int available = inventory.at(atom);
        maxDrinks = min(maxDrinks, available / required);
    }

    cout << "You can make " << maxDrinks << " " << drink << (maxDrinks == 1 ? "" : "s") << "." << endl;
}

void AtomWarehouse::handleUDPRequest(const string& input, int udp_fd, struct sockaddr_in& cli_addr, socklen_t cli_len) {
    istringstream iss(input);
    string cmd, molecule;
    int count;
    iss >> cmd >> ws;
    getline(iss, molecule, ' ');
    iss >> count;

    if (cmd != "DELIVER" || moleculeRequirements.find(molecule) == moleculeRequirements.end() || count < 0) {
        string msg = "Error: Invalid command -> " + input + "\n";
        cerr << msg;
        sendto(udp_fd, msg.c_str(), msg.length(), 0, (struct sockaddr*)&cli_addr, cli_len);
        return;
    }

    if (canDeliver(molecule, count)) {
        deliverMolecule(molecule, count);
        printInventory();
        string msg = "Delivered " + to_string(count) + " " + molecule + "\n";
        sendto(udp_fd, msg.c_str(), msg.length(), 0, (struct sockaddr*)&cli_addr, cli_len);
    } else {
        string msg = "Error: Not enough atoms to deliver " + to_string(count) + " " + molecule + "\n";
        cerr << msg;
        sendto(udp_fd, msg.c_str(), msg.length(), 0, (struct sockaddr*)&cli_addr, cli_len);
    }
}

void AtomWarehouse::handleTCPClientMessage(int clientFd, const string& input) {
    istringstream iss(input);
    string cmd, atom;
    int count;
    iss >> cmd >> atom >> count;

    if (cmd != "ADD" || !isValidAtom(atom) || count < 0) {
        string msg = "Error: Invalid command -> " + input + "\n";
        cerr << msg;
        send(clientFd, msg.c_str(), msg.length(), 0);
    } else {
        addAtom(atom, count);
        printInventory();
    }
}

void AtomWarehouse::runCombinedServer(int tcpPort, int udpPort) {
    int tcp_fd, udp_fd, new_socket, sd, activity;
    struct sockaddr_in tcp_addr{}, udp_addr{}, cli_addr{};
    socklen_t cli_len = sizeof(cli_addr);
    vector<int> clients;
    fd_set readfds;
    char buffer[BUFFER_SIZE];

    tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(tcp_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_addr.s_addr = INADDR_ANY;
    tcp_addr.sin_port = htons(tcpPort);
    bind(tcp_fd, (struct sockaddr*)&tcp_addr, sizeof(tcp_addr));
    listen(tcp_fd, 5);
    cout << "TCP server listening on port " << tcpPort << "...\n";

    udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    udp_addr.sin_family = AF_INET;
    udp_addr.sin_addr.s_addr = INADDR_ANY;
    udp_addr.sin_port = htons(udpPort);
    bind(udp_fd, (struct sockaddr*)&udp_addr, sizeof(udp_addr));
    cout << "UDP server listening on port " << udpPort << "...\n";

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(tcp_fd, &readfds);
        FD_SET(udp_fd, &readfds);
        FD_SET(STDIN_FILENO, &readfds);
        int max_fd = max(max(tcp_fd, udp_fd), STDIN_FILENO);

        for (int client : clients) {
            FD_SET(client, &readfds);
            if (client > max_fd) max_fd = client;
        }

        activity = select(max_fd + 1, &readfds, nullptr, nullptr, nullptr);
        if (activity < 0) {
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            handleConsoleInput();
        }

        if (FD_ISSET(tcp_fd, &readfds)) {
            new_socket = accept(tcp_fd, (struct sockaddr*)&cli_addr, &cli_len);
            clients.push_back(new_socket);
            cout << "New TCP client connected.\n";
        }

        if (FD_ISSET(udp_fd, &readfds)) {
            memset(buffer, 0, BUFFER_SIZE);
            recvfrom(udp_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&cli_addr, &cli_len);
            handleUDPRequest(string(buffer), udp_fd, cli_addr, cli_len);
        }

        for (auto it = clients.begin(); it != clients.end(); ) {
            sd = *it;
            if (FD_ISSET(sd, &readfds)) {
                memset(buffer, 0, BUFFER_SIZE);
                int valread = read(sd, buffer, BUFFER_SIZE);
                if (valread <= 0) {
                    close(sd);
                    it = clients.erase(it);
                    cout << "TCP client disconnected.\n";
                    continue;
                } else {
                    handleTCPClientMessage(sd, string(buffer));
                }
            }
            ++it;
        }
    }
}

} // namespace warehouse
