#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "wbrocore.h"
#include "wbrodevice.h"

namespace op_uc_info {
    bool operator==(ucInfo_t p1, ucInfo_t p2){
        if (p1.mode == p2.mode && p1.status == p2.status && p1.distance == p1.distance){
            return true;
        }
        return false;
    }
}


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
        err_device = device.Wbro_Dev_Com_connect("COM3");        
        err_device = device.Wbro_Dev_Cam_init();
    }
    void TearDown() override {
        err_device = device.Wbro_Dev_Com_disconnect();
        err_device = device.Wbro_Dev_Cam_close();
    }
};

// Tests for Core
TEST_F(CoreTest, test_readpixel){
    cv::Mat newImage = core.Wbro_loadimage("../assets/sample.png");
    std::vector<int> pixel = core.Wbro_readpixel3(&newImage, 50, 50);
    std::vector<int> vec_expected {110, 167, 206};
    EXPECT_EQ(pixel, vec_expected);
}

// Tests for Device
TEST_F(DeviceTest, test_com_connect){
    err_device = device.Wbro_Dev_Com_disconnect();
    EXPECT_EQ(err_device, ERR_SUCCESS);
    err_device = device.Wbro_Dev_Com_connect("COM3");
    EXPECT_EQ(err_device, ERR_SUCCESS);
}

TEST_F(DeviceTest, test_read_signal){
    std::string sig = "";
    DWORD c_sig = 0;
    err_device = device.Wbro_Dev_uC_read_from(sig, c_sig);
    EXPECT_EQ(err_device, ERR_SUCCESS);
}

TEST_F(DeviceTest, test_camera){
    
    ERR_DEVICE b_discover = device.Wbro_Dev_Cam_discover(devInfoList, svCamSysList, tlIDList);
    EXPECT_EQ(b_discover, ERR_SUCCESS);

    Camera* cam;
    ERR_DEVICE b_connect = device.Wbro_Dev_Cam_connect(devInfoList, svCamSysList, tlIDList, cam);
    EXPECT_EQ(b_connect, ERR_SUCCESS);

    ERR_DEVICE b_set = device.Wbro_Dev_Cam_setParam(cam, 100000); // us
    EXPECT_EQ(b_set, ERR_SUCCESS);

    ERR_DEVICE b_meas = device.Wbro_Dev_Cam_take_image(cam);
    EXPECT_EQ(b_meas, ERR_SUCCESS);

    std::string filename = "saved_image.png";
    ERR_DEVICE b_save = device.Wbro_Dev_Cam_save_image(cam, filename.c_str());
    EXPECT_EQ(b_save, ERR_SUCCESS);

    ERR_DEVICE b_disconnect = device.Wbro_Dev_Cam_disconnect(devInfoList, svCamSysList, tlIDList);
    EXPECT_EQ(b_disconnect, ERR_SUCCESS);
}


TEST_F(DeviceTest, test_communication){
    using namespace op_uc_info;
    std::string sig_1 = "[>>STB0<<]abc";
    std::string sig_2 = "[>>SWF1<<]";
    std::string sig_3 = "[>>OOF1<<]abc";
    std::string sig_4 = "[>>OWF0<<](200.0]";
    std::string sig_5 = "[>>OWF1<<](150.59)";
    std::string sig_6 = "[>>OWF1<<](";
    std::string sig_7 = "thisisaverylong testing string... [>>SWF0<<](1)";
    
    ucInfo_t info = {M_INVALID, 'A', 0};    
    ucInfo_t info_exp = {M_STANDBY, '0', 0};
    ERR_DEVICE b_getinfo = device.Wbro_Dev_uC_getInfo(sig_1, info);
    EXPECT_EQ(b_getinfo, ERR_SUCCESS);
    EXPECT_EQ(info == info_exp, true);

    info = {M_INVALID, 'A', 0};
    info_exp = {M_SURVEILLANCE, '1', 0};
    b_getinfo = device.Wbro_Dev_uC_getInfo(sig_2, info);
    EXPECT_EQ(b_getinfo, ERR_SUCCESS);
    EXPECT_EQ(info == info_exp, true);

    info = {M_INVALID, 'A', 0};
    info_exp = {M_INVALID, 'A', 0};
    b_getinfo = device.Wbro_Dev_uC_getInfo(sig_3, info);
    EXPECT_EQ(b_getinfo, ERR_UC_GETINFO);
    EXPECT_EQ(info == info_exp, true);

    info = {M_INVALID, 'A', 0};
    info_exp = {M_OPERATION, 'A', 0};
    b_getinfo = device.Wbro_Dev_uC_getInfo(sig_4, info);
    EXPECT_EQ(b_getinfo, ERR_UC_GETINFO);
    EXPECT_EQ(info == info_exp, true);

    info = {M_INVALID, 'A', 0};
    info_exp = {M_OPERATION, '1', 150.59};
    b_getinfo = device.Wbro_Dev_uC_getInfo(sig_5, info);
    EXPECT_EQ(b_getinfo, ERR_SUCCESS);
    EXPECT_EQ(info == info_exp, true);

    info = {M_INVALID, 'A', 0};
    info_exp = {M_OPERATION, 'A', 0};
    b_getinfo = device.Wbro_Dev_uC_getInfo(sig_6, info);
    EXPECT_EQ(b_getinfo, ERR_UC_GETINFO);
    EXPECT_EQ(info == info_exp, true);

    info = {M_INVALID, 'A', 0};
    info_exp = {M_INVALID, 'A', 0};
    b_getinfo = device.Wbro_Dev_uC_getInfo(sig_7, info);
    EXPECT_EQ(b_getinfo, ERR_SUCCESS);
    EXPECT_EQ(info == info_exp, true);

}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}