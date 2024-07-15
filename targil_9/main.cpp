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
//#include <poll.h>
#include <thread>
#include <mutex>
#include "Graph.cpp"
//#include "Proactor.cpp"
#include "Proactor.h"


#define PORT 9034
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 4


using namespace std;

Graph graph;
int server_fd;
mutex mtx;

class GraphServer {
private:
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

void* handle_client(int client_fd) {
    char buffer[1024];
    while (true) {
        int valread = read(client_fd, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            if (valread == 0) {
                cout << "Client disconnected: fd " << client_fd << endl;
            } else {
                perror("read error");
            }
            close(client_fd);
            return 0; // Exit the function to terminate the thread
        } else {
            string command(buffer, valread);
            if (command.find("Newgraph") == 0 || command.find("Newedge") == 0 || command.find("Removeedge") == 0 || command.find("Kosaraju") == 0) {
                lock_guard<mutex> lock(mtx); // Protect access to shared resource
                graph.Command_handle(client_fd, command);
            } else {
                string response = "Invalid command.\n";
                write(client_fd, response.c_str(), response.size());
            }
        }
    }
}


void sigint_handler(int sig) {
    cout << "\nCaught SIGINT, closing socket...\n";
    close(server_fd); // Close the socket
    exit(0); // Exit the program
}


int main() {
    signal(SIGINT, sigint_handler);
    GraphServer server(PORT);
    
    while (true) {
        int client_fd = server.new_client();
        if (client_fd >= 0) {
            string options_msg = "Options : Newgraph n,m     Kosaraju    Newedge i,j    Removeedge i,j\n";
            write(client_fd, options_msg.c_str(), options_msg.size());
            cout << "New client connected\n";
            
            // Start a proactor thread for each client
            pthread_t tid = startProactor(client_fd, handle_client);
            if (tid == 0) {
                cerr << "Failed to start proactor thread for client_fd: " << client_fd << endl;
                close(client_fd); // Close client socket on failure
            }
        }
    }

    return 0;
}
