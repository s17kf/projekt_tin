#include <iostream>

#include "ReadQueue.h"

int main() {
    std::string msg;
    std::cout<<0<<std::endl;

    ReadQueue *readQueue = new ReadQueue(QUEUE_NAME, O_CREAT | O_RDONLY, QUEUE_MODE, MSG_SIZE, MAX_MSGS);

    std::cout<<1<<std::endl;
    while (true){
        msg = readQueue->readToString();
        std::cout<<msg<<std::endl;

        if(msg == "q!")
            break;

    }

    delete(readQueue);

    return 0;
}