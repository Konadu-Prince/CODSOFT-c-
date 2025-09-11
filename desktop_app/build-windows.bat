@echo off
REM Library Management System - Windows Build Script
REM This script builds the desktop application for Windows

echo 🚀 Building Library Management System for Windows...

REM Check if Qt6 is installed
where qmake >nul 2>nul
if %errorlevel% neq 0 (
    echo ❌ Qt6 not found. Please install Qt6 development tools.
    echo Download from: https://www.qt.io/download-qt-installer
    echo Make sure to add Qt6 to your PATH environment variable.
    pause
    exit /b 1
)

REM Check if CMake is installed
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo ❌ CMake not found. Please install CMake.
    echo Download from: https://cmake.org/download/
    pause
    exit /b 1
)

REM Check if Visual Studio Build Tools are available
where cl >nul 2>nul
if %errorlevel% neq 0 (
    echo ❌ Visual Studio Build Tools not found.
    echo Please install Visual Studio Build Tools or Visual Studio Community.
    echo Download from: https://visualstudio.microsoft.com/downloads/
    pause
    exit /b 1
)

echo ✅ All dependencies found!

REM Create build directory
if exist build rmdir /s /q build
mkdir build
cd build

echo 🔧 Configuring with CMake...

REM Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo ❌ CMake configuration failed.
    pause
    exit /b 1
)

echo 🔨 Building application...

REM Build the application
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo ❌ Build failed.
    pause
    exit /b 1
)

echo ✅ Application built successfully!

REM Create Windows installer
echo 📦 Creating Windows installer...
cmake --build . --target package
if %errorlevel% neq 0 (
    echo ⚠️ Installer creation failed, but executable was built successfully.
    echo You can find the executable in: build\Release\LibraryManagementSystem.exe
) else (
    echo ✅ Windows installer created successfully!
    echo Installer location: build\LibraryManagementSystem-1.0.0-win64.exe
)

echo.
echo 🎉 Build process completed!
echo.
echo 📁 Output files:
echo    - Executable: build\Release\LibraryManagementSystem.exe
echo    - Installer: build\LibraryManagementSystem-1.0.0-win64.exe
echo.
echo 🚀 You can now distribute your Windows application!
pause
