@echo off
set CurrentDIR=%CD%
if "%TTERM_PATH%" == "" (
set TTERM_PATH=C:\Program Files\teraterm
)
pushd %TTERM_PATH%
ttpmacro.exe /V %CurrentDIR%\IceMode.ttl
popd
set OLD_PATH=%PATH%
PATH=C:\NMake;%PATH%
start sde-insight --command=init_IceMode.gdb %CurrentDIR%/eCosKit_Data/Release/eCosKit.axf
PATH=%OLD_PATH%
set CurrentDIR=
set OLD_PATH=