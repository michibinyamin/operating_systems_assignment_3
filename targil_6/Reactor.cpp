
#include <vector>
#include <poll.h>

#include <functional>
#include <unordered_map>
#include <iostream>
#include <algorithm> // Include the algorithm header for std::find_if
#include "Reactor.h"

// // Define the reactorFunc type
// typedef void (*reactorFunc) (int fd);

// // Define the reactor struct
// typedef struct Reactor {
//     std::vector<pollfd> fds;
//     std::vector<reactorFunc> callbacks;
// } reactor;

// Function declarations
void* startReactor(){
    Reactor* reactor = new Reactor();
    std::cout << "Reactor created and started." << std::endl;
    return reactor;
}

int addFdToReactor(void* reactor, int fd, reactorFunc handle){
    if (!reactor)
        throw std::invalid_argument("Reactor pointer is null.");

    Reactor* r = static_cast<Reactor*>(reactor);    // Cast reactor

    // Add the new file descriptor and its handler to the reactor
    r->fds.push_back({fd, POLLIN});
    r->callbacks.push_back(handle);

    std::cout << "Added fd " << fd << " to reactor." << std::endl;
    return 0; // Success
}

int removeFdFromReactor(void* reactor, int fd){
    if (!reactor)
        throw std::invalid_argument("Reactor pointer is null.");

    Reactor* r = static_cast<Reactor*>(reactor);

    // Find the index of the fd in the reactor's fds vector
    auto it = std::find_if(r->fds.begin(), r->fds.end(),
        [fd](const pollfd& pfd) { return pfd.fd == fd; });

    if (it != r->fds.end()) {
        // Remove the fd and its associated callback
        r->fds.erase(it);
        r->callbacks.erase(r->callbacks.begin() + std::distance(r->fds.begin(), it));
        std::cout << "Removed fd " << fd << " from reactor." << std::endl;
        return 0; // Success
    } else {
        std::cerr << "Failed to find fd " << fd << " in reactor." << std::endl;
        return -1; // Failure
    }
}

int stopReactor(void* reactor){
    if (!reactor)
        throw std::invalid_argument("Reactor pointer is null.");

    Reactor* r = static_cast<Reactor*>(reactor);    // Cast

    r->fds.clear();
    r->callbacks.clear();

    std::cout << "Reactor stopped." << std::endl;
    return 0; // Success
}

