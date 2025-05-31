#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char *argv[]) {
    // Check that exactly one argument (the name to search) is provided
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " \"Full Name\"" << endl;
        return 1;
    }

    int pipefd[2];
    pipe(pipefd); // Create a pipe: pipefd[0] = read, pipefd[1] = write

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        close(pipefd[0]); // Close read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to write end of the pipe
        close(pipefd[1]); // Close original write end after redirection

        // Prepare arguments for execve to run: grep -F "Full Name" phonebook.txt
        char* args[] = {
            (char*)"/usr/bin/grep",
            (char*)"-F",              // Fixed string match (handles spaces)
            argv[1],                  // The full name passed by user
            (char*)"phonebook.txt",   // The phonebook file
            nullptr
        };

        // Replace current process with grep
        execve("/usr/bin/grep", args, nullptr);

        // If execve fails
        perror("execve failed");
        return 1;
    } else {
        // Parent process
        close(pipefd[1]); // Close write end

        char buffer[1024];
        ssize_t count = read(pipefd[0], buffer, 1023); // Read from pipe
        buffer[count] = '\0'; // Null-terminate manually

        // Search for the comma and print everything after it (the phone number)
        bool foundComma = false;
        bool printed = false;

        for (int i = 0; i < count; i++) {
            if (foundComma) {
                cout << buffer[i];
                printed = true;
            } else if (buffer[i] == ',') {
                foundComma = true;
            }
        }

        if (!printed) {
            cerr << "Name not found or bad format" << endl;
        }

        close(pipefd[0]); // Close read end
        wait(nullptr);    // Wait for the child process to finish
    }

    return 0;
}
