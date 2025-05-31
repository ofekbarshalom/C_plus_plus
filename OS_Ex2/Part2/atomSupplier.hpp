#ifndef ATOM_SUPPLIER_HPP
#define ATOM_SUPPLIER_HPP

using namespace std;

#include <string>

namespace supplier {

class AtomSupplier {
public:
    AtomSupplier(const string& hostname, int port); // Constructor
    bool connectToServer(); // Connects to the TCP server
    void run(); // reads commands from user and sends to server

private:
    string hostname;
    int port;
    int sockfd;
};

#endif // ATOM_SUPPLIER_HPP

} // namespace supplier