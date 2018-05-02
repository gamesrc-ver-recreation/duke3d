@echo off
if "%1"=="NAM10" goto task
if "%1"=="WW2GI10" goto task
if "%1"=="EDK20023" goto task
echo You need to pass a supported UPPERCASE directory name as an argument.
echo Currently accepted: NAM10, WW2GI10, EDK20023
goto end
:task
md %1
wmake.exe clean "appver_exedef = %1"
:end
