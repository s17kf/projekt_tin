//
// Created by s17kf on 05.06.18.
//

#ifndef SERWER_QUEUEPACKET_H
#define SERWER_QUEUEPACKET_H


#include <cstdint>

class QueuePacket {
protected:
    unsigned char * bufor;
    uint32_t buf_size;

    QueuePacket(unsigned char *bufor, uint32_t buf_size);


};


#endif //SERWER_QUEUEPACKET_H
