//
// Created by s17kf on 06.06.18.
//

#include "serwer.h"

Serwer::Serwer(const char *privkeyFile, int port):privkey(privkeyFile),
  addQueue(QUEUE_NAME_TO_GONZO, O_CREAT | O_WRONLY, QUEUE_MODE, MSG_SIZE, MAX_MSGS),
  readQueue(QUEUE_NAME_FROM_GONZO, O_CREAT | O_RDONLY, QUEUE_MODE, MSG_SIZE, MAX_MSGS),
  portNr(port)
{




}

Serwer::~Serwer() {

//    mqSender.join();
    mqReceiver.join();
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

        Packet *received_packet = connection->receive(&androidClient);
        if(CHALL *chall = dynamic_cast<CHALL *>(received_packet)){
            std::cout<<"CHALL received -> go to log in sequence"<<std::endl;
            if(logInSequence(connection, &privkey, &androidClient, chall) < 0){
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
            addQueue.close();
        }else if(SERVICES *services = dynamic_cast<SERVICES *> (received_packet)){
            std::cout<<"SERVICES received"<<std::endl;
            if(servicesSequence(connection, &androidClient, &devices) < 0){
                //TODO
                std::cout<<"Some error while descriptors sending"<<std::endl;
            }
        }else if(GET * get = dynamic_cast<GET *>(received_packet)){
            std::cout<<"GET received, get id: ";
            hex_print(get->getId());
            get->print();
            int ret = getSequence(connection, &androidClient, get, &queueFromAndroid,
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
            int ret = setSequence(connection, &androidClient, set, &queueFromAndroid,
                    &queueToAndroid, &devices, &addQueue);
            if( ret < 0){
                if(ret==-1){
                    log(2, "received packet set of invalid device id: %d", set->getId());
                }else {
                    //TODO
                    std::cout << "Some error during set sequence" << std::endl;
                }
            }
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