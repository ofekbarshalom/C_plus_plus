#ifndef MOLECULE_REQUESTER_HPP
#define MOLECULE_REQUESTER_HPP

#include <string>
#include <netinet/in.h>
#include <sys/un.h>


using namespace std;

namespace requester {

class MoleculeRequester {
public:
    MoleculeRequester(const string& hostname, int port);
    MoleculeRequester(const string& udsPath);
    bool connectToServer(); // Resolves hostname and sets up UDP socket
    void run();  // Start the interaction loop

private:
    string hostname;
    int port = -1;

    string udsPath;

    int sockfd = -1;             
    bool useUDS = false;  

    struct sockaddr_in servaddr;
    struct sockaddr_un servaddr_un;
};

} // namespace requester

#endif // MOLECULE_REQUESTER_HPP
