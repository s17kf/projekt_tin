#ifndef TIN_SESSKEY
#define TIN_SESSKEY

#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdexcept>
#include <cstring>

#include "packet.h"
#include "privkey.h"

class KEY;

class Sesskey {
private:
    unsigned char key[16];
    EVP_CIPHER_CTX *ctx;
public:
    Sesskey();
    Sesskey(const KEY &key, const Privkey &privkey);
    ~Sesskey();
    /* Both encode and decode messages assumes that encrypted message has IV in the end */
    int encrypt(unsigned char *dest, const unsigned char *src, size_t src_size) const;
    int decrypt(unsigned char *dest, const unsigned char *src, size_t src_size) const;
    const unsigned char *getKeyBuf() const;

    void setKey(const unsigned char *new_key){ memcpy(key,new_key, 16);}
};
#endif //TIN_SESSKEY
