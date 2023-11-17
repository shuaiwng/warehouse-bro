/**
 * @file wbrodevice.h
 * @brief Defines SDK Device functions.
 */

#include "SVCamMiniSample.h"
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
    ERR_CMD_READSIG,
    ERR_CMD_CAM_INIT
};

class WbroDevice{
public:
    WbroDevice();
    ERR_DEVICE init();
    ERR_DEVICE discover_device();
    ERR_DEVICE Wbro_Dev_ConnectToCom(const std::string com_port);
    ERR_DEVICE Wbro_Dev_DisconnectCom();
    ERR_DEVICE Wbro_Dev_ReadSignal(int szbuf, std::string& sig, DWORD& c_sig);
    
    ERR_DEVICE Wbro_Dev_Cam_init();
    ERR_DEVICE Wbro_Dev_Cam_discover(std::vector<SV_DEVICE_INFO *>&, std::vector<char *>&, std::vector<SVCamSystem *>&);


private:
    HANDLE hSerial;
    std::vector<SV_DEVICE_INFO *> devInfoList;
    std::vector<SVCamSystem *> svCamSysList;
    std::vector<char *> tlIDList;
};