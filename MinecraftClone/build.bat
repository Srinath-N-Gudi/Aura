@echo off
echo === Building MinecraftClone ===

REM Add tools to PATH
set PATH=C:\MinGW\bin;C:\Program Files\CMake\bin;%PATH%

REM Clean and create build directory
if exist "build" rmdir /s /q "build"
mkdir build
cd build

REM Configure with CMake
echo.
echo Configuring...
cmake .. -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

REM Build
echo.
echo Building...
cmake --build .
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo === Build successful! ===
echo Run: build\MinecraftClone.exe
echo.
pause
