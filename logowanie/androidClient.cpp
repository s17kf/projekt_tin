//
// Created by s17kf on 02.06.18.
//

#include <cstring>
#include "androidClient.h"

//AndroidClient::AndroidClient(const unsigned char *key_encrypted, Privkey *privkey, unsigned char ssid_value){
AndroidClient::AndroidClient(Sesskey session_key, Privkey *privkey, unsigned char ssid_value) {
    sesskey = new Sesskey(session_key);
//    unsigned char *key_buf;
//    memcpy(key_buf, key_encrypted, 256);

//    KEY *key = KEY::createFromEncrypted(key_encrypted);
//    sesskey = new Sesskey(*key, *privkey);
//    ssid = new SSID(ssid_value);
    ssidValue = ssid_value;
}


//SSID* AndroidClient::getSsid() {
//    return ssid;
//}

unsigned char AndroidClient::getSsidValue() {
    return ssidValue;
}

Sesskey* AndroidClient::getSesskey() const {
    return sesskey;
}

AndroidClient::~AndroidClient() {
    delete sesskey;
//    delete ssid;
}

AndroidClient::AndroidClient() {
    sesskey = nullptr;
}