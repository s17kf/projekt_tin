#include <iostream>
#include <cstring>
#include <fstream>

#include "connection.h"
#include "RNG.h"
#include "privkey.h"
#include "sesskey.h"
#include "androidClient.h"
#include "log.h"

int logIn(Connection *connection, Privkey *privkey,Sesskey *sesskey, unsigned char *ssid_val){
    RNG rng;

    /*receiving CHALL */
    Packet *packet = connection->receive((Sesskey *)nullptr);
    if(packet == nullptr){
        std::cout<<"nullptr main.cpp l14, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }
    unsigned char chall_value[8];
    if(CHALL * chall = dynamic_cast<CHALL *>(packet)){
        memcpy(chall_value, chall->getChall(), 8);

        printf("chall received: ");
        for(int i=0;i<8;++i){
            std::cout<<std::hex<<chall_value[i];
        }
        std::cout<<std::endl;
    }else{
        std::cout<<"invslid type main.cpp l31, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }

    /*encrypting CHALL and sending CHALL_RESP */
    unsigned char encrypted[256];
    privkey->encrypt(chall_value, 8, encrypted);
    CHALL_RESP *chall_resp = CHALL_RESP::createFromEncrypted(encrypted);
    connection->send(chall_resp, nullptr);
    std::cout<<"response sended"<<std::endl;
    delete chall_resp;

    /*receiving KEY */
    packet = connection->receive((Sesskey *)nullptr);
    if(packet == nullptr){
        std::cout<<"nullptr KEY main.cpp l46, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }
    unsigned char key_encrypted[256];
//    unsigned char session_key[16];
    KEY *key;
    if( key = dynamic_cast<KEY *>(packet)){
        memcpy(key_encrypted, key->getKeyBuf(), 256);

        std::cout<<"key received"<<std::endl;

    }else{
        std::cout<<"invslid type main.cpp l56, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }
    Sesskey *new_sesskey = new Sesskey(*key,*privkey);
    //sesskey = new Sesskey( *key, *privkey);
    sesskey->setKey(new_sesskey->getKeyBuf());

    std::cout<<"session key encrypted"<<std::endl;


    /*SSID generating and sending */
    unsigned char ssid_value;
    rng.generate(&ssid_value, 1);
    SSID * ssid = new SSID(ssid_value);
    connection->send(ssid, sesskey);


    memcpy(ssid_val, &ssid_value, 1);

    std::cout<<"ssid sent, ssid value: "<<ssid_value<<std::endl;



    /*LOG (login and password) receiving */
    packet = connection->receive(sesskey);
    if(packet == nullptr){
        std::cout<<"nullptr login main.cpp l46, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }
    unsigned char login[31];
    unsigned char password[31];
    LOG *log;
    if( log = dynamic_cast<LOG *>(packet)){
        memcpy(login, log->getLogin(), 31);
        memcpy(password, log->getPassword(), 31);
        std::cout<<"login received"<<std::endl;
        std::cout<<login<<std::endl;
        std::cout<<password<<std::endl;

    }else{
        std::cout<<"invslid login main.cpp l56, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }


    //TODO: check if user(login, password) exist;


    /*sending ACK after sesskey received */
    unsigned char ack_id = 0x00;
    ACK *ack = new ACK(ack_id);
    connection->send(ack, sesskey);
    std::cout<<"ack sent"<<std::endl;
    delete ack;







    hex_print(sesskey->getKeyBuf(),16);








    delete ssid;
    delete key;
    delete log;


    return 0;

}

int logIn(Connection *connection, Privkey *privkey, AndroidClient *androidClient){
    RNG rng;

    /*receiving CHALL */
    Packet *packet = connection->receive((Sesskey *)nullptr);
    if(packet == nullptr){
        std::cout<<"nullptr main.cpp l14, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }
    unsigned char chall_value[8];
    if(CHALL * chall = dynamic_cast<CHALL *>(packet)){
        memcpy(chall_value, chall->getChall(), 8);

        printf("chall received: ");
        for(int i=0;i<8;++i){
            std::cout<<std::hex<<chall_value[i];
        }
        std::cout<<std::endl;
    }else{
        std::cout<<"invslid type main.cpp l31, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }

    /*encrypting CHALL and sending CHALL_RESP */
    unsigned char encrypted[256];
    privkey->encrypt(chall_value, 8, encrypted);
    CHALL_RESP *chall_resp = CHALL_RESP::createFromEncrypted(encrypted);
    connection->send(chall_resp, nullptr);
    std::cout<<"response sended"<<std::endl;
    delete chall_resp;

    /*receiving KEY */
    packet = connection->receive((Sesskey *)nullptr);
    if(packet == nullptr){
        std::cout<<"nullptr KEY main.cpp l46, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }
    unsigned char key_encrypted[256];
//    unsigned char session_key[16];
    KEY *key;
    if( key = dynamic_cast<KEY *>(packet)){
        memcpy(key_encrypted, key->getKeyBuf(), 256);

        std::cout<<"key received"<<std::endl;

    }else{
        std::cout<<"invslid type main.cpp l56, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }
    Sesskey *new_sesskey = new Sesskey(*key,*privkey);
    //sesskey = new Sesskey( *key, *privkey);
//    sesskey->setKey(new_sesskey->getKeyBuf());
    androidClient->setSesskey(*new_sesskey);

    std::cout<<"session key encrypted"<<std::endl;


    /*SSID generating and sending */
    unsigned char ssid_value;
    rng.generate(&ssid_value, 1);
    SSID * ssid = new SSID(ssid_value);
//    connection->send(ssid, sesskey);
    connection->send(ssid, androidClient->getSesskey());

//    memcpy(ssid_val, &ssid_value, 1);
    androidClient->setSssid(ssid_value);

    std::cout<<"ssid sent, ssid value: "<<ssid_value<<std::endl;



    /*LOG (login and password) receiving */
//    packet = connection->receive(sesskey);
    packet = connection->receive(androidClient);
    if(packet == nullptr){
        std::cout<<"nullptr login main.cpp l46, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }
    unsigned char login[31];
    unsigned char password[31];
    LOG *log;
    if( log = dynamic_cast<LOG *>(packet)){
        memcpy(login, log->getLogin(), 31);
        memcpy(password, log->getPassword(), 31);
        std::cout<<"login received"<<std::endl;
        std::cout<<login<<std::endl;
        std::cout<<password<<std::endl;

    }else{
        std::cout<<"invslid login main.cpp l56, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }


    //TODO: check if user(login, password) exist;


    /*sending ACK after sesskey received */
    unsigned char ack_id = 0x0A;
    ACK *ack = new ACK(ack_id);
//    connection->send(ack, sesskey);
    connection->send(ack, androidClient->getSesskey());
    std::cout<<"ack sent"<<std::endl;
    delete ack;







    hex_print(new_sesskey->getKeyBuf(),16);








    delete ssid;
    delete key;
    delete log;


    return 0;

}

int choseSequence(Connection *connection, Privkey *privkey, AndroidClient *androidClient){
    Packet *packet;

//    std::cout<<"ssid value: "<<androidClient->getSsidValue()<<std::endl;


//    std::cout<<"tu jestem31"<<std::endl;
    packet = connection->receive(androidClient);
//    std::cout<<"tu jestem32"<<std::endl;
    if(packet == nullptr){
        std::cout<<"nullptr main.cpp l136, #ERROR: "<<errno<<" "<<strerror(errno)<<std::endl;
        exit(12);
    }

    if(EOT *eot = dynamic_cast<EOT *>(packet)){
        std::cout<<"eot received "<<std::endl;
    }else{
        std::cout<<"invalid type main.cpp l143, #ERROR: "<<errno<<" "<<strerror(errno)<<std::endl;
        exit(12);
    }


    unsigned char ack_id = 0x03;
    ACK *ack = new ACK(ack_id);
//    EncryptedPacketWithSSID *packetToSend;
//    packetToSend = new EncryptedPacketWithSSID(androidClient->getSsidValue(),ack);
    connection->send(ack, androidClient->getSesskey());

    std::cout<<"ack sent"<<std::endl;
    delete ack;



//    delete packetToSend;

    return 0;
}

int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;
    if(argc<2){
        std::cout<<"too little arguments, input port Number"<<std::endl;
        exit(12);
    }

    std::string logfile = "/home/s17kf/Dokumenty/logfileServer.log";
    initLog(logfile, 2);


    Connection *connection = new Connection(atoi(argv[1]));

    Privkey *privkey = new Privkey("/home/s17kf/Pobrane/serwer_key-private.pem");
    Sesskey sesskey = Sesskey();

    AndroidClient androidClient;
    unsigned char ssid_value;
    unsigned char sesskey_encrypted[256];


    if(connection->connect() <0){
        std::cout<<"connecting error, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }
//    sleep(1);

//    logIn(connection, privkey, &sesskey, &ssid_value);
//    androidClient.setSesskey(sesskey);
//    androidClient.setSssid(ssid_value);

    logIn(connection, privkey, &androidClient);

//    std::cout<<"logged"<<std::endl;
//
//    std::cout<<ssid_value<<std::endl<<sesskey_encrypted<<std::endl;
//
//    std::cout<<"logged1"<<std::endl;

//    androidClient = new AndroidClient(sesskey_encrypted, privkey, ssid_value);
   // androidClient = new AndroidClient(sesskey, privkey, ssid_value);



    std::cout<<"logged2"<<std::endl;

    connection->endClientConnection();

    std::cout<<androidClient.getSesskey()->getKeyBuf()<<std::endl;
    hex_print(androidClient.getSesskey()->getKeyBuf(), 16);

    if(connection->connect() <0){
        std::cout<<"connecting error, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }

//    sleep(1);
    choseSequence(connection, privkey, &androidClient);





    delete connection;


//    unsigned char plain_text[10] = "plaintext";
//   // strcpy(static_cast<char *> (plain_text), "plaintext");
//    Privkey *privkey = new Privkey("/home/s17kf/Pobrane/serwer_key-private.pem");
//    unsigned char encrypted[256];
//    privkey->encrypt(plain_text, 9, encrypted);
//
//    std::fstream file;
//    file.open("/home/s17kf/Dokumenty/plik.out", std::ios::out);
//    for(int i=0;i<256;i++){
//        file<<encrypted[i];
//    }
//    file<<std::endl;




    return 0;
}




