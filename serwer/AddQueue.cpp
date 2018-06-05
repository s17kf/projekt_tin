//
// Created by s17kf on 31.05.18.
//

#include <cstring>
#include "AddQueue.h"

AddQueue::AddQueue(const std::string name, int oflags) {
    queueDescriptor = mq_open(name.c_str(), oflags);
    queueName = name;
    //TODO check error
    CHECK(queueDescriptor != (mqd_t)-1);

}

int AddQueue::addMessage(std::string msg) {
    addMessage(msg.c_str(), msg.size()+1);
}

int AddQueue::addMessage(const char *msg, int msgSize) {
    CHECK(0 <= mq_send(queueDescriptor, msg, msgSize, 0));
}

int AddQueue::addMessage(Packet *packet) {
    char msg[packet->getBufSize()];
    memcpy(&msg, packet->getBuf(), packet->getBufSize());
    addMessage(msg, packet->getBufSize());
}


std::string AddQueue::getName() {
    return queueName;
}

AddQueue::~AddQueue() {
    CHECK(mq_close(queueDescriptor) != (mqd_t)-1);
}

long AddQueue::getMessagesInQueue() {
    mq_attr attr;
    CHECK(mq_getattr(queueDescriptor, &attr) == 0);
    return attr.mq_curmsgs;

}
