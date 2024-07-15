#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <csignal>
#include <poll.h>
#include "Graph.cpp"
#include "Reactor.h"

#define PORT 9034
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 4


using namespace std;

Graph graph;
int server_fd;

class GraphServer {
private:
    //int server_fd;
    unordered_map<string, vector<vector<int>>> graphs;

public:
    GraphServer(int port) {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in servaddr;
        bzero(&servaddr, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(port);

        if ((bind(server_fd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
            perror("socket bind failed");
            exit(EXIT_FAILURE);
        }

        if ((listen(server_fd, 5)) != 0) {
            perror("listen failed");
            exit(EXIT_FAILURE);
        }
        cout << "Socket created.\n";
    }

    int new_client() {
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (new_socket < 0) {
            perror("accept error");
            exit(EXIT_FAILURE);
        }
        return new_socket;
    }

    int get_server_fd(){
        return server_fd;
    }

private:

};

void handle_client(int client_fd){
    char buffer[1024];
    int valread = read(client_fd, buffer, BUFFER_SIZE);
    if (valread <= 0) {
        if (valread == 0) {
            cout << "Client disconnected: fd " << client_fd << endl;
        } else {
            perror("read error");
        }
        close(client_fd);
        client_fd = -1;
    }else{
        string command(buffer);
        if (command.find("Newgraph") == 0 || command.find("Newedge") == 0 || command.find("Removeedge") == 0 || command.find("Kosaraju") == 0) {
            graph.Command_handle(client_fd, command);
        }
        else {
            string response = "Invalid command.\n";
            write(client_fd, response.c_str(), response.size());
        }
        //server.handleClient(client_fd.fd, buffer);
    }
}

void sigint_handler(int sig) {
    printf("\nCaught SIGINT, closing socket...\n");
    close(server_fd); // Close the socket
    exit(0); // Exit the program
}



int main() {
    // Create and start the reactor
    Reactor* reactor = static_cast<Reactor*>(startReactor());
    
    // Create a server
    GraphServer server(PORT);

    // Create a pointer to the function
    reactorFunc handler = handle_client;

    // Add the server to the reactor (which creates a poll_fd)
    addFdToReactor(reactor, server.get_server_fd(), handle_client);

    // Register signal handler for SIGINT
    signal(SIGINT, sigint_handler);


    while (true)
    {
        int ret = poll(reactor->fds.data(), reactor->fds.size(), -1);
        if (ret < 0) {
            std::cerr << "Poll error: " << strerror(errno) << std::endl;
            break;
        }
        
        for (int i = 0; i < reactor->fds.size(); ++i) {
            pollfd current = reactor->fds.at(i);
            if (current.revents & POLLIN){
                if (i == 0)     // New connection on the listening socket
                {
                    // New connection on the listening socket
                    int client_fd = server.new_client();
                    // Add the client to the reactor (which creates a poll_fd)
                    addFdToReactor(reactor, client_fd, handle_client);

                    string options_msg = "Options : Newgraph n,m     Kosaraju    Newedge i,j    Removeedge i,j\n";
                    write(client_fd, options_msg.c_str(), options_msg.size());
                    cout << "New client connected\n";
                    break;
                }
                else   // Data from an existing client
                {  
                    reactor->callbacks.at(i)(current.fd);
                    //graph.PrintEdges();
                }
            }
        }
    }
    

    //server.start();
    return 0;
}
