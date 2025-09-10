# Library Management System - Desktop Application

A modern, professional desktop application for managing library collections, built with Qt6 and C++. This is the desktop version of the console-based Library Management System, featuring a beautiful GUI, database persistence, and auto-update functionality.

## 🚀 Features

### Core Functionality
- **📚 Complete Library Management**: Add, edit, delete, and search books
- **🔍 Advanced Search**: Search by title, author, ISBN, or genre (case-insensitive)
- **📊 Real-time Statistics**: View library statistics and availability rates
- **💾 Data Persistence**: SQLite database for reliable data storage
- **📤 Export/Import**: JSON export/import functionality

### User Experience
- **🎨 Modern Dark Theme**: Professional, easy-on-the-eyes interface
- **📱 Responsive Design**: Adapts to different screen sizes
- **⌨️ Keyboard Shortcuts**: Efficient navigation and operations
- **🔔 Status Updates**: Real-time feedback on operations

### Advanced Features
- **🔄 Auto-Update System**: Automatic checking and installation of updates
- **📋 Book Validation**: ISBN validation and duplicate checking
- **🎯 Smart Filtering**: Advanced search with multiple criteria
- **📈 Analytics**: Detailed library statistics and reporting

## 🖥️ Screenshots

### Main Interface
- Clean, modern interface with book table
- Real-time statistics panel
- Advanced search functionality
- Professional dark theme

### Book Management
- Add/Edit book dialog with validation
- ISBN validation (10 and 13 digit support)
- Genre selection with custom options
- Year validation

### Auto-Update System
- Automatic update checking
- Download progress tracking
- Changelog display
- One-click installation

## 🛠️ Installation

### Prerequisites
- Linux x86_64 (Ubuntu 20.04+, Fedora 35+, Arch Linux)
- Qt6 runtime libraries
- SQLite3

### Quick Installation

#### Option 1: AppImage (Recommended)
```bash
# Download the latest AppImage
wget https://github.com/Konadu-Prince/CODSOFT-c-/releases/latest/download/LibraryManagementSystem-x86_64.AppImage

# Make executable
chmod +x LibraryManagementSystem-x86_64.AppImage

# Run
./LibraryManagementSystem-x86_64.AppImage
```

#### Option 2: DEB Package (Ubuntu/Debian)
```bash
# Download the DEB package
wget https://github.com/Konadu-Prince/CODSOFT-c-/releases/latest/download/library-management-system_1.0.0_amd64.deb

# Install
sudo dpkg -i library-management-system_1.0.0_amd64.deb

# Run from applications menu or command line
LibraryManagementSystem
```

### Manual Installation
```bash
# Install dependencies
sudo apt install qt6-base qt6-tools libsqlite3-0

# Download and extract
wget https://github.com/Konadu-Prince/CODSOFT-c-/archive/refs/heads/main.zip
unzip main.zip
cd CODSOFT-c--main/desktop_app

# Build
./build.sh --deps --type appimage
```

## 🏗️ Building from Source

### Requirements
- Qt6 development packages
- CMake 3.16+
- C++17 compatible compiler
- SQLite3 development libraries

### Build Steps

#### Ubuntu/Debian
```bash
# Install dependencies
sudo apt update
sudo apt install -y build-essential cmake qt6-base-dev qt6-tools-dev libsqlite3-dev

# Clone repository
git clone https://github.com/Konadu-Prince/CODSOFT-c-.git
cd CODSOFT-c-/desktop_app

# Build
./build.sh --deps --type all
```

#### Fedora
```bash
# Install dependencies
sudo dnf install -y gcc-c++ cmake qt6-qtbase-devel qt6-qttools-devel sqlite-devel

# Build
./build.sh --type all
```

#### Arch Linux
```bash
# Install dependencies
sudo pacman -S --needed base-devel cmake qt6-base qt6-tools sqlite

# Build
./build.sh --type all
```

### Build Options
```bash
# Build AppImage only
./build.sh --type appimage

# Build DEB package only
./build.sh --type deb

# Build both
./build.sh --type all

# Install dependencies and build
./build.sh --deps --type all
```

## 📖 Usage

### Getting Started
1. **Launch the Application**: Run the AppImage or installed binary
2. **Explore the Interface**: Familiarize yourself with the main window
3. **Add Books**: Click "Add Book" to add your first book
4. **Search**: Use the search bar to find books quickly
5. **Check Statistics**: View library statistics in the bottom panel

### Key Features

#### Adding Books
1. Click "Add Book" button
2. Fill in the required information:
   - Title (required)
   - Author (required)
   - ISBN (10 or 13 digits, validated)
   - Genre (select from dropdown or enter custom)
   - Year (validated range)
3. Click "Add" to save

#### Searching Books
- Use the search bar to find books by:
  - Title
  - Author
  - ISBN
  - Genre
- Search is case-insensitive and supports partial matches

#### Managing Books
- **Edit**: Select a book and click "Edit Book"
- **Delete**: Select a book and click "Delete Book" (with confirmation)
- **Checkout**: Select a book and click "Checkout" (enter borrower name)
- **Return**: Select a checked-out book and click "Return"

#### Export/Import
- **Export**: File → Export Books (saves as JSON)
- **Import**: File → Import Books (loads from JSON file)

### Keyboard Shortcuts
- `Ctrl+N`: Add new book
- `Ctrl+E`: Edit selected book
- `Delete`: Delete selected book
- `Ctrl+F`: Focus search bar
- `F5`: Refresh library
- `Ctrl+U`: Check for updates

## 🔧 Configuration

### Data Storage
- Database location: `~/.local/share/LibraryManagementSystem/library.db`
- Configuration: `~/.config/LibraryManagementSystem/`

### Auto-Update Settings
- Updates are checked automatically every 24 hours
- Manual check: Tools → Check for Updates
- Update server: GitHub Releases API

## 🐛 Troubleshooting

### Common Issues

#### "Qt6 not found" Error
```bash
# Ubuntu/Debian
sudo apt install qt6-base qt6-tools

# Fedora
sudo dnf install qt6-qtbase qt6-qttools

# Arch
sudo pacman -S qt6-base qt6-tools
```

#### AppImage Won't Run
```bash
# Make executable
chmod +x LibraryManagementSystem-x86_64.AppImage

# Check dependencies
ldd LibraryManagementSystem-x86_64.AppImage
```

#### Database Issues
```bash
# Reset database (WARNING: This will delete all data)
rm ~/.local/share/LibraryManagementSystem/library.db
```

### Getting Help
- Check the [Issues](https://github.com/Konadu-Prince/CODSOFT-c-/issues) page
- Create a new issue with:
  - Operating system and version
  - Error messages
  - Steps to reproduce

## 🔄 Auto-Update System

The application includes a sophisticated auto-update system:

### Features
- **Automatic Checking**: Checks for updates every 24 hours
- **Manual Check**: Tools → Check for Updates
- **Download Progress**: Visual progress bar during download
- **Changelog Display**: Shows what's new in each update
- **One-Click Install**: Simple installation process

### Update Process
1. Application checks GitHub Releases API
2. Compares version numbers
3. Downloads update if newer version available
4. Shows changelog and download progress
5. Installs update and restarts application

### Security
- Updates are downloaded from official GitHub releases
- SHA256 checksums are verified
- Digital signatures are validated

## 🏗️ Architecture

### Technology Stack
- **Framework**: Qt6 (Widgets, Core, Network)
- **Language**: C++17
- **Database**: SQLite3
- **Build System**: CMake
- **Packaging**: AppImage, DEB

### Project Structure
```
desktop_app/
├── CMakeLists.txt          # Build configuration
├── main.cpp                # Application entry point
├── mainwindow.h/cpp        # Main window implementation
├── bookmodel.h/cpp         # Data model for book table
├── bookdialog.h/cpp        # Add/Edit book dialog
├── database.h/cpp          # Database operations
├── updatedialog.h/cpp      # Auto-update system
├── build.sh                # Build script
└── README.md               # This file
```

### Key Components

#### MainWindow
- Central application window
- Menu bar, toolbar, and status bar
- Book table and search functionality
- Statistics display

#### BookModel
- Qt model for book table display
- Search and filtering capabilities
- Data validation and formatting

#### Database
- SQLite database operations
- CRUD operations for books
- Search and statistics queries
- Sample data initialization

#### UpdateDialog
- Auto-update checking
- Download management
- Installation process
- Error handling

## 🤝 Contributing

We welcome contributions! Here's how you can help:

### Development Setup
1. Fork the repository
2. Clone your fork
3. Install development dependencies
4. Make your changes
5. Test thoroughly
6. Submit a pull request

### Code Style
- Follow Qt/C++ best practices
- Use meaningful variable names
- Add comments for complex logic
- Ensure all code compiles without warnings

### Testing
- Test on multiple Linux distributions
- Verify all features work correctly
- Check memory leaks with Valgrind
- Test update system functionality

## 📄 License

This project is developed as part of the CODSOFT C++ Internship Program.

## 🙏 Acknowledgments

- **CODSOFT** for providing the internship opportunity
- **Qt Company** for the excellent Qt6 framework
- **SQLite** for the reliable database engine
- **GitHub** for hosting and CI/CD services

## 📞 Support

For support and questions:
- Create an issue on GitHub
- Email: info@codsoft.in
- Visit: https://www.codsoft.in

---

*Developed with ❤️ as part of CODSOFT C++ Internship Program*
