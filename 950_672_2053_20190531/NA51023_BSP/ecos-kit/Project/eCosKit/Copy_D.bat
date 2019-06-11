@echo off
if "%NVT_STG_PATH%" == "" (
echo Please set NVT_STG_PATH in Windows environment
echo.
echo Input removable storage path. For example H:/
set /p NVT_STG_PATH="> "
)
cp -f -v ./eCosKit_Data/Debug/FW96650T.bin %NVT_STG_PATH%
pause