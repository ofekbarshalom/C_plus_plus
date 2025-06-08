#ifndef PROACTOR_HPP
#define PROACTOR_HPP

#include <pthread.h>

// Define the function type: takes a socket (int), returns void*
typedef void* (*proactorFunc)(int sockfd);

// Starts a new proactor thread for a socket and function, returns the thread ID
pthread_t startProactor(int sockfd, proactorFunc threadFunc);

// Stops a proactor thread using pthread_cancel
int stopProactor(pthread_t tid);

#endif
