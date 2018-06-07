//
// Created by s17kf on 06.06.18.
//

#ifndef SERWER_DEVDESCRIPTOR_H
#define SERWER_DEVDESCRIPTOR_H

#include <iostream>
#include "packet.h"
#include "queuePacket.h"

class DevDescriptor {
    unsigned char id;
    unsigned char clas;
    std::string name;
    std::string unit;
    float min;
    float max;
public:
    DevDescriptor(unsigned char dev_id, unsigned char dev_class, std::string dev_name, \
        std::string dev_unit, float min_value, float max_value) : id(dev_id), \
        clas(dev_class), name(dev_name), unit(dev_unit), min(min_value), max(max_value) {}
//    explicit DevDescriptor(DESC *desc) : DevDescriptor(desc->getDeviceId(), desc->getDeviceClass(), \
//        desc->getName(), desc->getUnit(), desc->getMin(), desc->getMax()) {}
    DevDescriptor(Q_DESC *qDesc) : DevDescriptor(qDesc->getDeviceId(), qDesc->getDeviceClass(), \
        qDesc->getName(), qDesc->getUnit(), qDesc->getMin(), qDesc->getMax()) {}
    unsigned char getId() const;
    unsigned char getClass() const;
    const std::string getName() const;
    const std::string getUnit() const;
    float getMax() const;
    float getMin() const;

};


#endif //SERWER_DEVDESCRIPTOR_H