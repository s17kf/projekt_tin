msgReceiver: main.o ReadQueue.o
	g++ main.o ReadQueue.o -lrt -o runMsgReceiver

ReadQueue.o: ReadQueue.cpp ReadQueue.h common.h
	g++ -c ReadQueue.cpp -lrt

main.o: main.cpp ReadQueue.h
	g++ -c main.cpp
