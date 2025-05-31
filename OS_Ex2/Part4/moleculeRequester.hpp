#ifndef MOLECULE_REQUESTER_HPP
#define MOLECULE_REQUESTER_HPP

#include <string>
#include <netinet/in.h>

using namespace std;

namespace requester {

class MoleculeRequester {
public:
    MoleculeRequester(const string& hostname, int port);
    bool connectToServer(); // Resolves hostname and sets up UDP socket
    void run();  // Start the interaction loop

private:
    string hostname;
    int port;
    int sockfd;
    struct sockaddr_in servaddr;
};

} // namespace requester

#endif // MOLECULE_REQUESTER_HPP
