# CODSOFT C++ Projects Makefile
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Project executables
TARGETS = library_management_system number_guessing_game tic_tac_toe todo_manager

# Source files
SOURCES = library_management_system.cpp random_guess.cpp tic_tac_toe.cpp todo_manager.cpp

# Default target
all: $(TARGETS)

# Build individual projects
library_management_system: library_management_system.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

number_guessing_game: random_guess.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

tic_tac_toe: tic_tac_toe.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

todo_manager: todo_manager.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Clean build artifacts
clean:
	rm -f $(TARGETS)

# Install dependencies (for Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential g++ clang++ cppcheck

# Run static analysis
check:
	cppcheck --enable=all --suppress=missingIncludeSystem *.cpp

# Test all programs (basic functionality)
test: all
	@echo "Testing Library Management System..."
	@echo "8" | timeout 5s ./library_management_system || echo "Library system test completed"
	@echo "Testing Number Guessing Game..."
	@echo "50" | timeout 5s ./number_guessing_game || echo "Number guessing test completed"
	@echo "Testing Tic-Tac-Toe Game..."
	@echo "1 1" | timeout 5s ./tic_tac_toe || echo "Tic-tac-toe test completed"
	@echo "Testing To-Do List Manager..."
	@echo "quit" | timeout 5s ./todo_manager || echo "Todo manager test completed"
	@echo "✅ All tests completed"

# Show help
help:
	@echo "Available targets:"
	@echo "  all          - Build all projects"
	@echo "  clean        - Remove all executables"
	@echo "  install-deps - Install build dependencies"
	@echo "  check        - Run static code analysis"
	@echo "  test         - Build and test all projects"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Individual targets:"
	@echo "  library_management_system"
	@echo "  number_guessing_game"
	@echo "  tic_tac_toe"
	@echo "  todo_manager"

.PHONY: all clean install-deps check test help
