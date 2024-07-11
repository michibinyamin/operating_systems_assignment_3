#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <typeinfo>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

#define BUFFER_SIZE 1024

class Graph{
    private :
        int size;
        int current_client_fd;
        vector<vector<int>> edges;

        bool New_graph(int v, int m){
            char buffer[BUFFER_SIZE];
            int valread;
            string msg = "Please enter the edges: (for example- 1 2)\n";
            write(current_client_fd,msg.c_str(),msg.size());
            //cout << "Please enter the edges: (for example- 1 2)\n"; // Make it print to client!!
            for (int i = 0; i < m; ++i) {
                int src, dest;
                valread = read(current_client_fd, buffer, BUFFER_SIZE);
                buffer[valread] = '\0';
                istringstream iss(buffer);
                // Extract the two integers
                iss >> src >> dest;
                if(dest < 1 || dest > v || src < 1 || src > v)
                {
                    return false;
                }
                edges.push_back({src, dest});
            }
            return true;
        }

        bool Removeedge(int v, int m){
            for (auto it = edges.begin(); it != edges.end(); ++it) {
                if ((*it)[0] == v && (*it)[1] == m) {
                    edges.erase(it);
                    return true;
                }
            }
            return false;
        }

        bool dfs_Algo(int curr, int des, vector<vector<int> >& adj,vector<int>& vis)
        {
            if (curr == des) {
                return true;
            }
            vis[curr] = 1;
            for (auto x : adj[curr]) {
                if (!vis[x]) {
                    if (dfs_Algo(x, des, adj, vis)) {
                        return true;
                    }
                }
            }
            return false;
        }

        bool isPath(int src, int des, vector<vector<int> >& adj)
        {
            vector<int> vis(adj.size() + 1, 0);
            return dfs_Algo(src, des, adj, vis);
        }

        vector<vector<int>> findSCC(int n,vector<vector<int>>& a)
        {
            vector<vector<int>> ans;
            vector<int> is_scc(n + 1, 0);
            vector<vector<int>> adj(n + 1);

            for (int i = 0; i < a.size(); i++)
            {
                adj[a[i][0]].push_back(a[i][1]);
            }

            for (int i = 1; i <= n; i++) {

                if (!is_scc[i]) {
                    vector<int> scc;
                    scc.push_back(i);

                    for (int j = i + 1; j <= n; j++) {

                        if (!is_scc[j] && isPath(i, j, adj)
                            && isPath(j, i, adj)) {
                            is_scc[j] = 1;
                            scc.push_back(j);
                        }
                    }
                    ans.push_back(scc);
                }
            }

            return ans;
    }
    public :
        void Command_handle(int client, string command){
            current_client_fd = client;
            int v = 0;
            int m = 0;

            if (command.find("Newgraph") == 0) {
                string numbers_part = command.substr(9);
                istringstream iss(numbers_part);
                char comma;
                // Read the numbers separated by comma
                iss >> v >> comma >> m;
                size = v;
                edges.clear();
                if(New_graph(v,m)){
                    string success_msg = "New graph created with " + to_string(v) + " vertices and " + to_string(m) + " edges.\n";
                    write(current_client_fd, success_msg.c_str(), success_msg.size());
                    //cout << "New graph created with " << v << " vertices and " << m << " edges.\n"; // Make it print to client!!
                }
                else{
                    string error_msg = "Error: Invalid edge input.\n";
                    write(current_client_fd, error_msg.c_str(), error_msg.size());
                    //cout << "error\n";
                }

            }else if(command.find("Newedge") == 0){
                string numbers_part = command.substr(8);
                istringstream iss(numbers_part);
                char comma;
                // Read the numbers separated by comma
                iss >> v >> comma >> m;
                //size++;
                edges.push_back({v, m});
                string success_msg = "Edge (" + to_string(v) + " -> " + to_string(m) + ") added.\n";
                write(current_client_fd, success_msg.c_str(), success_msg.size());
                //cout << "Edge (" << v << " -> " << m << ") added.\n";   // Make it print to client!!                      
            }else if(command.find("Removeedge") == 0){
                string numbers_part = command.substr(11);
                istringstream iss(numbers_part);
                char comma;
                // Read the numbers separated by comma
                iss >> v >> comma >> m;
                if(Removeedge(v,m)){
                    string success_msg = "Edge (" + to_string(v) + " -> " + to_string(m) + ") removed.\n";
                    write(current_client_fd, success_msg.c_str(), success_msg.size());
                    //cout << "Edge (" << v << " -> " << m << ") removed.\n"; // Make it print to client!!
                }
                else{
                    string error_msg = "No such edge found.\n";
                    write(current_client_fd, error_msg.c_str(), error_msg.size());
                    //cout << "No such edge";
                }
            }else if(command.find("Kosaraju") == 0){
                vector<vector<int>> ans = findSCC(size, edges);
                string scc_msg = "Strongly Connected Components are:\n";
                //cout << "Strongly Connected Components are:\n";
                for (auto x : ans) {
                    for (auto y : x) {
                        scc_msg += to_string(y) + " ";
                        //cout << y << " ";
                    }
                write(current_client_fd, scc_msg.c_str(), scc_msg.size());
                //cout << "\n";   // Print as client
                }
            }else {
                string error_msg = "Invalid command.\n";
                write(current_client_fd, error_msg.c_str(), error_msg.size());
                //cout << "Invalid command.\n";
            }
        }
};