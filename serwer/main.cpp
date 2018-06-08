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
    std::cout << "part2 of server running" << std::endl;
    if(argc<4){
        std::cout<<"too little arguments, input configfile name and verbosity of logs"<<std::endl;
        exit(1);
        //argumenty confname, readqueue, addqueue
    }


    int verbosity = atoi(argv[2]);



    std::ifstream configfile(argv[1]);
//    std::cout<<"here"<<std::endl;
    std::string privkeyFileName;
    int portNr;
    int logV;
    Serwer *serwer;
    if(configfile.is_open()){
        try{
            if(!(configfile>>logV)){
                std::cout<<"missing verbosity of logs in config file";
                exit(-1);
            }
            std::string logfile = "server2logfile.log";
            initLog(logfile, verbosity);

            log(5,"configfile opened");
            if(!(configfile>>privkeyFileName)){
                log(1, "missing privkey file name in configfile");
                exit(-1);
            }
            if(!(configfile>>portNr)){
                log(1, "missing port nr in configfile");
                exit(-1);
            }
//            std::cout<<portNr;
            std::string loginFile;
//            if(!(configfile>>loginFile)){
//                log(1, "missing login file name");
//                exit(-1);
//            }
//            if(!Serwer::testLoginFile(loginFile.c_str())){
//                log(1, "invalid login file");
//                exit(-1);
//            }


            serwer = new Serwer(privkeyFileName.c_str(), portNr, loginFile.c_str(),
                    argv[2], argv[3]);


        }catch (...){
            log(1, "unable to start server, try check config file");
            exit(-1);
        }


    }else{
        std::cout<<"Unable to open configfile";
//        log(1, "Unable to open configfile");
        exit(-1);
    }


//    Serwer serwer("/home/s17kf/Pobrane/serwer_key-private.pem", atoi(argv[1]));




    serwer->mainLoop();



    delete serwer;

    return 0;
}
