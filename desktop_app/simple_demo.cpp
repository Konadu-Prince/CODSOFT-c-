#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>

// Simple Book structure
struct Book {
    std::string title;
    std::string author;
    std::string ISBN;
    std::string genre;
    int year;
    bool checkedOut;
    
    Book(const std::string& t, const std::string& a, const std::string& i, 
         const std::string& g, int y, bool co = false)
        : title(t), author(a), ISBN(i), genre(g), year(y), checkedOut(co) {}
};

// Simple Library Management System
class SimpleLibrary {
private:
    std::vector<Book> books;
    
public:
    SimpleLibrary() {
        // Add some sample books
        addBook("The Great Gatsby", "F. Scott Fitzgerald", "9780743273565", "Fiction", 1925);
        addBook("To Kill a Mockingbird", "Harper Lee", "9780061120084", "Fiction", 1960);
        addBook("1984", "George Orwell", "9780451524935", "Fiction", 1949);
        addBook("Clean Code", "Robert C. Martin", "9780132350884", "Programming", 2008);
        addBook("Design Patterns", "Gang of Four", "9780201633610", "Programming", 1994);
    }
    
    void addBook(const std::string& title, const std::string& author, 
                 const std::string& ISBN, const std::string& genre, int year) {
        books.emplace_back(title, author, ISBN, genre, year);
        std::cout << "✅ Book added: " << title << " by " << author << std::endl;
    }
    
    void displayAllBooks() {
        std::cout << "\n📚 Library Catalog:\n";
        std::cout << std::string(80, '=') << std::endl;
        std::cout << std::left << std::setw(25) << "Title" 
                  << std::setw(20) << "Author" 
                  << std::setw(15) << "ISBN" 
                  << std::setw(12) << "Genre" 
                  << std::setw(6) << "Year" 
                  << std::setw(10) << "Status" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (const auto& book : books) {
            std::cout << std::left << std::setw(25) << book.title.substr(0, 24)
                      << std::setw(20) << book.author.substr(0, 19)
                      << std::setw(15) << book.ISBN.substr(0, 14)
                      << std::setw(12) << book.genre.substr(0, 11)
                      << std::setw(6) << book.year
                      << std::setw(10) << (book.checkedOut ? "Checked Out" : "Available") << std::endl;
        }
        std::cout << std::string(80, '=') << std::endl;
    }
    
    void searchBooks(const std::string& query) {
        std::cout << "\n🔍 Search Results for: \"" << query << "\"\n";
        std::cout << std::string(60, '=') << std::endl;
        
        bool found = false;
        for (const auto& book : books) {
            if (book.title.find(query) != std::string::npos ||
                book.author.find(query) != std::string::npos ||
                book.genre.find(query) != std::string::npos ||
                book.ISBN.find(query) != std::string::npos) {
                
                std::cout << "📖 " << book.title << " by " << book.author << std::endl;
                std::cout << "   ISBN: " << book.ISBN << " | Genre: " << book.genre 
                          << " | Year: " << book.year << std::endl;
                std::cout << "   Status: " << (book.checkedOut ? "Checked Out" : "Available") << std::endl;
                std::cout << std::string(40, '-') << std::endl;
                found = true;
            }
        }
        
        if (!found) {
            std::cout << "❌ No books found matching your search." << std::endl;
        }
    }
    
    void displayStatistics() {
        int total = books.size();
        int available = 0;
        int checkedOut = 0;
        
        for (const auto& book : books) {
            if (book.checkedOut) {
                checkedOut++;
            } else {
                available++;
            }
        }
        
        double availabilityRate = total > 0 ? (double)available / total * 100 : 0;
        
        std::cout << "\n📊 Library Statistics:\n";
        std::cout << std::string(40, '=') << std::endl;
        std::cout << "Total Books: " << total << std::endl;
        std::cout << "Available: " << available << std::endl;
        std::cout << "Checked Out: " << checkedOut << std::endl;
        std::cout << "Availability Rate: " << std::fixed << std::setprecision(1) 
                  << availabilityRate << "%" << std::endl;
        std::cout << std::string(40, '=') << std::endl;
    }
    
    void exportToJSON() {
        std::ofstream file("library_export.json");
        if (file.is_open()) {
            file << "{\n  \"books\": [\n";
            for (size_t i = 0; i < books.size(); ++i) {
                const auto& book = books[i];
                file << "    {\n";
                file << "      \"title\": \"" << book.title << "\",\n";
                file << "      \"author\": \"" << book.author << "\",\n";
                file << "      \"ISBN\": \"" << book.ISBN << "\",\n";
                file << "      \"genre\": \"" << book.genre << "\",\n";
                file << "      \"year\": " << book.year << ",\n";
                file << "      \"checkedOut\": " << (book.checkedOut ? "true" : "false") << "\n";
                file << "    }";
                if (i < books.size() - 1) file << ",";
                file << "\n";
            }
            file << "  ]\n}\n";
            file.close();
            std::cout << "✅ Library data exported to library_export.json" << std::endl;
        } else {
            std::cout << "❌ Failed to export data" << std::endl;
        }
    }
};

void displayMenu() {
    std::cout << "\n🏛️  Library Management System - Desktop Demo\n";
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "1. View All Books\n";
    std::cout << "2. Search Books\n";
    std::cout << "3. Add New Book\n";
    std::cout << "4. Library Statistics\n";
    std::cout << "5. Export to JSON\n";
    std::cout << "6. Exit\n";
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Enter your choice (1-6): ";
}

int main() {
    SimpleLibrary library;
    int choice;
    std::string input;
    
    std::cout << "🚀 Welcome to the Library Management System Desktop Demo!\n";
    std::cout << "This demonstrates the core functionality of our desktop application.\n";
    
    while (true) {
        displayMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                library.displayAllBooks();
                break;
                
            case 2: {
                std::cout << "Enter search term: ";
                std::cin.ignore();
                std::getline(std::cin, input);
                library.searchBooks(input);
                break;
            }
            
            case 3: {
                std::string title, author, ISBN, genre;
                int year;
                
                std::cout << "Enter book title: ";
                std::cin.ignore();
                std::getline(std::cin, title);
                
                std::cout << "Enter author: ";
                std::getline(std::cin, author);
                
                std::cout << "Enter ISBN: ";
                std::getline(std::cin, ISBN);
                
                std::cout << "Enter genre: ";
                std::getline(std::cin, genre);
                
                std::cout << "Enter year: ";
                std::cin >> year;
                
                library.addBook(title, author, ISBN, genre, year);
                break;
            }
            
            case 4:
                library.displayStatistics();
                break;
                
            case 5:
                library.exportToJSON();
                break;
                
            case 6:
                std::cout << "👋 Thank you for using the Library Management System!\n";
                std::cout << "This demo shows the core functionality that will be available\n";
                std::cout << "in the full desktop application with modern GUI.\n";
                return 0;
                
            default:
                std::cout << "❌ Invalid choice. Please enter 1-6.\n";
                break;
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }
    
    return 0;
}
