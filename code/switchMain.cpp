#include "switch.hpp"




int main(int argc, char** argv) {
	canread = true;
	portNumbers = atoi(argv[1]);
	id = atoi(argv[2]);
	fcntl(atoi(argv[3]), F_SETFL, O_NONBLOCK);
    close(atoi(argv[4]));
        
	while(true) {
		messageReceive(atoi(argv[3]));
		listen();
		lookupTable();
		forward();
	}
	exit(0);

}

