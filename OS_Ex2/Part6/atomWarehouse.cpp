#include "atomWarehouse.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
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

AtomWarehouse::AtomWarehouse(const string& saveFile) : saveFilePath(saveFile) {
    if (!saveFilePath.empty()) {
        bool exists = access(saveFilePath.c_str(), F_OK) == 0;

        saveFileFd = open(saveFilePath.c_str(), O_RDWR | O_CREAT, 0666);
        if (saveFileFd < 0) {
            perror("Failed to open save file");
            exit(EXIT_FAILURE);
        }

        // Ensure file size is enough
        if (ftruncate(saveFileFd, sizeof(SharedInventory)) < 0) {
            perror("ftruncate failed");
            exit(EXIT_FAILURE);
        }

        void* map = mmap(nullptr, sizeof(SharedInventory), PROT_READ | PROT_WRITE, MAP_SHARED, saveFileFd, 0);
        if (map == MAP_FAILED) {
            perror("mmap failed");
            exit(EXIT_FAILURE);
        }

        sharedInventory = static_cast<SharedInventory*>(map);

        if (!exists) {
            sharedInventory->carbon = 0;
            sharedInventory->hydrogen = 0;
            sharedInventory->oxygen = 0;
        }

    } else {
        inventory = {
            {"CARBON", 0},
            {"OXYGEN", 0},
            {"HYDROGEN", 0}
        };
    }
}


AtomWarehouse::~AtomWarehouse() {
    if (sharedInventory) {
        munmap(sharedInventory, sizeof(SharedInventory));
        sharedInventory = nullptr;
    }

    if (saveFileFd != -1) {
        close(saveFileFd);
        saveFileFd = -1;
    }
}


void AtomWarehouse::initializeSharedInventory(int carbon, int hydrogen, int oxygen) {
    if (sharedInventory) {
        sharedInventory->carbon = carbon;
        sharedInventory->hydrogen = hydrogen;
        sharedInventory->oxygen = oxygen;
    } else {
        inventory["CARBON"] = carbon;
        inventory["HYDROGEN"] = hydrogen;
        inventory["OXYGEN"] = oxygen;
    }
}


bool AtomWarehouse::isValidAtom(const string& type) const {
    return type == "CARBON" || type == "HYDROGEN" || type == "OXYGEN";
}


void AtomWarehouse::addAtom(const string& type, unsigned int count) {
    if (sharedInventory) {
        if (type == "CARBON") {
            sharedInventory->carbon += count;
        } else if (type == "HYDROGEN") {
            sharedInventory->hydrogen += count;
        } else if (type == "OXYGEN") {
            sharedInventory->oxygen += count;
        }
    } else {
        inventory[type] += count;
    }
}


void AtomWarehouse::printInventory() const {
    cout << "------------------------\n";
    cout << "Inventory:\n";

    if (sharedInventory) {
        cout << "CARBON: " << sharedInventory->carbon << endl;
        cout << "OXYGEN: " << sharedInventory->oxygen << endl;
        cout << "HYDROGEN: " << sharedInventory->hydrogen << endl;
    } else {
        for (const auto& pair : inventory) {
            cout << pair.first << ": " << pair.second << endl;
        }
    }

    cout << "------------------------\n";
}


bool AtomWarehouse::canDeliver(const string& molecule, unsigned int count) const {
    const auto& req = moleculeRequirements.at(molecule);

    for (const auto& pair : req) {
        const string& atom = pair.first;
        unsigned int needed = static_cast<unsigned int>(pair.second) * count;

        if (sharedInventory) {
            if (atom == "CARBON" && static_cast<unsigned int>(sharedInventory->carbon) < needed) return false;
            if (atom == "HYDROGEN" && static_cast<unsigned int>(sharedInventory->hydrogen) < needed) return false;
            if (atom == "OXYGEN" && static_cast<unsigned int>(sharedInventory->oxygen) < needed) return false;
        } else {
            if (inventory.at(atom) < needed) return false;
        }
    }

    return true;
}


void AtomWarehouse::deliverMolecule(const string& molecule, int count) {
    const auto& req = moleculeRequirements.at(molecule);

    for (const auto& pair : req) {
        const string& atom = pair.first;
        int toSubtract = pair.second * count;

        if (sharedInventory) {
            if (atom == "CARBON") sharedInventory->carbon -= toSubtract;
            else if (atom == "HYDROGEN") sharedInventory->hydrogen -= toSubtract;
            else if (atom == "OXYGEN") sharedInventory->oxygen -= toSubtract;
        } else {
            inventory[atom] -= toSubtract;
        }
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
        int available = 0;

        if (sharedInventory) {
            if (atom == "CARBON") available = sharedInventory->carbon;
            else if (atom == "HYDROGEN") available = sharedInventory->hydrogen;
            else if (atom == "OXYGEN") available = sharedInventory->oxygen;
        } else {
            available = inventory.at(atom);
        }
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


void AtomWarehouse::handleUnixDatagramRequest(const string& input, int sockfd, struct ::sockaddr_un& cli_addr, socklen_t cli_len) {
    istringstream iss(input);
    string cmd, molecule;
    int count;
    iss >> cmd >> ws;
    getline(iss, molecule, ' ');
    iss >> count;

    if (cmd != "DELIVER" || moleculeRequirements.find(molecule) == moleculeRequirements.end() || count < 0) {
        string msg = "Error: Invalid command -> " + input + "\n";
        cerr << msg;
        sendto(sockfd, msg.c_str(), msg.length(), 0, (struct sockaddr*)&cli_addr, cli_len);
        return;
    }

    if (canDeliver(molecule, count)) {
        deliverMolecule(molecule, count);
        printInventory();
        string msg = "Delivered " + to_string(count) + " " + molecule + "\n";
        sendto(sockfd, msg.c_str(), msg.length(), 0, (struct sockaddr*)&cli_addr, cli_len);
    } else {
        string msg = "Error: Not enough atoms to deliver " + to_string(count) + " " + molecule + "\n";
        cerr << msg;
        sendto(sockfd, msg.c_str(), msg.length(), 0, (struct sockaddr*)&cli_addr, cli_len);
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


void AtomWarehouse::setupUnixStreamSocket(const string& path) {
    unlink(path.c_str());   // Remove the socket file if it exists

    unixStreamFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (unixStreamFd < 0) {
        perror("stream socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct ::sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);

    if (bind(unixStreamFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind stream socket failed");
        exit(EXIT_FAILURE);
    }

    if (listen(unixStreamFd, 5) < 0) {
        perror("listen stream socket failed");
        exit(EXIT_FAILURE);
    }

    streamPath = path;
    cout << "Unix STREAM socket listening on " << path << endl;
}


void AtomWarehouse::setupUnixDatagramSocket(const string& path) {
    unlink(path.c_str());
    
    unixDatagramFd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (unixDatagramFd < 0) {
        perror("datagram socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct ::sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);

    if (bind(unixDatagramFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind datagram socket failed");
        exit(EXIT_FAILURE);
    }

    datagramPath = path;
    cout << "Unix DATAGRAM socket listening on " << path << endl;
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
        if (unixStreamFd != -1) FD_SET(unixStreamFd, &readfds);
        if (unixDatagramFd != -1) FD_SET(unixDatagramFd, &readfds);
        int max_fd = max(max(max(tcp_fd, udp_fd), STDIN_FILENO), max(unixStreamFd, unixDatagramFd));

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
            if (timeout > 0) alarm(timeout);
        }

        if (FD_ISSET(tcp_fd, &readfds)) {
            new_socket = accept(tcp_fd, (struct sockaddr*)&cli_addr, &cli_len);
            clients.push_back(new_socket);
            cout << "New TCP client connected.\n";
            if (timeout > 0) alarm(timeout);
        }

        if (unixStreamFd != -1 && FD_ISSET(unixStreamFd, &readfds)) {
            int client_fd = accept(unixStreamFd, nullptr, nullptr);
            if (client_fd >= 0) {
                clients.push_back(client_fd);
                cout << "New UDS STREAM client connected.\n";
                if (timeout > 0) alarm(timeout);
            }
        }
        
        if (FD_ISSET(udp_fd, &readfds)) {
            memset(buffer, 0, BUFFER_SIZE);
            recvfrom(udp_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&cli_addr, &cli_len);
            handleUDPRequest(string(buffer), udp_fd, cli_addr, cli_len);
            if (timeout > 0) alarm(timeout);
        }

        if (unixDatagramFd != -1 && FD_ISSET(unixDatagramFd, &readfds)) {
            memset(buffer, 0, BUFFER_SIZE);
            struct ::sockaddr_un cli_addr;
            memset(&cli_addr, 0, sizeof(cli_addr));
            socklen_t cli_len = sizeof(cli_addr);
            recvfrom(unixDatagramFd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&cli_addr, &cli_len);

            string input(buffer);
            handleUnixDatagramRequest(input, unixDatagramFd, cli_addr, cli_len);
            if (timeout > 0) alarm(timeout);
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
                    if (timeout > 0) alarm(timeout);
                }
            }
            ++it;
        }
    }
}

} // namespace warehouse
