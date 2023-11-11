#include <string>
#include <iostream>

class WbroCore{
public:
    WbroCore();
    const std::string getVersion();
    float multiply(float num1, float num2);

private:
    std::string mVersion;

};