cmake_minimum_required(VERSION 3.16)
project(LibraryManagementSystem VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt5 components (fallback to Qt5 if Qt6 not available)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network)

# Enable Qt's automatic MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Source files
set(SOURCES
    main.cpp
    mainwindow.cpp
    bookmodel.cpp
    bookdialog.cpp
    updatedialog.cpp
    database.cpp
)

# Header files
set(HEADERS
    mainwindow.h
    bookmodel.h
    bookdialog.h
    updatedialog.h
    database.h
)

# UI files
set(UI_FILES
    mainwindow.ui
    bookdialog.ui
    updatedialog.ui
)

# Create executable
add_executable(LibraryManagementSystem ${SOURCES} ${HEADERS} ${UI_FILES})

# Link Qt libraries
target_link_libraries(LibraryManagementSystem
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
)

# Set application properties
set_target_properties(LibraryManagementSystem PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Windows-specific settings
if(WIN32)
    # Set Windows subsystem
    set_target_properties(LibraryManagementSystem PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    # Add Windows-specific libraries
    target_link_libraries(LibraryManagementSystem
        ws2_32
        winspool
        shell32
        ole32
        oleaut32
        uuid
        comdlg32
        advapi32
    )
    
    # Set Windows version info
    set_target_properties(LibraryManagementSystem PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
    )
endif()

# Installation
install(TARGETS LibraryManagementSystem
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

# Windows installer configuration
if(WIN32)
    include(CPack)
    set(CPACK_PACKAGE_NAME "Library Management System")
    set(CPACK_PACKAGE_VENDOR "CODSOFT")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A modern desktop application for managing library collections")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "Library Management System")
    set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/icon.ico")
    
    # NSIS installer
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "Library Management System")
    set(CPACK_NSIS_PACKAGE_NAME "Library Management System")
    set(CPACK_NSIS_CONTACT "info@codsoft.in")
    set(CPACK_NSIS_URL "https://github.com/Konadu-Prince/CODSOFT-c-")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    
    # Create desktop shortcut
    set(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortCut '$DESKTOP\\\\Library Management System.lnk' '$INSTDIR\\\\LibraryManagementSystem.exe'"
    )
    set(CPACK_NSIS_DELETE_ICONS_EXTRA
        "Delete '$DESKTOP\\\\Library Management System.lnk'"
    )
endif()