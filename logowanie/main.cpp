#include <iostream>
#include <cstring>

#include "connection.h"
#include "RNG.h"

int logIn(Connection *connection){
    RNG rng;


    Packet *packet = connection->receive(nullptr);

    if(CHALL * chall = dynamic_cast<CHALL *>(packet)){
        unsigned char chall_value[8];
        memcpy(chall_value, chall->getChall(), 8);

        printf("chall received: %u", chall_value);

    }



//    uint8_t key;
//    connection->receiveUint8_t(&key);
//
//    key == PCK_CHALL ? std::cout<<"PCK_CHALL received"<<std::endl :\
//    std::cout<<"not PCK_CHALL received"<<std::endl;
//
//
//
//    if(key != PCK_CHALL) {
//        connection->send(PCK_NAK);
//        return -1;
//    }
//
////    uint64_t randomNumber;
////    connection->receiveUint64_t(&randomNumber);
//    unsigned char randomNumber[8];
//    connection->receiveUCharArray(randomNumber);
//    std::cout<<"challenge received: "<<(uint64_t *)randomNumber<<std::endl;
//    connection->send(PCK_ACK);
//
//    return 0;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    Connection *connection = new Connection;


    if(connection->connect() <0){
        std::cout<<"connecting error, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }

    logIn(connection);


    delete(connection);


    return 0;
}