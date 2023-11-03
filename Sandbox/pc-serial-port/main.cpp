#include <windows.h>
#include <chrono>
#include <thread>
#include <iostream>


int main(){
    HANDLE hSerial;

    hSerial = CreateFile("COM3",
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
    //error getting state
    }
    dcbSerialParams.BaudRate=CBR_115200;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;
    if(!SetCommState(hSerial, &dcbSerialParams)){
    //error setting serial port state
    }

    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=50;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=10;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    if(!SetCommTimeouts(hSerial, &timeouts)){
    //error occureed. Inform user
    }

    while(true){
        int n = 10;
        char szBuff[n + 1] = {0};
        DWORD dwBytesRead = 0;
        if(!ReadFile(hSerial, szBuff, n, &dwBytesRead, NULL)){
        //error occurred. Report to user.
        } else {
            std::cout << "number of bytes read: "<< dwBytesRead << " string: "<< std::string(szBuff) << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }




CloseHandle(hSerial);


}