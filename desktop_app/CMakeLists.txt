cmake_minimum_required(VERSION 3.16)
project(LibraryManagementSystem VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

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
qt6_add_executable(LibraryManagementSystem ${SOURCES} ${HEADERS} ${UI_FILES})

# Link Qt libraries
target_link_libraries(LibraryManagementSystem
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)

# Set application properties
set_target_properties(LibraryManagementSystem PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Installation
install(TARGETS LibraryManagementSystem
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)
