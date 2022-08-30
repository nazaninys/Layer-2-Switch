#include "system.hpp"

int main(int argc, char** argv) {
	connected = false;
	string tempId(argv[1]);
	filenum = 0;
	while (tempId.size() < 5) 
		tempId = "0" + tempId;
	id = tempId;
	mkdir(id.c_str(), 0777);
	fcntl(atoi(argv[2]), F_SETFL, O_NONBLOCK);
	close(atoi(argv[3]));
	while(1) {
		messageReceive(atoi(argv[2]));
		if(connected)
			listen();
		
	}
	exit(0);
}



