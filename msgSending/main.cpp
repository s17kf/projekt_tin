#include <iostream>

#include "AddQueue.h"

int main() {

    std::string msg;
    AddQueue *addQueue = new AddQueue(QUEUE_NAME, O_WRONLY);

    while(true){
        std::getline(std::cin, msg);
        addQueue->addMessage(msg);

        if(msg=="q!")
            break;
    }

    delete(addQueue);

    return 0;
}