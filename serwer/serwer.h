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
#include <map>
#include "privkey.h"
#include "DevDescriptor.h"
#include "packet.h"
#include "queuePacket.h"
#include "connection.h"

#include "androidSequentions.h"


class Serwer {
    Privkey privkey;
    AddQueue addQueue;
    ReadQueue readQueue;
    int portNr;
    std::map<unsigned char, DevDescriptor*> devices;
    std::map<unsigned char, AndroidClient*> clients;
    std::map<std::string, std::string> users;

//    AndroidClient androidClient;
    std::queue<Packet *> queueToAndroid;
//    std::queue<Packet *> queueFromAndroid;

    std::thread mqReceiver;


//    std::thread mqSender;
//    std::thread androidController;

    int logInSequence(Connection *connection, Privkey *privkey, Serwer *serwer, CHALL *chall );
    int endSessionSequence(Connection *connection, Serwer *serwer, unsigned char ssidValue);
    int servicesSequence(Connection *connection, const AndroidClient *androidClient,
                         std::map<unsigned char, DevDescriptor*> *devices);
    int getSequence(Connection *connection, Serwer *serwer, unsigned char ssidValue,
                    GET *get, std::queue<Packet *> *queueToAndroid,
                    std::map<unsigned char, DevDescriptor*> *devices, AddQueue *addQueue);
    int setSequence(Connection *connection, const AndroidClient *androidClient, SET *set,
                    std::queue<Packet *> *queueToAndroid,
                    std::map<unsigned char, DevDescriptor*> *devices, AddQueue *addQueue);

    int loadLoginFile(const char *filename);
    int deleteDevice(const char devId);
    bool findUser(std::string login);
    bool checkPassword(std::string login, std::string password);

public:
    explicit Serwer(const char *privkeyFile, int portNr, const char *loginFilename,
                    const char *addQueueName, const char *readQueueName);

    static bool testLoginFile(const char *filename);
    void mqReceiveLoop();
//    void mqSendLoop();
//    void androidControlLoop(int portNr);

    void mainLoop();

    const AndroidClient *getClient(const unsigned char ssidValue);

    int addClient(const unsigned char ssidValue, Sesskey *sesskey);
    Sesskey *getSesskey(unsigned char ssidValue);
    int removeClient(unsigned char ssidValue);


    ~Serwer();


};


#endif //SERWER_SERWER_H
