@echo off
cls
type BATCHLST.TXT
choice /C:12345670 /N Please select what to clean:
echo.
if ERRORLEVEL 8 goto end
if ERRORLEVEL 1 set DIR=DN3D13B
if ERRORLEVEL 2 set DIR=DN3D14B
if ERRORLEVEL 3 set DIR=DN3D14G
if ERRORLEVEL 4 set DIR=DN3D15
if ERRORLEVEL 5 set DIR=NAM10
if ERRORLEVEL 6 set DIR=WW2GI10
if ERRORLEVEL 7 set DIR=EDK20023

del %DIR%\*.*
:end
