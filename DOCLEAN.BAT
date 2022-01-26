@echo off
set MSG=Please select what to clean:
call REVSEL.BAT

if "%CHOICE%" == "" goto end

del %CHOICE%\*.*
:end
set CHOICE=
