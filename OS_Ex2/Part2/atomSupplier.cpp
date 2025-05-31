#include "atomSupplier.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

#define BUFFER_SIZE 1024

using namespace std;

namespace supplier {

AtomSupplier::AtomSupplier(const string& hostname, int port): hostname(hostname), port(port), sockfd(-1) {}

bool AtomSupplier::connectToServer() {
    struct hostent* server = gethostbyname(hostname.c_str());
    if (!server) {
        cerr << "Error: No such host: " << hostname << endl;
        return false;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return false;
    }

    struct sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to server");
        return false;
    }

    cout << "Connected to " << hostname << ":" << port << endl;
    return true;
}

void AtomSupplier::run() {
    cout << "Enter commands, Type 'exit' to quit." << endl;
    string input;

    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input == "exit") break;

        send(sockfd, input.c_str(), input.size(), 0);
    }

    close(sockfd);
}

} // namespace supplier