@echo off
cls
type BATCHLST.TXT
choice /C:12345670 /N Please select what to build:
echo.
if ERRORLEVEL 8 goto end
if ERRORLEVEL 1 set TARGET=DN3D13B
if ERRORLEVEL 2 set TARGET=DN3D14B
if ERRORLEVEL 3 set TARGET=DN3D14G
if ERRORLEVEL 4 set TARGET=DN3D15
if ERRORLEVEL 5 set TARGET=NAM10
if ERRORLEVEL 6 set TARGET=WW2GI10
if ERRORLEVEL 7 set TARGET=EDK20023

set EXENAME=game.exe

if %TARGET% == DN3D13B goto dn3d13b
if %TARGET% == DN3D14B goto dn3d14b
if %TARGET% == DN3D14G goto dn3d14g
goto task
:dn3d13b
:dn3d14b
if not exist ..\build\19960724\build.obj goto engineerror
if not exist ..\build\19960724\cache1d.obj goto engineerror
if not exist ..\build\19960724\engine.obj goto engineerror
set EXENAME=build.exe
goto task
:dn3d14g
if not exist ..\build\19960724\cache1d.obj goto engineerror
if not exist ..\build\19960820\engine.obj goto engineerror
goto task
:engineerror
echo Can't recreate Duke3D game or Build Editor EXE, you need
echo compatible Build Engine object files ready under ..\build.
goto end
:task
mkdir %TARGET%
wmake.exe %TARGET%\%EXENAME% "appver_exedef = %TARGET%"
:end
