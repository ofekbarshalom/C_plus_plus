#include <iostream>
#include <csignal>
#include <unistd.h>     // for pause()
#include <cstdlib>      // for exit()

using namespace std;

volatile sig_atomic_t received_number = 0;
volatile sig_atomic_t bit_position = 0;

void handle_sigusr1(int) {
    // Bit 1
    received_number |= (1 << bit_position);
    bit_position++;
}

void handle_sigusr2(int) {
    // Bit 0 (do nothing to the number, just move to next bit)
    bit_position++;
}

int main() {
    // Print own PID for the sender
    pid_t pid = getpid();
    cout << "Receiver PID: " << pid << endl;
    cout << "Waiting for 8 bits..." << endl;

    // Register signal handlers
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);

    // Wait until we've received 8 bits
    while (bit_position < 8) {
        pause(); // Wait for signal
    }

    cout << "Received number: " << received_number << endl;

    return 0;
}
