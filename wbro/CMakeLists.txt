cmake_minimum_required(VERSION 3.12)

project(
    wbro
    VERSION 1.0
    LANGUAGES CXX
)

add_executable(MyTest ${SOURCE_FILES} mytest.cpp)

include_directories(src src/wbrocore src/wbrodevice)

add_subdirectory(src/wbrocore)
add_subdirectory(src/wbrodevice)

target_link_libraries(MyTest WbroCore WbroDevice)
