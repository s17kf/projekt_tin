//
// Created by s17kf on 02.06.18.
//

#ifndef LOGOWANIE_ANDROIDCLIENT_H
#define LOGOWANIE_ANDROIDCLIENT_H

#include "sesskey.h"
#include "packet.h"
#include "privkey.h"


class AndroidClient {
    Sesskey *sesskey;
//    SSID *ssid;
    unsigned char ssidValue;
public:
//    AndroidClient( const unsigned char *key_encrypted, Privkey *privkey, unsigned char ssid_value);
    AndroidClient(Sesskey session_key, Privkey *privkey, unsigned char ssid_value);
    //    SSID *getSsid();
    AndroidClient();

    void setSesskey(Sesskey newSesskey){ sesskey = new Sesskey(newSesskey);}
    void setSssid(unsigned char ssid_value){ ssidValue = ssid_value;}

    unsigned char getSsidValue();
    Sesskey *getSesskey() const;
    ~AndroidClient();


};


#endif //LOGOWANIE_ANDROIDCLIENT_H
