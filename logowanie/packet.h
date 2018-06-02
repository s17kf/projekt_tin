//
// Created by s17kf on 02.06.18.
//

#ifndef LOGOWANIE_PACKET_H
#define LOGOWANIE_PACKET_H


#include <iostream>
#include <cstdio>

typedef char Sesskey;

class Packet {
protected:
    unsigned char *buf;
    uint32_t buf_size;

    Packet(const unsigned char *buf_in, uint32_t buf_len);
    Packet(size_t size);


public:
    static Packet * packetFactory(int soc_desc, const Sesskey *sesskey);

    virtual ssize_t send(int soc_desc, const Sesskey *sesskey) const = 0;

    virtual ~Packet();



};

class PlainPacket : public Packet{
protected:
    PlainPacket(const unsigned char *buf, uint32_t buf_len): Packet(buf, buf_len){}
    PlainPacket(size_t size): Packet(size){}

public:
    ssize_t send(int sock_desc, const Sesskey *sesskey) const override;
};

class CHALL : public PlainPacket{
private:
    CHALL(const unsigned char *buf): PlainPacket(buf,9){}
public:
    const unsigned char *getChall() const;
    static CHALL *createFromMessage(const unsigned char *msg);
    static CHALL *createFromRandom(const unsigned char *rand);

};


#endif //LOGOWANIE_PACKET_H
