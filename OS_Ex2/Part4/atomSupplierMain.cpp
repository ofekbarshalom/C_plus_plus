#include "atomSupplier.hpp"
#include <iostream>
#include <getopt.h>
#include <cstdlib>

using namespace std;
using namespace supplier;

int main(int argc, char* argv[]) {
    string hostname;
    int port = -1;

    int opt;
    while ((opt = getopt(argc, argv, "h:p:")) != -1) {
        switch (opt) {
            case 'h': hostname = optarg; break;
            case 'p': port = atoi(optarg); break;
            default:
                cerr << "Usage: " << argv[0] << " -h <hostname> -p <port>" << endl;
                return 1;
        }
    }

    if (hostname.empty() || port == -1) {
        cerr << "Error: hostname and port are required.\n";
        return 1;
    }

    AtomSupplier client(hostname, port);
    if (client.connectToServer()) {
        client.run();
    }

    return 0;
}
