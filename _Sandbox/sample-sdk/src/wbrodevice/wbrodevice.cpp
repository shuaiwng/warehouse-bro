#include "wbrodevice.h"

WbroDevice::WbroDevice(){
    std::cout << "Device initialized." << std::endl;
}

const std::string WbroDevice::getSignal(){
    return in_signal;
}

void WbroDevice::sendSignal(const std::string sig){
    in_signal = sig;
}