#include "atomWarehouse.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <vector>

#define BUFFER_SIZE 1024

using namespace std;

namespace warehouse {

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

void AtomWarehouse::runTCPServer(int port) {
	int server_fd, new_socket, max_sd, sd, activity;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[BUFFER_SIZE];
	fd_set readfds;
	vector<int> clients;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

	bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    cout << "Atom warehouse TCP server listening on port " << port << "...\n";

while (true) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (int client : clients) {
            FD_SET(client, &readfds);
            if (client > max_sd) max_sd = client;
        }

        activity = select(max_sd + 1, &readfds, nullptr, nullptr, nullptr);

        if (activity < 0) {
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(server_fd, &readfds)) {
            new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
            clients.push_back(new_socket);
            cout << "New client connected.\n";
        }

        for (auto it = clients.begin(); it != clients.end(); ) {
            sd = *it;
            if (FD_ISSET(sd, &readfds)) {
                memset(buffer, 0, BUFFER_SIZE);
                int valread = read(sd, buffer, BUFFER_SIZE);
                if (valread <= 0) {
                    close(sd);
                    it = clients.erase(it);
                    cout << "Client disconnected.\n";
                    continue;
                } else {
                    string input(buffer);
                    istringstream iss(input);
                    string cmd, atom;
                    int count;
                    iss >> cmd >> atom >> count;

                    if (cmd != "ADD" || !isValidAtom(atom) || count < 0) {
                        cerr << "Error: Invalid command -> " << input << endl;
                    } else {
                        addAtom(atom, count);
                        printInventory();
                    }
                }
            }
            ++it;
        }
    }
}

} // namespace warehouse