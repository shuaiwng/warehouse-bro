#include <string>
#include <iostream>

class WbroCore{
public:
    WbroCore();
    const std::string getVersion();

private:
    std::string mVersion;

};