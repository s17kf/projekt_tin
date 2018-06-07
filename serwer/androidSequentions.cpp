//
// Created by s17kf on 05.06.18.
//

#include "androidSequentions.h"
#include "DevDescriptor.h"

int logInSequence(Connection *connection, Privkey *privkey, AndroidClient *androidClient, CHALL *chall ){
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
    connection->send(chall_resp, androidClient->getSesskey());
    delete chall_resp;
    std::cout<<"chall resp sent"<<std::endl;


    /* receiving session key*/
    unsigned char key_encrypted[256];
    received_packet = connection->receive(androidClient);
    if(KEY *key = dynamic_cast<KEY *>(received_packet)){
        std::cout<<"key received"<<std::endl;
        memcpy(key_encrypted, key->getKeyBuf(), 256);
        hex_print(key_encrypted, 256);

        Sesskey *sesskey = new Sesskey(*key, *privkey);
        hex_print(sesskey->getKeyBuf(),16);
        androidClient->setSesskey(*sesskey);
    }else{
        std::cout<<"wrong packet received, expected KEY"<<std::endl<<"received: ";
//        hex_print(received_packet->getBuf(),received_packet->getBufSize());
        received_packet->print();
        return -1;
    }

    hex_print(androidClient->getSesskey(), 16);

    /*SSID generating and sending*/
    std::cout<<"ssid generating"<<std::endl;
    unsigned char ssid_value;
    rng.generate(&ssid_value, 1);
    SSID *ssid = new SSID(ssid_value);
    connection->send(ssid, androidClient->getSesskey());
    androidClient->setSssid(ssid_value);
    std::cout<<"ssid sent"<<std::endl;


    /*LOG receiving and checking login and password*/
    received_packet = connection->receive(androidClient);
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
    connection->send(ack, androidClient->getSesskey());
    std::cout<<"ack sent"<<std::endl;
    delete ack;


    return 0;
}


int endSessionSequence(Connection *connection, AndroidClient *androidClient){

    unsigned char ack_id = 0x01;
    ACK *ack = new ACK(ack_id);
    connection->send(ack, androidClient->getSesskey());

    androidClient->reset();

    std::cout<<"ack sent"<<std::endl;
    delete ack;

    return 0;
}

int servicesSequence(Connection *connection, AndroidClient *androidClient, std::map<unsigned char, DevDescriptor> *devices){

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

int getSequence(Connection *connection, AndroidClient *androidClient, GET *get,
                std::queue<Packet *> *queueFromAndroid,
                std::queue<Packet *> *queueToAndroid,
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
                connection->send(val, androidClient->getSesskey());
            }else{
                std::cout<<"bad packet received from Gonzo, expected VAL"<<std::endl;
                queueToAndroid->pop();
                return -2;
            }
        }

        EOT *eot = new EOT();
        connection->send(eot, androidClient->getSesskey());
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
        if(iter != devices->end()){
            Q_GET * q_get = new Q_GET(get->getId());
            q_get->addToQueue(addQueue);
            std::cout<<"get add to queue: ";
            q_get->print();
            while(queueToAndroid->empty());
            if(VAL *val = dynamic_cast<VAL *>(queueToAndroid->front())){
                std::cout<<"timestamp: "<<val->getTimestamp()<<std::endl;
                queueToAndroid->pop();
                connection->send(val, androidClient->getSesskey());
            }
        }else{
            NAK * nak = new NAK(get->getId());
            connection->send(nak, androidClient->getSesskey());
            std::cout<<"nak sent to android: ";
            nak->print();
            return -1;
        }
    }




    return 0;
}


int setSequence(Connection *connection, AndroidClient *androidClient, SET *set,
                std::queue<Packet *> *queueFromAndroid, std::queue<Packet *> *queueToAndroid,
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