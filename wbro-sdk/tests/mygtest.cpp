#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "wbrocore.h"
#include "wbrodevice.h"

class CoreTest : public testing::Test {
protected:
    CoreTest() {}
    ~CoreTest() override {}
    void SetUp() override {}
    void TearDown() override {}
};

class DeviceTest : public testing::Test {
protected:
    DeviceTest() {}
    ~DeviceTest() override {}
    void SetUp() override {}
    void TearDown() override {}
};


TEST_F(CoreTest, readpixel_test){
    WbroCore core;
    cv::Mat newImage = core.loadimage("../assets/sample.png");
    std::vector<int> pixel = core.readpixel3(&newImage, 50, 50);

    std::vector<int> vec_expected {110, 167, 206};
    EXPECT_EQ(pixel, vec_expected);
}

TEST_F(DeviceTest, verify_signal){
    WbroDevice device;
    std::string sig = "MySignal";
    device.sendSignal(sig);
    EXPECT_EQ(device.getSignal(), sig);
}


int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}