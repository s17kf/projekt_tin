//
// Created by s17kf on 05.06.18.
//

#include "androidSequentions.h"

int logInSequence(Connection *connection, Privkey *privkey, AndroidClient *androidClient, CHALL *chall ){
    Packet *received_packet;
    RNG rng;

    unsigned char chall_value[8];
    memcpy(chall_value, chall->getChall(), 8);

    /* encrypting chall and sendint responce */
    unsigned char encrypted[256];
    privkey->encrypt(chall_value, 8, encrypted);
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

        Sesskey *sesskey = new Sesskey(*key, *privkey);
//        hex_print(sesskey->getKeyBuf(),16);
        androidClient->setSesskey(*sesskey);
    }else{
        std::cout<<"wrong packet received, expected KEY"<<std::endl;
        return -1;
    }

    /*SSID generating and sending*/
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

int servicesSequence(Connection *connection, AndroidClient *androidClient, std::vector<DESC *> descriptors){

    for( DESC * desc : descriptors){
        connection->send(desc, androidClient->getSesskey());
        //TODO: check if no errors during sending
    }
    EOT * eot = new EOT();
    connection->send(eot, androidClient->getSesskey());
    //TODO: check if no errors during sendig

    return 0;
}

int getSequence(Connection *connection, AndroidClient *androidClient, GET *get, \
std::queue<Packet *> *queueFromAndroid, std::queue<Packet *> *queueToAndroid){
    if(get->getId()==0){
        GET *get_to_queue = new GET(get->getBuf());
        queueFromAndroid->push(get);

        while(true) {
            while (queueToAndroid->empty());
            if (VAL *val = dynamic_cast<VAL *>(queueToAndroid->front())) {
                queueToAndroid->pop();
                connection->send(val, androidClient->getSesskey());
            }else if (EOT *eot = dynamic_cast<EOT *>(queueToAndroid->front())) {
                queueToAndroid->pop();
                connection->send(eot, androidClient->getSesskey());
                break;
            }else{
                std::cout<<"bad packet received, expected VAL or EOT"<<std::endl;
                queueToAndroid->pop();
                return -1;
            }


        }


    } else{
        queueFromAndroid->push(get);
        while(queueToAndroid->empty());
        if(VAL *val = dynamic_cast<VAL *>(queueToAndroid->front())){
            queueToAndroid->pop();
            connection->send(val, androidClient->getSesskey());
        }else{
            std::cout<<"bad packet received, expected VAL"<<std::endl;
            queueToAndroid->pop();
            return -1;
        }
    }




    return 0;
}