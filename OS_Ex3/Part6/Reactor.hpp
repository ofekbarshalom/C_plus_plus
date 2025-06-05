#ifndef REACTOR_HPP
#define REACTOR_HPP

typedef void (*reactorFunc)(int fd);
typedef struct reactorStruct* reactor;

// Starts the reactor and returns a pointer to it
reactor startReactor();

// Adds fd to reactor (for reading); returns 0 on success
int addFdToReactor(reactor r, int fd, reactorFunc func);

// Runs the reactor's main event loop (blocking)
void runReactor(reactor r);

// Removes fd from reactor
int removeFdFromReactor(reactor r, int fd);

// Stops the reactor
int stopReactor(reactor r);

#endif
