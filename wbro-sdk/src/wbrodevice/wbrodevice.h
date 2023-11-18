/**
 * @file wbrodevice.h
 * @brief Defines SDK Device functions.
 */

#include "svscam/SVCamMiniSample.h"
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
    ERR_CMD_CAM_INIT,
    ERR_CMD_CAM_CLOSE,
    ERR_CMD_CAM_DISCOVER,
    ERR_CMD_CAM_CONN,
    ERR_CMD_CAM_DISCONN,
    ERR_CMD_CAM_SETPARAM,
    ERR_CMD_CAM_MEASURE,
    ERR_CMD_CAM_SAVEIMAGE
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
    ERR_DEVICE Wbro_Dev_Cam_close();

    ERR_DEVICE Wbro_Dev_Cam_discover(std::vector<SV_DEVICE_INFO *>&, std::vector<SVCamSystem *>&, std::vector<char *>&);
    ERR_DEVICE Wbro_Dev_Cam_connect(std::vector<SV_DEVICE_INFO *>, std::vector<SVCamSystem *>, std::vector<char *>, Camera*&);
    ERR_DEVICE Wbro_Dev_Cam_disconnect(std::vector<SV_DEVICE_INFO *>, std::vector<SVCamSystem *>, std::vector<char *>);

    ERR_DEVICE Wbro_Dev_Cam_setParam(Camera* cam, int expTime_ns);
    ERR_DEVICE Wbro_Dev_Cam_take_image(Camera* cam);
    ERR_DEVICE Wbro_Dev_Cam_save_image(Camera* cam, const char* img_name);

    // ERR_DEVICE Wbro_Dev_MC_send_signal();
    // ERR_DEVICE Wbro_Dev_MC_receive_signal();
    


private:
    HANDLE hSerial;
    std::vector<SV_DEVICE_INFO *> devInfoList;
    std::vector<SVCamSystem *> svCamSysList;
    std::vector<char *> tlIDList;
    Camera * cam;
    int _expTime_ns;
};