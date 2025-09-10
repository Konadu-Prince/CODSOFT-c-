#!/bin/bash

# Library Management System - Build Script
# This script builds the desktop application for multiple platforms

set -e

echo "🚀 Building Library Management System Desktop App..."

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

# Check if Qt6 is installed
check_qt6() {
    print_status "Checking for Qt6 installation..."
    
    if ! command -v qmake6 &> /dev/null && ! command -v qmake &> /dev/null; then
        print_error "Qt6 not found. Please install Qt6 development packages."
        print_status "Ubuntu/Debian: sudo apt install qt6-base-dev qt6-tools-dev"
        print_status "Fedora: sudo dnf install qt6-qtbase-devel qt6-qttools-devel"
        print_status "Arch: sudo pacman -S qt6-base qt6-tools"
        exit 1
    fi
    
    print_success "Qt6 found"
}

# Install dependencies
install_dependencies() {
    print_status "Installing build dependencies..."
    
    # Detect package manager and install dependencies
    if command -v apt &> /dev/null; then
        sudo apt update
        sudo apt install -y build-essential cmake qt6-base-dev qt6-tools-dev libsqlite3-dev
    elif command -v dnf &> /dev/null; then
        sudo dnf install -y gcc-c++ cmake qt6-qtbase-devel qt6-qttools-devel sqlite-devel
    elif command -v pacman &> /dev/null; then
        sudo pacman -S --needed base-devel cmake qt6-base qt6-tools sqlite
    else
        print_warning "Unknown package manager. Please install Qt6 and CMake manually."
    fi
    
    print_success "Dependencies installed"
}

# Create build directory
setup_build() {
    print_status "Setting up build environment..."
    
    if [ -d "build" ]; then
        print_status "Cleaning previous build..."
        rm -rf build
    fi
    
    mkdir -p build
    cd build
    
    print_success "Build environment ready"
}

# Configure with CMake
configure_cmake() {
    print_status "Configuring with CMake..."
    
    # Try to find Qt6
    if command -v qmake6 &> /dev/null; then
        QT_DIR=$(qmake6 -query QT_INSTALL_PREFIX)
    else
        QT_DIR=$(qmake -query QT_INSTALL_PREFIX)
    fi
    
    cmake .. \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_PREFIX_PATH="$QT_DIR" \
        -DCMAKE_INSTALL_PREFIX=/usr/local
    
    print_success "CMake configuration complete"
}

# Build the application
build_app() {
    print_status "Building application..."
    
    make -j$(nproc)
    
    print_success "Application built successfully"
}

# Create AppImage (Linux)
create_appimage() {
    print_status "Creating AppImage..."
    
    # Install linuxdeploy
    if ! command -v linuxdeploy &> /dev/null; then
        print_status "Installing linuxdeploy..."
        wget -q https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
        chmod +x linuxdeploy-x86_64.AppImage
        sudo mv linuxdeploy-x86_64.AppImage /usr/local/bin/linuxdeploy
    fi
    
    # Create AppDir
    mkdir -p AppDir/usr/bin
    cp LibraryManagementSystem AppDir/usr/bin/
    
    # Create desktop file
    cat > AppDir/LibraryManagementSystem.desktop << EOF
[Desktop Entry]
Name=Library Management System
Comment=A modern desktop application for managing library collections
Exec=LibraryManagementSystem
Icon=library-management-system
Terminal=false
Type=Application
Categories=Office;Education;
StartupNotify=true
EOF
    
    # Create icon (simple placeholder)
    mkdir -p AppDir/usr/share/icons/hicolor/256x256/apps
    # You can add a real icon here
    
    # Create AppImage
    linuxdeploy --appdir AppDir --output appimage
    
    print_success "AppImage created: LibraryManagementSystem-x86_64.AppImage"
}

# Create DEB package
create_deb_package() {
    print_status "Creating DEB package..."
    
    # Create package structure
    mkdir -p debian/usr/bin
    mkdir -p debian/usr/share/applications
    mkdir -p debian/usr/share/icons/hicolor/256x256/apps
    mkdir -p debian/DEBIAN
    
    # Copy binary
    cp LibraryManagementSystem debian/usr/bin/
    
    # Create desktop file
    cat > debian/usr/share/applications/library-management-system.desktop << EOF
[Desktop Entry]
Name=Library Management System
Comment=A modern desktop application for managing library collections
Exec=LibraryManagementSystem
Icon=library-management-system
Terminal=false
Type=Application
Categories=Office;Education;
StartupNotify=true
EOF
    
    # Create control file
    cat > debian/DEBIAN/control << EOF
Package: library-management-system
Version: 1.0.0
Section: office
Priority: optional
Architecture: amd64
Depends: libqt6core6, libqt6widgets6, libqt6network6, libsqlite3-0
Maintainer: CODSOFT <info@codsoft.in>
Description: Library Management System
 A modern desktop application for managing library collections.
 Developed as part of CODSOFT C++ Internship Program.
EOF
    
    # Build package
    dpkg-deb --build debian library-management-system_1.0.0_amd64.deb
    
    print_success "DEB package created: library-management-system_1.0.0_amd64.deb"
}

# Main build process
main() {
    print_status "Starting build process..."
    
    # Check if we're in the right directory
    if [ ! -f "CMakeLists.txt" ]; then
        print_error "CMakeLists.txt not found. Please run this script from the desktop_app directory."
        exit 1
    fi
    
    # Parse command line arguments
    BUILD_TYPE="appimage"
    INSTALL_DEPS=false
    
    while [[ $# -gt 0 ]]; do
        case $1 in
            --deps)
                INSTALL_DEPS=true
                shift
                ;;
            --type)
                BUILD_TYPE="$2"
                shift 2
                ;;
            --help)
                echo "Usage: $0 [OPTIONS]"
                echo "Options:"
                echo "  --deps     Install build dependencies"
                echo "  --type     Build type: appimage, deb, or all (default: appimage)"
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
    
    # Check Qt6
    check_qt6
    
    # Setup build
    setup_build
    
    # Configure and build
    configure_cmake
    build_app
    
    # Create packages
    case $BUILD_TYPE in
        "appimage")
            create_appimage
            ;;
        "deb")
            create_deb_package
            ;;
        "all")
            create_appimage
            create_deb_package
            ;;
        *)
            print_error "Unknown build type: $BUILD_TYPE"
            exit 1
            ;;
    esac
    
    print_success "Build process completed successfully!"
    print_status "You can now distribute your desktop application."
}

# Run main function
main "$@"
