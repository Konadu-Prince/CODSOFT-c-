#!/bin/bash

# Library Management System - Windows Cross-Compilation Script
# This script builds the Windows version using MinGW-w64 on Linux

set -e

echo "🚀 Building Library Management System for Windows (Cross-Compilation)..."

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're on Linux
if [[ "$OSTYPE" != "linux-gnu"* ]]; then
    print_error "This script is designed for Linux cross-compilation to Windows."
    print_status "For native Windows builds, use build-windows.bat"
    exit 1
fi

# Install MinGW-w64 and Qt6 for Windows
install_dependencies() {
    print_status "Installing Windows cross-compilation dependencies..."
    
    # Detect package manager and install dependencies
    if command -v apt &> /dev/null; then
        sudo apt update
        sudo apt install -y \
            mingw-w64 \
            qt6-base-dev \
            qt6-tools-dev \
            cmake \
            build-essential \
            libsqlite3-dev \
            wine \
            nsis
    elif command -v dnf &> /dev/null; then
        sudo dnf install -y \
            mingw64-gcc-c++ \
            mingw64-qt6-qtbase \
            mingw64-qt6-qttools \
            cmake \
            gcc-c++ \
            sqlite-devel \
            wine \
            nsis
    elif command -v pacman &> /dev/null; then
        sudo pacman -S --needed \
            mingw-w64-gcc \
            mingw-w64-qt6-base \
            mingw-w64-qt6-tools \
            cmake \
            base-devel \
            sqlite \
            wine \
            nsis
    else
        print_warning "Unknown package manager. Please install MinGW-w64 and Qt6 manually."
    fi
    
    print_success "Dependencies installed"
}

# Setup cross-compilation environment
setup_cross_compile() {
    print_status "Setting up cross-compilation environment..."
    
    # Set MinGW-w64 paths
    export MINGW_PREFIX="/usr/x86_64-w64-mingw32"
    export PATH="$MINGW_PREFIX/bin:$PATH"
    
    # Set Qt6 paths for Windows
    export QT6_WINDOWS_PREFIX="/usr/x86_64-w64-mingw32/lib/qt6"
    
    # Check if MinGW-w64 is available
    if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
        print_error "MinGW-w64 not found. Please install mingw-w64 package."
        exit 1
    fi
    
    print_success "Cross-compilation environment ready"
}

# Create build directory
setup_build() {
    print_status "Setting up build environment..."
    
    if [ -d "build-windows" ]; then
        print_status "Cleaning previous Windows build..."
        rm -rf build-windows
    fi
    
    mkdir -p build-windows
    cd build-windows
    
    print_success "Build environment ready"
}

# Configure with CMake for Windows
configure_cmake() {
    print_status "Configuring with CMake for Windows..."
    
    # Create toolchain file
    cat > ../mingw-w64-toolchain.cmake << EOF
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_PREFIX_PATH /usr/x86_64-w64-mingw32/lib/qt6)
EOF
    
    cmake .. \
        -DCMAKE_TOOLCHAIN_FILE=../mingw-w64-toolchain.cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32
    
    print_success "CMake configuration complete"
}

# Build the application
build_app() {
    print_status "Building Windows application..."
    
    make -j$(nproc)
    
    print_success "Windows application built successfully"
}

# Create Windows installer
create_installer() {
    print_status "Creating Windows installer..."
    
    # Create NSIS script
    cat > ../installer.nsi << 'EOF'
!define APPNAME "Library Management System"
!define COMPANYNAME "CODSOFT"
!define DESCRIPTION "A modern desktop application for managing library collections"
!define VERSIONMAJOR 1
!define VERSIONMINOR 0
!define VERSIONBUILD 0
!define HELPURL "https://github.com/Konadu-Prince/CODSOFT-c-"
!define UPDATEURL "https://github.com/Konadu-Prince/CODSOFT-c-/releases"
!define ABOUTURL "https://www.codsoft.in"
!define INSTALLSIZE 50000

RequestExecutionLevel admin
InstallDir "$PROGRAMFILES64\${APPNAME}"
Name "${APPNAME}"
Icon "icon.ico"
outFile "LibraryManagementSystem-1.0.0-win64.exe"

!include LogicLib.nsh

page directory
page instfiles

!macro VerifyUserIsAdmin
UserInfo::GetAccountType
pop $0
${If} $0 != "admin"
    messageBox mb_iconstop "Administrator rights required!"
    setErrorLevel 740
    quit
${EndIf}
!macroend

function .onInit
    setShellVarContext all
    !insertmacro VerifyUserIsAdmin
functionEnd

section "install"
    setOutPath $INSTDIR
    file "LibraryManagementSystem.exe"
    file "*.dll"
    
    writeUninstaller "$INSTDIR\uninstall.exe"
    
    createDirectory "$SMPROGRAMS\${APPNAME}"
    createShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\LibraryManagementSystem.exe"
    createShortCut "$SMPROGRAMS\${APPNAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe"
    createShortCut "$DESKTOP\${APPNAME}.lnk" "$INSTDIR\LibraryManagementSystem.exe"
    
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "InstallLocation" "$\"$INSTDIR$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayIcon" "$\"$INSTDIR\LibraryManagementSystem.exe$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "Publisher" "${COMPANYNAME}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "HelpLink" "${HELPURL}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "URLUpdateInfo" "${UPDATEURL}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "URLInfoAbout" "${ABOUTURL}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayVersion" "${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "VersionMajor" ${VERSIONMAJOR}
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "VersionMinor" ${VERSIONMINOR}
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoRepair" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "EstimatedSize" ${INSTALLSIZE}
sectionEnd

section "uninstall"
    delete "$INSTDIR\LibraryManagementSystem.exe"
    delete "$INSTDIR\*.dll"
    delete "$INSTDIR\uninstall.exe"
    
    delete "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk"
    delete "$SMPROGRAMS\${APPNAME}\Uninstall.lnk"
    delete "$DESKTOP\${APPNAME}.lnk"
    rmDir "$SMPROGRAMS\${APPNAME}"
    
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
    
    rmDir $INSTDIR
sectionEnd
EOF
    
    # Copy Qt6 DLLs
    print_status "Copying Qt6 DLLs..."
    cp /usr/x86_64-w64-mingw32/lib/qt6/bin/*.dll . 2>/dev/null || true
    
    # Create installer
    makensis ../installer.nsi
    
    print_success "Windows installer created: LibraryManagementSystem-1.0.0-win64.exe"
}

# Main build process
main() {
    print_status "Starting Windows cross-compilation process..."
    
    # Check if we're in the right directory
    if [ ! -f "CMakeLists.txt" ]; then
        print_error "CMakeLists.txt not found. Please run this script from the desktop_app directory."
        exit 1
    fi
    
    # Parse command line arguments
    INSTALL_DEPS=false
    
    while [[ $# -gt 0 ]]; do
        case $1 in
            --deps)
                INSTALL_DEPS=true
                shift
                ;;
            --help)
                echo "Usage: $0 [OPTIONS]"
                echo "Options:"
                echo "  --deps     Install cross-compilation dependencies"
                echo "  --help     Show this help message"
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                exit 1
                ;;
        esac
    done
    
    # Install dependencies if requested
    if [ "$INSTALL_DEPS" = true ]; then
        install_dependencies
    fi
    
    # Setup cross-compilation
    setup_cross_compile
    
    # Setup build
    setup_build
    
    # Configure and build
    configure_cmake
    build_app
    
    # Create installer
    create_installer
    
    print_success "Windows build process completed successfully!"
    print_status "You can now distribute your Windows application."
    print_status "Files created:"
    print_status "  - Executable: build-windows/LibraryManagementSystem.exe"
    print_status "  - Installer: build-windows/LibraryManagementSystem-1.0.0-win64.exe"
}

# Run main function
main "$@"
