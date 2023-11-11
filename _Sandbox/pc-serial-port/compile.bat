SET MINGW_PATH=C:\Home\Tools\x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0\mingw64\bin
SET PATH=%PATH%;%MINGW_PATH%

SET SVS_HEADER_PATH=C:\Program Files\SVS-VISTEK GmbH\SVCam Kit\SDK\include
SET SVS_LIB_PATH=C:\Program Files\SVS-VISTEK GmbH\SVCam Kit\SDK\bin


mingw32-make all HPATH="%SVS_HEADER_PATH%" LIBPATH="%SVS_LIB_PATH%"

if not exist build mkdir build

set bin_std=libgcc_s_seh-1.dll,libstdc++-6.dll,libwinpthread-1.dll
@for %%f in (%bin_std%) do (
        copy "%MINGW_PATH%\%%f" "build\%%f"
		)

set bin_svs=libpng16.dll,SVGenSDK64.dll,SVLog64.dll
@for %%f in (%bin_svs%) do (
        copy "%SVS_LIB_PATH%\%%f" "build/%%f"
		)

for %%f in (*.dll) do move "%%f" "build/"
for %%f in (*.exe) do move "%%f" "build/"

for %%f in (*.o) do del "%%f"
