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
    SET *set = new SET(0xAA, 2.12);

    std::cout<<set->getValue();

    while (true) {
        std::getline(std::cin, msg);
        if (msg == "curr")
            std::cout << addQueue->getMessagesInQueue() << std::endl;

        if (msg == "a") {
            ack->setPacketID(PCK_Q_ACK);
            addQueue->addMessage(ack);
        } else if (msg == "s") {
            set->setPacketID(PCK_Q_SET);
            addQueue->addMessage(set);
        } else {
            eot->setPacketID(PCK_Q_EOT);
            addQueue->addMessage(eot);
            break;
        }
    }


    delete(addQueue);

    return 0;
}