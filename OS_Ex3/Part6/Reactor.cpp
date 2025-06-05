#include "Reactor.hpp"
#include <unordered_set>
#include <sys/select.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

#define MAX_FD 1024

struct reactorStruct {
    unordered_set<int> fds;                 // Set of active file descriptors
    reactorFunc funcs[MAX_FD] = {nullptr};  // Callback functions per fd
    bool running = false;                   // Whether the loop is running
};

reactor startReactor() {
    reactor r = new reactorStruct;
    r->running = false;
    return r;
}

int addFdToReactor(reactor r, int fd, reactorFunc func) {
    if (fd < 0 || fd >= MAX_FD) return -1;
    r->fds.insert(fd);
    r->funcs[fd] = func;
    return 0;
}

int removeFdFromReactor(reactor r, int fd) {
    r->fds.erase(fd);
    r->funcs[fd] = nullptr;
    return 0;
}

int stopReactor(reactor r) {
    r->running = false;
    return 0;
}

void runReactor(reactor r) {
    r->running = true;
    fd_set readfds;
    struct timeval tv;

    while (r->running) {
        FD_ZERO(&readfds);
        int maxfd = 0;

        for (int fd : r->fds) {
            FD_SET(fd, &readfds);
            if (fd > maxfd) maxfd = fd;
        }

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int ready = select(maxfd + 1, &readfds, nullptr, nullptr, &tv);
        if (ready < 0) {
            perror("select");
            break;
        }

        vector<int> activeFds(r->fds.begin(), r->fds.end());

        for (int fd : activeFds) {
            if (FD_ISSET(fd, &readfds)) {
                if (r->funcs[fd]) {
                    r->funcs[fd](fd);
                }
            }
        }
    }
}
