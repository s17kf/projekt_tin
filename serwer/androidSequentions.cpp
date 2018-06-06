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

int servicesSequence(Connection *connection, AndroidClient *androidClient, std::vector<DevDescriptor> descriptors){

//    for( DESC * desc : descriptors){
    for(DevDescriptor descriptor : descriptors){
        DESC * desc = new DESC(descriptor.getId(), descriptor.getClass(), descriptor.getName(), descriptor.getUnit(), descriptor.getMin(), descriptor.getMax());
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
                std::vector<DevDescriptor> *devices){
    if(get->getId()==0){
        GET *get_to_queue = new GET(get->getId());
//        hex_print(get->getBuf(),2);

//        hex_print(queueFromAndroid->front()->getBuf(), 2);


        for(auto dev : *devices){
            get->print();
            GET *get_i = new GET(dev.getId());
            queueFromAndroid->push(get_i);
            while(queueToAndroid->empty());
            if(VAL *val = dynamic_cast<VAL *>(queueToAndroid->front())){
                std::cout<<"timestamp: "<<val->getTimestamp()<<std::endl;
                queueToAndroid->pop();
                connection->send(val, androidClient->getSesskey());
            }else{
                std::cout<<"bad packet received from Gonzo, expected VAL"<<std::endl;
                queueToAndroid->pop();
                return -1;
            }
        }

        EOT *eot = new EOT();
        connection->send(eot, androidClient->getSesskey());
        std::cout<<"eot after all values sent"<<std::endl;



    } else{
//        hex_print(get->getBuf(),2);
        get->print();
        queueFromAndroid->push(get);
        while(queueToAndroid->empty());
        if(VAL *val = dynamic_cast<VAL *>(queueToAndroid->front())){
            std::cout<<"timestamp: "<<val->getTimestamp()<<std::endl;
            queueToAndroid->pop();
            connection->send(val, androidClient->getSesskey());
        }else{
            std::cout<<"bad packet received from Gonzo, expected VAL"<<std::endl;
            queueToAndroid->pop();
            return -1;
        }
    }




    return 0;
}


int setSequence(Connection *connection, AndroidClient *androidClient, SET *set,
std::queue<Packet *> *queueFromAndroid, std::queue<Packet *> *queueToAndroid,
std::vector<DevDescriptor> *devices){
    std::cout<<"SET id: "<<set->getId()<<"; set value"<<set->getValue()<<std::endl;

    /*checking if id is correct*/
    bool id_correct = false;
    for(auto dev : *devices){
        if(dev.getId() == set->getId()){
            id_correct = true;
            break;
        }
    }
    //TODO: teraz jak id poprawne wyslij set do serwera i ack do androida lub nak do androida


    queueFromAndroid->push(set);
    while(queueToAndroid->empty());
    if(ACK *ack = dynamic_cast<ACK *>(queueToAndroid->front())){
        queueToAndroid->pop();
        connection->send(ack,androidClient->getSesskey());
        std::cout<<"ack sent"<<std::endl;
    }else if(NAK * nak = dynamic_cast<NAK *>(queueToAndroid->front())){
        queueToAndroid->pop();
        connection->send(nak,androidClient->getSesskey());
        std::cout<<"nak sent"<<std::endl;
    }else {
        std::cout << "bad packet received from Gonzo, expected VAL" << std::endl;
        queueToAndroid->pop();
        return -1;
    }




    return 0;
}