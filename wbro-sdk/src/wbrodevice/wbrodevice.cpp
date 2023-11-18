#include "wbrodevice.h"

WbroDevice::WbroDevice(){}

ERR_DEVICE WbroDevice::Wbro_Dev_Cam_init(){
    bool b_init = svcam_init_sdk();
    if (b_init){
        return ERR_SUCCESS;
    } else {
        return ERR_CAM_INIT;
    }
}

ERR_DEVICE WbroDevice::Wbro_Dev_Cam_close(){
    bool b_close = svcam_close_sdk();
    if (b_close){
        return ERR_SUCCESS;
    } else {
        return ERR_CAM_CLOSE;
    }
}

ERR_DEVICE WbroDevice::Wbro_Dev_Cam_discover(std::vector<SV_DEVICE_INFO *>& devInfoList, std::vector<SVCamSystem *>& svCamSysList, std::vector<char *>& tlIDList){
    bool b_discover = svcam_discover(devInfoList, svCamSysList, tlIDList);
    if (b_discover){
        return ERR_SUCCESS;
    } else {
        return ERR_CAM_DISCOVER;
    }
}

ERR_DEVICE WbroDevice::Wbro_Dev_Cam_connect(std::vector<SV_DEVICE_INFO *> devInfoList, std::vector<SVCamSystem *> svCamSysList, std::vector<char *> tlIDList, Camera*& cam){
    bool b_connect = svcam_connect(devInfoList, svCamSysList, tlIDList, cam);
    if (b_connect){
        return ERR_SUCCESS;
    } else {
        return ERR_CAM_CONN;
    }
}

ERR_DEVICE WbroDevice::Wbro_Dev_Cam_disconnect(std::vector<SV_DEVICE_INFO *> devInfoList, std::vector<SVCamSystem *> svCamSysList, std::vector<char *> tlIDList){
    bool b_disconnect = svcam_disconnect(devInfoList, svCamSysList, tlIDList);
    if (b_disconnect){
        return ERR_SUCCESS;
    } else {
        return ERR_CAM_DISCONN;
    }
}


ERR_DEVICE WbroDevice::Wbro_Dev_Cam_setParam(Camera* cam, int expTime_ns){
    _expTime_ns = expTime_ns;
    bool b_set = svcam_set_parameter(cam, _expTime_ns);
    if (b_set){
        return ERR_SUCCESS;
    } else {
        return ERR_CAM_SETPARAM;
    }
}

ERR_DEVICE WbroDevice::Wbro_Dev_Cam_take_image(Camera* cam){
    bool b_meas = svcam_acquire_image(cam, _expTime_ns);
    if (b_meas){
        return ERR_SUCCESS;
    } else {
        return ERR_CAM_MEASURE;
    }
}

ERR_DEVICE WbroDevice::Wbro_Dev_Cam_save_image(Camera* cam, const char* img_name){
    bool b_save = svcam_save_image(cam, img_name);
    if (b_save){
        return ERR_SUCCESS;
    } else {
        return ERR_CAM_SAVEIMAGE;
    }
}

ERR_DEVICE WbroDevice::Wbro_Dev_Com_connect(const std::string com_port){
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


ERR_DEVICE WbroDevice::Wbro_Dev_Com_disconnect(){
    if(CloseHandle(hSerial)){
        return ERR_SUCCESS;
    } else {
        return ERR_COM_CLOSEHANDLE;
    }
}


ERR_DEVICE WbroDevice::Wbro_Dev_uC_send_to(const char * cmd_send){
    DWORD dwBytesWritten = 0;
    if(!WriteFile(hSerial, cmd_send, 1, &dwBytesWritten, NULL)){
        return ERR_UC_SENDTO;
    } else {
        return ERR_SUCCESS;
    }
}


ERR_DEVICE WbroDevice::Wbro_Dev_uC_read_from(std::string& sig, DWORD& c_sig){
    char buff[SZ_BUFF + 1] = {0};
    if(!ReadFile(hSerial, buff, SZ_BUFF, &c_sig, NULL)){
        return ERR_UC_READSIG;
    } else {
        sig = std::string(buff);
        return ERR_SUCCESS;
    }
}


ERR_DEVICE WbroDevice::Wbro_Dev_uC_getInfo(std::string sig, ucInfo_t & info){
    info.mode = M_INVALID;

    if (sig.length() < 10){
        return ERR_SUCCESS;
    }

    std::string res;
    if (sig.substr(0,3) == "[>>" && sig.substr(7,3) == "<<]"){
        res = sig.substr(3,3);
        if (res == "STB") {
            info.mode = M_STANDBY;
        } 
        else if (res == "SWF"){
            info.mode = M_SURVEILLANCE;
        }
        else if (res == "OWF"){
            info.mode = M_OPERATION;

            if (sig.substr(10,1) == "(" && sig.length() >= 13){
                size_t pos_found = sig.find_first_of(')');
                if (string::npos == pos_found){
                    return ERR_UC_GETINFO;
                }
                info.distance = std::stof(sig.substr(11, pos_found-11));
            } else {
                return ERR_UC_GETINFO;
            } 
        }
        else {
            return ERR_UC_GETINFO;
        }

        info.status = sig.substr(6,1)[0];
    }

    return ERR_SUCCESS;
}
