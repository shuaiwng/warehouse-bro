cmake_minimum_required(VERSION 3.12)

project(
    wbro
    VERSION 1.0
    LANGUAGES CXX
)

SET(GTEST_DIR "C:/Home/Tools/googletest-1.14.0")

add_executable(MyTest ${SOURCE_FILES} tests/mygtest.cpp)

include_directories(
    src/wbrocore
    src/wbrodevice
    ${GTEST_DIR}/googletest/include
)

add_subdirectory(src/wbrocore)
add_subdirectory(src/wbrodevice)

target_link_libraries(MyTest WbroCore WbroDevice)
target_link_libraries(MyTest ${GTEST_DIR}/lib/libgtest.a)

