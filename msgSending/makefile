msgSender: main.o AddQueue.o
	g++ main.o AddQueue.o -lrt -o runMsgSender

AddQueue.o: AddQueue.cpp AddQueue.h common.h
	g++ -c AddQueue.cpp -lrt

main.o: main.cpp AddQueue.h
	g++ -c main.cpp