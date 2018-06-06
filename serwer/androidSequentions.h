//
// Created by s17kf on 05.06.18.
//

#ifndef SERWER_ANDROIDSEQUENTIONS_H
#define SERWER_ANDROIDSEQUENTIONS_H

#include <vector>
#include <queue>

#include "connection.h"
#include "RNG.h"
#include "privkey.h"
#include "sesskey.h"
#include "androidClient.h"
#include "log.h"
#include "DevDescriptor.h"



int logInSequence(Connection *connection, Privkey *privkey, AndroidClient *androidClient, CHALL *chall );
int endSessionSequence(Connection *connection, AndroidClient *androidClient);
int servicesSequence(Connection *connection, AndroidClient *androidClient, std::vector<DevDescriptor> descriptors);
int getSequence(Connection *connection, AndroidClient *androidClient, GET *get,
                std::queue<Packet *> *queueFromAndroid,
                std::queue<Packet *> *queueToAndroid,
                std::vector<DevDescriptor> *devices);
int setSequence(Connection *connection, AndroidClient *androidClient, SET *set,
    std::queue<Packet *> *queueFromAndroid, std::queue<Packet *> *queueToAndroid,
    std::vector<DevDescriptor> *devices);



#endif //SERWER_ANDROIDSEQUENTIONS_H
