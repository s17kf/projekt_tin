


#include "sesskey.h"
#include "log.h"
#include "RNG.h"

//extern RNG rng;

Sesskey::Sesskey() {
    RNG rng;
    if (rng.generate(key, (size_t) 16) == -1) {
        log(3, "Unable to create sesskey.");
    }
    ctx = EVP_CIPHER_CTX_new();
}
Sesskey::Sesskey(const KEY &keyPck, const Privkey &privkey) {
    ctx = EVP_CIPHER_CTX_new();
    unsigned char buf[256];
    if (privkey.decrypt(keyPck.getKeyBuf(), 256, buf) == -1){
        log(3, "Unable to decrypt session key");
    }
	memcpy(key, buf, 16);
}

Sesskey::~Sesskey() {
    EVP_CIPHER_CTX_free(ctx);
}

// This function is called only for messages that needs encryption
int Sesskey::encrypt(unsigned char *dest, const unsigned char *src, size_t src_size) const{
    int len, ciphertext_len;
    unsigned char iv[16];
    RNG rng;
    ciphertext_len = 0;
    if (rng.generate(iv, (size_t) 16) == -1) {
        log(3, "Unable to create IV");
        return -1;
    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) {
        log(3, "Unable to init EVP encryption.");
        return -1;
    }
    //while (ciphertext_len < src_size) {
    if (1 != EVP_EncryptUpdate(ctx, dest, &len, src, src_size)){
        log(3, "Unable to encrypt.");
        return -1;
    }
    ciphertext_len += len;
    //}
    if(1 != EVP_EncryptFinal_ex(ctx, dest + len, &len)) {
        log(3, "Unable to finalize EVP encryption.");
        return -1;
    }
    ciphertext_len += len;
    memcpy(dest + ciphertext_len, iv, 16);
    ciphertext_len += 16;
    log(5,"Encrypted bytes: %d, from: %d",ciphertext_len,src_size);

    return ciphertext_len;
}

// This function is called only for messages that needs decryption
int Sesskey::decrypt(unsigned char *dest, const unsigned char *src, size_t src_size) const{
    const unsigned char *iv;
    unsigned char plain_size;
    int len;
    int msg_buf_size;

    msg_buf_size = src_size - 16;// -16 for iv

    iv = &src[src_size - 16];// -16 for iv, -1 for zero-based counting
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) {
        log(3, "Unable to init EVP decryption.");
        return -1;
    }

    if (1 != EVP_DecryptUpdate(ctx, dest, &len, src, msg_buf_size)) {
        log(3, "Unable to decrypt.");
        return -1;
    }

    if (1 != EVP_DecryptFinal_ex(ctx, dest + len, &len)) {
        ERR_print_errors_fp(stderr);
        log(3, "Unable to finalize EVP decryption.");
        return -1;
    }

    return 0;
}
const unsigned char* Sesskey::getKeyBuf() const {
    return key;
}
