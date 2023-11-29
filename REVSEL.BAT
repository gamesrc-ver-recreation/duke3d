@echo off
if "%MSG%" == "" goto error
set CHOICE=
cls
echo [1] Duke Nukem 3D v1.3d Build Editor (Watcom 10.0)
echo [2] Duke Nukem 3D: Atomic Edition v1.4 (and 1.5) Build Editor (Watcom 10.0)
echo [3] Duke Nukem 3D: Atomic Edition v1.4 (Watcom 10.0)
echo [4] Duke Nukem 3D: Atomic Edition v1.5 (Watcom 10.0)
echo [5] NAM (NAPALM) Full Version 1.0 (Watcom 10.6)
echo [6] WW2GI Full Version 1.0 (Watcom 10.6)
echo [7] Enhanced Duke (EDuke) 2.00.23 (Watcom 10.6)
echo.
echo [0] Cancel and quit
echo.
echo %MSG%
set MSG=
choice /S /C:12345670 /N
echo.

if ERRORLEVEL 8 goto end
if ERRORLEVEL 7 goto EDK20023
if ERRORLEVEL 6 goto WW2GI10
if ERRORLEVEL 5 goto NAM10
if ERRORLEVEL 4 goto DN3D15
if ERRORLEVEL 3 goto DN3D14G
if ERRORLEVEL 2 goto DN3D14B
if ERRORLEVEL 1 goto DN3D13B

:DN3D13B
set CHOICE=DN3D13B
goto end
:DN3D14B
set CHOICE=DN3D14B
goto end
:DN3D14G
set CHOICE=DN3D14G
goto end
:DN3D15
set CHOICE=DN3D15
goto end
:NAM10
set CHOICE=NAM10
goto end
:WW2GI10
set CHOICE=WW2GI10
goto end
:EDK20023
set CHOICE=EDK20023
goto end

:error
echo This script shouldn't be run independently

:end
