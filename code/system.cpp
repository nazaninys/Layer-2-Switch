#include "system.hpp"

string id;
string pipeName;
string listenPipe;
vector<string> tokens;
int pipefd;
bool connected;
map<string, vector<string>> files;
int filenum;
queue <string> inputFrames;

void tokenize(string inp) {
	tokens.clear();
	string word;
	istringstream req(inp);
	while (req >> word) 
	tokens.push_back(word);
}

void sending(vector<string> frames, string rec, string sender) {
	cout<<frames.size()<<endl;
	int fd = open(pipeName.c_str(), O_WRONLY);
	while(rec.size() < 5)
		rec = "0" + rec;
	for(int i=0; i< frames.size(); i++) {
		string size = to_string(frames[i].size());
		while(size.size() < 5)
			size = "0" + size;
		
		string message = "11110101" + rec + sender + size + frames[i];
		write(fd, message.c_str(), message.size() + 1);
		
	}
	close(fd);
}

void sendRecReq() {
	int fd = open(pipeName.c_str(), O_WRONLY);
	string rec = tokens[1];
	while(rec.size() < 5)
		rec = "0" + rec;
	
	
	string size = to_string(tokens[2].size());
	while(size.size() < 5)
		size = "0" + size;
	
	string message = "11111111" + rec + id + size + tokens[2];
	write(fd, message.c_str(), message.size() + 1);
		
	
	close(fd);
}

void framing(string content, int total, string rec, string sender) {
	int size = 0;
	vector<string> frames;
	while(total - size >= 1500) {
		string temp = content.substr(size, 1500);
		frames.push_back(temp);
		size += 1499;
	}
	if(total - size >= 0) {
		string temp = content.substr(size, total - size + 1);
		frames.push_back(temp);
	}
	sending(frames, rec, sender);

} 

void readFile(string fileName, string rec, string sender) {
	FILE* file_;
	char buffer[2000000];
	file_ = fopen(fileName.c_str(), "r");
	int total = fread(buffer, 1, sizeof(buffer) + 1, file_);
	fclose(file_);
	string content(buffer);
	framing(content, total, rec, sender);
}

void messageDetect() {
	if(tokens[0].compare("connect") == 0) {
		pipeName = tokens[1];
		listenPipe = tokens[2];
		mkfifo(pipeName.c_str(), 0777);
		mkfifo(listenPipe.c_str(), 0777);
		cout<<"system " << id << " sending on pipe " <<pipeName<<endl;
		cout<<"system " << id << " receiving from pipe " << listenPipe <<endl;
		pipefd = open(listenPipe.c_str(), O_NONBLOCK, O_RDONLY);
		connected = true;
	}
	if(tokens[0].compare("send") == 0)
		readFile(tokens[2], tokens[1], id);
	if(tokens[0].compare("receive") == 0)
		sendRecReq();
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

void writeData(string sender) {
	cout<<"system " << id << " writing to file"<<endl;
	FILE* cur;
	cur = fopen((id + "/" + to_string(filenum) + ".txt").c_str(), "w");
	for(int i=0; i<files[sender].size(); i++)
		fwrite(files[sender][i].c_str(),files[sender][i].size() + 1, 1, cur);

	fclose(cur);
	files.erase(sender);
	filenum ++;
}

void checkFrame() {
	string frame = inputFrames.front();
	inputFrames.pop();
	string rec = frame.substr(8, 5);
	if(rec.compare(id) == 0) {
		cout<<"frame is for system " << id <<endl;
		string sender = frame.substr(13, 5);
		if(frame.substr(0, 8).compare("11111111") == 0) {
			readFile(frame.substr(23, frame.size() - 23), frame.substr(13, 5), frame.substr(8, 5));
			return;
		}
		if(files.find(sender) == files.end()) {
			vector<string> temp;
			temp.push_back(frame.substr(23, frame.size() - 23));
			files.insert({sender, temp});
		}
		else
			files[sender].push_back(frame.substr(23, frame.size() - 23));
		if(frame.size() < 1523)
			writeData(sender);
	}

}

void listen() {
	char str1[1524];
	int nread = read(pipefd, str1, 1524);
	if (nread > 0) {
		cout<<"system " << id << " received"<<endl;
		string frame(str1);
		inputFrames.push(frame);
		checkFrame();
	}
}
