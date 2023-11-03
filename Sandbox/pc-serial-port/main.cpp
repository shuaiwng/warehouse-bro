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
        0,
        0);

    if (hSerial == INVALID_HANDLE_VALUE) 
    {
        printf ("CreateFile failed with error %ld.\n", GetLastError());
        return (1);
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cout << "[!] GetCommState error." << std::endl;
    }
    dcbSerialParams.BaudRate=CBR_115200;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;
    if(!SetCommState(hSerial, &dcbSerialParams)){
        std::cout << "[!] SetCommState error." << std::endl;
    }

    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=50;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=50;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    if(!SetCommTimeouts(hSerial, &timeouts)){
        std::cout << "[!] SetCommTimeouts error." << std::endl;
    }

    while(true){
        int n = 50;
        char szBuff[n + 1] = {0};
        DWORD dwBytesRead = 0;
        if(!ReadFile(hSerial, szBuff, n, &dwBytesRead, NULL)){
            std::cout << "[!] Nothing read." << std::endl;
        } else {
            std::cout << "Number of bytes read: "<< dwBytesRead << "\n" << std::flush;
            std::cout << "String: "<< std::string(szBuff) << "\n" << std::flush;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }




CloseHandle(hSerial);


}