//
// Created by s17kf on 02.06.18.
//

#ifndef LOGOWANIE_PRIVKEY_H
#define LOGOWANIE_PRIVKEY_H

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <stdexcept>

class Privkey {
private:
    RSA *key;

public:
    int decrypt(const unsigned char *encrypted, size_t encrypted_len, unsigned char *data) const;

    int encrypt(const unsigned char *data, size_t data_len, unsigned char *encrypted) const;

    int sign(const unsigned char *data, size_t data_len, unsigned char *sign, unsigned int *sign_len) const;

    Privkey(const char *file_name);

    Privkey();

    ~Privkey();
};

#endif //LOGOWANIE_PRIVKEY_H
