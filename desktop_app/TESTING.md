# 🧪 Testing Guide - Library Management System Desktop App

This guide will help you test your desktop application on different platforms and ensure everything works correctly.

## 🐧 **Testing on Linux (Your Current System)**

### **Step 1: Download and Build**
```bash
# Clone your repository
git clone https://github.com/Konadu-Prince/CODSOFT-c-.git
cd CODSOFT-c-/desktop_app

# Install dependencies and build
./build.sh --deps --type appimage
```

### **Step 2: Test the AppImage**
```bash
# Make executable and run
chmod +x LibraryManagementSystem-x86_64.AppImage
./LibraryManagementSystem-x86_64.AppImage
```

### **Step 3: Test All Features**
1. **Launch Application**: Should open with dark theme
2. **Add Books**: Click "Add Book" and fill in details
3. **Search**: Use the search bar to find books
4. **Edit Books**: Select a book and click "Edit"
5. **Delete Books**: Select a book and click "Delete"
6. **Checkout/Return**: Test book checkout and return
7. **Statistics**: Check the statistics panel
8. **Export/Import**: Test JSON export/import
9. **Auto-Update**: Check "Tools → Check for Updates"

### **Step 4: Test DEB Package**
```bash
# Build DEB package
./build.sh --type deb

# Install and test
sudo dpkg -i library-management-system_1.0.0_amd64.deb
LibraryManagementSystem
```

## 🪟 **Testing on Windows**

### **Option 1: Native Windows Build**
If you have a Windows machine:

1. **Install Prerequisites**:
   - Visual Studio 2022 Community (free)
   - Qt6 development tools
   - CMake

2. **Build**:
   ```cmd
   cd desktop_app
   build-windows.bat
   ```

3. **Test**:
   - Run the generated installer
   - Test all application features
   - Check auto-update functionality

### **Option 2: Cross-Compilation from Linux**
```bash
# Install cross-compilation tools
sudo apt install mingw-w64 qt6-base-dev cmake

# Build Windows version
./build-windows.sh --deps

# Test with Wine
wine build-windows/LibraryManagementSystem.exe
```

## 🍎 **Testing on macOS**

### **Option 1: Native macOS Build**
If you have a Mac:

1. **Install Prerequisites**:
   - Xcode Command Line Tools
   - Qt6 development tools
   - CMake

2. **Build**:
   ```bash
   cd desktop_app
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   make -j$(sysctl -n hw.ncpu)
   ```

3. **Test**:
   - Run the application
   - Test all features
   - Create DMG package

### **Option 2: GitHub Actions Build**
The GitHub Actions workflow will automatically build macOS version when you create a release.

## 🚀 **Creating a Release for Testing**

### **Step 1: Create a Release Tag**
```bash
# Create and push a release tag
git tag v1.0.0
git push origin v1.0.0
```

### **Step 2: Monitor GitHub Actions**
1. Go to your GitHub repository
2. Click "Actions" tab
3. Watch the build process for all platforms
4. Download the built packages when complete

### **Step 3: Test Release Packages**
- **Linux**: Test AppImage and DEB package
- **Windows**: Test the .exe installer
- **macOS**: Test the .dmg package

## 🔍 **Feature Testing Checklist**

### **Core Functionality**
- [ ] Application launches successfully
- [ ] Dark theme displays correctly
- [ ] Database initializes with sample data
- [ ] All menu items work
- [ ] Keyboard shortcuts function

### **Book Management**
- [ ] Add new books with validation
- [ ] Edit existing books
- [ ] Delete books with confirmation
- [ ] ISBN validation works (10 and 13 digit)
- [ ] Genre selection works
- [ ] Year validation works

### **Search and Filtering**
- [ ] Real-time search works
- [ ] Case-insensitive search
- [ ] Search by title, author, ISBN, genre
- [ ] Clear search results
- [ ] Table sorting works

### **Checkout System**
- [ ] Checkout books (enter borrower name)
- [ ] Return books
- [ ] Status updates correctly
- [ ] Visual indicators for checked out books

### **Statistics**
- [ ] Total books count updates
- [ ] Available books count updates
- [ ] Checked out books count updates
- [ ] Availability percentage calculates correctly

### **Data Management**
- [ ] Export to JSON works
- [ ] Import from JSON works
- [ ] Data validation on import
- [ ] Database persistence works

### **Auto-Update System**
- [ ] Update check works
- [ ] Download progress displays
- [ ] Changelog shows correctly
- [ ] Installation process works
- [ ] Version comparison works

### **User Interface**
- [ ] Responsive design works
- [ ] Status bar updates
- [ ] Progress bars work
- [ ] Error messages display
- [ ] Confirmation dialogs work

## 🐛 **Common Issues and Solutions**

### **Build Issues**
```bash
# Qt6 not found
sudo apt install qt6-base-dev qt6-tools-dev

# CMake not found
sudo apt install cmake

# Build tools missing
sudo apt install build-essential
```

### **Runtime Issues**
```bash
# Missing Qt6 libraries
sudo apt install qt6-base qt6-tools

# Database permission issues
chmod 755 ~/.local/share/LibraryManagementSystem/
```

### **AppImage Issues**
```bash
# AppImage won't run
chmod +x LibraryManagementSystem-x86_64.AppImage
./LibraryManagementSystem-x86_64.AppImage

# Missing dependencies
ldd LibraryManagementSystem-x86_64.AppImage
```

## 📊 **Performance Testing**

### **Load Testing**
1. Add 100+ books to test performance
2. Test search with large datasets
3. Test export/import with large files
4. Monitor memory usage

### **Stress Testing**
1. Rapid add/edit/delete operations
2. Multiple search operations
3. Large data imports
4. Long-running sessions

## 🔒 **Security Testing**

### **Input Validation**
- Test with malicious input
- Test with very long strings
- Test with special characters
- Test with SQL injection attempts

### **File Operations**
- Test with read-only files
- Test with corrupted files
- Test with insufficient permissions
- Test with full disk space

## 📱 **Cross-Platform Testing**

### **Linux Variants**
- Ubuntu 20.04+
- Fedora 35+
- Arch Linux
- Debian 11+

### **Windows Versions**
- Windows 10
- Windows 11
- Different screen resolutions
- Different DPI settings

### **macOS Versions**
- macOS 10.15+
- macOS 11+
- macOS 12+
- Different screen sizes

## 🎯 **User Acceptance Testing**

### **Test Scenarios**
1. **New User**: First-time installation and usage
2. **Power User**: Advanced features and shortcuts
3. **Data Migration**: Import from other systems
4. **Backup/Restore**: Data backup and recovery

### **Usability Testing**
- Intuitive navigation
- Clear error messages
- Helpful tooltips
- Keyboard accessibility
- Screen reader compatibility

## 📝 **Reporting Issues**

When you find issues:

1. **Document the Problem**:
   - Operating system and version
   - Steps to reproduce
   - Expected vs. actual behavior
   - Screenshots if applicable

2. **Create GitHub Issue**:
   - Use the issue template
   - Include system information
   - Provide detailed steps

3. **Test Fixes**:
   - Verify the fix works
   - Test on multiple platforms
   - Update documentation

## 🎉 **Success Criteria**

Your desktop application is ready for release when:

- [ ] All features work on all target platforms
- [ ] No critical bugs or crashes
- [ ] Performance is acceptable
- [ ] User interface is intuitive
- [ ] Auto-update system works
- [ ] Installation packages work correctly
- [ ] Documentation is complete

---

**Happy Testing! 🚀**

Remember: Thorough testing ensures your users have a great experience with your desktop application.
