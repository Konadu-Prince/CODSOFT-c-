#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

// Book structure to store book information
struct Book {
    string title;
    string author;
    string ISBN;
    bool checkedOut;
    string genre;
    int year;
};

// Function to clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to validate ISBN format (basic validation)
bool isValidISBN(const string& isbn) {
    if (isbn.length() != 10 && isbn.length() != 13) {
        return false;
    }
    for (char c : isbn) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Function to display all books
void displayAllBooks(const vector<Book>& library) {
    if (library.empty()) {
        cout << "\nNo books in the library." << endl;
        return;
    }
    
    cout << "\n" << string(80, '=') << endl;
    cout << "LIBRARY CATALOG" << endl;
    cout << string(80, '=') << endl;
    cout << left << setw(25) << "TITLE" 
         << setw(20) << "AUTHOR" 
         << setw(15) << "ISBN" 
         << setw(15) << "GENRE" 
         << setw(8) << "YEAR" 
         << setw(12) << "STATUS" << endl;
    cout << string(80, '-') << endl;
    
    for (const Book& book : library) {
        cout << left << setw(25) << book.title.substr(0, 24)
             << setw(20) << book.author.substr(0, 19)
             << setw(15) << book.ISBN
             << setw(15) << book.genre.substr(0, 14)
             << setw(8) << book.year
             << setw(12) << (book.checkedOut ? "Checked Out" : "Available") << endl;
    }
    cout << string(80, '=') << endl;
}

// Function to search for books
void searchBooks(const vector<Book>& library, const string& query) {
    vector<Book> results;
    string lowerQuery = query;
    transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    
    for (const Book& book : library) {
        string lowerTitle = book.title;
        string lowerAuthor = book.author;
        string lowerGenre = book.genre;
        
        transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        transform(lowerAuthor.begin(), lowerAuthor.end(), lowerAuthor.begin(), ::tolower);
        transform(lowerGenre.begin(), lowerGenre.end(), lowerGenre.begin(), ::tolower);
        
        if (lowerTitle.find(lowerQuery) != string::npos || 
            lowerAuthor.find(lowerQuery) != string::npos || 
            book.ISBN.find(query) != string::npos ||
            lowerGenre.find(lowerQuery) != string::npos) {
            results.push_back(book);
        }
    }
    
    if (results.empty()) {
        cout << "\nNo books found matching your search criteria." << endl;
        return;
    }
    
    cout << "\nSearch Results (" << results.size() << " found):" << endl;
    cout << string(80, '-') << endl;
    cout << left << setw(25) << "TITLE" 
         << setw(20) << "AUTHOR" 
         << setw(15) << "ISBN" 
         << setw(15) << "GENRE" 
         << setw(8) << "YEAR" 
         << setw(12) << "STATUS" << endl;
    cout << string(80, '-') << endl;
    
    for (const Book& book : results) {
        cout << left << setw(25) << book.title.substr(0, 24)
             << setw(20) << book.author.substr(0, 19)
             << setw(15) << book.ISBN
             << setw(15) << book.genre.substr(0, 14)
             << setw(8) << book.year
             << setw(12) << (book.checkedOut ? "Checked Out" : "Available") << endl;
    }
}

// Function to add a new book
void addBook(vector<Book>& library) {
    Book newBook;
    
    cout << "\n--- Add New Book ---" << endl;
    
    cout << "Enter book title: ";
    clearInputBuffer();
    getline(cin, newBook.title);
    if (newBook.title.empty()) {
        cout << "Error: Title cannot be empty." << endl;
        return;
    }
    
    cout << "Enter author name: ";
    getline(cin, newBook.author);
    if (newBook.author.empty()) {
        cout << "Error: Author cannot be empty." << endl;
        return;
    }
    
    cout << "Enter ISBN (10 or 13 digits): ";
    getline(cin, newBook.ISBN);
    if (!isValidISBN(newBook.ISBN)) {
        cout << "Error: Invalid ISBN format. Please enter 10 or 13 digits." << endl;
        return;
    }
    
    // Check if ISBN already exists
    for (const Book& book : library) {
        if (book.ISBN == newBook.ISBN) {
            cout << "Error: A book with this ISBN already exists." << endl;
            return;
        }
    }
    
    cout << "Enter genre: ";
    getline(cin, newBook.genre);
    if (newBook.genre.empty()) {
        newBook.genre = "Unknown";
    }
    
    cout << "Enter publication year: ";
    cin >> newBook.year;
    if (cin.fail() || newBook.year < 1000 || newBook.year > 2024) {
        cout << "Error: Invalid year. Please enter a valid year." << endl;
        clearInputBuffer();
        return;
    }
    
    newBook.checkedOut = false;
    library.push_back(newBook);
    cout << "\nBook added successfully!" << endl;
}

// Function to remove a book
void removeBook(vector<Book>& library) {
    if (library.empty()) {
        cout << "\nNo books in the library to remove." << endl;
        return;
    }
    
    string ISBN;
    cout << "\n--- Remove Book ---" << endl;
    cout << "Enter ISBN of the book to remove: ";
    clearInputBuffer();
    getline(cin, ISBN);
    
    for (auto it = library.begin(); it != library.end(); ++it) {
        if (it->ISBN == ISBN) {
            if (it->checkedOut) {
                cout << "Error: Cannot remove a checked-out book. Please return it first." << endl;
                return;
            }
            
            cout << "Are you sure you want to remove '" << it->title << "' by " << it->author << "? (y/n): ";
            char confirm;
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                library.erase(it);
                cout << "Book removed successfully!" << endl;
            } else {
                cout << "Operation cancelled." << endl;
            }
            return;
        }
    }
    cout << "Book not found." << endl;
}

// Function to checkout a book
void checkoutBook(vector<Book>& library, const string& ISBN) {
    for (Book& book : library) {
        if (book.ISBN == ISBN) {
            if (book.checkedOut) {
                cout << "Error: This book is already checked out." << endl;
            } else {
                book.checkedOut = true;
                cout << "Success: '" << book.title << "' has been checked out successfully!" << endl;
            }
            return;
        }
    }
    cout << "Error: Book not found." << endl;
}

// Function to return a book
void returnBook(vector<Book>& library, const string& ISBN) {
    for (Book& book : library) {
        if (book.ISBN == ISBN) {
            if (book.checkedOut) {
                book.checkedOut = false;
                cout << "Success: '" << book.title << "' has been returned successfully!" << endl;
            } else {
                cout << "Error: This book is not checked out." << endl;
            }
            return;
        }
    }
    cout << "Error: Book not found." << endl;
}

// Function to display library statistics
void displayStatistics(const vector<Book>& library) {
    if (library.empty()) {
        cout << "\nNo books in the library." << endl;
        return;
    }
    
    int totalBooks = library.size();
    int checkedOutBooks = 0;
    int availableBooks = 0;
    
    for (const Book& book : library) {
        if (book.checkedOut) {
            checkedOutBooks++;
        } else {
            availableBooks++;
        }
    }
    
    cout << "\n--- Library Statistics ---" << endl;
    cout << "Total Books: " << totalBooks << endl;
    cout << "Available Books: " << availableBooks << endl;
    cout << "Checked Out Books: " << checkedOutBooks << endl;
    cout << "Availability Rate: " << fixed << setprecision(1) 
         << (double)availableBooks / totalBooks * 100 << "%" << endl;
}

int main() {
    vector<Book> library;
    
    // Initialize with sample data
    library.push_back({"The Great Gatsby", "F. Scott Fitzgerald", "9780743273565", false, "Fiction", 1925});
    library.push_back({"To Kill a Mockingbird", "Harper Lee", "9780061120084", true, "Fiction", 1960});
    library.push_back({"1984", "George Orwell", "9780451524935", false, "Dystopian", 1949});
    library.push_back({"Pride and Prejudice", "Jane Austen", "9780141439518", false, "Romance", 1813});
    library.push_back({"The Catcher in the Rye", "J.D. Salinger", "9780316769174", true, "Fiction", 1951});
    library.push_back({"Introduction to Algorithms", "Thomas H. Cormen", "9780262033848", false, "Computer Science", 2009});
    library.push_back({"Clean Code", "Robert C. Martin", "9780132350884", false, "Programming", 2008});

    cout << "===============================================" << endl;
    cout << "    WELCOME TO LIBRARY MANAGEMENT SYSTEM" << endl;
    cout << "===============================================" << endl;

    while (true) {
        cout << "\n" << string(50, '-') << endl;
        cout << "MAIN MENU" << endl;
        cout << string(50, '-') << endl;
        cout << "1. View All Books" << endl;
        cout << "2. Search Books" << endl;
        cout << "3. Add New Book" << endl;
        cout << "4. Remove Book" << endl;
        cout << "5. Checkout Book" << endl;
        cout << "6. Return Book" << endl;
        cout << "7. Library Statistics" << endl;
        cout << "8. Exit" << endl;
        cout << string(50, '-') << endl;
        cout << "Enter your choice (1-8): ";

        int choice;
        cin >> choice;

        // Input validation
        if (cin.fail()) {
            cout << "\nError: Invalid input. Please enter a number." << endl;
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1: {
                displayAllBooks(library);
                break;
            }
            case 2: {
                string query;
                cout << "\nEnter search query (Title, Author, ISBN, or Genre): ";
                clearInputBuffer();
                getline(cin, query);
                if (query.empty()) {
                    cout << "Error: Search query cannot be empty." << endl;
                    break;
                }
                searchBooks(library, query);
                break;
            }
            case 3: {
                addBook(library);
                break;
            }
            case 4: {
                removeBook(library);
                break;
            }
            case 5: {
                string ISBN;
                cout << "\nEnter the ISBN of the book to checkout: ";
                clearInputBuffer();
                getline(cin, ISBN);
                if (ISBN.empty()) {
                    cout << "Error: ISBN cannot be empty." << endl;
                    break;
                }
                checkoutBook(library, ISBN);
                break;
            }
            case 6: {
                string ISBN;
                cout << "\nEnter the ISBN of the book to return: ";
                clearInputBuffer();
                getline(cin, ISBN);
                if (ISBN.empty()) {
                    cout << "Error: ISBN cannot be empty." << endl;
                    break;
                }
                returnBook(library, ISBN);
                break;
            }
            case 7: {
                displayStatistics(library);
                break;
            }
            case 8: {
                cout << "\nThank you for using the Library Management System!" << endl;
                cout << "Goodbye!" << endl;
                return 0;
            }
            default: {
                cout << "\nError: Invalid choice. Please enter a number between 1 and 8." << endl;
                break;
            }
        }
        
        // Pause before showing menu again
        cout << "\nPress Enter to continue...";
        clearInputBuffer();
    }

    return 0;
}
