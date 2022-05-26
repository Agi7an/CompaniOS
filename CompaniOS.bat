@ECHO OFF
CLS
TITLE CompaniOS
ECHO [46;30m=============[0m
ECHO [46;30;1;97m^| CompaniOS ^|[0m
ECHO [46;30m=============[0m
ECHO.
ECHO.
ECHO [94m============[0m
ECHO [44mWindows Info[0m
ECHO [94m============[0m
systeminfo | findstr /c:"OS Name"
systeminfo | findstr /c:"OS Version"
systeminfo | findstr /c:"System Type"
ECHO.
ECHO [94m=============[0m
ECHO [44mHardware Info[0m
ECHO [94m=============[0m
systeminfo | findstr /c:"Total Physical Memory"
wmic cpu get name
wmic diskdrive get name,model,size
wmic path win32_videocontroller get name
ECHO [94m============[0m
ECHO [44mNetwork Info[0m
ECHO [94m============[0m
ipconfig | findstr IPv4
ipconfig | findstr IPv6
"d:\TCS\fourthSem\OS Lab\Package\CompaniOS\CompaniOS.exe"
PAUSE