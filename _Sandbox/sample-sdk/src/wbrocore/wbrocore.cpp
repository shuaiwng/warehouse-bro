#include "wbrocore.h"

WbroCore::WbroCore(){
    mVersion = "1.1.0";
    std::cout << "Core initialized." << std::endl;
}

const std::string WbroCore::getVersion(){
    return mVersion;
}


float WbroCore::multiply(float num1, float num2){
    return num1 * num2;
}
