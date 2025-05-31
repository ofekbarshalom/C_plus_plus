#include "atomWarehouse.hpp"

using namespace warehouse;

int main() {
    AtomWarehouse wh;
    wh.runCombinedServer(12345, 7777);  // TCP port, UDP port
    return 0;
}
