:: Since we really don't link against any of the built libraries, and we use specific
:: Versions of the github projects, we really shouldn't pollute the userspace with the
:: installed files. This could lead to a lot of trouble for fellow developers. Therefore
:: this batch file "installs" the built libraries in the project source folder.

@echo off
setlocal enableextensions
setlocal enabledelayedexpansion
:: Installing dependencies needed to build CemicalFun on WINDOWS
:: (actually we won't install anything, we will just clone and build files in the directory)

:: Temporarily uncomment rows for packages that need to be re-installed
::sudo rm -rf /usr/local/include/nlohmann
::sudo rm -rf /usr/local/include/eigen3/Eigen/Eigen
::sudo rm -rf /usr/local/include/pybind11
::sudo rm -rf /usr/local/include/spdlog

echo Downloading and building dependencies

:: nlohmann/json library v. 3.6.1 or up
:: if not located in /extern/json/include/nlohmann)
cd %~dp0
IF EXIST "%~dp0extern\" (
echo Folder extern already exists
) ELSE (
echo Creating folder extern
mkdir extern
)

cd %~dp0extern

IF DEFINED EMSDK (
IF EXIST "%~dp0extern\installedwasm\" (
echo Folder installedwasm already exists
) ELSE (
echo Creating folder installedwasm
mkdir installedwasm
)
) ELSE (
IF EXIST "%~dp0extern\installed\" (
echo Folder installed already exists
) ELSE (
echo Creating folder installedwasm
mkdir installed
)
)

IF DEFINED EMSDK (
:: Set CMake command line args
REM We don't need to build the *.pyd with emscripten on Windows because we would rather build that with pyodide on linux
REM echo EMSDK defined
REM "%EMSDK_PYTHON%" -c "import sysconfig; print(sysconfig.get_path('include'))" >> Output1.txt
REM for /f "usebackq delims=" %%a in ("%~dp0extern\Output1.txt") do set "PINCDIR=%%a"
REM DEL Output1.txt
REM "%EMSDK_PYTHON%" -c "import pathlib; import sys; v = sys.version.split('.'); p = pathlib.Path(r'%EMSDK_PYTHON%'); print(pathlib.Path(p.parent, p.stem + v[0] + v[1] + '.dll'))" >> Output1.txt
REM for /f "usebackq delims=" %%a in ("%~dp0extern\Output1.txt") do set "PLIBDIR=%%a"
REM DEL Output1.txt
REM "%EMSDK_PYTHON%" -c "import pathlib; import sys; p = pathlib.Path(r'%EMSDK_PYTHON%'); print(p.parent)" >> Output1.txt
REM for /f "usebackq delims=" %%a in ("%~dp0extern\Output1.txt") do set "PROOTDIR=%%a"
REM DEL Output1.txt
REM SET "CMAKE_ARGUMENTS=-DCMAKE_TOOLCHAIN_FILE="%EMSDK%/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -DCMAKE_CROSSCOMPILING_EMULATOR="%EMSDK_NODE%" -DPython_ROOT_DIR="!PROOTDIR!" -DPython_EXECUTABLE:FILEPATH="%EMSDK_PYTHON%" -DPython_INCLUDE_DIR="!PINCDIR!" -DPython_LIBRARY="!PLIBDIR!" -G Ninja"
REM Note: For whatever reason, calling emcmake cmake ... messes up the paths, so use the Toolchainfile below
SET "CMAKE_ARGUMENTS=-DCMAKE_TOOLCHAIN_FILE="%EMSDK%/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" -DCMAKE_CROSSCOMPILING_EMULATOR="%EMSDK_NODE%" -G Ninja"
SET "INSTALLPATHROOT=%~dp0extern\installedwasm"
) ELSE (
echo EMSDK not defined
SET "CMAKE_ARGUMENTS= "
SET "INSTALLPATHROOT=%~dp0extern\installed"
)

IF NOT EXIST "%INSTALLPATHROOT%\include\nlohmann\json.hpp" (
:: Getting nlohmann/json library
echo Downloading json library && ^
git clone https://github.com/nlohmann/json.git --depth 1 && ^
cd json && ^
echo Making JSON library && ^
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DJSON_BuildTests=OFF -DJSON_MultipleHeaders=ON %CMAKE_ARGUMENTS% && ^
cmake --build build/ && ^
echo Installing in "%INSTALLPATHROOT%" && ^
cmake --install build/ --prefix "%INSTALLPATHROOT%" && ^
cd "%~dp0extern" && ^
rmdir json /s /q

) ELSE (
echo extern\json\include\nlohmann\json.hpp already exists
)

cd "%~dp0extern"
 
:: Eigen3 math library (added for building and installing xGEMS)
:: if not installed in /usr/local/include/eigen3)
IF NOT EXIST "%INSTALLPATHROOT%\include\eigen3\\" (
:: Building eigen library
echo Downloading eigen library
git clone https://gitlab.com/libeigen/eigen.git -b 3.4.0 --depth 1 && ^
cd eigen && ^
cmake -S . -B build %CMAKE_ARGUMENTS% && ^
cmake --build build/ && ^
echo Installing in "%INSTALLPATHROOT%" && ^
cmake --install build/ --prefix "%INSTALLPATHROOT%" && ^
cd "%~dp0extern" && ^
rmdir eigen /s /q

) ELSE (
echo eigen already exists
)

cd "%~dp0extern"

:: spdlog 
:: if no spdlog installed in /usr/local/include/spdlog (copy only headers)
IF NOT EXIST "%INSTALLPATHROOT%\include\spdlog\\" (
:: Building spdlog library
echo Downloading eigen library
git clone https://github.com/gabime/spdlog --depth 1 && ^
cd spdlog && ^
cd include && ^
IF NOT EXIST "%INSTALLPATHROOT%\include\spdlog\" mkdir "%INSTALLPATHROOT%\include\spdlog" && ^
xcopy /s/e spdlog "%INSTALLPATHROOT%\include\spdlog" && ^
cd "%~dp0extern" && ^
rmdir spdlog /s /q

) ELSE (
echo spdlog already exists
)

REM We don't need this for WASM if we don't use the pyodide environemnt, which only works on linux currently
IF NOT DEFINED EMSDK (
::Pybind11
IF NOT EXIST "%INSTALLPATHROOT%\include\pybind11\\" (
:: Building spdlog library
echo Downloading pybind11 library
git clone https://github.com/pybind/pybind11.git --depth 1 && ^
cd pybind11 && ^
cmake -S . -B build -DPYBIND11_TEST=OFF %CMAKE_ARGUMENTS% && ^
cmake --build build/ && ^
echo Installing in "%INSTALLPATHROOT%" && ^
cmake --install build/ --prefix "%INSTALLPATHROOT%" && ^
cd "%~dp0extern" && ^
rmdir pybind11 /s /q

) ELSE (
echo pybind11 already exists
)
)
cd "%~dp0"
endlocal
endlocal