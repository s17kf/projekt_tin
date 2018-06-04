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
        std::cout<<"1"<<std::endl;
        Packet *packet = readQueue->readToPacket();
        std::cout<<"2"<<std::endl;

        ACK *ack;
        EOT *eot;

        if( ack = dynamic_cast<ACK *>(packet)){
            std::cout<<"ACK read"<<std::endl;
        }
        else if(eot = dynamic_cast<EOT *>(packet)){
            std::cout<<"EOT read"<<std::endl;
            break;
        }
        else{
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