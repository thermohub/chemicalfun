:: Since we really don't link against any of the built libraries, and we use specific
:: Versions of the github projects, we really shouldn't pollute the userspace with the
:: installed files. This could lead to a lot of trouble for fellow developers. Therefore
:: this batch file "installs" the built libraries in the project source folder.

@echo off
setlocal enableextensions
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
IF EXIST "%~dp0extern\" (
echo Folder extern already exists
) ELSE (
echo Creating folder extern
mkdir extern
)

cd extern

IF EXIST "%~dp0extern\installed\" (
echo Folder installed already exists
) ELSE (
echo Creating folder installed
mkdir installed
)

IF NOT EXIST "%~dp0extern\installed\include\nlohmann\json.hpp" (
:: Getting nlohmann/json library
echo Downloading json library && ^
git clone https://github.com/nlohmann/json.git --depth 1 && ^
cd json && ^
echo Making JSON library && ^
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DJSON_BuildTests=OFF -DJSON_MultipleHeaders=ON && ^
cmake --build build/ && ^
echo Installing in "%~dp0\extern\installed" && ^
cmake --install build/ --prefix "%~dp0extern\installed" && ^
cd "%~dp0extern" && ^
rmdir json /s /q

) ELSE (
echo extern\json\include\nlohmann\json.hpp already exists
)

cd "%~dp0extern"
 
:: Eigen3 math library (added for building and installing xGEMS)
:: if not installed in /usr/local/include/eigen3)
IF NOT EXIST "%~dp0extern\installed\include\eigen\" (
:: Building eigen library
echo Downloading eigen library
git clone https://gitlab.com/libeigen/eigen.git -b 3.4.0 --depth 1 && ^
cd eigen && ^
cmake -S . -B build && ^
cmake --build build/ && ^
echo Installing in "%~dp0extern\installed" && ^
cmake --install build/ --prefix "%~dp0extern\installed" && ^
cd "%~dp0extern" && ^
rmdir eigen /s /q

) ELSE (
echo eigen already exists
)

cd "%~dp0extern"

:: spdlog 
:: if no spdlog installed in /usr/local/include/spdlog (copy only headers)
IF NOT EXIST "%~dp0extern\installed\include\spdlog\" (
:: Building spdlog library
echo Downloading eigen library
git clone https://github.com/gabime/spdlog -b v1.11.0 --depth 1 && ^
cd spdlog && ^
cd include && ^
IF NOT EXIST "%~dp0extern\installed\include\spdlog\" mkdir "%~dp0extern\installed\include\spdlog" && ^
xcopy /s/e spdlog "%~dp0extern\installed\include\spdlog" && ^
cd "%~dp0extern" && ^
rmdir spdlog /s /q

) ELSE (
echo spdlog already exists
)

::Pybind11
IF NOT EXIST "%~dp0extern\installed\include\pybind11\" (
:: Building spdlog library
echo Downloading pybind11 library
git clone https://github.com/pybind/pybind11.git --depth 1 && ^
cd pybind11 && ^
cmake -S . -B build -DPYBIND11_TEST=OFF && ^
cmake --build build/ && ^
echo Installing in "%~dp0extern\installed" && ^
cmake --install build/ --prefix "%~dp0extern\installed" && ^
cd "%~dp0extern" && ^
rmdir pybind11 /s /q

) ELSE (
echo pybind11 already exists
)

cd "%~dp0"
