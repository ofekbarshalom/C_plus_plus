#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>


using namespace std;

int main() {
    pid_t receiver_pid;
    int message;

    // Handle input
    cout << "Enter receiver PID: ";
    cin >> receiver_pid;

    cout << "Enter message (0–255): ";
    cin >> message;

    // The number must be 8 bytes at most
    if (message < 0 || message > 255) {
        cerr << "Error: message must be between 0 and 255." << endl;
        exit(1);
    }

    // Send the bits LSB to MSB (right to left)
    for (int i = 0; i < 8; ++i) {
        int bit = (message >> i) & 1;

        if (bit == 1) {
            kill(receiver_pid, SIGUSR1);
        } else {
            kill(receiver_pid, SIGUSR2);
        }

        // Sleep to avoid flooding
        usleep(300); // 300 microseconds — tweak if needed
    }

    return 0;
}
