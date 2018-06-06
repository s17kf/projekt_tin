//
// Created by s17kf on 04.06.18.
//
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <vector>

#include "connection.h"
#include "AddQueue.h"
#include "ReadQueue.h"
#include "androidClient.h"
#include "androidSequentions.h"
#include "packet.h"
#include "pubkey.h"
#include "queuePacket.h"
#include "DevDescriptor.h"

std::queue <Packet *> queueToAndroid;
std::queue <Packet *> queueFromAndroid;

std::vector <DevDescriptor> descriptors;


AddQueue *addQueue;
ReadQueue *readQueue;




void *androidController(void *);
void *serverPart2Controller(void *);


int main(int argc, char **argv){
    std::cout << "Hello, World!" << std::endl;
    if(argc<2){
        std::cout<<"too little arguments, input port Number"<<std::endl;
        exit(1);
    }
    addQueue = new AddQueue(QUEUE_NAME_TO_GONZO, O_CREAT | O_WRONLY, QUEUE_MODE, MSG_SIZE, MAX_MSGS);
    readQueue = new ReadQueue(QUEUE_NAME_FROM_GONZO, O_CREAT | O_RDONLY, QUEUE_MODE, MSG_SIZE, MAX_MSGS);

    std::string logfile = "/home/s17kf/Dokumenty/serverlogfile.log";
    initLog(logfile, 2);

    pthread_t android_thread_id;
    pthread_t server_thread_id;

    pthread_create(&android_thread_id, nullptr, androidController, (void *)atoi(argv[1]));
    pthread_create(&server_thread_id, nullptr, serverPart2Controller, 0);

    pthread_join(android_thread_id, (void **) nullptr);
    pthread_join(server_thread_id, (void **) nullptr);

















    return 0;
}




void *androidController(void *port_nr){
    Connection *connection = new Connection(*((int*)(&port_nr)));
    AndroidClient androidClient;

    OPENSSL_config(nullptr);
    ERR_load_crypto_strings();

    Privkey *privkey = new Privkey("/home/s17kf/Pobrane/serwer_key-private.pem");
    Pubkey *pubkey = new Pubkey("/home/s17kf/Pobrane/serwer_key-public.pem");

    unsigned char plainn [256]= { 0x00, 0x1, 0x00};//, 0xc4, 0x5b, 0xc7, 0x77, 0x8d};
    unsigned char encrypt[256];
    unsigned char decrypt[256];
    for(int i=0;i<256;++i){
        plainn[i]=0x01;
    }
    hex_print(plainn,256);
    pubkey->encrypt(plainn, 256, encrypt);
    hex_print(encrypt, 256);
    privkey->decrypt(encrypt, 256, decrypt);
    hex_print(decrypt, 256);

    unsigned char encrypted[256];
    std::string plains = "plaintext";
    unsigned char plain[9];
    memcpy(plain, plains.c_str(), 9);
    hex_print(plain, 9);
    unsigned int sign_len = 256;
    privkey->sign(plain,9, encrypted , &sign_len);

    hex_print(encrypted,256);


    while(true) {

        if(connection->connect() <0){
            //TODO: error service
            std::cout<<"connecting error, #Error: "<<errno<<std::endl<<strerror(errno)<<std::endl;
            exit(1);
        }

        Packet *received_packet = connection->receive(&androidClient);
        if(CHALL *chall = dynamic_cast<CHALL *>(received_packet)){
            std::cout<<"CHALL received -> go to log in sequence"<<std::endl;
            if(logInSequence(connection, privkey, &androidClient, chall) < 0){
                //TODO: service wrong login sewuence
                std::cout<<"Some error while log in"<<std::endl;
            }
//            hex_print(androidClient.getSesskey(), 16);
        }else if(EOT *chall = dynamic_cast<EOT *>(received_packet)){
            std::cout<<"EOT received"<<std::endl;
            if(endSessionSequence(connection, &androidClient) < 0){
                //TODO: service wrong ending session sewuence
                std::cout<<"Some error while ending session"<<std::endl;
            }
        }else if(SERVICES *services = dynamic_cast<SERVICES *> (received_packet)){
            std::cout<<"SERVICES received"<<std::endl;
            if(servicesSequence(connection, &androidClient, descriptors) < 0){
                //TODO
                std::cout<<"Some error while descriptors sending"<<std::endl;
            }
        }else if(GET * get = dynamic_cast<GET *>(received_packet)){
            std::cout<<"GET received, get id: ";
            hex_print(get->getId());
            if(getSequence(connection, &androidClient, get, &queueFromAndroid, &queueToAndroid) < 0){
                //TODO
                std::cout<<"Some error during get sequence"<<std::endl;
            }
        }else if(SET * set = dynamic_cast<SET *>(received_packet)){
            std::cout<<"SET received:"<<std::endl;
//            hex_print(set->getBuf(), set->getBufSize());
            set->print();
            if(setSequence(connection, &androidClient, set, &queueFromAndroid, &queueToAndroid) < 0){
                //TODO
                std::cout<<"Some error during set sequence"<<std::endl;
            }
        }


        std::cout << "android thread" << std::endl;
//        sleep(1);



    }
}

void *serverPart2Controller(void *){
    while (true) {

        if(!queueFromAndroid.empty()){

            std::cout<<"queue from android is not empty"<<std::endl;
//            hex_print(queueFromAndroid.front()->getBuf(),2);
            queueFromAndroid.front()->print();
            if(GET *get = dynamic_cast<GET *> (queueFromAndroid.front())){
//                get->setPacketID(PCK_Q_GET);
                Q_GET * q_get = new Q_GET(get->getId());
                q_get->addToQueue(addQueue);
            }else if(SET *set = dynamic_cast<SET *>(queueFromAndroid.front())){
//                set->setPacketID(PCK_Q_SET);
                Q_SET * q_set = new Q_SET(set->getId(), set->getValue());
                q_set->addToQueue(addQueue);
            }else{
                std::cout<<"This packet should not be sent to Gonzo"<<std::endl;
                queueFromAndroid.front()->print();
//                hex_print(queueFromAndroid.front()->getBuf(),queueFromAndroid.front()->getBufSize());
            }
//            addQueue->addMessage(queueFromAndroid.front());
            queueFromAndroid.pop();
        }

        if(readQueue->getMessagesInQueue() > 0){
//            std::cout<<"msg in queue from Gonzo"<<std::endl;
            QueuePacket *packetFromGonzo = QueuePacket::packetFromQueue(readQueue);
//            std::cout<<"msg read"<<std::endl;
            if(Q_DESC *q_desc = dynamic_cast<Q_DESC *>(packetFromGonzo)){
                std::cout<<"Received descriptor from Gonzo"<<std::endl;
                std::cout<<"dev id: ";
                hex_print(q_desc->getDeviceId());
                std::cout<<"dev class: ";
                hex_print(q_desc->getDeviceClass());
                std::cout<<"dev name: "<<q_desc->getName()<<std::endl;
                std::cout<<"dev unit: "<<q_desc->getUnit()<<std::endl;
                std::cout<<"min value: "<<q_desc->getMin()<<std::endl;
                std::cout<<"max vaalue: "<<q_desc->getMax()<<std::endl;

                DevDescriptor devDescriptor(q_desc);
                descriptors.push_back(devDescriptor);

                std::cout<<"all descriptors:"<<std::endl;
                for(DevDescriptor descriptor : descriptors){
                    std::cout<<"id: ";
                    hex_print_noendl(descriptor.getId());
                    std::cout<<"class: ";
                    hex_print_noendl(descriptor.getClass());
                    std::cout<<descriptor.getName()<<" "<<descriptor.getUnit()<<" ";
                    std::cout<<descriptor.getMin()<<" "<<descriptor.getMax();
                }

            }else if(Q_VAL *q_val = dynamic_cast<Q_VAL *>(packetFromGonzo)){
                std::cout<<"received value from gonzo"<<std::endl;
                std::cout<<"dev id: ";
                hex_print(q_val->getServiceId());
                std::cout<<"value: "<<q_val->getValue()<<std::endl;
                std::cout<<"time stamp: "<<q_val->getTimestamp()<<std::endl;
//                queueToAndroid.push(new VAL());
            }else if(Q_EXIT *q_exit = dynamic_cast<Q_EXIT *>(packetFromGonzo)){
                std::cout<<"received exit from Gonzo, exit id: ";
                hex_print(q_exit->getId());
            }

//            Packet *packetFromGonzo = readQueue->readToPacket();
//            DESC *desc;
//            if(desc = dynamic_cast<DESC *>(packetFromGonzo)){
//                DESC *desc_to_vector = new DESC(desc->getBuf(), desc->getBufSize());
//                descriptors.push_back(desc_to_vector);
//                std::cout<<"new device added"<<std::endl;
//
//                std::cout<<"registered devices:"<<std::endl;
//                for(DESC *desc: descriptors){
//                    hex_print(desc->getBuf(),desc->getBufSize());
//                }
//            }else if(VAL *val = dynamic_cast<VAL *>(packetFromGonzo)){
//                VAL *val_to_queue = new VAL(val->getBuf());
//                queueToAndroid.push(val_to_queue);
//                std::cout<<"val received from Gonzo and added to queue"<<std::endl;
//            }else if(EOT *eot = dynamic_cast<EOT *>(packetFromGonzo)){
//                EOT *eot_to_queue = new EOT(eot->getBuf());
//                queueToAndroid.push(eot_to_queue);
//                std::cout<<"eot received from Gonzo and added to queue"<<std::endl;
//            }else if(EXIT *exit = dynamic_cast<EXIT *>(packetFromGonzo)){
//                EXIT *exit_to_queue = new EXIT(exit->getBuf());
//                queueToAndroid.push(exit_to_queue);
//                std::cout<<"exit received from Gonzo and added to queue"<<std::endl;
//            }
//            else{
//                std::cout<<"BAD packet received from Gonzo"<<std::endl;
//            }

        }

//        std::cout << "server thread" << std::endl;
//        sleep(1);
    }
}
