//
// Created by s17kf on 02.06.18.
//

#include <cstring>
#include <unistd.h>
#include <iostream>
#include "packet.h"
#include "consts.h"
#include "log.h"

int readTillDone(int soc_desc, unsigned char *buf, ssize_t msg_size) {
    ssize_t i = 0;
    ssize_t readed;

    while (i < msg_size) {
        readed = read(soc_desc, &buf[i], (size_t) msg_size - i);
        switch (readed) {
            case -1:
                //log(3, "Reading from socket exited with: %s.", strerror(errno));
                std::cout <<"Reading error (packet.cpp, l20) 273"<< strerror(errno);
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

inline int encryptedLen(int plain_len) {
    return (plain_len + (16 - plain_len % 16) + 16);// message_size + padding + iv
}

Packet* Packet::packetFactory(int soc_desc, const Sesskey *sesskey) {
    Packet *new_packet;
    int expected_size;
    unsigned char *encrypted;
    uint32_t plain_len;
    unsigned char is_crypted;
    unsigned char *new_buf;
//    std::cout<<"packet.cpp l 73"<<std::endl;
    if(readTillDone(soc_desc, (unsigned char *) &plain_len, sizeof(plain_len)) < 1)
        return nullptr;
//    std::cout<<"len readed (packet.cpp 76)"<<std::endl;
    if(readTillDone(soc_desc, &is_crypted, 1) < 1)
        return nullptr;

    if(is_crypted){
        if(sesskey == nullptr) {
            printf("Received encrypted message before setting session key.\n");
            exit(23);
        }
        expected_size = encryptedLen(plain_len);
    }else{
        expected_size = plain_len;
    }

    new_buf = new unsigned char [plain_len + 16];
    encrypted = new unsigned char [expected_size];

    if(readTillDone(soc_desc, encrypted, expected_size) < 1)
        return nullptr;

    if(is_crypted) {
        sesskey->decrypt(new_buf, encrypted, expected_size);
        log(4, "Received encrypted packet, plaintext length = %d, encrpypted length = %d.",
            plain_len, encryptedLen(plain_len));
    } else{
        //TODO: check if packed should be encrypted
        memcpy(new_buf, encrypted, expected_size);
    }


    delete[] encrypted;

    switch(new_buf[0]){
        case PCK_ACK:
            new_packet = new ACK(new_buf);
            break;
        case PCK_NAK:
            new_packet = new NAK(new_buf);
            break;
        case PCK_CHALL:
            new_packet= CHALL::createFromMessage(new_buf);
            break;
        case PCK_CHALL_RESP:
            new_packet = CHALL_RESP::createFromMessage(new_buf);
            break;
        case PCK_KEY:
            new_packet = KEY::createFromMessage(new_buf);
            break;
        case PCK_LOG:
            new_packet = new LOG(new_buf);
            break;
        case PCK_SSID:
            new_packet = new SSID(new_buf);
            break;
        default:
            std::cout<<"wrong packet received :("<<std::endl;


    }

    delete[] new_buf;
    return new_packet;

}

ssize_t EncrptedPacket::send(int soc_desc, const Sesskey *sesskey) const {
    unsigned char *encrypted;
    ssize_t ret;
    unsigned char boolean = (unsigned char) true;
    int32_t encrypted_size = encryptedLen(buf_size);
    encrypted = new unsigned char[encrypted_size];
    sesskey->encrypt(encrypted, buf, buf_size);
    ret = writeTillDone(soc_desc, (unsigned char*) &buf_size, sizeof(buf_size));
    if (ret <= 0)
        return ret;
    ret = writeTillDone(soc_desc, (unsigned char*) &boolean, sizeof(unsigned char));
    if (ret <= 0)
        return ret;
    ret = writeTillDone(soc_desc, encrypted, encrypted_size);
    delete[] encrypted;
    return ret;
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

ACK::ACK(unsigned char id): EncrptedPacket(2) {
    buf[0] = PCK_ACK;
    buf[1] = id;
}
unsigned char ACK::getId() const {
    return buf[1];
}

NAK::NAK(unsigned char id): EncrptedPacket(2) {
    buf[0] = PCK_NAK;
    buf[1] = id;
}
unsigned char NAK::getId() const {
    return buf[1];
}

SSID::SSID(unsigned char value) : EncrptedPacket(2){
    buf[0] = PCK_SSID;
    buf[1] = value;
}

unsigned char SSID::getValue() const {
    return buf[1];
}


LOG::LOG(unsigned char *login, unsigned char *password): EncrptedPacket(63) {
    buf[0] = PCK_LOG;
    memcpy(&buf[1], login, 31);
    memcpy(&buf[32], password, 31);
}

unsigned char* LOG::getLogin() const {
    return &buf[1];
}
unsigned char* LOG::getPassword() const {
    return &buf[32];
}


CHALL* CHALL::createFromMessage(const unsigned char *msg) {
    return new CHALL(msg);
}

const unsigned char* CHALL::getChall() const {
    return &buf[1];
}
CHALL* CHALL::createFromRandom(const unsigned char *rand) {
    unsigned char buf[9];
    buf[0] = PCK_CHALL;
    memcpy(&buf[1], rand, 8);
    return new CHALL(buf);
}


const unsigned char* CHALL_RESP::getResp() const {
    return &buf[1];
}

CHALL_RESP* CHALL_RESP::createFromMessage(const unsigned char *msg) {
    return new CHALL_RESP(msg);
}
CHALL_RESP* CHALL_RESP::createFromEncrypted(const unsigned char *encrypt) {
    unsigned char buf[257];
    buf[0] = PCK_CHALL_RESP;
    memcpy(&buf[1], encrypt, 256);
    return new CHALL_RESP(buf);
}


KEY* KEY::createFromMessage(const unsigned char *msg) {
    return new KEY(msg);
}
KEY* KEY::createFromEncrypted(const unsigned char *encrypt) {
    unsigned char buf[257];
    buf[0] = PCK_KEY;
    memcpy(&buf[1], encrypt, 256);
    return new KEY(buf);
}
const unsigned char* KEY::getKeyBuf() const {
    return &buf[1];
}




















