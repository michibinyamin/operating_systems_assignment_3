#ifndef REACTOR_H
#define REACTOR_H

#include <vector>
#include <poll.h>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <algorithm> // Include the algorithm header for std::find_if

//Define the reactorFunc type
typedef void (*reactorFunc) (int fd);

// Define the reactor struct
typedef struct Reactor {
    std::vector<pollfd> fds;
    std::vector<reactorFunc> callbacks;
} reactor;

// Function declarations
void* startReactor();
int addFdToReactor(void* reactor, int fd, reactorFunc handle);
int removeFdFromReactor(void* reactor, int fd);
int stopReactor(void* reactor);

#endif // REACTOR_H
