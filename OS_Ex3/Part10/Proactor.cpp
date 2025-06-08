#include "Proactor.hpp"
#include <pthread.h>
#include <cstdlib>
#include <iostream>

// Wrapper to convert int sockfd into void* for pthread
struct ProactorArgs {
    int sockfd;
    proactorFunc func;
};

// Thread entry point wrapper
void* threadEntry(void* arg) {
    ProactorArgs* args = static_cast<ProactorArgs*>(arg);
    void* result = args->func(args->sockfd);
    delete args;
    return result;
}

pthread_t startProactor(int sockfd, proactorFunc threadFunc) {
    pthread_t tid;
    ProactorArgs* args = new ProactorArgs{sockfd, threadFunc};

    if (pthread_create(&tid, nullptr, threadEntry, args) != 0) {
        std::cerr << "Failed to create proactor thread\n";
        delete args;
        return 0;
    }

    return tid;
}

int stopProactor(pthread_t tid) {
    return pthread_cancel(tid);  // Sends cancellation request
}
