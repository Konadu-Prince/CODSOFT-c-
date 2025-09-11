# Simple Makefile for Library Management System Desktop App
# Using Qt5 since Qt6 development tools are not available

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
QT5_CFLAGS = $(shell pkg-config --cflags Qt5Core Qt5Widgets Qt5Network)
QT5_LIBS = $(shell pkg-config --libs Qt5Core Qt5Widgets Qt5Network)
SQLITE_LIBS = -lsqlite3

# Source files
SOURCES = main.cpp mainwindow.cpp bookmodel.cpp bookdialog.cpp updatedialog.cpp database.cpp
HEADERS = mainwindow.h bookmodel.h bookdialog.h updatedialog.h database.h
OBJECTS = $(SOURCES:.cpp=.o)

# Target executable
TARGET = LibraryManagementSystem

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(QT5_LIBS) $(SQLITE_LIBS)

# Compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(QT5_CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt update
	sudo apt install -y build-essential pkg-config libqt5-dev libsqlite3-dev

# Run the application
run: $(TARGET)
	./$(TARGET)

# Create AppImage (simplified version)
appimage: $(TARGET)
	@echo "Creating simple AppImage..."
	@mkdir -p AppDir/usr/bin
	@cp $(TARGET) AppDir/usr/bin/
	@echo "#!/bin/bash" > AppDir/AppRun
	@echo "cd \$$(dirname \$$0)" >> AppDir/AppRun
	@echo "exec ./usr/bin/$(TARGET)" >> AppDir/AppRun
	@chmod +x AppDir/AppRun
	@echo "AppImage structure created in AppDir/"
	@echo "To create actual AppImage, install linuxdeploy and run:"
	@echo "linuxdeploy --appdir AppDir --output appimage"

.PHONY: all clean install-deps run appimage
