//
// Created by s17kf on 31.05.18.
//

#include "ReadQueue.h"


ReadQueue::ReadQueue(const std::string name, int oflags) {
    queueDescriptor = mq_open(name.c_str(), oflags);
    queueName = name;
    if(queueDescriptor == (mqd_t)-1)
        log(2, "Openong addQueue %s error: %s", name, strerror(errno));

    lastMsg = new char[getMsgSize()+1];

}

ReadQueue::ReadQueue(const std::string name, int oflags, int queueMode, int msgSize, int maxMsgs) {

    attributes = new mq_attr();
    attributes->mq_msgsize = msgSize;
    attributes->mq_maxmsg = maxMsgs;

    queueDescriptor = mq_open(name.c_str(), oflags, queueMode, attributes);

    queueName = name;
    CHECK(queueDescriptor != (mqd_t)-1);

    lastMsg = new char[getMsgSize()+1];

}



int ReadQueue::readToCharArray(char *dest) {

    int bytesRead = mq_receive(queueDescriptor, dest, getMsgSize(), NULL);
    return bytesRead;
}



std::string ReadQueue::getName() {
    return queueName;
}

int ReadQueue::getMsgSize() {
    mq_attr attr;

    if(mq_getattr(queueDescriptor, &attr) == -1)
        return -1;
    return attr.mq_msgsize;
}

ReadQueue::~ReadQueue() {
    if(mq_close(queueDescriptor)<0){
        log(4, "error during closing readQueue: %s", strerror(errno));
    }
    if(mq_close(queueDescriptor)<0){
        log(4, "error during unlinking readQueue: %s", strerror(errno));
    }
    delete(attributes);
    delete(lastMsg);
}

long ReadQueue::getMessagesInQueue() {
    mq_attr attr;
    if(mq_getattr(queueDescriptor, &attr) == -1)
        return -1;
    return attr.mq_curmsgs;

}