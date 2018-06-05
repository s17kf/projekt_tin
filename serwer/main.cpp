//
// Created by s17kf on 04.06.18.
//
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <queue>

#include "connection.h"
#include "AddQueue.h"
#include "ReadQueue.h"
#include "androidClient.h"
#include "androidSequentions.h"

std::queue <Packet *> queueToAndroid;
std::queue <Packet *> queueFromAndroid;




void *androidController(void *);
void *serverPart2Controller(void *);


int main(int argc, char **argv){
    std::cout << "Hello, World!" << std::endl;
    if(argc<2){
        std::cout<<"too little arguments, input port Number"<<std::endl;
        exit(1);
    }


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

    Privkey *privkey = new Privkey("/home/s17kf/Pobrane/serwer_key-private.pem");

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
            hex_print(androidClient.getSesskey(), 16);
        }


        std::cout << "android thread" << std::endl;
        sleep(1);



    }
}

void *serverPart2Controller(void *){
    while (true) {

        if(queueFromAndroid.empty()){
            std::cout<<"queue from android is empty"<<std::endl;
        }
        std::cout << "server thread" << std::endl;
        sleep(2);
    }
}
