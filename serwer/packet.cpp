//
// Created by s17kf on 02.06.18.
//

#include <cstring>
#include <unistd.h>
#include <iostream>
#include "packet.h"
#include "consts.h"
#include "log.h"
#include "androidClient.h"
#include "serwer.h"


int readTillDone(int soc_desc, unsigned char *buf, ssize_t msg_size) {
    ssize_t i = 0;
    ssize_t readed;

    while (i < msg_size) {
        readed = read(soc_desc, &buf[i], (size_t) msg_size - i);
        switch (readed) {
            case -1:
                log(3, "Reading from socket exited with: %s.", strerror(errno));
//                std::cout <<"Reading error (packet.cpp, l20) 273"<< strerror(errno);
                return -1;
            case 0:
                log(3, "Client socket have been closed");
//                std::cout << "readTillDone: " << strerror(errno);
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
                log(3, "Writing to socket exited with: %s.", strerror(errno));
                std::cout << strerror(errno)<<std::endl;
                return -1;
            case 0:
                log(3, "Client socket have been closed");
                std::cout << strerror(errno)<<std::endl;
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



Packet* Packet::packetFactory(int soc_desc, Serwer *serwer, unsigned char *ssidValue) {
    Packet *new_packet;
    int expected_size;
    unsigned char *encrypted;
    uint32_t plain_len;
    unsigned char is_crypted;
    unsigned char *new_buf;
    log(5, "receiving packet ...");

    if(readTillDone(soc_desc, (unsigned char *) &plain_len, sizeof(plain_len)) < 1)
        return nullptr;
    if(readTillDone(soc_desc, &is_crypted, 1) < 1)
        return nullptr;
//    std::cout<<"tu jestem"<<std::endl;
    Sesskey * sesskey;
    if(is_crypted){
        log(5, "received encrypted packet header ...");
        unsigned char ssid_buf[2];
        if(readTillDone(soc_desc, ssid_buf, 2) < 1)
            return nullptr;
        SSID *ssid = new SSID(ssid_buf);
        *ssidValue = ssid->getValue();
        sesskey = serwer->getSesskey(ssid->getValue());
        if(sesskey == nullptr){
            log(1, "Received encrypted msg with invalid ssid");
            return nullptr;
        }

        expected_size = encryptedLen(plain_len-2);
    }else{
        log(4, "received header of non encrypted packet ...");
        expected_size = plain_len;
    }


    new_buf = new unsigned char [plain_len + 16];
    encrypted = new unsigned char [expected_size];
//    std::cout<<"tu jestem1"<<std::endl;
    if(readTillDone(soc_desc, encrypted, expected_size) < 1)
        return nullptr;
//    std::cout<<"tu jestem2"<<std::endl;

    if(is_crypted) {
//        std::cout<<"tu jestem21"<<std::endl;
        sesskey->decrypt(new_buf, encrypted, expected_size);
        log(4, "Received encrypted packet, plaintext length = %d, encrpypted length = %d.",
            plain_len, encryptedLen(plain_len));
    } else{
        switch (encrypted[0]){
            case (PCK_CHALL):
                new_packet = CHALL::createFromMessage(encrypted);
                break;
            case (PCK_KEY):
                new_packet = KEY::createFromMessage(encrypted);
                break;
            default:
                log(1, "Received not encrypted packet which should be encrypted, packet id: %d", encrypted[0]);
        }
        delete[] new_buf;
        delete[] encrypted;

        return new_packet;
    }

//    std::cout<<"tu jestem3"<<std::endl;

    delete[] encrypted;

    switch(new_buf[0]){
        case PCK_ACK:
            new_packet = new ACK(new_buf);
            break;
        case PCK_NAK:
            new_packet = new NAK(new_buf);
            break;
//        case PCK_CHALL:
//            new_packet= CHALL::createFromMessage(new_buf);
//            break;
        case PCK_CHALL_RESP:
            new_packet = CHALL_RESP::createFromMessage(new_buf);
            break;
//        case PCK_KEY:
//            new_packet = KEY::createFromMessage(new_buf);
//            break;
        case PCK_LOG:
            new_packet = new LOG(new_buf);
            break;
        case PCK_EOT:
            new_packet = new EOT(new_buf);
            break;
        case PCK_DESC:
            new_packet = new DESC(new_buf, plain_len);
            break;
        case PCK_VAL:
            new_packet = new VAL(new_buf);
            break;
        case PCK_GET:
            new_packet = new GET(new_buf);
            break;
        case PCK_SET:
            new_packet = new SET(new_buf);
            break;
        case PCK_SERVICES:
            new_packet = new SERVICES(new_buf);
            break;
//        case PCK_SSID:
//            std::cout<<"ssid as encrypted part of EncryptedWithSsid or other error";
        default:
            log(1, "received encrypted packet which should be not, packet id: %d", new_buf[0]);
//            std::cout<<"wrong packet received :("<<std::endl;


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
    log(5, "Packet ready to encrypt plain %d, encrypted %d, packet id: %d", buf_size, encrypted_size, buf[0]);
//    hex_print(buf, buf_size);
    sesskey->encrypt(encrypted, buf, buf_size);
    unsigned char msg[encrypted_size+5];
    memcpy(msg, &buf_size , sizeof(uint32_t));
    memcpy(&msg[4], &boolean, 1);
    memcpy(&msg[5], encrypted, encrypted_size);
//    ret = writeTillDone(soc_desc, (unsigned char*) &buf_size, sizeof(buf_size));
//    if (ret <= 0)
//        return ret;
//    ret = writeTillDone(soc_desc, (unsigned char*) &boolean, sizeof(unsigned char));
//    if (ret <= 0)
//        return ret;
//    ret = writeTillDone(soc_desc, encrypted, encrypted_size);
    delete[] encrypted;
    log(5, "Packet encrypted before sending");
    ret = writeTillDone(soc_desc, msg, encrypted_size+5);
    log(4, "sent %d bytes", encrypted_size+5);
//    std::cout<<"sent "<<encrypted_size+5<<"bytes"<<std::endl;
    return ret;
}

ssize_t EncryptedPacketWithSSID::send(int soc_desc, const Sesskey *sesskey) const {
    unsigned char *encrypted;
    ssize_t ret;
    unsigned char boolean = (unsigned char) true;
    int32_t encrypted_size = encryptedLen(buf_size-2);
    encrypted = new unsigned char[encrypted_size];
    sesskey->encrypt(encrypted, &buf[2], buf_size-2);
    ret = writeTillDone(soc_desc, (unsigned char *) &buf_size, sizeof(buf_size));
    if(ret<=0)
        return ret;
    ret = writeTillDone(soc_desc, &boolean, sizeof(unsigned char));
    if(ret <= 0)
        return ret;
    ret = writeTillDone(soc_desc, buf, sizeof(unsigned char) * 2);
    if(ret<=0)
        return ret;
    ret = writeTillDone(soc_desc, encrypted, encrypted_size);
    delete[] encrypted;
    return ret;
}


ssize_t PlainPacket::send(int sock_desc, const Sesskey *sesskey) const {
    ssize_t ret;
    unsigned char boolean = (unsigned char) false;
    unsigned char msg[buf_size+5];
    memcpy(msg, &buf_size , sizeof(uint32_t));
    memcpy(&msg[4], &boolean, 1);
    memcpy(&msg[5], buf, buf_size);

//    ret = writeTillDone(sock_desc, (unsigned char *) &buf_size, sizeof(buf_size));
//    if(ret<=0)
//        return ret;
//    ret = writeTillDone(sock_desc, (unsigned char*) &boolean, sizeof(unsigned char));
//    if(ret <=0)
//        return ret;

//    ret = writeTillDone(sock_desc, buf, buf_size);
    ret = writeTillDone(sock_desc, msg, buf_size+5);
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


//EncryptedPacketWithSSID::EncryptedPacketWithSSID(unsigned char ssid_value, Packet *encrypted_packet) : \
// Packet(encrypted_packet->getBufSize() + 2){
//    buf[0] = PCK_SSID;
//    buf[1] = ssid_value;
//    memcpy(&buf[2], encrypted_packet->getBuf(), encrypted_packet->getBufSize());
//}

unsigned char EncryptedPacketWithSSID::getSsidValue() {
    return buf[1];
}

Packet* EncryptedPacketWithSSID::getEncryptedPacket() {
    Packet *encrypted_packet;
    switch( buf[2] ){
        case PCK_ACK:
            encrypted_packet = new ACK(&buf[2]);
            break;
        case PCK_NAK:
            encrypted_packet = new NAK(&buf[2]);
            break;
        case PCK_EOT:
            encrypted_packet = new EOT(&buf[2]);
            break;
        case PCK_DESC:
            encrypted_packet = new DESC(&buf[2], buf_size - 2);
            break;
        case PCK_VAL:
            encrypted_packet = new VAL(&buf[2]);
            break;
        case PCK_GET:
            encrypted_packet = new GET(&buf[2]);
            break;
        case PCK_SET:
            encrypted_packet = new SET(&buf[2]);
            break;
        case PCK_SERVICES:
        case PCK_LOG:
            encrypted_packet = new LOG(&buf[2]);
            break;
        case PCK_SSID:
            std::cout<<"SSID cannoct be send as encrypted with ssid"<<std::endl;
        default:
            std::cout<<"Bad packet id: ";
            hex_print(&buf[2],1);
    }
    return encrypted_packet;
}

EOT::EOT(): EncrptedPacket(1) {
    buf[0] = PCK_EOT;
}

SERVICES::SERVICES() :EncrptedPacket(1) {
    buf[0] = PCK_SERVICES;
}


DESC::DESC(unsigned char dev_id, unsigned char dev_class, const std::string &name, const std::string &unit,\
 float min, float max) :EncrptedPacket(name.length() + 16) { //1 byte fir header, 1 for device id, 1 for type, 1 for terminating 0, 4 for unit, 4 for min, 4 for max, total 16
    if(unit.size() > 3) {
        delete[] buf;
        throw(std::runtime_error("Unit name is too long."));
    }
    buf[0] = PCK_DESC;
    buf[1] = dev_class;
    buf[2] = dev_id;
    memcpy(&buf[3], name.c_str(), name.size()+1);
    memcpy(&buf[buf_size-12], unit.c_str(), 4);
    memcpy(&buf[buf_size-8], &min, sizeof(float));
    memcpy(&buf[buf_size-4], &max, sizeof(float));
}

unsigned char DESC::getDeviceId() const {
    return buf[2];
}

unsigned char DESC::getDeviceClass() const {
    return  buf[1];
}

const char* DESC::getName() const {
    return reinterpret_cast<char *> (&buf[3]);
}

const char* DESC::getUnit() const {
    return reinterpret_cast<char *>(&buf[buf_size-12]);
}

float DESC::getMin() const {
    float min;
    memcpy(&min, &buf[buf_size-8], sizeof(float));
    return min;
}

float DESC::getMax() const {
    float max;
    memcpy(&max, &buf[buf_size-4], sizeof(float));
    return max;
}


VAL::VAL(unsigned char id, float value, time_t time_stamp) :EncrptedPacket(10) {
    buf[0] = PCK_VAL;
    buf[1] = id;
    memcpy(&buf[2], &value, sizeof(float));
    memcpy(&buf[6], &time_stamp, sizeof(time_t));
}


unsigned char VAL::getServiceId() const {
    return buf[1];
}
float VAL::getValue() const {
    float val;
    memcpy(&val, &buf[2], sizeof(float));
    return val;
}
time_t VAL::getTimestamp() const {
    time_t t_stmp;
    memcpy(&t_stmp, &buf[6], sizeof(time_t));
    return t_stmp;
}

GET::GET(unsigned char id):EncrptedPacket(2) {
    buf[0] = PCK_GET;
    buf[1] = id;
}

unsigned char GET::getId() {
    return buf[1];
}

SET::SET(unsigned char id, float value): EncrptedPacket(6) {
    buf[0] = PCK_SET;
    buf[1] = id;
    memcpy(&buf[2], &value, sizeof(float));
}

float SET::getValue() const {
    float val;
    memcpy(&val, &buf[2], sizeof(float));
    return val;
}

unsigned char SET::getId() const {
    return buf[1];
}

//EXIT::EXIT(unsigned char code): EncrptedPacket(2) {
//    buf[0] = PCK_EXIT;
//    buf[1] = code;
//}
//unsigned char EXIT::getCode() const {
//    return buf[1];
//}

//void Packet::print() {
//    hex_print(buf, buf_size);
//}




