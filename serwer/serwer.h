//
// Created by s17kf on 06.06.18.
//

#ifndef SERWER_SERWER_H
#define SERWER_SERWER_H

#include <iostream>
#include <unistd.h>
#include <thread>
#include <vector>
#include <queue>
#include "privkey.h"
#include "DevDescriptor.h"
#include "packet.h"
#include "queuePacket.h"
#include "connection.h"

#include "androidSequentions.h"
#include <map>


class Serwer {
    Privkey privkey;
    std::map<unsigned char, DevDescriptor> devices;
    AddQueue addQueue;
    ReadQueue readQueue;
    AndroidClient androidClient;
    std::queue<Packet *> queueToAndroid;
    std::queue<Packet *> queueFromAndroid;

    std::thread mqReceiver;

    int portNr;
//    std::thread mqSender;
//    std::thread androidController;

public:
    explicit Serwer(const char *privkeyFile, int portNr);

    void mqReceiveLoop();
//    void mqSendLoop();
//    void androidControlLoop(int portNr);

    void mainLoop();

    ~Serwer();


};


#endif //SERWER_SERWER_H
