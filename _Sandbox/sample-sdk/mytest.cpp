#include "wbrocore.h"
#include "wbrodevice.h"

int main(){
    WbroCore testCore = WbroCore();
    WbroDevice testDevice = WbroDevice();

    std::string ver = testCore.getVersion();
    std::string sig = testDevice.getSignal();
    testDevice.sendSignal("MyNewSignal");

    std::cout << "Version: " << ver << "\n";
    std::cout << "Signal got: " << testDevice.getSignal() << "\n";

    return 0;
}
