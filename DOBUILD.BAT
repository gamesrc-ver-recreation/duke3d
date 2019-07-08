@echo off
cls
type BATCHLST.TXT
choice /C:1234560 /N Please select what to build:
echo.
if ERRORLEVEL 7 goto end
if ERRORLEVEL 1 set TARGET=DN3D14B
if ERRORLEVEL 2 set TARGET=DN3D14G
if ERRORLEVEL 3 set TARGET=DN3D15
if ERRORLEVEL 4 set TARGET=NAM10
if ERRORLEVEL 5 set TARGET=WW2GI10
if ERRORLEVEL 6 set TARGET=EDK20023

if %TARGET% == DN3D14B goto dn3d14b
if %TARGET% == DN3D14G goto dn3d14g
goto task
:dn3d14b
if not exist ..\build\19960724\build.obj goto dn3d14error
if not exist ..\build\19960724\cache1d.obj goto dn3d14error
if not exist ..\build\19960724\engine.obj goto dn3d14error
goto task
:dn3d14g
if not exist ..\build\19960724\cache1d.obj goto dn3d14error
if not exist ..\build\19960820\engine.obj goto dn3d14error
goto task
:dn3d14error
echo Can't recreate Duke3D 1.4 game or Build Editor EXE, you need
echo compatible Build Engine object files ready under ..\build.
goto end
:task
mkdir %TARGET%
wmake.exe "appver_exedef = %TARGET%"
:end
