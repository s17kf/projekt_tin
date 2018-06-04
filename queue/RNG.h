//
// Created by s17kf on 02.06.18.
//

#ifndef LOGOWANIE_RNG_H
#define LOGOWANIE_RNG_H

#include <cstdlib>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/rand.h>
#include <iostream>


class RNG {
public:
    int generate(unsigned char *buf, size_t size);
};


#endif //LOGOWANIE_RNG_H
