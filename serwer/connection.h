//
// Created by s17kf on 02.06.18.
//

#ifndef LOGOWANIE_CONNECTION_H
#define LOGOWANIE_CONNECTION_H

#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#include "consts.h"
//#include "readWrite.h"
#include "packet.h"

class Connection {

    struct sockaddr_in serverAddr;
    int socketDesc;

    struct sockaddr_in client;
    int clientSocket;

public:

    Connection(int port);
    ~Connection();

    void endClientConnection();
    int connect();
//    int endConnection();

    ssize_t send (Packet *packet, Sesskey * sesskey);
//    Packet* receive(Sesskey *sesskey);
    Packet* receive(Serwer *serwer, unsigned char *ssidValue);


    ssize_t send(uint32_t value);
    ssize_t sendUint8_t(uint8_t value);
    int receiveUint32_t(uint32_t *dest);
    int receiveUint64_t(uint64_t *dest);
    int receiveUint8_t(uint8_t *dest);
    ssize_t  send(unsigned char value);
    int receiveUCharArray(unsigned char * dest);




private:
    int readTillDone(int soc_desc, unsigned char *buf, ssize_t msg_size) {
        ssize_t i = 0;
        ssize_t readed;

        while (i < msg_size) {
            readed = read(soc_desc, &buf[i], (size_t) msg_size - i);
            switch (readed) {
                case -1:
                    //log(3, "Reading from socket exited with: %s.", strerror(errno));
                    std::cout << strerror(errno);
                    return -1;
                case 0:
//                log(3, "Client socket have been closed");
                    std::cout << "readTillDone: " << strerror(errno);
                    return 0;
            }
            i += readed;
        }
        return i;
    }

    ssize_t writeTillDone(int soc_desc, const unsigned char *buf, ssize_t msg_size) {
        ssize_t i = 0;
        ssize_t written;

        while (i < msg_size) {
            written = write(soc_desc, &buf[i], (size_t) msg_size - i);
            switch (written) {
                case -1:
//                log(3, "Writing to socket exited with: %s.", strerror(errno));
                    std::cout << strerror(errno);
                    return -1;
                case 0:
//                log(3, "Client socket have been closed");
                    std::cout << strerror(errno);
                    return 0;
            }
            i += written;
        }
        return i;
    }
};


#endif //LOGOWANIE_CONNECTION_H
