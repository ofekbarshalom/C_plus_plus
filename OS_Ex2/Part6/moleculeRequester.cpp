#include "moleculeRequester.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/un.h>

#define BUFFER_SIZE 1024

using namespace std;

namespace requester {

MoleculeRequester::MoleculeRequester(const string& hostname, int port): hostname(hostname), port(port), sockfd(-1) {}

MoleculeRequester::MoleculeRequester(const string& path): udsPath(path), sockfd(-1), useUDS(true) {}


bool MoleculeRequester::connectToServer() {
    if (useUDS) {
        sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            perror("UDS socket creation failed");
            return false;
        }

        // Create a unique path for this client
        string tempPath = "/tmp/mreq_client_" + to_string(getpid());
        struct sockaddr_un client_addr;
        memset(&client_addr, 0, sizeof(client_addr));
        client_addr.sun_family = AF_UNIX;
        strncpy(client_addr.sun_path, tempPath.c_str(), sizeof(client_addr.sun_path) - 1);

        // Bind the client to the temp path
        unlink(tempPath.c_str()); // Remove if exists
        if (bind(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
            perror("UDS client bind failed");
            return false;
        }

        // Set server address
        memset(&servaddr_un, 0, sizeof(servaddr_un));
        servaddr_un.sun_family = AF_UNIX;
        strncpy(servaddr_un.sun_path, udsPath.c_str(), sizeof(servaddr_un.sun_path) - 1);

        // Store temp path so we can unlink it later
        udsPath = tempPath + "|" + udsPath;

        return true;
    }

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

        char buffer[BUFFER_SIZE] = {0};
        socklen_t len;

        if (useUDS) {
            sendto(sockfd, input.c_str(), input.length(), 0,
                   (struct sockaddr*)&servaddr_un, sizeof(servaddr_un));

            len = sizeof(servaddr_un);
            int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                             (struct sockaddr*)&servaddr_un, &len);

            if (n > 0) {
                cout << string(buffer, n);
            } else {
                cerr << "No response from server." << endl;
            }
        } else {
            sendto(sockfd, input.c_str(), input.length(), 0,
                   (struct sockaddr*)&servaddr, sizeof(servaddr));

            len = sizeof(servaddr);
            int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                             (struct sockaddr*)&servaddr, &len);

            if (n > 0) {
                cout << string(buffer, n);
            } else {
                cerr << "No response from server." << endl;
            }
        }
    }

    if (useUDS) {
        string clientPath = udsPath.substr(0, udsPath.find('|'));
        unlink(clientPath.c_str());
    }

    close(sockfd);
}

} // namespace requester
