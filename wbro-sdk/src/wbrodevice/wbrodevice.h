/**
 * @file wbrodevice.h
 * @brief Defines SDK Device functions.
 */

#include "svcam/SVCamMiniSample.h"
#include <windows.h>
#include <string>
#include <iostream>

#define SZ_BUFF 50

enum ERR_DEVICE{
    ERR_SUCCESS = 0,
    ERR_COM_CREATEFILE,
    ERR_COM_GETCOMM,
    ERR_COM_SETCOMM,
    ERR_COM_SETTIMEOUT,
    ERR_COM_CLOSEHANDLE,
    ERR_CAM_INIT,
    ERR_CAM_CLOSE,
    ERR_CAM_DISCOVER,
    ERR_CAM_CONN,
    ERR_CAM_DISCONN,
    ERR_CAM_SETPARAM,
    ERR_CAM_MEASURE,
    ERR_CAM_SAVEIMAGE,
    ERR_UC_SENDTO,
    ERR_UC_READSIG,
    ERR_UC_GETINFO
};

enum UC_MODE{
    M_INVALID,
    M_STANDBY,
    M_SURVEILLANCE,
    M_OPERATION
};

struct ucInfo_t{
    UC_MODE mode;
    char status;
    float distance;
};

class WbroDevice{
public:
    WbroDevice();

    ERR_DEVICE Wbro_Dev_Com_connect(const std::string com_port);
    ERR_DEVICE Wbro_Dev_Com_disconnect();
    
    ERR_DEVICE Wbro_Dev_Cam_init();
    ERR_DEVICE Wbro_Dev_Cam_close();

    ERR_DEVICE Wbro_Dev_Cam_discover(std::vector<SV_DEVICE_INFO *>&, std::vector<SVCamSystem *>&, std::vector<char *>&);
    ERR_DEVICE Wbro_Dev_Cam_connect(std::vector<SV_DEVICE_INFO *>, std::vector<SVCamSystem *>, std::vector<char *>, Camera*&);
    ERR_DEVICE Wbro_Dev_Cam_disconnect(std::vector<SV_DEVICE_INFO *>, std::vector<SVCamSystem *>, std::vector<char *>);

    ERR_DEVICE Wbro_Dev_Cam_setParam(Camera* cam, int expTime_ns);
    ERR_DEVICE Wbro_Dev_Cam_take_image(Camera* cam);
    ERR_DEVICE Wbro_Dev_Cam_save_image(Camera* cam, const char* img_name);

    ERR_DEVICE Wbro_Dev_uC_send_to(const char * cmd_send);
    ERR_DEVICE Wbro_Dev_uC_read_from(std::string& sig, DWORD& c_sig);
    ERR_DEVICE Wbro_Dev_uC_getInfo(std::string sig, ucInfo_t & info);


private:
    HANDLE hSerial;
    std::vector<SV_DEVICE_INFO *> devInfoList;
    std::vector<SVCamSystem *> svCamSysList;
    std::vector<char *> tlIDList;
    Camera * cam;
    int _expTime_ns;
};