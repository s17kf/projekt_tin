//
// Created by s17kf on 06.06.18.
//

#include "DevDescriptor.h"


unsigned char DevDescriptor::getId() const {
    return id;
}

unsigned char DevDescriptor::getClass() const {
    return clas;
}

const std::string DevDescriptor::getName() const {
    return name;
}

const std::string DevDescriptor::getUnit() const{
    return unit;
}

float DevDescriptor::getMin() const {
    return min;
}

float DevDescriptor::getMax() const {
    return max;
}