//
// Created by s17kf on 04.06.18.
//

#include <iostream>

#include "AddQueue.h"

int main() {

    std::string msg;
    AddQueue *addQueue = new AddQueue(QUEUE_NAME, O_WRONLY);

    ACK *ack = new ACK(2);
    EOT *eot = new EOT();

    while(true){
        std::getline(std::cin, msg);
        if(msg == "curr")
            std::cout<<addQueue->getMessagesInQueue()<<std::endl;

        if(msg == "a"){
            ack->setPacketID(PCK_Q_ACK);
            addQueue->addMessage(ack);
        }else{
            eot->setPacketID(PCK_Q_EOT);
            addQueue->addMessage(eot);
        }

        if(msg=="q!")
            break;
    }

    delete(addQueue);

    return 0;
}