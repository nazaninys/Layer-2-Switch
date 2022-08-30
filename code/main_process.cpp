#include <iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <vector>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <dirent.h>
#include <limits>
#include <set>
#include <iterator>
#include <map>
#include "Network.hpp"
using namespace std;

vector<string> tokens;
map<string, vector<int>> pipes;
Network network ;
////////////// spanning tree ////////////////////////////////////////////////////////////////////////
set<int> switches;
map<int,vector<int>> connections; //graph

void addConnection() {

	network.setPortsConnections(tokens[1],tokens[2],tokens[3]);
	network.setPortsConnections(tokens[2],tokens[1],tokens[4]);

	int sID1 = stoi(tokens[1]);
	int sID2 = stoi(tokens[2]);
	connections[sID1].push_back(sID2);
	connections[sID2].push_back(sID1);
	
}

int findRoot(set<int> switches){ 
    int root = INT_MAX;
    for (auto elem : switches){
           if(root > elem)
              root = elem;
    }
    return root;
}

set<string> BFS(){

	map<int,bool> visited ;
	map<int,int> parent;
    map<int,int> d;
    list<int> queue;
	set<string> deletedEdges;

    int root = findRoot(switches);

    visited[root] = true;
    d[root] = 0;
    queue.push_back(root);

    while(!queue.empty()) {
        int s = queue.front();
        queue.pop_front();
 
        for (int i = 0; i < connections[s].size();i++) {
            int adj = connections[s][i];
			if(visited[adj]){
				if(parent[s] != adj){
					string s1s2 = to_string(s) + "@" + to_string(adj);
					string s2s1 = to_string(adj) + "@" + to_string(s);

					if((deletedEdges.find(s1s2) == deletedEdges.end()) && (deletedEdges.find(s2s1) == deletedEdges.end())){
						deletedEdges.insert(s1s2);
						cout <<"switch "<<s<<" port "<< network.getPort(to_string(s),to_string(adj))
						<<" and switch "<<adj<<" port "<< network.getPort(to_string(adj),to_string(s))<<endl;
					}
				}	
			} else {
                visited[adj] = true;
                d[adj] = d[s] + 1;
				parent[adj] = s;
                queue.push_back(adj);
            }
        }

        if(!queue.empty() && d[s] != d[queue.front()]){ 
              queue.sort(); 
        }
    }
	return deletedEdges;
}

void tokenize(string inp) {
	tokens.clear();
	string word;
	istringstream req(inp);
	while (req >> word) 
	tokens.push_back(word);
}

void createPipe(int num) {
	int pipenum[2];
	pipe(pipenum);
	vector<int> temp;
	temp.push_back(pipenum[0]);
	temp.push_back(pipenum[1]);
	pipes.insert({tokens[num], temp});

}

void createSwitch() {
	if (network.MySwitchHasError(tokens)) 
		return;
	
	createPipe(2);
	
	network.addToIDs(tokens[2]);
	network.setNumberOfPorts(tokens[2],stoi(tokens[1]));
	switches.insert(stoi(tokens[2]));//for finding root
	
	int id = fork();
	
	string readPipe = to_string(pipes[tokens[2]][0]);
	string writePipe = to_string(pipes[tokens[2]][1]);
	if (id == 0)
		execl("./switch", "switch.cpp", tokens[1].c_str(), tokens[2].c_str(), readPipe.c_str(), writePipe.c_str(), NULL);  
	cout<<"switch created"<<endl; 
}

void createSystem() {
	if (network.MySystemHasError(tokens))
		return;

	network.addToIDs(tokens[1]);

	createPipe(1);
	string readPipe = to_string(pipes[tokens[1]][0]);
	string writePipe = to_string(pipes[tokens[1]][1]);
	int id = fork();
	if (id == 0)
		execl("./system", "system.cpp", tokens[1].c_str(), readPipe.c_str(), writePipe.c_str(), NULL); 
	cout<<"system created"<<endl; 
}

void sendPipe() {
	if (network.ConnectCommandHasError(tokens))
		return;

	network.setPortsConnections(tokens[2],tokens[1],tokens[3]);

	string message = "connect " + tokens[1] + "@" + tokens[2] + " " + tokens[2] + "@" + tokens[1]+ " " + tokens[3];
	write(pipes[tokens[1]][1], message.c_str(), message.size() + 1);
	write(pipes[tokens[2]][1], message.c_str(), message.size() + 1);
}

void sendReq() {
	
	if(network.SendCommandHasError(tokens))
		return;

	string message = "send " + tokens[2] + " " + tokens[3];
	write(pipes[tokens[1]][1], message.c_str(), message.size() + 1);
	cout<< "system " + tokens[1] << " started sending"<<endl;
}

void switchPipes() {
	if(network.ConnectSwitchHasError(tokens))
		return ;

    addConnection();

	string message1 = "connect " + tokens[1] + "@" + tokens[2] + " " + tokens[2] + "@" + tokens[1]+ " " + tokens[3];
	string message2 = "connect " + tokens[2] + "@" + tokens[1] + " " + tokens[1] + "@" + tokens[2]+ " " + tokens[3];
	write(pipes[tokens[1]][1], message1.c_str(), message1.size() + 1);
	write(pipes[tokens[2]][1], message2.c_str(), message2.size() + 1);
}

void recReq() {   
	if(network.ReceiveCommandHasError(tokens))
		return;
		 
	string message = "receive " + tokens[2] + " " + tokens[3];
	write(pipes[tokens[1]][1], message.c_str(), message.size() + 1); 
}

void disconnect(string pipeName) {
	int delim = pipeName.find('@');
	string first = pipeName.substr(0, delim);
	string second = pipeName.substr(delim + 1, pipeName.size() - delim - 1);
	string message = "disconnect " + pipeName + " " + second + "@" + first;

	network.disconnect(first,second);
	network.disconnect(second,first);

	write(pipes[first][1], message.c_str(), message.size() + 1);
	write(pipes[second][1], message.c_str(), message.size() + 1);
}

void commandRec() {
	if(tokens[0].compare("MySwitch") == 0)
		createSwitch();
	else if (tokens[0].compare("MySystem") == 0) 
		createSystem();
	else if (tokens[0].compare("Connect") == 0) 
		sendPipe();
	else if (tokens[0].compare("Send") == 0) 
		sendReq();
	else if (tokens[0].compare("ConnectSwitch") == 0) 
		switchPipes();
	else if (tokens[0].compare("Receive") == 0) 
		recReq();
    else if (tokens[0].compare("STA") == 0) {
		set<string> edges = BFS();
		for(auto e:edges)
			disconnect(e);
    }
	
}



int main() {
	string inp;
	
	while (true) {
		getline(cin, inp);
		tokenize(inp);
		commandRec();
	}
}