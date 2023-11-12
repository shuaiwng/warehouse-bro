#include <windows.h>
#include <chrono>
#include <thread>
#include <iostream>
#include "camera/SVCamMiniSample.h"


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

    int inc = 0;
    const char* cmds[3] = {"0", "1", "2"};
    while(true){
        int n = 100;
        char szBuff[n + 1] = {0};
        DWORD dwBytesRead = 0;
        if(!ReadFile(hSerial, szBuff, n, &dwBytesRead, NULL)){
            std::cout << "[!] Error with ReadFile." << std::endl;
        } else {
            std::string strBuff = std::string(szBuff);
            std::cout << "Number of bytes read: "<< dwBytesRead << "\nContent:\n---\n";
            std::cout << strBuff << "\n";

            // evaluate the request
            std::string req;
            if (strBuff.length() >= 10) {
                if (strBuff.substr(0,3) == "[>>" && strBuff.substr(7,3) == "<<]"){
                    req = strBuff.substr(3,4);

                    int m = 1;
                    DWORD dwBytesWritten = 0;
                    const char* cmd_send = cmds[inc%3];
                    if(!WriteFile(hSerial, cmd_send, m, &dwBytesWritten, NULL)){
                        std::cout << "[!] Error with WriteFile." << std::endl;
                    } else {
                        std::cout << "CMD sent: "<< std::string(cmd_send) << std::endl;
                    }
                }
            }

            acquire_image(500000);

            inc++;
        }
    }

CloseHandle(hSerial);
}