#include "wbrodevice.h"

WbroDevice::WbroDevice(){}

ERR_DEVICE WbroDevice::Wbro_Dev_Cam_init(){
    bool b_init = InitSDK();
    if (b_init){
        return ERR_SUCCESS;
    } else {
        return ERR_CMD_CAM_INIT;
    }
}

ERR_DEVICE WbroDevice::Wbro_Dev_Cam_discover(std::vector<SV_DEVICE_INFO *>& devInfoList, std::vector<char *>& tlIDList, std::vector<SVCamSystem *>& svCamSysList){
    bool b_discover = svcam_discover(devInfoList, tlIDList, svCamSysList);
    if (b_discover){
        return ERR_SUCCESS;
    } else {
        return ERR_CMD_CAM_INIT;
    }
}




ERR_DEVICE WbroDevice::Wbro_Dev_ConnectToCom(const std::string com_port){
    hSerial = CreateFile(com_port.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        0,
        0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        return ERR_COM_CREATEFILE;
    }
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        return ERR_COM_GETCOMM;
    }
    dcbSerialParams.BaudRate=CBR_115200;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;
    if(!SetCommState(hSerial, &dcbSerialParams)){
        return ERR_COM_SETCOMM;
    }

    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=50;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=50;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    if(!SetCommTimeouts(hSerial, &timeouts)){
        return ERR_COM_SETTIMEOUT;
    }
    return ERR_SUCCESS;
}


ERR_DEVICE WbroDevice::Wbro_Dev_DisconnectCom(){
    if(CloseHandle(hSerial)){
        return ERR_SUCCESS;
    } else {
        return ERR_COM_CLOSEHANDLE;
    }
}


ERR_DEVICE WbroDevice::Wbro_Dev_ReadSignal(int szbuf, std::string& sig, DWORD& c_sig){
    char buff[szbuf + 1] = {0};
    if(!ReadFile(hSerial, buff, szbuf, &c_sig, NULL)){
        return ERR_CMD_READSIG;
    } else {
        sig = std::string(buff);
        return ERR_SUCCESS;
    }
}

