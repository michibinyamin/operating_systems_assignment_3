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
#include <poll.h>
#include "Graph.cpp"

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

void sigint_handler(int sig) {
    printf("\nCaught SIGINT, closing socket...\n");
    close(server_fd); // Close the socket
    exit(0); // Exit the program
}

int main() {
    GraphServer server(PORT);
    struct pollfd poll_fds[MAX_CLIENTS + 1];
    poll_fds[0].fd = server.get_server_fd();    // Create server fd
    poll_fds[0].events = POLLIN;

    // Register signal handler for SIGINT
    signal(SIGINT, sigint_handler);

    for (int i = 1; i <= MAX_CLIENTS; ++i) {
        poll_fds[i].fd = -1; // Initialize client fds to -1
    }
    while (true)
    {
        int poll_count = poll(poll_fds, MAX_CLIENTS + 1, -1);
        if (poll_count < 0) {
            perror("poll error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i <= MAX_CLIENTS; ++i) {    
            if (poll_fds[i].revents & POLLIN) {     // Enters only when data is available on this socket
                if (i == 0)         // New connection on the listening socket
                {
                    // Add new socket to poll_fds array
                    for (int j = 1; j <= MAX_CLIENTS; ++j) {
                        if (poll_fds[j].fd == -1) {
                            poll_fds[j].fd = server.new_client();
                            poll_fds[j].events = POLLIN;
                            string options_msg = "Options : Newgraph n,m     Kosaraju    Newedge i,j    Removeedge i,j\n";
                            write(poll_fds[j].fd, options_msg.c_str(), options_msg.size());
                            cout << "New client connected\n";
                            break;
                        }
                    }
                }
                else  // Data from an existing client
                {
                    char buffer[1024];
                    int valread = read(poll_fds[i].fd, buffer, BUFFER_SIZE);
                    if (valread <= 0) {
                        if (valread == 0) {
                            cout << "Client disconnected: fd " << poll_fds[i].fd << endl;
                        } else {
                            perror("read error");
                        }
                        close(poll_fds[i].fd);
                        poll_fds[i].fd = -1;
                    }else{
                        string command(buffer);
                        if (command.find("Newgraph") == 0 || command.find("Newedge") == 0 || command.find("Removeedge") == 0 || command.find("Kosaraju") == 0) {
                            graph.Command_handle(poll_fds[i].fd, command);
                        }
                        else {
                            string response = "Invalid command.\n";
                            write(poll_fds[i].fd, response.c_str(), response.size());
                        }
                        //server.handleClient(poll_fds[i].fd, buffer);
                    }
                }
            }

        }

    }
    

    //server.start();
    return 0;
}
