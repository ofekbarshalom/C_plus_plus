#include "atomWarehouse.hpp"
#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include <csignal>
#include <unistd.h>

using namespace std;
using namespace warehouse;

void handle_timeout(int sig) {
    cerr << "Timeout occurred. Server is shutting down." << endl;
    exit(0);
}

int main(int argc, char* argv[]) {
    int tcpPort = -1, udpPort = -1;
    int carbon = 0, hydrogen = 0, oxygen = 0;
    int timeout = -1;
    string streamPath, datagramPath;

    const struct option long_options[] = {
        {"tcp-port", required_argument, nullptr, 'T'},
        {"udp-port", required_argument, nullptr, 'U'},
        {"carbon", required_argument, nullptr, 'c'},
        {"hydrogen", required_argument, nullptr, 'h'},
        {"oxygen", required_argument, nullptr, 'o'},
        {"timeout", required_argument, nullptr, 't'},
        {"stream-path", required_argument, nullptr, 's'},
        {"datagram-path", required_argument, nullptr, 'd'},
        {nullptr, 0, nullptr, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "T:U:c:h:o:t:s:d:", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'T': tcpPort = atoi(optarg); break;
            case 'U': udpPort = atoi(optarg); break;
            case 'c': carbon = atoi(optarg); break;
            case 'h': hydrogen = atoi(optarg); break;
            case 'o': oxygen = atoi(optarg); break;
            case 't': timeout = atoi(optarg); break;
            case 's': streamPath = optarg; break;
            case 'd': datagramPath = optarg; break;
            default:
                cerr << "Usage: " << argv[0] << " -T <tcp-port> -U <udp-port> [-c <carbon>] [-h <hydrogen>] [-o <oxygen>] [-t <timeout>]" << endl;
                return 1;
        }
    }

    if (!streamPath.empty() && !datagramPath.empty() && streamPath == datagramPath) {
        cerr << "Error: stream-path and datagram-path cannot be the same.\n";
        return 1;
    }

    if (tcpPort == -1 || udpPort == -1) {
        cerr << "Error: TCP and UDP ports are required.\n";
        return 1;
    }

    AtomWarehouse wh;

    if (timeout > 0) {
        wh.setTimeout(timeout);
        signal(SIGALRM, handle_timeout);
        alarm(timeout);
    }

    // Construct warehouse with initial atoms
    wh.addAtom("CARBON", carbon);
    wh.addAtom("HYDROGEN", hydrogen);
    wh.addAtom("OXYGEN", oxygen);

    if (!streamPath.empty()) {
        wh.setupUnixStreamSocket(streamPath);
    }

    if (!datagramPath.empty()) {
        wh.setupUnixDatagramSocket(datagramPath);
    }

    wh.runCombinedServer(tcpPort, udpPort);
    return 0;
}
