//
// Created by s17kf on 31.05.18.
//

#ifndef MSGSENDING_ADDQUEUE_H
#define MSGSENDING_ADDQUEUE_H

#include <iostream>
#include <mqueue.h>
#include <fcntl.h>

#include "packet.h"
#include "consts.h"


class AddQueue {
    mqd_t queueDescriptor;
    std::string queueName;
//    int queueOflags;

public:
    AddQueue( const std::string name, int oflags);

    int addMessage(std::string msg);
    int addMessage(const char *msg, int msgSize);
    int addMessage(Packet *packet);

    long getMessagesInQueue();

    std::string getName();

    ~AddQueue();

};


#endif //MSGSENDING_ADDQUEUE_H
