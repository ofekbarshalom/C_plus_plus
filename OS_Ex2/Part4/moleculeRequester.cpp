#include "moleculeRequester.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

using namespace std;

namespace requester {

MoleculeRequester::MoleculeRequester(const string& hostname, int port): hostname(hostname), port(port), sockfd(-1) {}

bool MoleculeRequester::connectToServer() {
    struct hostent* server = gethostbyname(hostname.c_str());
    if (!server) {
        cerr << "Error: No such host: " << hostname << endl;
        return false;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return false;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    memcpy(&servaddr.sin_addr, server->h_addr, server->h_length);

    return true;
}

void MoleculeRequester::run() {
    cout << "Enter DELIVER commands (e.g., DELIVER WATER 2), or 'exit':" << endl;

    while (true) {
        cout << "> ";
        string input;
        getline(cin, input);
        if (input == "exit") break;

        sendto(sockfd, input.c_str(), input.length(), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));

        char buffer[BUFFER_SIZE] = {0};
        socklen_t len = sizeof(servaddr);
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&servaddr, &len);

        if (n > 0) {
            cout << string(buffer, n);
        } else {
            cerr << "No response from server." << endl;
        }
    }

    close(sockfd);
}

} // namespace requester
