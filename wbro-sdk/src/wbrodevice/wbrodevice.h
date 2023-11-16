/**
 * @file wbrodevice.h
 * @brief Defines SDK Device functions.
 */

#include <windows.h>
#include <string>
#include <iostream>

enum ERR_DEVICE{
    ERR_SUCCESS = 0,
    ERR_COM_CREATEFILE,
    ERR_COM_GETCOMM,
    ERR_COM_SETCOMM,
    ERR_COM_SETTIMEOUT,
    ERR_COM_CLOSEHANDLE,
    ERR_CMD_READSIG
};

class WbroDevice{
public:
    WbroDevice();
    ERR_DEVICE Wbro_Dev_ConnectToCom(const std::string com_port);
    ERR_DEVICE Wbro_Dev_DisconnectCom();
    ERR_DEVICE Wbro_Dev_ReadSignal(int szbuf, std::string& sig, DWORD& c_sig);
    

private:
    HANDLE hSerial;

};