@echo off
set CurrentDIR=%CD%
pushd C:\Program Files\teraterm
ttpmacro.exe /V %CurrentDIR%\IceMode.ttl
popd
set OLD_PATH=%PATH%
PATH=C:\NMake;C:\mips-sde\bin;%PATH%
start sde-insight --command=init_IceMode.gdb %CurrentDIR%/DemoKit_Data/Debug/DemoKit_D.axf
PATH=%OLD_PATH%
set CurrentDIR=
set OLD_PATH=