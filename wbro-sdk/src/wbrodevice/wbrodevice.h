/**
 * @file wbrodevice.h
 * @brief Defines SDK Device functions.
 */


#include <string>
#include <iostream>

class WbroDevice{
public:
    WbroDevice();
    const std::string getSignal();
    void sendSignal(const std::string);

private:
    std::string in_signal;
};