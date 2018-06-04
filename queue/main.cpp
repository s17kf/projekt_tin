#include <iostream>
#include <unistd.h>

#include "ReadQueue.h"
#include "consts.h"

int main() {
//    std::string msg;
    char msg[256];
//    std::cout<<0<<std::endl;

    ReadQueue *readQueue = new ReadQueue(QUEUE_NAME, O_CREAT | O_RDONLY, QUEUE_MODE, MSG_SIZE, MAX_MSGS);

//    std::cout<<1<<std::endl;
    while (true){
//        msg = readQueue->readToString();
        Packet *packet = readQueue->readToPacket();

        ACK *ack;
        EOT *eot;
        SET *set;

        if( ack = dynamic_cast<ACK *>(packet)){
            std::cout<<"ACK read ";//<<std::endl;
            hex_print(ack->getBuf(), ack->getBufSize());
        }

        else if(eot = dynamic_cast<EOT *>(packet)){
            std::cout<<"EOT read"<<std::endl;
            break;
        }else if(set = dynamic_cast<SET *>(packet)){
            std::cout<<"SET read"<<std::endl<<"PACKET ID: ";
            hex_print(set->getBuf(), set->getBufSize());
            std::cout<<"SET id: "<<set->getId()<<std::endl<<"SET value: "<<set->getValue()<<std::endl;

        }else{

            std::cout<<"Wrong packet read"<<std::endl;
            break;
        }

//        int bytesRead = readQueue->readToCharArray(msg);
////        std::cout<<msg<<std::endl;
//        hex_print(msg, bytesRead);
//
//        if(msg == "q!")
//            break;
//
//        if(msg=="sleep")
//            sleep(3);

    }

    delete(readQueue);

    return 0;
}