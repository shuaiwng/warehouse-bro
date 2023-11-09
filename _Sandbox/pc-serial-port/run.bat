SET PUTTY_PATH=C:\Program Files\PuTTY

START "" "%PUTTY_PATH%\putty.exe" -serial "COM3" -sercfg "115200,8,n,1,N"
sleep(1)
taskkill /f /im putty.exe

%~dp0/build/serial.exe

pause