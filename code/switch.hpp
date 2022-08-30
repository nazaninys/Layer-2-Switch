#include <iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/stat.h>
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
#include <queue>
using namespace std;

extern int portNumbers, id;
extern vector<string> tokens;
extern map<int, string> ports;
extern queue<string> frames;
extern queue<string> pipeNames;
extern vector<int> fds;
extern map<string, string> table;
extern bool canread;
extern map<string, string> toFrom;

void tokenize(string);
void messageDetect();
void messageReceive(int);
void listen();
void lookupTable();
void forward();
void connect();
void disconnect();