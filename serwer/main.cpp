//
// Created by s17kf on 04.06.18.
//
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <vector>
#include <fstream>

#include "connection.h"
#include "AddQueue.h"
#include "ReadQueue.h"
#include "androidClient.h"
#include "androidSequentions.h"
#include "packet.h"
#include "pubkey.h"
#include "queuePacket.h"
#include "DevDescriptor.h"
#include "serwer.h"





int main(int argc, char **argv){
    std::cout << "server started!" << std::endl;
    if(argc<3){
        std::cout<<"too little arguments, input configfile name and verbosity of logs"<<std::endl;
        exit(1);
    }


    int verbosity = atoi(argv[2]);

    std::string logfile = "serverlogfile.log";
    initLog(logfile, verbosity);

    std::ifstream configfile(argv[1]);
//    std::cout<<"here"<<std::endl;
    std::string privkeyFileName;
    int portNr;
    Serwer *serwer;
    if(configfile.is_open()){
        log(5,"configfile opened");
        try{
            if(!(configfile>>privkeyFileName)){
                log(1, "missing privkey file name in configfile");
                exit(-1);
            }
            if(!(configfile>>portNr)){
                log(1, "missing port nr in configfile");
                exit(-1);
            }
//            std::cout<<portNr;

            serwer = new Serwer(privkeyFileName.c_str(), portNr);


        }catch (...){
            log(1, "unable to start server");
            exit(-1);
        }


    }else{
        log(1, "Unable to open configfile");
        exit(-1);
    }


//    Serwer serwer("/home/s17kf/Pobrane/serwer_key-private.pem", atoi(argv[1]));




    serwer->mainLoop();



    delete serwer;

    return 0;
}
