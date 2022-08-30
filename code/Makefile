CC = g++
FLAGS = -std=c++11 

all: main_process switch system Network

main_process: main_process.o Network.o
	$(CC) $(FLAGS) main_process.o Network.o -o network

main_process.o: main_process.cpp
	$(CC) $(FLAGS) -c main_process.cpp

switch: switch.o switchMain.o
	$(CC) $(FLAGS) switch.o switchMain.o -o switch

switch.o: switch.cpp switch.hpp
	$(CC) $(FLAGS) -c switch.cpp

switchMain.o: switchMain.cpp switch.hpp
	$(CC) $(FLAGS) -c switchMain.cpp

system: system.o systemMain.o
	$(CC) $(FLAGS) system.o systemMain.o -o system

system.o: system.cpp system.hpp
	$(CC) $(FLAGS) -c system.cpp

systemMain.o: systemMain.cpp system.hpp
	$(CC) $(FLAGS) -c systemMain.cpp

Network: Network.o
	$(CC) $(FLAGS) -c Network.cpp
	
Network.o: Network.cpp Network.hpp
	$(CC) $(FLAGS) -c Network.cpp

.PHONY: clean

clean:
	rm *.o 
	rm network
	rm switch
	rm system
	