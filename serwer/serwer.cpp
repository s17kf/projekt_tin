//
// Created by s17kf on 06.06.18.
//

#include <fstream>
#include "serwer.h"

Serwer::Serwer(const char *privkeyFile, int port, const char *loginFileName,
               const char *addQueueName, const char *readQueueName) :
        privkey(privkeyFile),
        addQueue(addQueueName, O_WRONLY),
        readQueue(readQueueName, O_RDONLY),
        portNr(port)
{

    log(5, "Queue add descriptor: %d", addQueue.getQueueDescriptor());

    int usersCount = loadLoginFile(loginFileName);
    log(3, "Added %d users", usersCount);


}

Serwer::~Serwer() {

//    mqSender.join();
    mqReceiver.join();
}

const AndroidClient* Serwer::getClient(const unsigned char ssidValue) {
    auto iter = clients.find(ssidValue);
    if(iter != clients.end()){
        return iter->second;
    }

    return nullptr;
}


int Serwer::addClient(const unsigned char ssidValue, Sesskey *sesskey) {
    auto iter = clients.find(ssidValue);
    if(iter != clients.end()){
        log(2, "try to add new android client with already existing ssid");
        return -1;
    }
    clients.insert(std::pair<unsigned char, AndroidClient *>(ssidValue, new AndroidClient(*sesskey, ssidValue)));
    log(2, "new android client added");
    return 0;

}

Sesskey* Serwer::getSesskey(unsigned char ssidValue) {
    auto iter = clients.find(ssidValue);
    if(iter != clients.end()){
        return iter->second->getSesskey();
    }

    return nullptr;
}

int Serwer::removeClient(unsigned char ssidValue) {
    auto iter = clients.find(ssidValue);
    if(iter == clients.end()){
        return -1;
    }

    clients.erase(iter);
    log(3, "client deleted");
    return 0;
}




void Serwer::mainLoop() {
    std::cout<<"server main loop"<<std::endl;
    Connection *connection = new Connection(portNr);
//    AndroidClient androidClient;

    OPENSSL_config(nullptr);
    ERR_load_crypto_strings();

    mqReceiver = std::thread(&Serwer::mqReceiveLoop, this);
//    mqSender = std::thread(&Serwer::mqSendLoop, this);
//    androidController = std::thread(&Serwer::androidControlLoop, this, portNr);



    while(true) {

        if(connection->connect() <0){
            //TODO: error service
            std::cout<<"connecting error, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
            exit(1);
        }

        unsigned char ssidValue;
        Packet *received_packet = connection->receive(this, &ssidValue);

        if(CHALL *chall = dynamic_cast<CHALL *>(received_packet)){
            std::cout<<"CHALL received -> go to log in sequence"<<std::endl;
            if(logInSequence(connection, &privkey, this, chall) < 0){
                //TODO: service wrong login sewuence
                std::cout<<"Some error while log in"<<std::endl;
            }
//            hex_print(androidClient.getSesskey(), 16);
        }else if(EOT *chall = dynamic_cast<EOT *>(received_packet)){
            std::cout<<"EOT received"<<std::endl;
            if(endSessionSequence(connection, this, ssidValue) < 0){
                log(1, "try to remove not logged client, ssid: %d", ssidValue);
            }
            addQueue.close();
        }else if(SERVICES *services = dynamic_cast<SERVICES *> (received_packet)){
            std::cout<<"SERVICES received"<<std::endl;
            if(servicesSequence(connection, getClient(ssidValue), &devices) < 0){
                //TODO
                std::cout<<"Some error while descriptors sending"<<std::endl;
            }
        }else if(GET * get = dynamic_cast<GET *>(received_packet)){
            std::cout<<"GET received, get id: ";
            hex_print(get->getId());
            get->print();
            int ret = getSequence(connection, this, ssidValue, get,
                    &queueToAndroid, &devices, &addQueue);
            if( ret < 0){
                if(ret==-1){
                    log(2, "received packet get of invalid device id: %d", get->getId());
                }else {
                    //TODO
                    std::cout << "Some error during get sequence" << std::endl;
                }
            }
        }else if(SET * set = dynamic_cast<SET *>(received_packet)){
            std::cout<<"SET received:"<<std::endl;
            set->print();
            int ret = setSequence(connection, getClient(ssidValue), set,
                    &queueToAndroid, &devices, &addQueue);
            if( ret < 0){
                if(ret==-1){
                    log(2, "received packet set of invalid device id: %d", set->getId());
                }else {
                    log(1, "can not add mq msg");
                    connection->endClientConnection();
                }
            }
        }else if(received_packet == nullptr){
            connection->endClientConnection();
        }


        std::cout << "android thread" << std::endl;
//        sleep(1);



    }




}



void Serwer::mqReceiveLoop() {

    while(true) {
        QueuePacket *packetFromGonzo = QueuePacket::packetFromQueue(&readQueue);
        if (packetFromGonzo != nullptr) {

            if (Q_DESC *q_desc = dynamic_cast<Q_DESC *>(packetFromGonzo)) {

                DevDescriptor *newDev = new DevDescriptor(q_desc);
                devices.insert(std::pair<unsigned char, DevDescriptor*>(newDev->getId(),
                                                                       newDev));

                log(2, "New device registered: id %d, class %d, name %s, unit %s, min %f, max %f",
                    newDev->getId(), newDev->getClass(), newDev->getName(), newDev->getUnit(),
                    newDev->getMin(), newDev->getMax());

            } else if (Q_VAL *q_val = dynamic_cast<Q_VAL *>(packetFromGonzo)) {
                std::cout << "received value from gonzo" << std::endl;
                std::cout << "dev id: ";
                hex_print(q_val->getServiceId());
                std::cout << "value: " << q_val->getValue() << std::endl;
                std::cout << "time stamp: " << q_val->getTimestamp() << std::endl;
                queueToAndroid.push(new VAL(q_val->getServiceId(), q_val->getValue(), q_val->getTimestamp()));
            } else if (Q_EXIT *q_exit = dynamic_cast<Q_EXIT *>(packetFromGonzo)) {
                if(deleteDevice(q_exit->getId()) < 0)
                    log(2, "read exit from mq of non existing device id: %d", q_exit->getId());

            }else if (Q_NAK *q_nak = dynamic_cast<Q_NAK *>(packetFromGonzo)) {
                log(3, "read nak from mq queue nak id: %d", q_nak->getId());
                if(deleteDevice(q_nak->getId()) < 0)
                    log(2, "read exit from mq of non existing device id: %d", q_nak->getId());

            }else{
                log(1, "read bad packet type from mq qgueue");
//                packetFromGonzo->print();
            }

        } else {
//            std::cout << "some read error on mq queue" << std::endl;
            log(1, "read error on mq queue");
        }

    }


}


int Serwer::logInSequence(Connection *connection, Privkey *privkey, Serwer *serwer, CHALL *chall ){
    Packet *received_packet;
    RNG rng;

    unsigned char chall_value[8];
    memcpy(chall_value, chall->getChall(), 8);

    /* encrypting chall and sendint responce */
    unsigned char encrypted[256];
//    privkey->encrypt(chall_value, 8, encrypted);
    unsigned int resp_size = 256;
    privkey->sign(chall_value, 8, encrypted, &resp_size);
    CHALL_RESP *chall_resp = CHALL_RESP::createFromEncrypted(encrypted);
    connection->send(chall_resp, nullptr);
    delete chall_resp;
    std::cout<<"chall resp sent"<<std::endl;


    /* receiving session key*/
    Sesskey * sesskey;
    unsigned char key_encrypted[256];
    received_packet = connection->receive(serwer, nullptr);
    if(KEY *key = dynamic_cast<KEY *>(received_packet)){
        std::cout<<"key received"<<std::endl;
        memcpy(key_encrypted, key->getKeyBuf(), 256);
        hex_print(key_encrypted, 256);

        sesskey = new Sesskey(*key, *privkey);
        hex_print(sesskey->getKeyBuf(),16);
//        androidClient->setSesskey(*sesskey);
    }else{
        std::cout<<"wrong packet received, expected KEY"<<std::endl<<"received: ";
//        hex_print(received_packet->getBuf(),received_packet->getBufSize());
        received_packet->print();
        return -1;
    }

//    hex_print(androidClient->getSesskey(), 16);
    hex_print(sesskey, 16);
    /*SSID generating and sending*/
    std::cout<<"ssid generating"<<std::endl;
    unsigned char ssid_value;
    rng.generate(&ssid_value, 1);
    SSID *ssid = new SSID(ssid_value);
    serwer->addClient(ssid_value, sesskey);
    connection->send(ssid, sesskey);
//    androidClient->setSssid(ssid_value);
    std::cout<<"ssid sent"<<std::endl;

    unsigned char ssidValue;
    /*LOG receiving and checking login and password*/
    received_packet = connection->receive(serwer, &ssidValue);
//    std::cout<<"log packet received"<<std::endl;
    if(received_packet == nullptr){
//        std::cout<<"nullptr received"<<std::endl;
        log(2, "nullptr received, expected LOG packet");
        //TODO service or meybe reurned value is good
        return -2;
    }
    char loginUnsig[31];
    unsigned char passwordUnsig[31];
//    std::string login(31, 'c');
//    std::string password(31, 'c');

    if(LOG *logPacket = dynamic_cast<LOG *>(received_packet)){
//        memcpy(&loginUnsig, logPacket->getLogin(), 31);
//        memcpy(&password, logPacket->getPassword(), 31);
//        std::cout<<"login received"<<std::endl;
        std::string login((char *)logPacket->getLogin());
        std::string password((char *)logPacket->getPassword());
//        int login_len =0;
//        std::string login(login_len, 'c');
//        for(; loginUnsig[login_len]!='\0';++login_len);
//        int i = 0;
//        for(; loginUnsig[i]!='\0'; ++i){
//            login[i] = loginUnsig[i];
//        }
//        login[i] = loginUnsig[i];

        log(2, "new user %s want to log in", login);
        std::cout<<login<<" "<<password<<std::endl;


        if(findUser(login)){
            if(checkPassword(login, password)){
//                addClient(ssidValue, sesskey);
            }else{
                log(2, "try to log in with wrong password");
                /*sending NAK after bad received */
                unsigned char nak_id = 0x02;
                NAK *nak = new NAK(nak_id);
                connection->send(nak, serwer->getSesskey(ssid_value));
                std::cout<<"nak sent"<<std::endl;
                delete nak;

                removeClient(ssid_value);
                return 0;

            }


        }else{
            log(2, "try to log in on no existing login");

            /*sending NAK after bad received */
            unsigned char nak_id = 0x01;
            NAK *nak = new NAK(nak_id);
            connection->send(nak, serwer->getSesskey(ssid_value));
            std::cout<<"nak sent"<<std::endl;
            delete nak;

            removeClient(ssid_value);
            return 0;
        }

    }else{

//        std::cout<<"invslid login main.cpp l56, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        return -1;
    }


    serwer->addClient(ssid_value, sesskey);
    /*sending ACK after login received */
    unsigned char ack_id = 0x01;
    ACK *ack = new ACK(ack_id);
    connection->send(ack, serwer->getSesskey(ssid_value));
    std::cout<<"ack sent"<<std::endl;
    delete ack;


    return 0;
}


int Serwer::endSessionSequence(Connection *connection, Serwer *serwer, unsigned char ssidValue){

    unsigned char ack_id = 0x01;
    ACK *ack = new ACK(ack_id);
    connection->send(ack, serwer->getSesskey(ssidValue));

//    androidClient->reset();
    serwer->removeClient(ssidValue);
    log(2, "client with id %d has log out", ssidValue);

    std::cout<<"ack sent"<<std::endl;
    delete ack;

    return 0;
}

int Serwer::servicesSequence(Connection *connection, const AndroidClient *androidClient, std::map<unsigned char, DevDescriptor*> *devices){

    for(auto iter = devices->begin();iter!=devices->end();iter++){
        DESC * desc = new DESC(iter->second->getId(), iter->second->getClass(), iter->second->getName(), iter->second->getUnit(), iter->second->getMin(), iter->second->getMax());
        connection->send(desc, androidClient->getSesskey());
        log(3, "desc sent");
        desc->print();
        Q_GET *q_get = new Q_GET(desc->getDeviceId());
        q_get->addToQueue(&addQueue);
        std::cout<<"get added";
        while(queueToAndroid.empty());
        if(VAL * val = dynamic_cast<VAL *>(queueToAndroid.front())){
//            std::cout<<"timestamp: "<<val->getTimestamp()<<std::endl;
            queueToAndroid.pop();
            connection->send(val, androidClient->getSesskey());
            log(3, "val sent");
            val->print();
        }
        else if(NAK * nak = dynamic_cast<NAK *>(queueToAndroid.front())){
            queueToAndroid.pop();
            log(2, "nak read from queue, id: %d", nak->getId());
        }
        //TODO: check if no errors during sending
    }
    EOT * eot = new EOT();
    connection->send(eot, androidClient->getSesskey());
    log(3, "eot sent");
    //TODO: check if no errors during sendig

    return 0;
}

int Serwer::getSequence(Connection *connection, Serwer *serwer, unsigned char ssidValue,
                GET *get, std::queue<Packet *> *queueToAndroid,
                std::map<unsigned char, DevDescriptor*> *devices, AddQueue *addQueue){
    if(get->getId()==0){
//        GET *get_to_queue = new GET(get->getId());

        for(auto iter = devices->begin();iter!=devices->end();iter++){
            get->print();
            GET *get_i = new GET(iter->second->getId());
            Q_GET * q_get = new Q_GET(get_i->getId());
            q_get->addToQueue(addQueue);
            std::cout<<"sent to server: ";
            q_get->print();
            while(queueToAndroid->empty());
            if(VAL *val = dynamic_cast<VAL *>(queueToAndroid->front())){
                std::cout<<"timestamp: "<<val->getTimestamp()<<std::endl;
                queueToAndroid->pop();
                connection->send(val, serwer->getSesskey(ssidValue));
            }else{
                std::cout<<"bad packet received from Gonzo, expected VAL"<<std::endl;
                queueToAndroid->pop();
                return -2;
            }
        }

        EOT *eot = new EOT();
        connection->send(eot, serwer->getSesskey(ssidValue));
        std::cout<<"eot after all values sent"<<std::endl;

    } else{
        get->print();

        /*checking if id is correct*/
        auto iter = devices->find(get->getId());

        if(iter != devices->end()) {
            Q_GET *q_get = new Q_GET(get->getId());
            int addedBytes = q_get->addToQueue(addQueue);
            if(addedBytes < 0){
                log(1, "(GET) error adding to queue: errno%d: %s", errno, strerror(errno));
                log(5, "addQueue descriptor: %d", addQueue->getQueueDescriptor());
                return -2;
            }else if(addedBytes < 2){
                log(1, "(GET): Added to queue %d bytes! Should be 2 bytes", addedBytes);
            }else{
                log(4, "(GET): Added to queue %d bytes", addedBytes);
            }
//            std::cout<<"get add to queue: ";
            q_get->print();
            while(queueToAndroid->empty());
            if(VAL *val = dynamic_cast<VAL *>(queueToAndroid->front())){
                std::cout<<"timestamp: "<<val->getTimestamp()<<std::endl;
                queueToAndroid->pop();
                connection->send(val, serwer->getSesskey(ssidValue));
            }
        }else{
            NAK * nak = new NAK(get->getId());
            connection->send(nak, serwer->getSesskey(ssidValue));
            std::cout<<"nak sent to android: ";
            nak->print();
            return -1;
        }
    }




    return 0;
}


int Serwer::setSequence(Connection *connection, const AndroidClient *androidClient, SET *set,
                std::queue<Packet *> *queueToAndroid,
                std::map<unsigned char, DevDescriptor*> *devices, AddQueue *addQueue){

    std::cout<<"SET id: "<<set->getId()<<"; set value"<<set->getValue()<<std::endl;

    /*checking if id is correct*/
    auto iter = devices->find(set->getId());
//    if(id_correct){
    if(iter != devices->end()){
        Q_SET * q_set = new Q_SET(set->getId(), set->getValue());
        q_set->addToQueue(addQueue);

        ACK * ack = new ACK(set->getId());
        connection->send(ack, androidClient->getSesskey());
        std::cout<<"ack sent to android: ";
        ack->print();
    }
    else{
        NAK * nak = new NAK(set->getId());
        connection->send(nak, androidClient->getSesskey());
        std::cout<<"nak sent to android: ";
        nak->print();
        return -1;
    }




    return 0;
}

bool Serwer::testLoginFile(const char *filename) {
    std::ifstream file(filename);
    if(!file.is_open()){
        log(2, "cannot open login file");
        return false;
    }
    std::string login, password;
    bool fileOK = false;
    while(file>>login){
        if(!(file>>password))
        if(login.size()>30 || password.size()>30){
            return false;
        }
        fileOK = true;
    }

    return fileOK;
}


int Serwer::loadLoginFile(const char *filename) {
    std::ifstream file(filename);
    if(!file.is_open()){
        return -1;
    }
    int readRecords = 0;
    std::string login, password;
    while(file>>login){
        file>>password;
        bool inserted = users.insert(std::pair<std::string, std::string>(login, password) ).second;
        if(inserted){
            log(3, "user %s added", login);
            ++readRecords;
        }else{
            log(3, "user %s not added", login);
        }
    }


    return readRecords;
}

int Serwer::deleteDevice(const char devId) {
    auto iter = devices.find(devId);
    if(iter!=devices.end()){
        devices.erase(iter);
        log(2, "device id %d deleted", devId);
        return 0;
    }else{
        return -1;
    }
}

bool Serwer::findUser(std::string login) {
//    auto iter = users.find(login);
    return users.find(login) != users.end();
}

bool Serwer::checkPassword(std::string login, std::string password) {
    auto iter = users.find(login);
    return iter->second == password;
}












