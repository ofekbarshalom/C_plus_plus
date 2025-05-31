#include "moleculeRequester.hpp"
#include <iostream>
#include <getopt.h>
#include <cstdlib>

using namespace std;
using namespace requester;

int main(int argc, char* argv[]) {
    string hostname;
    string udsPath;
    int port = -1;

    int opt;
    while ((opt = getopt(argc, argv, "h:p:f:")) != -1) {
        switch (opt) {
            case 'h': hostname = optarg; break;
            case 'p': port = atoi(optarg); break;
            case 'f': udsPath = optarg; break;
            default:
                cerr << "Usage: " << argv[0] << " -h <hostname> -p <port>" << endl;
                return 1;
        }
    }

    if ((!udsPath.empty() && (!hostname.empty() || port != -1)) ||
        (udsPath.empty() && (hostname.empty() || port == -1))) {
        cerr << "Error: Must use either -f <sockpath> or -h/-p, but not both.\n";
        return 1;
    }

    if (!udsPath.empty()) {
        MoleculeRequester client(udsPath);
        if (client.connectToServer()) {
            client.run();
        }
    } else {
        MoleculeRequester client(hostname, port);
        if (client.connectToServer()) {
            client.run();
        }
    }

    return 0;
}
