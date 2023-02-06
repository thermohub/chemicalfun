if "%APPVEYOR_BUILD_WORKER_IMAGE%"=="Visual Studio 2017" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
)
if "%APPVEYOR_BUILD_WORKER_IMAGE%"=="Visual Studio 2019" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
)

echo "Configuring..."
cmake -G"Visual Studio 16 2019" -DCHEMICALFUN_PYTHON_INSTALL_PREFIX:PATH="C:/Miniconda3-x64/envs/chemicalfun/" -A x64 -S . -DUSE_SPDLOG_PRECOMPILED=ON -B build
echo "Building..." 
cmake --build build --config %CONFIGURATION% --target install
