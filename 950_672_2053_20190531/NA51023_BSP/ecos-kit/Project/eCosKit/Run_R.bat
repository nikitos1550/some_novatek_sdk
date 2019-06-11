@echo off
set CurrentDIR=%CD%
if "%TTERM_PATH%" == "" (
set TTERM_PATH=C:\Program Files\teraterm
)
pushd %TTERM_PATH%
start ttpmacro.exe /V %CurrentDIR%\LoadCode.ttl %CurrentDIR:\=/% Release Resume
set CurrentDIR=
popd