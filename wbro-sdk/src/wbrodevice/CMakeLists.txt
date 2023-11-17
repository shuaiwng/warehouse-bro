
file(GLOB SOURCE_FILES wbrodevice.cpp wbrodevice.h svscam/*.h svscam/*.cpp ${SVSCAM_DIR}/include/sv_gen_sdk.h)

add_library(WbroDevice STATIC ${SOURCE_FILES})