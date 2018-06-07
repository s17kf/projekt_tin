//
// Created by s17kf on 06.06.18.
//

#include "serwer.h"

Serwer::Serwer(const char *privkeyFile, int port):privkey(privkeyFile),
  addQueue(QUEUE_NAME_TO_GONZO, O_CREAT | O_WRONLY, QUEUE_MODE, MSG_SIZE, MAX_MSGS),
  readQueue(QUEUE_NAME_FROM_GONZO, O_CREAT | O_RDONLY, QUEUE_MODE, MSG_SIZE, MAX_MSGS),
  portNr(port)
{

    log(5, "Queue add descriptor: %d", addQueue.getQueueDescriptor());



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
//            std::cout<<"msg in queue from Gonzo"<<std::endl;

//            std::cout<<"msg read"<<std::endl;
            if (Q_DESC *q_desc = dynamic_cast<Q_DESC *>(packetFromGonzo)) {
                std::cout << "Received descriptor from Gonzo" << std::endl;
                std::cout << "dev id: ";
                hex_print_noendl(q_desc->getDeviceId());
                std::cout << "dev class: ";
                hex_print_noendl(q_desc->getDeviceClass());
                std::cout << "dev name: " << q_desc->getName()<< "dev unit: " << q_desc->getUnit();
                std::cout << "min value: " << q_desc->getMin();
                std::cout << "max vaalue: " << q_desc->getMax() << std::endl;

                DevDescriptor devDescriptor(q_desc);
                devices.insert(std::pair<unsigned char, DevDescriptor>(devDescriptor.getId(),
                                                                       devDescriptor));

                std::cout << "all devices:" << std::endl;
                for (auto iter = devices.begin(); iter!=devices.end(); iter++) {
                    std::cout << "id: ";
                    hex_print_noendl(iter->second.getId());
                    std::cout << "class: ";
                    hex_print_noendl(iter->second.getClass());
                    std::cout << iter->second.getName() << " " << iter->second.getUnit() << " ";
                    std::cout << iter->second.getMin() << " " << iter->second.getMax() << std::endl;
                }

            } else if (Q_VAL *q_val = dynamic_cast<Q_VAL *>(packetFromGonzo)) {
                std::cout << "received value from gonzo" << std::endl;
                std::cout << "dev id: ";
                hex_print(q_val->getServiceId());
                std::cout << "value: " << q_val->getValue() << std::endl;
                std::cout << "time stamp: " << q_val->getTimestamp() << std::endl;
                queueToAndroid.push(new VAL(q_val->getServiceId(), q_val->getValue(), q_val->getTimestamp()));
            } else if (Q_EXIT *q_exit = dynamic_cast<Q_EXIT *>(packetFromGonzo)) {

                auto iter = devices.find(q_exit->getId());

                if(iter!=devices.end()){
                    devices.erase(iter);
                    std::cout<<"device id: ";
                    hex_print_noendl(q_exit->getId());
                    std::cout<<" deleted"<<std::endl;
                    std::cout << "all devices:" << std::endl;
                    for (auto itt = devices.begin();itt!=devices.end(); itt++) {
                        std::cout << "id: ";
                        hex_print_noendl(itt->second.getId());
                        std::cout << "class: ";
                        hex_print_noendl(itt->second.getClass());
                        std::cout << itt->second.getName() << " " << itt->second.getUnit() << " ";
                        std::cout << itt->second.getMin() << " " << itt->second.getMax() << std::endl;
                    }
                    std::cout<<"no more devices"<<std::endl;
                }else{
                    log(2, "read exit from mq of non existing device id: %d", q_exit->getId());
                }

//                for(auto it = devices.begin(); it!= devices.end(); it++){
//                    if(it->getId() == q_exit->getId()){
//                        devices.erase(it);
//                        std::cout<<"device id: ";
//                        hex_print_noendl(q_exit->getId());
//                        std::cout<<" deleted"<<std::endl;
//
//                        std::cout << "all devices:" << std::endl;
//                        for (auto itt = devices.begin();itt!=devices.end(); itt++) {
//                            std::cout << "id: ";
//                            hex_print_noendl(itt->getId());
//                            std::cout << "class: ";
//                            hex_print_noendl(itt->getClass());
//                            std::cout << itt->getName() << " " << itt->getUnit() << " ";
//                            std::cout << itt->getMin() << " " << itt->getMax() << std::endl;
//                        }
//                        std::cout<<"no more devices"<<std::endl;
//                        break;
//                    }
//                }
                std::cout << "received exit from Gonzo, exit id: ";
                hex_print(q_exit->getId());
            }else if (Q_NAK *q_nak = dynamic_cast<Q_NAK *>(packetFromGonzo)) {
//                for(auto it = devices.begin(); it!= devices.end(); it++){
//                    if(it->getId() == q_nak->getId()){
//                        devices.erase(it);
//                        std::cout<<"device id: ";
//                        hex_print_noendl(q_nak->getId());
//                        std::cout<<" deleted"<<std::endl;
//
//                        std::cout << "all devices:" << std::endl;
//                        for (auto itt = devices.begin();itt!=devices.end(); itt++) {
//                            std::cout << "id: ";
//                            hex_print_noendl(itt->getId());
//                            std::cout << "class: ";
//                            hex_print_noendl(itt->getClass());
//                            std::cout << itt->getName() << " " << itt->getUnit() << " ";
//                            std::cout << itt->getMin() << " " << itt->getMax() << std::endl;
//                        }
//                        std::cout<<"no more devices"<<std::endl;
//                        break;
//                    }
//                }
                std::cout << "received nak from Gonzo, nak id: ";
                hex_print(q_nak->getId());
            }else{
                std::cout<<"received wrong packet from Gonzo: "<<std::endl;
                packetFromGonzo->print();
            }

        } else {
            std::cout << "some read error on mq queue" << std::endl;
        }

    }


}


//void Serwer::mqSendLoop() {
//    while (true) {
//
//        if(!queueFromAndroid.empty()){
//
//            std::cout<<"queue from android is not empty"<<std::endl;
//            queueFromAndroid.front()->print();
//            if(GET *get = dynamic_cast<GET *> (queueFromAndroid.front())){
//                Q_GET * q_get = new Q_GET(get->getId());
//                q_get->addToQueue(&addQueue);
//            }else if(SET *set = dynamic_cast<SET *>(queueFromAndroid.front())){
//                Q_SET * q_set = new Q_SET(set->getId(), set->getValue());
//                q_set->addToQueue(&addQueue);
//            }else{
//                std::cout<<"This packet should not be sent to Gonzo"<<std::endl;
//                queueFromAndroid.front()->print();
////                hex_print(queueFromAndroid.front()->getBuf(),queueFromAndroid.front()->getBufSize());
//            }
////            addQueue->addMessage(queueFromAndroid.front());
//            queueFromAndroid.pop();
//        }
//
//
////        std::cout << "server thread" << std::endl;
////        sleep(1);
//    }
//}

//void Serwer::androidControlLoop(int portNr) {
//    Connection *connection = new Connection(portNr);
//    AndroidClient androidClient;
//
//    OPENSSL_config(nullptr);
//    ERR_load_crypto_strings();
//
//
//    Privkey *privkey = new Privkey("/home/s17kf/Pobrane/serwer_key-private.pem");
//
//
//    while(true) {
//
//        if(connection->connect() <0){
//            //TODO: error service
//            std::cout<<"connecting error, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
//            exit(1);
//        }
//
//        Packet *received_packet = connection->receive(&androidClient);
//        if(CHALL *chall = dynamic_cast<CHALL *>(received_packet)){
//            std::cout<<"CHALL received -> go to log in sequence"<<std::endl;
//            if(logInSequence(connection, privkey, &androidClient, chall) < 0){
//                //TODO: service wrong login sewuence
//                std::cout<<"Some error while log in"<<std::endl;
//            }
////            hex_print(androidClient.getSesskey(), 16);
//        }else if(EOT *chall = dynamic_cast<EOT *>(received_packet)){
//            std::cout<<"EOT received"<<std::endl;
//            if(endSessionSequence(connection, &androidClient) < 0){
//                //TODO: service wrong ending session sewuence
//                std::cout<<"Some error while ending session"<<std::endl;
//            }
//            addQueue.close();
//        }else if(SERVICES *services = dynamic_cast<SERVICES *> (received_packet)){
//            std::cout<<"SERVICES received"<<std::endl;
//            if(servicesSequence(connection, &androidClient, devices) < 0){
//                //TODO
//                std::cout<<"Some error while descriptors sending"<<std::endl;
//            }
//        }else if(GET * get = dynamic_cast<GET *>(received_packet)){
//            std::cout<<"GET received, get id: ";
//            hex_print(get->getId());
//            get->print();
//            if(getSequence(connection, &androidClient, get, &queueFromAndroid, &queueToAndroid, &devices, &addQueue) < 0){
//                //TODO
//                std::cout<<"Some error during get sequence"<<std::endl;
//            }
//        }else if(SET * set = dynamic_cast<SET *>(received_packet)){
//            std::cout<<"SET received:"<<std::endl;
////            hex_print(set->getBuf(), set->getBufSize());
//            set->print();
//            if(setSequence(connection, &androidClient, set, &queueFromAndroid, &queueToAndroid, &devices) < 0){
//                //TODO
//                std::cout<<"Some error during set sequence"<<std::endl;
//            }
//        }
//
//
//        std::cout << "android thread" << std::endl;
////        sleep(1);
//
//
//
//    }
//}




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
    std::cout<<"log packet received"<<std::endl;
    if(received_packet == nullptr){
        std::cout<<"nullptr received"<<std::endl;
        //TODO service or meybe reurned value is good
        return -2;
    }
    unsigned char login[31];
    unsigned char password[31];
    if(LOG *log = dynamic_cast<LOG *>(received_packet)){
        memcpy(login, log->getLogin(), 31);
        memcpy(password, log->getPassword(), 31);
        std::cout<<"login received"<<std::endl;
        std::cout<<login<<std::endl;
        std::cout<<password<<std::endl;
    }else{
        std::cout<<"invslid login main.cpp l56, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        return -1;
    }


    /*sending ACK after sesskey received */
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

int Serwer::servicesSequence(Connection *connection, const AndroidClient *androidClient, std::map<unsigned char, DevDescriptor> *devices){

//    for( DESC * desc : devices){
//    for(DevDescriptor descriptor : *devices){
    for(auto iter = devices->begin();iter!=devices->end();iter++){
        DESC * desc = new DESC(iter->second.getId(), iter->second.getClass(), iter->second.getName(), iter->second.getUnit(), iter->second.getMin(), iter->second.getMax());
        connection->send(desc, androidClient->getSesskey());
        //TODO: check if no errors during sending
    }
    EOT * eot = new EOT();
    connection->send(eot, androidClient->getSesskey());
    //TODO: check if no errors during sendig

    return 0;
}

int Serwer::getSequence(Connection *connection, Serwer *serwer, unsigned char ssidValue,
                GET *get, std::queue<Packet *> *queueToAndroid,
                std::map<unsigned char, DevDescriptor> *devices, AddQueue *addQueue){
    if(get->getId()==0){
//        GET *get_to_queue = new GET(get->getId());

        for(auto iter = devices->begin();iter!=devices->end();iter++){
            get->print();
            GET *get_i = new GET(iter->second.getId());
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

//        bool id_correct = false;
//        for(auto dev : *devices){
//            if(dev.getId() == get->getId()){
//                id_correct = true;
//                break;
//            }
//        }

//        if(id_correct){
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
                std::map<unsigned char, DevDescriptor> *devices, AddQueue *addQueue){

    std::cout<<"SET id: "<<set->getId()<<"; set value"<<set->getValue()<<std::endl;

    /*checking if id is correct*/
//    bool id_correct = false;
//    for(auto dev : *devices){
//        if(dev.getId() == set->getId()){
//            id_correct = true;
//            break;
//        }
//    }
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