//
// Created by s17kf on 31.05.18.
//

#include "AddQueue.h"

AddQueue::AddQueue(const std::string name, int oflags) {
    queueDescriptor = mq_open(name.c_str(), oflags);
    queueName = name;
    CHECK(queueDescriptor != (mqd_t)-1);

}

int AddQueue::addMessage(std::string msg) {
    addMessage(msg.c_str(), msg.size()+1);
}

int AddQueue::addMessage(const char *msg, int msgSize) {
    CHECK(0 <= mq_send(queueDescriptor, msg, msgSize, 0));
}

std::string AddQueue::getName() {
    return queueName;
}

AddQueue::~AddQueue() {
    CHECK(mq_close(queueDescriptor) != (mqd_t)-1);
}