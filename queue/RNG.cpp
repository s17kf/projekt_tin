//
// Created by s17kf on 02.06.18.
//

#include "RNG.h"

int RNG::generate(unsigned char *buf,size_t size){
    int rc = RAND_bytes(buf, size);
    if(rc != 1) {
       // log(3, "Random generating failed\n");
        std::cout<<"Random generating failed"<<std::endl;
        return -1;
    }
    return 0;
}