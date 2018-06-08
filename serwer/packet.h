//
// Created by s17kf on 02.06.18.
//

#ifndef LOGOWANIE_PACKET_H
#define LOGOWANIE_PACKET_H


#include <iostream>
#include <cstdio>

#include "sesskey.h"



class Sesskey;
class AndroidClient;
class Serwer;



class Packet {
protected:
    enum pck_type {
        PCK_ACK	= 0x01,
        PCK_NAK = 0x02,
        PCK_EOT	= 0x03,
        PCK_CHALL = 0x04,
        PCK_CHALL_RESP = 0x05,
        PCK_KEY = 0x06,
        PCK_DESC = 0x07,
        PCK_VAL = 0x08,
        PCK_GET = 0x09,
        PCK_SET = 0x0A,
        PCK_SSID = 0x0B,
        PCK_LOG = 0x0C,
        PCK_SERVICES = 0x0D
    };
    unsigned char *buf;
    uint32_t buf_size;
    Packet(const unsigned char *buf_in, uint32_t buf_len);
    explicit Packet(size_t size) ;
public:
    static Packet *packetFactory(int soc_desc, Serwer *serwer, unsigned char *ssidValue);
    virtual ~Packet();
    virtual ssize_t send(int soc_desc, const Sesskey *sesskey) const = 0;

    //TODO: it should be deleted?
    virtual void print();
};


/* This class keeps packets that travels via net encrypted. */
class EncrptedPacket : public Packet {
protected:
    EncrptedPacket(const unsigned char *buf, uint32_t buf_len): Packet(buf, buf_len){}
    EncrptedPacket(size_t size): Packet(size){}
public:
    ssize_t send(int soc_desc, const Sesskey *sesskey) const override ;
};

/* This class keeps packets that travels via net in plaintext or decrypted with RSA. */
class PlainPacket : public Packet{
protected:
    PlainPacket(const unsigned char *buf, uint32_t buf_len): Packet(buf, buf_len){}
    PlainPacket(size_t size): Packet(size){}

public:
    ssize_t send(int sock_desc, const Sesskey *sesskey) const override;
};

class ACK : public EncrptedPacket {
public:
    unsigned char getId() const;
    //ACK(Packet &&packet);
    ACK(unsigned char *buf): EncrptedPacket(buf, 2) {}
    ACK(unsigned char id);
};

class NAK : public EncrptedPacket {
public:
    unsigned char getId() const;
    //NAK(Packet &&packet);
    NAK(unsigned char *buf): EncrptedPacket(buf, 2) {}
    NAK(unsigned char id);
};

class LOG : public EncrptedPacket{
public:
    unsigned char * getLogin() const;
    unsigned char * getPassword() const;

    LOG(unsigned char *buf): EncrptedPacket(buf,63){}
    LOG(unsigned char *login, unsigned char *password);

};

class CHALL : public PlainPacket{
private:
    CHALL(const unsigned char *buf): PlainPacket(buf,9){}
public:
    const unsigned char *getChall() const;
    static CHALL *createFromMessage(const unsigned char *msg);
    static CHALL *createFromRandom(const unsigned char *rand);

};

class CHALL_RESP : public PlainPacket{
private:
    CHALL_RESP(const unsigned char *buf):PlainPacket(buf, 257){}

public:
    const unsigned char *getResp() const;
    static CHALL_RESP *createFromMessage(const unsigned char *msg);
    static CHALL_RESP *createFromEncrypted(const unsigned char *encrypt);
};

class KEY : public PlainPacket {
private:
    KEY(const unsigned char* buf): PlainPacket(buf, 257) {}
public:
    const unsigned char *getKeyBuf() const;
    //KEY(const unsigned char* encoded);
    static KEY *createFromMessage(const unsigned char *msg);
    static KEY *createFromEncrypted(const unsigned char *encrypt);
};

class SSID : public EncrptedPacket{
public:
    unsigned char getValue() const;
    //NAK(Packet &&packet);
    SSID(unsigned char *buf): EncrptedPacket(buf, 2) {}
    SSID(unsigned char value);
};


class EncryptedPacketWithSSID : public Packet{
public:
    ssize_t send(int soc_desc, const Sesskey *sesskey) const override ;
    unsigned char getSsidValue();
    Packet *getEncryptedPacket();

    /*
     * *buf contain both SSID and Encrypted packet part
     * buf_len *buf len
    */
    EncryptedPacketWithSSID(unsigned char *buf, uint32_t buf_len) : Packet(buf, buf_len){}
//    EncryptedPacketWithSSID(unsigned char ssid_value, Packet *encrypted_packet);



};

class EOT : public EncrptedPacket {
public:
    //EOT(Packet &&packet);
    EOT(unsigned char *buf): EncrptedPacket(buf, 1) {}
    EOT();
};

class SERVICES : public EncrptedPacket{
public:
    SERVICES(unsigned char *buf):EncrptedPacket(buf,1){}
    SERVICES();
};

class DESC : public EncrptedPacket{
public:
    unsigned char getDeviceId() const;
    unsigned char getDeviceClass() const;
    const char *getName() const;
    const char *getUnit() const;
    float getMin() const;
    float getMax() const;
    DESC(unsigned char dev_id, unsigned char dev_class, const std::string &name, const std::string &unit,\
            float min, float max);
    DESC(unsigned char *buf, size_t buf_len): EncrptedPacket(buf, buf_len) {}

};

class VAL : public EncrptedPacket {
public:
    unsigned char getServiceId() const;
    float getValue() const;
    time_t getTimestamp() const;
    VAL(unsigned char *buf): EncrptedPacket(buf, 10) {}
    VAL(unsigned char id, float value, time_t time_stamp);
    //VAL(Packet &&packet);
};

class GET : public EncrptedPacket {
public:
    GET(unsigned char *buf): EncrptedPacket(buf, 2){}
    GET(unsigned char id);
    unsigned char getId();
};

class SET : public EncrptedPacket {
public:
    SET(unsigned char *buf): EncrptedPacket(buf, 6){}
    SET(unsigned char id, float value);
    float getValue() const;
    unsigned char getId() const;
};

//class EXIT : public EncrptedPacket {
//public:
//    unsigned char getCode() const;
//    EXIT(unsigned char *buf): EncrptedPacket(buf, 2) {}
//    //EXIT(Packet &&packet);
//    EXIT(unsigned char code);
//};

#endif //LOGOWANIE_PACKET_H
