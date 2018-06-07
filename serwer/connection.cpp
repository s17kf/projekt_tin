//
// Created by s17kf on 02.06.18.
//

#include <iostream>

#include "connection.h"
#include "log.h"


Connection::Connection(int port) {
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    socketDesc = socket(serverAddr.sin_family, SOCK_STREAM, IPPROTO_TCP);

    if(bind(socketDesc, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0){
        log(1, "Error occurred during binding socket on port %d", port);
//        std::cout<<"001 Error occurred during binding socket on port " << port<<std::endl;
        exit(2);
    }

    if (listen(socketDesc, MAX_CONNECTIONS) < 0) {
        log(1, "Error occurred during listening on socket on port %d", port);
//        std::cout<<"002Error occurred during listening on socket on port "<<PORT_NR<<std::endl;
        exit(1);
    }
//    std::cout<<"Server registered on portt "<<port<<std::endl;
    log(1, "Server registered on port: %d", port);
}

Connection::~Connection(){
     shutdown(clientSocket, SHUT_RDWR);
     shutdown(socketDesc, SHUT_RDWR);
}

int Connection::connect() {

    socklen_t clientLen = sizeof(client);
    std::cout<<"waiting for connection"<<std::endl;
    clientSocket = accept(socketDesc, (struct sockaddr *) &client, &clientLen);
    std::cout<<"something connected"<<std::endl;

    return clientSocket;

}

ssize_t Connection::send(Packet *packet, Sesskey *sesskey) {
    return packet->send(clientSocket, sesskey);
}

//Packet* Connection::receive(Sesskey *sesskey) {
//    return Packet::packetFactory(clientSocket, sesskey);
//}

Packet* Connection::receive(AndroidClient *androidClient) {
    return Packet::packetFactory(clientSocket, androidClient);
}



void Connection::endClientConnection() {
    shutdown(clientSocket, SHUT_RDWR);
}




ssize_t Connection::send(uint32_t value){

    return writeTillDone(clientSocket, (unsigned char *) &value, sizeof(value));

}

ssize_t Connection::sendUint8_t(uint8_t value) {
    return writeTillDone(clientSocket, &value, sizeof(value));
}

int Connection::receiveUint32_t(uint32_t *dest) {

    int readed = readTillDone(clientSocket, (unsigned char *) dest, sizeof(*dest));
    return readed;
}

int Connection::receiveUint64_t(uint64_t *dest) {

    int readed = readTillDone(clientSocket, (unsigned char *) dest, sizeof(*dest));
    return readed;
}

int Connection::receiveUint8_t(uint8_t *dest) {
    int readed = readTillDone(clientSocket, dest, sizeof(*dest));
    return readed;
}

int Connection::receiveUCharArray(unsigned char *dest) {
    int readed = readTillDone(clientSocket, dest, sizeof(*dest));
    return readed;
}

ssize_t Connection::send(unsigned char value) {
    return writeTillDone(clientSocket,  &value, sizeof(value));

}