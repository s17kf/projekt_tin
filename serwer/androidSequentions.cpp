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
        hex_print(sesskey->getKeyBuf(),16);
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
    








    return 0;
}
