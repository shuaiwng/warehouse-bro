#include "wbrocore.h"
#include "wbrodevice.h"
#include <iostream>
#include <thread>

#define DISTANCE_TO_CART 150

int main(){
    ERR_DEVICE stat;
    Camera* cam;

    std::vector<SV_DEVICE_INFO *> devInfoList;
    std::vector<SVCamSystem *> svCamSysList;
    std::vector<char *> tlIDList;

    WbroCore wbrocore;
    WbroDevice wbrodevice;

    std::cout << "SDK-Version: " << wbrocore.Wbro_getVersion() << "\n-----\n";

    stat = wbrodevice.Wbro_Dev_Com_connect("COM3");
    stat = wbrodevice.Wbro_Dev_Cam_init();

    stat = wbrodevice.Wbro_Dev_Cam_discover(devInfoList, svCamSysList, tlIDList);
    stat = wbrodevice.Wbro_Dev_Cam_connect(devInfoList, svCamSysList, tlIDList, cam);
    stat = wbrodevice.Wbro_Dev_Cam_setParam(cam, 100000); // us

    std::string sig = "";
    DWORD c_sig = 0;
    ucInfo_t info = {M_INVALID, 'A', 0};

    // Reset to Standby
    std::cout << "Reset to Standby mode\n-----\n";
    stat = wbrodevice.Wbro_Dev_uC_send_to(std::string("1").c_str());
    std::this_thread::sleep_for(5000ms);

    // Standby mode
    std::cout << "Start Standby mode\n-----\n";
    for (int i = 0; i < 20; i++) {
        if (wbrodevice.Wbro_Dev_uC_read_from(sig, c_sig)){
            std::cout << "Error reading Signal.\n";
        }
        std::cout << "RawSignal: " << sig << std::endl;
        stat = wbrodevice.Wbro_Dev_uC_getInfo(sig, info);
        std::cout << "GotInfoFromUC -> mode: " << info.mode << " status: " << info.status << "\n";
        if (info.mode == M_STANDBY) {
            std::this_thread::sleep_for(50ms);
            std::cout << ">> standing by...\n";
        }
    }

    // Switch to Surveillance mode
    std::cout << "Start Surveillance mode\n-----\n";
    stat = wbrodevice.Wbro_Dev_uC_send_to(std::string("2").c_str());
    std::this_thread::sleep_for(5000ms);

    for (int i = 0; i < 30; i++) {
        stat = wbrodevice.Wbro_Dev_uC_read_from(sig, c_sig);
        std::cout << "RawSignal: " << sig << std::endl;
        stat = wbrodevice.Wbro_Dev_uC_getInfo(sig, info);
        std::cout << "GotInfoFromUC -> mode: " << info.mode << " status: " << info.status << "\n";
        if (info.mode == M_SURVEILLANCE && info.status == '1') {
            wbrodevice.Wbro_Dev_Cam_take_image(cam);
            wbrodevice.Wbro_Dev_Cam_save_image(cam, std::string("image_svl_"+std::to_string(i)+".png").c_str());
        }
        std::this_thread::sleep_for(50ms);
        std::cout << ">> surveillance mode...\n";
    }

    // Switch to Operation mode
    std::cout << "Start Operation mode\n-----\n";
    stat = wbrodevice.Wbro_Dev_uC_send_to(std::string("3").c_str());
    std::this_thread::sleep_for(5000ms);

    for (int i = 0; i < 30; i++) {
        stat = wbrodevice.Wbro_Dev_uC_read_from(sig, c_sig);
        std::cout << "RawSignal: " << sig << std::endl;
        stat = wbrodevice.Wbro_Dev_uC_getInfo(sig, info);
        std::cout << "GotInfoFromUC -> mode: " << info.mode << " status: " << info.status << " Distance[cm]: " << info.distance << "\n";
        if (info.mode == M_OPERATION && info.status == '1' && info.distance < DISTANCE_TO_CART) {
            wbrodevice.Wbro_Dev_Cam_take_image(cam);
            wbrodevice.Wbro_Dev_Cam_save_image(cam, std::string("image_opr_"+std::to_string(i)+".png").c_str());
        }
        std::this_thread::sleep_for(50ms);
        std::cout << ">> operation mode...\n";
    }

    // Close work
    stat = wbrodevice.Wbro_Dev_Cam_disconnect(devInfoList, svCamSysList, tlIDList);
    stat = wbrodevice.Wbro_Dev_Cam_close();
    stat = wbrodevice.Wbro_Dev_Com_disconnect();

    return 0;
}