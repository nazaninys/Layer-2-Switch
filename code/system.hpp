#ifndef SYSTEM
#define SEYSTEM 
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
extern string id;
extern string pipeName;
extern string listenPipe;
extern vector<string> tokens;
extern int pipefd;
extern bool connected;
extern map<string, vector<string>> files;
extern int filenum;
extern queue <string> inputFrames;

void tokenize(string);
void sending(vector<string>, string, string);
void sendRecReq();
void framing(string, int, string, string);
void readFile(string, string, string);
void messageDetect();
void messageReceive(int fd);
void writeData(string);
void checkFrame();
void listen();

#endif
