//
// Created by s17kf on 05.06.18.
//

#ifndef SERWER_ANDROIDSEQUENTIONS_H
#define SERWER_ANDROIDSEQUENTIONS_H

#include "connection.h"
#include "RNG.h"
#include "privkey.h"
#include "sesskey.h"
#include "androidClient.h"
#include "log.h"



int logInSequence(Connection *connection, Privkey *privkey, AndroidClient *androidClient, CHALL *chall );


#endif //SERWER_ANDROIDSEQUENTIONS_H
