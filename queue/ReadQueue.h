//
// Created by s17kf on 31.05.18.
//

#ifndef MSGRECEIVING_READQUEUE_H
#define MSGRECEIVING_READQUEUE_H

#include <iostream>
#include <fcntl.h>
#include <mqueue.h>

#include "common.h"
#include "packet.h"

class ReadQueue {

    mqd_t queueDescriptor;
    std::string queueName;

    mq_attr *attributes;
    char *lastMsg;

public:
    ReadQueue(const std::string name, int oflags);
    ReadQueue( const std::string name, int oflags, int queueMode, int msgSize, int maxMsgs);

//    std::string readToString();
    int readToCharArray(char * dest);
    Packet * readToPacket();

    std::string getName();

    int getMsgSize();

    ~ReadQueue();

};


#endif //MSGRECEIVING_READQUEUE_H
