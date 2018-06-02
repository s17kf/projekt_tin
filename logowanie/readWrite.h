//
// Created by s17kf on 02.06.18.
//

#ifndef LOGOWANIE_READWRITE_H
#define LOGOWANIE_READWRITE_H

#include <unistd.h>
#include <iostream>
#include <cstring>


class ReadWrite {

public:

    static int readTillDone(int soc_desc, unsigned char *buf, ssize_t msg_size) {
        ssize_t i = 0;
        ssize_t readed;

        while (i < msg_size) {
            readed = read(soc_desc, &buf[i], (size_t) msg_size - i);
            switch (readed) {
                case -1:
                    //log(3, "Reading from socket exited with: %s.", strerror(errno));
                    std::cout << strerror(errno);
                    return -1;
                case 0:
//                log(3, "Client socket have been closed");
                    std::cout << "readTillDone: " << strerror(errno);
                    return 0;
            }
            i += readed;
        }
        return i;
    }

    static ssize_t writeTillDone(int soc_desc, const unsigned char *buf, ssize_t msg_size) {
        ssize_t i = 0;
        ssize_t written;

        while (i < msg_size) {
            written = write(soc_desc, &buf[i], (size_t) msg_size - i);
            switch (written) {
                case -1:
//                log(3, "Writing to socket exited with: %s.", strerror(errno));
                    std::cout << strerror(errno);
                    return -1;
                case 0:
//                log(3, "Client socket have been closed");
                    std::cout << strerror(errno);
                    return 0;
            }
            i += written;
        }
        return i;
    }

};

#endif //LOGOWANIE_READWRITE_H
