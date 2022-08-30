
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
using namespace std;


class Network {
public:
    Network();
	void addToIDs(string);
	void setNumberOfPorts(string,int);
	void setPortsConnections(string,string,string);
	void disconnect(string,string);
	string getPort(string,string);
	bool MySwitchHasError(vector<string>);
	bool MySystemHasError(vector<string>);
	bool ConnectCommandHasError(vector<string>);
	bool ConnectSwitchHasError(vector<string>);
	bool SendCommandHasError(vector<string>);
	bool ReceiveCommandHasError(vector<string>);
	bool systemConnected(string);

private:
	set<string> numbers;
	map<string,int> switchPorts;
	map<string,map<string,string>> portsConnections; // switchID -> [ ID -> port ]
	bool idExists(string);
	bool fileExists(string);
	bool switchExists(string);
	bool systemExists(string);
	bool portNotExists(string,string);
	bool portIsBusy(string,string);
};
