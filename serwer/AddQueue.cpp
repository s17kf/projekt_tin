//
// Created by s17kf on 31.05.18.
//

#include <cstring>
#include "AddQueue.h"

AddQueue::AddQueue(const std::string name, int oflags) {
    queueDescriptor = mq_open(name.c_str(), oflags);
    queueName = name;
    if(queueDescriptor == (mqd_t)-1)
        log(4, "Openong addQueue error: ", strerror(errno));

}

AddQueue::AddQueue(const std::string name, int oflags, int queueMode, int msgSize, int maxMsgs) {

    attributes = new mq_attr();
    attributes->mq_msgsize = msgSize;
    attributes->mq_maxmsg = maxMsgs;

    queueDescriptor = mq_open(name.c_str(), oflags, queueMode, attributes);

    queueName = name;
    if(queueDescriptor == (mqd_t)-1)
        log(4, "Openong addQueue error: ", strerror(errno));


}

int AddQueue::addMessage(std::string msg) {
    addMessage(msg.c_str(), msg.size()+1);
}

int AddQueue::addMessage(const char *msg, int msgSize) {
    return mq_send(queueDescriptor, msg, msgSize, 0);
}

mqd_t AddQueue::getQueueDescriptor() {
    return queueDescriptor;
}

std::string AddQueue::getName() {
    return queueName;
}

int AddQueue::close() {
    return mq_close(queueDescriptor);
}


AddQueue::~AddQueue() {
    if(mq_close(queueDescriptor)<0){
        log(4, "error during closing addQueue: %s", strerror(errno));
    }
}



long AddQueue::getMessagesInQueue() {
    mq_attr attr;
    if(mq_getattr(queueDescriptor, &attr) == -1)
        return -1;
    return attr.mq_curmsgs;

}
