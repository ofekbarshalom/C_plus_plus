#include "moleculeRequester.hpp"
#include <iostream>

using namespace std;
using namespace requester;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <hostname/IP> <port>" << endl;
        return 1;
    }

    string hostname = argv[1];
    int port = stoi(argv[2]);

    MoleculeRequester client(hostname, port);
    if (client.connectToServer()) {
        client.run();
    }

    return 0;
}
