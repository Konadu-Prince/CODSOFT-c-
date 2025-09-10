# CODSOFT C++ Internship Projects

This repository contains C++ projects completed as part of the CODSOFT internship program. The projects demonstrate various programming concepts including data structures, algorithms, and user interaction.

## 📁 Project Overview

This repository contains 4 C++ applications:

1. **Library Management System** - A console-based library system for managing books
2. **Number Guessing Game** - An interactive number guessing game
3. **Tic-Tac-Toe Game** - A two-player tic-tac-toe game
4. **To-Do List Manager** - A task management application

## 🚀 Projects

### 1. Library Management System (`library_management_system.cpp`)
A comprehensive library management system that allows users to:
- Search for books by title, author, or ISBN
- Check out books
- Return books
- View book availability status

**Features:**
- Book search functionality
- Checkout/return system
- ISBN-based book identification
- Interactive menu system

### 2. Number Guessing Game (`random_guess.cpp`)
A fun number guessing game where players try to guess a random number between 1 and 100.

**Features:**
- Random number generation (1-100)
- Attempt counter
- Input validation
- Helpful hints (too high/too low)

### 3. Tic-Tac-Toe Game (`tic_tac_toe.cpp`)
A classic two-player tic-tac-toe game with a 3x3 grid.

**Features:**
- Interactive game board display
- Win condition detection (rows, columns, diagonals)
- Draw detection
- Player switching
- Play again functionality

### 4. To-Do List Manager (`todo_manager.cpp`)
A simple task management application for organizing daily tasks.

**Features:**
- Add new tasks
- View all tasks with completion status
- Mark tasks as completed
- Remove tasks
- Command-based interface

## 🛠️ Compilation and Usage

### Prerequisites
- C++ compiler (g++, clang++, or MSVC)
- Standard C++ library support

### Compilation Instructions

Compile each program individually:

```bash
# Library Management System
g++ -o library_management_system library_management_system.cpp

# Number Guessing Game
g++ -o number_guessing_game random_guess.cpp

# Tic-Tac-Toe Game
g++ -o tic_tac_toe tic_tac_toe.cpp

# To-Do List Manager
g++ -o todo_manager todo_manager.cpp
```

### Running the Programs

```bash
# Run Library Management System
./library_management_system

# Run Number Guessing Game
./number_guessing_game

# Run Tic-Tac-Toe Game
./tic_tac_toe

# Run To-Do List Manager
./todo_manager
```

## 📋 Usage Examples

### Library Management System
```
Library Management System
1. Search Books
2. Checkout Book
3. Return Book
4. Exit
Enter your choice: 1
Enter search query (Title, Author, or ISBN): Author1
```

### Number Guessing Game
```
Welcome to the Number Guessing Game!
Enter your guess: 50
Too high! Try again.
Enter your guess: 25
Too low! Try again.
```

### Tic-Tac-Toe Game
```
Welcome to Tic-Tac-Toe!
Current board:
  |   |  
---------
  |   |  
---------
  |   |  
Player X, enter your move (row and column, e.g., 1 2): 1 1
```

### To-Do List Manager
```
Commands: add, view, complete, remove, quit
Enter a command: add
Enter the task description: Complete project documentation
Task added: Complete project documentation
```

## 🏗️ Project Structure

```
CODSOFT-c-/
├── library_management_system.cpp    # Library management system
├── random_guess.cpp                 # Number guessing game
├── tic_tac_toe.cpp                  # Tic-tac-toe game
├── todo_manager.cpp                 # To-do list manager
└── README.md                        # Project documentation
```

## 🎯 Learning Objectives

These projects demonstrate:
- **Object-Oriented Programming**: Use of structures and functions
- **Data Structures**: Vectors, arrays, and string manipulation
- **User Input/Output**: Console-based interaction
- **Control Flow**: Loops, conditionals, and switch statements
- **Algorithm Implementation**: Search, validation, and game logic

## 📚 About CODSOFT

This project is part of the CODSOFT internship program. CODSOFT provides practical learning opportunities for students and professionals to enhance their programming skills through real-world projects.

**Website**: [https://www.codsoft.in/internships](https://www.codsoft.in/internships)

## 📝 License

This project is created for educational purposes as part of the CODSOFT internship program.

---

*Developed as part of CODSOFT C++ Internship Program*
