#include <iostream>
#include "gtest/gtest.h"
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


TEST_F(CoreTest, calc_test){
    WbroCore core;
    EXPECT_NEAR(core.multiply(5.1, 3.0), 15.3, 0.001);
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