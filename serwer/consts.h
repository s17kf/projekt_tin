//
// Created by s17kf on 02.06.18.
//

#ifndef LOGOWANIE_CONSTS_H
#define LOGOWANIE_CONSTS_H

#include <iostream>

#define PORT_NR 4003
#define MAX_CONNECTIONS 10
#define MAX_MSG_LEN 100


//queue parameters
#define QUEUE_NAME_TO_GONZO  "/queueToGonzo"
#define QUEUE_NAME_FROM_GONZO "/queueFromGonzo"
#define MSG_SIZE 128
#define MAX_MSGS 10

#define QUEUE_MODE S_IRWXU




#define PCK_ACK 0x01
#define PCK_NAK 0x02
#define PCK_EOT 0x03
#define PCK_CHALL 0x04
#define PCK_CHALL_RESP 0x05
#define PCK_KEY 0x06
#define PCK_DESC 0x07
#define PCK_VAL 0x08
#define PCK_GET 0x09
#define PCK_SET 0x0A
#define PCK_SSID 0x0B
#define PCK_LOG 0x0C
#define PCK_SERVICES 0x0D

#define PCK_EXIT 0x0E

#define PCK_Q_ACK   0x11
#define PCK_Q_NAK   0x12
#define PCK_Q_EOT   0x13
#define PCK_Q_DESC  0x17
#define PCK_Q_VAL   0x18
#define PCK_Q_GET   0x19
#define PCK_Q_SET   0x1A
#define PCK_Q_EXIT	0x14


#define ANALOG_IN 0x00
#define ANALOG_OUT 0x01
#define DIGITAL_IN 0x02
#define DIGITAL_OUT 0x03


void hex_print(const void* pv, size_t len);
void hex_print(unsigned char val);


#define CHECK(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x); \
            /*std::cout<<std::strerror(errno)<<std::endl;*/\
            std::cout<<"# error: "<<errno<<std::endl;\
            exit(-1); \
        } \
    } while (0) \


#endif //LOGOWANIE_CONSTS_H
