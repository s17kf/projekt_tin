cmake_minimum_required(VERSION 3.10)
project(serwer)

set(CMAKE_CXX_STANDARD 11)



include_directories(/usr/lib/)
link_libraries(ssl)
link_libraries(crypto)
link_libraries(pthread)
link_libraries(rt)

add_executable(serwer main.cpp consts.h connection.cpp connection.h
        RNG.cpp RNG.h packet.cpp packet.h log.cpp log.h
        privkey.cpp privkey.h pubkey.cpp pubkey.h
        sesskey.cpp sesskey.h
        androidClient.cpp androidClient.h consts.cpp
        AddQueue.cpp AddQueue.h ReadQueue.cpp ReadQueue.h
        androidSequentions.cpp androidSequentions.h
        queuePacket.cpp queuePacket.h DevDescriptor.cpp DevDescriptor.h serwer.cpp serwer.h)