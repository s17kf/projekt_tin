//
// Created by s17kf on 04.06.18.
//

#include "consts.h"

void hex_print(const void* pv, size_t len)
{
    const unsigned char * p = (const unsigned char*)pv;
    if (NULL == pv)
        printf("NULL");
    else
    {
        size_t i = 0;
        for (; i<len;++i)
            printf("%02X ", *p++);
    }
    printf("\n");
}

void hex_print(unsigned char val){
    printf("%02X\n", val);

}

void hex_print_noendl(unsigned char val){
    printf("%02X ", val);

}