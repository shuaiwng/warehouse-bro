SET MINGW_PATH=C:\Home\Tools\x86_64-13.2.0-release-win32-seh-ucrt-rt_v11-rev0\mingw64\bin
SET PATH=%PATH%;%MINGW_PATH%

mingw32-make

set dlllist=libgcc_s_seh-1.dll,libstdc++-6.dll
@for %%f in (%dlllist%) do (
        copy "%MINGW_PATH%\%%f" "%%f"
		)

pause