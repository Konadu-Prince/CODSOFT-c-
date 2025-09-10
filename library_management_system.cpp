#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Book structure to store book information
struct Book {
    string title;
    string author;
    string ISBN;
    bool checkedOut;
};

// Function to search for books
void searchBooks(const vector<Book>& library, const string& query) {
    cout << "Search Results:" << endl;
    for (const Book& book : library) {
        if (book.title.find(query) != string::npos || book.author.find(query) != string::npos || book.ISBN.find(query) != string::npos) {
            cout << "Title: " << book.title << ", Author: " << book.author << ", ISBN: " << book.ISBN;
            if (book.checkedOut) {
                cout << " (Checked Out)";
            }
            cout << endl;
        }
    }
}

// Function to checkout a book
void checkoutBook(vector<Book>& library, const string& ISBN) {
    for (Book& book : library) {
        if (book.ISBN == ISBN) {
            if (book.checkedOut) {
                cout << "This book is already checked out." << endl;
            } else {
                book.checkedOut = true;
                cout << "Book checked out successfully." << endl;
            }
            return;
        }
    }
    cout << "Book not found." << endl;
}

// Function to return a book
void returnBook(vector<Book>& library, const string& ISBN) {
    for (Book& book : library) {
        if (book.ISBN == ISBN) {
            if (book.checkedOut) {
                book.checkedOut = false;
                cout << "Book returned successfully." << endl;
            } else {
                cout << "This book is not checked out." << endl;
            }
            return;
        }
    }
    cout << "Book not found." << endl;
}

int main() {
    vector<Book> library;
    library.push_back({"Book1", "Author1", "1234567890", false});
    library.push_back({"Book2", "Author2", "2345678901", true});
    library.push_back({"Book3", "Author3", "3456789012", false});

    while (true) {
        cout << "Library Management System" << endl;
        cout << "1. Search Books" << endl;
        cout << "2. Checkout Book" << endl;
        cout << "3. Return Book" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                cin.ignore(); // Clear the newline character from the buffer
                string query;
                cout << "Enter search query (Title, Author, or ISBN): ";
                getline(cin, query);
                searchBooks(library, query);
                break;
            }
            case 2: {
                string ISBN;
                cout << "Enter the ISBN of the book to checkout: ";
                cin >> ISBN;
                checkoutBook(library, ISBN);
                break;
            }
            case 3: {
                string ISBN;
                cout << "Enter the ISBN of the book to return: ";
                cin >> ISBN;
                returnBook(library, ISBN);
                break;
            }
            case 4:
                return 0;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}
