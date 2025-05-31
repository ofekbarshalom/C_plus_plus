#ifndef ATOM_SUPPLIER_HPP
#define ATOM_SUPPLIER_HPP

using namespace std;

#include <string>
#include <sys/un.h>

namespace supplier {

class AtomSupplier {
public:
    AtomSupplier(const string& hostname, int port); // Constructor
    bool connectToServer(); // Connects to the TCP server
    AtomSupplier(const string& udsPath);    // UDS mode
    void run(); // reads commands from user and sends to server

private:
    string hostname;
    int port = -1;

    string udsPath;

    int sockfd = -1;
    bool useUDS = false;

    struct sockaddr_un servaddr_un;
};

} // namespace supplier

#endif // ATOM_SUPPLIER_HPP
