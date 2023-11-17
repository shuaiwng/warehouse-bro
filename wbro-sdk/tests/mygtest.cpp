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
    DeviceTest() {}
    ~DeviceTest() override {}
    void SetUp() override {
        err_device = device.Wbro_Dev_ConnectToCom("COM3");
    }
    void TearDown() override {
        err_device = device.Wbro_Dev_DisconnectCom();
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
    err_device = device.Wbro_Dev_Cam_init();
    EXPECT_EQ(err_device, ERR_SUCCESS);
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}