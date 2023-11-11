SET PATH=%PATH%;C:\Users\%USERNAME%\AppData\Local\Programs\Python\Python312
SET PATH=%PATH%;C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2021.10\bin
SET PATH=%PATH%;C:\Home\Tools\x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0\mingw64\bin
SET PATH=%PATH%;C:\Program Files\CMake\bin
SET PICO_SDK_PATH=C:\Home\Tools\pico-sdk

if not exist build mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make -j4

pause