#include "atomWarehouse.hpp"

using namespace warehouse;

int main() {
    AtomWarehouse wh;
    wh.runTCPServer(12345);
    return 0;
}
