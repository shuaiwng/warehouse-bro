SET MINGW_PATH=C:\Home\Tools\x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0\mingw64\bin
SET OPENCV_PATH=C:\Home\Tools\opencv\build\bin
SET PATH=%PATH%;C:\Program Files\CMake\bin
SET PATH=%PATH%;%MINGW_PATH%


if not exist build mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make -j4


set bin_std=libgcc_s_seh-1.dll,libstdc++-6.dll,libwinpthread-1.dll
@for %%f in (%bin_std%) do (
        copy "%MINGW_PATH%\%%f" "%%f"
		)

copy "%~dp0\build\src\wbrocore\libWbroCore.dll" %cd%
copy "%~dp0\build\src\wbrodevice\libWbroDevice.dll" %cd%

set bin_cv=libopencv_core470.dll,libopencv_imgcodecs470.dll,libopencv_imgproc470.dll
@for %%f in (%bin_cv%) do (
        copy "%OPENCV_PATH%\%%f" "%%f"
		)


pause