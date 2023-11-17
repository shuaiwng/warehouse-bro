cmake_minimum_required(VERSION 3.12)

project(
    wbro
    VERSION 1.0
    LANGUAGES CXX
)

SET(GTEST_DIR "C:/Home/Tools/googletest-1.14.0")
SET(OPENCV_DIR "C:/Home/Tools/opencv/build")
SET(SVSCAM_DIR "C:/Program Files/SVS-VISTEK GmbH/SVCam Kit/SDK")

add_executable(MyTest ${SOURCE_FILES} tests/mygtest.cpp)

include_directories(
    src/wbrocore
    src/wbrodevice
    ${GTEST_DIR}/googletest/include
    ${GTEST_DIR}/googlemock/include
    ${OPENCV_DIR}/include
)
include_directories(${SVSCAM_DIR}/include)
include_directories(src/wbrodevice/svscam)

add_subdirectory(src/wbrocore)
add_subdirectory(src/wbrodevice)

target_link_libraries(MyTest WbroCore WbroDevice ${SVSCAM_DIR}/bin/SVGenSDK64.dll)

target_link_libraries(MyTest 
    ${GTEST_DIR}/lib/libgtest.a
    ${GTEST_DIR}/lib/libgmock.a
    )
target_link_libraries(MyTest 
    ${OPENCV_DIR}/lib/libopencv_core470.dll.a
    ${OPENCV_DIR}/lib/libopencv_highgui470.dll.a
    ${OPENCV_DIR}/lib/libopencv_imgcodecs470.dll.a 
)


