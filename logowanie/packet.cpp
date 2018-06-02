//
// Created by s17kf on 02.06.18.
//

#include <cstring>
#include <unistd.h>
#include <iostream>
#include "packet.h"
#include "consts.h"

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

Packet::Packet(const unsigned char *buf_in, uint32_t buf_len): buf_size(buf_len){
    buf = new unsigned char[buf_len];
    memcpy(buf, buf_in, buf_len);
}

Packet::Packet(size_t size): buf_size(size){
    buf = new unsigned char[size];
}

Packet::~Packet() {
    delete[] buf;
}

Packet* Packet::packetFactory(int soc_desc, const Sesskey *sesskey) {
    Packet *new_packet;
    int expected_size;
    unsigned char *encrypted;
    uint32_t plain_len;
    unsigned char is_crypted;
    unsigned char *new_buf;

    if(readTillDone(soc_desc, (unsigned char *) &plain_len, sizeof(plain_len)) < 1)
        return nullptr;
    if(readTillDone(soc_desc, &is_crypted, 1) < 1)
        return nullptr;

    if(is_crypted){
        if(sesskey == nullptr){
            printf("Received encrypted message before setting session key.\n");
            exit(23);
        }
        printf("Received encrypted packet, plaintext length = %d\n", plain_len);
        return nullptr;
    }else{
        expected_size = plain_len;
    }

    new_buf = new unsigned char [plain_len + 16];
    encrypted = new unsigned char [expected_size];

    if(readTillDone(soc_desc, encrypted, expected_size) < 1)
        return nullptr;

    if(is_crypted) {
        //TODO
        std::cout<<"Crypted msg received"<<std::endl;
    } else{
        //TODO: check if packed should be encrypted
        memcpy(new_buf, encrypted, expected_size);
    }

    delete[] encrypted;

    switch(new_buf[0]){
        case PCK_CHALL:


    }

}


ssize_t PlainPacket::send(int sock_desc, const Sesskey *sesskey) const {
    ssize_t ret;
    unsigned char boolean = (unsigned char) false;
    ret = writeTillDone(sock_desc, (unsigned char *) &buf_size, sizeof(buf_size));
    if(ret<=0)
        return ret;
    ret = writeTillDone(sock_desc, (unsigned char*) &boolean, sizeof(unsigned char));
    if(ret <=0)
        return ret;

    ret = writeTillDone(sock_desc, buf, buf_size);
    return ret;
}

const unsigned char* CHALL::getChall() const {
    return &buf[1];
}

CHALL* CHALL::createFromMessage(const unsigned char *msg) {
    return new CHALL(msg);
}

CHALL* CHALL::createFromRandom(const unsigned char *rand) {
    unsigned char buf[9];
    buf[0] = PCK_CHALL;
    memcpy(&buf[1], rand, 8);
    return new CHALL(buf);
}


























