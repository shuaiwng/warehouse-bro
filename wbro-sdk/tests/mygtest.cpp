#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "wbrocore.h"
#include "wbrodevice.h"

class CoreTest : public testing::Test {
protected:
    WbroCore core;
    CoreTest() {}
    ~CoreTest() override {}
    void SetUp() override {}
    void TearDown() override {}
};

class DeviceTest : public testing::Test {
protected:
    WbroDevice device;
    ERR_DEVICE err_device;

    std::vector<SV_DEVICE_INFO *> devInfoList;
    std::vector<SVCamSystem *> svCamSysList;
    std::vector<char *> tlIDList;

    DeviceTest() {}
    ~DeviceTest() override {}
    void SetUp() override {
        err_device = device.Wbro_Dev_ConnectToCom("COM3");
        
        err_device = device.Wbro_Dev_Cam_disconnect(devInfoList, svCamSysList, tlIDList);
        err_device = device.Wbro_Dev_Cam_init();
    }
    void TearDown() override {
        err_device = device.Wbro_Dev_DisconnectCom();
        err_device = device.Wbro_Dev_Cam_disconnect(devInfoList, svCamSysList, tlIDList);
    }
};


TEST_F(CoreTest, test_readpixel){
    cv::Mat newImage = core.Wbro_loadimage("../assets/sample.png");
    std::vector<int> pixel = core.Wbro_readpixel3(&newImage, 50, 50);
    std::vector<int> vec_expected {110, 167, 206};
    EXPECT_EQ(pixel, vec_expected);
}

TEST_F(DeviceTest, test_com_connect){
    err_device = device.Wbro_Dev_DisconnectCom();
    EXPECT_EQ(err_device, ERR_SUCCESS);
    err_device = device.Wbro_Dev_ConnectToCom("COM3");
    EXPECT_EQ(err_device, ERR_SUCCESS);
}

TEST_F(DeviceTest, test_read_signal){
    std::string sig = "";
    DWORD c_sig = 0;
    err_device = device.Wbro_Dev_ReadSignal(100, sig, c_sig);
    EXPECT_EQ(err_device, ERR_SUCCESS);
}


TEST_F(DeviceTest, test_camera){

    ERR_DEVICE b_discover = device.Wbro_Dev_Cam_discover(devInfoList, svCamSysList, tlIDList);
    EXPECT_EQ(b_discover, ERR_SUCCESS);

    Camera* cam;
    ERR_DEVICE b_connect = device.Wbro_Dev_Cam_connect(devInfoList, svCamSysList, tlIDList, cam);
    EXPECT_EQ(b_connect, ERR_SUCCESS);

    ERR_DEVICE b_set = device.Wbro_Dev_Cam_setParam(cam, 1000000); // 1s
    EXPECT_EQ(b_set, ERR_SUCCESS);

    ERR_DEVICE b_meas = device.Wbro_Dev_Cam_take_image(cam, 1000000); // 1s
    EXPECT_EQ(b_meas, ERR_SUCCESS);

    std::string filename = "saved_image.png";
    ERR_DEVICE b_save = device.Wbro_Dev_Cam_save_image(cam, filename.c_str());
    EXPECT_EQ(b_save, ERR_SUCCESS);
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}