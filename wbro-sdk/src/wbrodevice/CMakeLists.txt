
file(GLOB SOURCE_FILES wbrodevice.cpp svscam/*.cpp)

add_library(WbroDevice STATIC ${SOURCE_FILES})