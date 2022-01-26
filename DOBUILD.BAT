@echo off
set MSG=Please select what to build:
call REVSEL.BAT

if "%CHOICE%" == "" goto end

set EXENAME=game.exe

if %CHOICE% == DN3D13B goto dn3d13b
if %CHOICE% == DN3D14B goto dn3d14b
if %CHOICE% == DN3D14G goto dn3d14g
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
mkdir %CHOICE%
wmake.exe %CHOICE%\%EXENAME% "appver_exedef = %CHOICE%"
:end
set CHOICE=
