
file(GLOB SOURCE_FILES wbrodevice.cpp svcam/*.cpp)

add_library(WbroDevice STATIC ${SOURCE_FILES})