@Echo off

REM ##### Color support #####
SETLOCAL EnableDelayedExpansion
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do     rem"') do (
  set "DEL=%%a"
)
SET aqua=0b
SET light_green=0a
SET red=04
SET yellow=0e
SET title_color=%aqua%
SET header_color=%aqua%
REM ##### Color support #####

REM ##### local vars #####
set windows_sdk_version=0
set update_submodules=0
set clean_build=0
set run_doxygen=0

REM if left blank it stays in the root folder of the .bat file
set workspace_path=""
set build_folder="build"
REM ##### local vars #####








call :colorEcho %title_color% "====================================="
call :colorEcho %title_color% "==         INSPRO Installer        =="
call :colorEcho %title_color% "====================================="

REM ##### install #####
call :getWindowsSDK

call :colorEcho %aqua% "#### Options ####"
call :optionCleanBuild
REM # call :optionUpdateSubmodules
REM # call :optionDoxygen

call :doCleanBuild
REM # call :updateSubmodules
REM #call :runDoxygen

call :genVS17Win64

call :colorEcho %light_green% "Installation Finished!"
echo.

pause
EXIT

REM ##### install #####










rem ##### get latest windows SDK #####
:getWindowsSDK
call :colorEcho %aqua% "#### Gathering your latest Windows SDK version ####"
FOR /F "delims=" %%i IN ('dir "C:\Program Files (x86)\Windows Kits\10\Include" /b /ad-h /t:c /o-n') DO (
    SET windows_sdk_version=%%i
    GOTO :found
)
call :colorEcho %red% No Windows SDK found in location: C:\Program Files (x86)\Windows Kits\10\Include
EXIT 1
:found
echo Windows SDK required:		10.0.18362.0 or newer
echo Latest installed Windows SDK:	%windows_sdk_version%
echo.
EXIT /B 0
rem ##### get latest windows SDK #####


REM ##### Option to update submodules #####
:optionUpdateSubmodules
choice /c yn /m "Do you want to update submodules?"
if errorlevel 2 (
  call :colorEcho %yellow% "Submodules will NOT be updated"
) else (
  set update_submodules=1
  call :colorEcho %yellow% "Submodules will be updated"
)
echo.
EXIT /B 0
REM ##### Option to update submodules #####

REM ##### UPDATE SUBMODULES #####
:updateSubmodules
if "%update_submodules%" == "1" (
  call :colorEcho %aqua% "#### Update submodules ####"
  cd "%workspace_path%"
  git submodule init
  git submodule update --recursive
  echo.
)
EXIT /B 0
REM ##### UPDATE SUBMODULES #####


REM ##### Option to do a clean build #####
:optionCleanBuild
choice /c yn /m "Do you want a clean build?"
if errorlevel 2 (
  call :colorEcho %yellow% "The current build will be overwritten"
) else (
  set clean_build=1
  call :colorEcho %yellow% "The current build will be removed and a new build will be generated"
)
echo.
EXIT /B 0
REM ##### Option to do a clean build #####

REM ##### DO CLEAN BUILD #####
:doCleanBuild
if "%clean_build%" == "1" (
  call :colorEcho %aqua% "#### Do a clean build ####"
  if exist ".\%build_folder%\" (
  @RD /S /Q "".\%build_folder%"
)
  echo.
)
EXIT /B 0
REM ##### DO CLEAN BUILD #####


REM ##### Option to generate doxygen #####
:optionDoxygen
choice /c yn /m "Do you want to generate documentation with Doxygen?"
if errorlevel 2 (
  call :colorEcho %yellow% "Documentation will NOT be generated."
) else (
  set run_doxygen=1
  call :colorEcho %yellow% "Documentation will be generated."
)
echo.
EXIT /B 0
REM ##### Option to generate doxygen #####

REM ##### GENERATE DOXYGEN #####
:runDoxygen
if "%run_doxygen%" == "1" (
  call :colorEcho %aqua% "#### Generate documentation (Doxygen) ####"
  call doxygen.bat
  echo.
)
EXIT /B 0
REM ##### GENERATE DOXYGEN #####


REM ##### GEN PROJECTS #####
:genVS17Win64
call :colorEcho %header_color% "#### Generating Visual Studio 16 2019 Win64 Project. ####"
cd "%workspace_path%"
echo current path: "%cd%"
if exist ".\%build_folder%\" (
  del ".\%build_folder%\CMakeCache.txt"
)
mkdir %build_folder%
cd "%build_folder%"
echo cmake -DCMAKE_SYSTEM_VERSION=%windows_sdk_version% -G "Visual Studio 16 2019" -A x64 ..
cmake -DCMAKE_SYSTEM_VERSION=%windows_sdk_version% -G "Visual Studio 16 2019" -A x64 ..

if errorlevel 1 call :colorecho %red% "CMake finished with errors"
cd ..
echo.
EXIT /B 0

REM ##### GEN CMAKE PROJECTS #####


REM ##### COLOR SUPPORT #####
:colorEcho
<nul set /p ".=%DEL%" > "%~2"
findstr /v /a:%1 /R "^$" "%~2" nul
del "%~2" > nul 2>&1i
echo.
EXIT /B 0
REM ##### COLOR SUPPORT #####