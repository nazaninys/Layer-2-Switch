#include "switch.hpp"

int portNumbers, id;
vector<string> tokens;
map<int, string> ports;
queue<string> frames;
queue<string> pipeNames;
vector<int> fds;
map<string, string> table;
bool canread;
map<string, string> toFrom;

void tokenize(string inp) {
	tokens.clear();
	string word;
	istringstream req(inp);
	while (req >> word) 
		tokens.push_back(word);
}

void connect() {
	mkfifo(tokens[1].c_str(), 0777);
	mkfifo(tokens[2].c_str(), 0777);
	int fd1 = open(tokens[1].c_str(), O_NONBLOCK, O_RDONLY);
	ports.insert({fd1, tokens[1]});
	fds.push_back(fd1);
	toFrom.insert({tokens[1], tokens[2]});
}

void disconnect() {
	if(toFrom.find(tokens[1]) != toFrom.end())
		toFrom.erase(tokens[1]);
	else
		toFrom.erase(tokens[2]);
	int curFd;
	for(auto i:ports) {
		if((i.second.compare(tokens[1]) == 0) || (i.second.compare(tokens[1]) == 0)) {
			curFd = i.first;
			ports.erase(i.first);
			break;
		}
	}
	for(auto i = fds.begin(); i != fds.end(); i++) {
		if (*i == curFd) {
			fds.erase(i);
			break;
		}
	}
}

void messageDetect() {
	if(tokens[0].compare("connect") == 0) {
		cout<<"switch "<<id << " sends on pipe " <<tokens[2] << endl;
		cout<<"switch "<<id << " listens on pipe " <<tokens[1] << endl;
		connect();
	}
	if(tokens[0].compare("disconnect") == 0) 
		disconnect();
}


void messageReceive(int fd) {
	int nread;
    char buf[100];
	nread = read(fd , buf, 100);
	if(nread != -1) {
		string inp(buf);
		tokenize(inp);
		messageDetect();
	}
}

void listen() {
	for (int i=0; i<fds.size(); i++) {
		char str1[1524];
		memset(str1, 0, 1524);
		int nread = read(fds[i], str1, 1524);

		if (nread > 0) {
			
			cout<<"switch "<<id<<" received" <<endl;
			string temp(str1);
			frames.push(temp);
			pipeNames.push(ports[fds[i]]);
		}
	}
}

void lookupTable() {
	if(frames.size() == 0)
		return;
	string sender = frames.front().substr(13, 5);
	if(table.find(sender) == table.end()) {
		table.insert({sender, toFrom[pipeNames.front()]});
	}

}

void forward() {
	
	if(frames.size() == 0) 
		return;
	
	canread = false;
	string msg = frames.front();
	string rec = msg.substr(8, 5);
	if(table.find(rec) != table.end()) {
		int fd = open(table[rec].c_str(), O_WRONLY);
		write(fd, msg.c_str(), msg.size() + 1);
		cout<<"sent from switch " <<id<<" to "<<table[rec]<<endl;
		close(fd);

	}
	else {
		string sender = msg.substr(13, 5);
		for(auto item : toFrom) {
			if(item.second.compare(table[sender]) == 0)
				continue;
			int fd = open(item.second.c_str(), O_WRONLY);
			write(fd, msg.c_str(), msg.size() + 1);
			cout<<"sent from switch " <<id<<" to "<<item.second<<endl;
			close(fd);
			
		}
	}
	frames.pop();
	pipeNames.pop();
	canread = true;
}