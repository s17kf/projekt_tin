#include <iostream>
#include <cstring>
#include <fstream>

#include "connection.h"
#include "RNG.h"
#include "privkey.h"
#include "sesskey.h"

int logIn(Connection *connection, Privkey *privkey){
    RNG rng;


    Packet *packet = connection->receive(nullptr);

    if(packet == nullptr){
        std::cout<<"nullptr main.cpp l14, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }

    unsigned char chall_value[8];
    if(CHALL * chall = dynamic_cast<CHALL *>(packet)){
        memcpy(chall_value, chall->getChall(), 8);

        printf("chall received: ");
        for(int i=0;i<8;++i){
//            printf("%x", chall_value);
            std::cout<<std::hex<<chall_value[i];
        }
//        printf("\n");
        std::cout<<std::endl;
    }else{
        std::cout<<"invslid type main.cpp l31, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }

    unsigned char encrypted[256];
    privkey->encrypt(chall_value, 8, encrypted);
    CHALL_RESP *chall_resp = CHALL_RESP::createFromEncrypted(encrypted);
    connection->send(chall_resp, nullptr);
    std::cout<<"response sended"<<std::endl;
    delete chall_resp;

    packet = connection->receive(nullptr);

    if(packet == nullptr){
        std::cout<<"nullptr KEY main.cpp l46, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }
    unsigned char key_encrypted[256];
    unsigned char session_key[16];
    KEY *key;
    if( key = dynamic_cast<KEY *>(packet)){
        memcpy(chall_value, key->getKeyBuf(), 256);

        std::cout<<"key received"<<std::endl;

    }else{
        std::cout<<"invslid type main.cpp l56, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }
    Sesskey *sesskey = new Sesskey(*key,*privkey);

    std::cout<<"session key encrypted"<<std::endl;



    ACK *ack = new ACK((unsigned char)0);
    connection->send(ack, sesskey);

    std::cout<<"ack sent"<<std::endl;
    delete ack;



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

    unsigned char ssid_value;
    rng.generate(&ssid_value, 1);
    SSID * ssid = new SSID(ssid_value);
    connection->send(ssid, sesskey);

    std::cout<<"ssid sent, ssid value: "<<ssid_value<<std::endl;
    delete ssid;


    packet = connection->receive(sesskey);
//    std::cout<<"a tu?"<<std::endl;
    if(packet == nullptr){
        std::cout<<"nullptr main.cpp l71, #ERROR: "<<errno<<" "<<strerror(errno)<<std::endl;
        exit(12);
    }
    //unsigned char response[256];
    if(ack = dynamic_cast<ACK *>(packet)){
        std::cout<<"ack received, ack id: "<<(char)ack->getId()<<std::endl;
    }else{
        std::cout<<"invalid type main.cpp l78, #ERROR: "<<errno<<" "<<strerror(errno)<<std::endl;
        exit(12);
    }




    return 0;

}

int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;
    if(argc<2){
        std::cout<<"too little arguments, input port Number"<<std::endl;
        exit(12);
    }

    Connection *connection = new Connection(atoi(argv[1]));

    Privkey *privkey = new Privkey("/home/s17kf/Pobrane/serwer_key-private.pem");



    if(connection->connect() <0){
        std::cout<<"connecting error, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
        exit(1);
    }
//    sleep(1);

    logIn(connection, privkey);


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