#include <iostream>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " \"Full Name\" \"PhoneNumber\"" << endl;
        return 1;
    }

    // Open file in append mode. Create if it doesn't exist.
    int fd = open("phonebook.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd == -1) {
        perror("Failed to open phonebook.txt");
        return 1;
    }

    // Write full name
    for (int i = 0; argv[1][i] != '\0'; i++) {
        write(fd, &argv[1][i], 1);
    }

    // Write comma
    char comma = ',';
    write(fd, &comma, 1);

    // Write phone number
    for (int i = 0; argv[2][i] != '\0'; i++) {
        write(fd, &argv[2][i], 1);
    }

    // Write newline
    char newline = '\n';
    write(fd, &newline, 1);

    close(fd);
    cout << "Line added successfully." << endl;
    return 0;
}
