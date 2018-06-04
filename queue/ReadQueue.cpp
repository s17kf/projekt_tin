//
// Created by s17kf on 31.05.18.
//

#include "ReadQueue.h"


ReadQueue::ReadQueue(const std::string name, int oflags) {
    queueDescriptor = mq_open(name.c_str(), oflags);
    queueName = name;
    CHECK(queueDescriptor != (mqd_t)-1);

    CHECK(mq_getattr(queueDescriptor, attributes) != 0);

    lastMsg = new char[getMsgSize()+1];

}

ReadQueue::ReadQueue(const std::string name, int oflags, int queueMode, int msgSize, int maxMsgs) {

    attributes = new mq_attr();
    attributes->mq_msgsize = msgSize;
    attributes->mq_maxmsg = maxMsgs;

//    std::cout<<01<<std::endl;
    queueDescriptor = mq_open(name.c_str(), oflags, queueMode, attributes);
//    std::cout<<02<<std::endl;
    queueName = name;
    CHECK(queueDescriptor != (mqd_t)-1);

    lastMsg = new char[getMsgSize()+1];

}

//std::string ReadQueue::readToString() {
//
//    return readToCharArray();
//
//}

int ReadQueue::readToCharArray(char *dest) {
    int bytesRead = mq_receive(queueDescriptor, lastMsg, getMsgSize(), NULL);
//    CHECK(bytesRead >= 0);
    if(bytesRead <= 0)
        return bytesRead;

    memcpy(dest, lastMsg, bytesRead);

    return bytesRead;
}

Packet* ReadQueue::readToPacket() {
    int bytesRead = mq_receive(queueDescriptor, lastMsg, getMsgSize(), NULL);
//    CHECK(bytesRead >= 0);
    if(bytesRead <= 0)
        return nullptr;


    unsigned char msg[bytesRead];
    memcpy(msg, lastMsg, bytesRead);

    Packet *packet = Packet::packetFromQueue(msg, bytesRead);

    return packet;

}



std::string ReadQueue::getName() {
    return queueName;
}

int ReadQueue::getMsgSize() {
    return attributes->mq_msgsize;
}

ReadQueue::~ReadQueue() {
    CHECK(mq_close(queueDescriptor) != (mqd_t)-1);
    CHECK(mq_unlink(queueName.c_str()) != (mqd_t)-1);
    delete(attributes);
    delete(lastMsg);
}