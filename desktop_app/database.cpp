#include "database.h"
#include <QApplication>
#include <QSqlRecord>

Database& Database::instance()
{
    static Database instance;
    return instance;
}

bool Database::initialize()
{
    // Set up database path
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    QString dbPath = dataPath + "/library.db";
    
    // Initialize SQLite database
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(dbPath);
    
    if (!m_database.open()) {
        qDebug() << "Failed to open database:" << m_database.lastError().text();
        return false;
    }
    
    // Create tables
    if (!createTables()) {
        qDebug() << "Failed to create tables";
        return false;
    }
    
    // Insert sample data if database is empty
    if (getTotalBooks() == 0) {
        insertSampleData();
    }
    
    return true;
}

bool Database::createTables()
{
    QSqlQuery query;
    
    // Create books table
    QString createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS books (
            isbn TEXT PRIMARY KEY,
            title TEXT NOT NULL,
            author TEXT NOT NULL,
            genre TEXT NOT NULL,
            year INTEGER NOT NULL,
            checked_out BOOLEAN NOT NULL DEFAULT 0,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";
    
    if (!query.exec(createTableSQL)) {
        qDebug() << "Failed to create books table:" << query.lastError().text();
        return false;
    }
    
    // Create index for faster searches
    query.exec("CREATE INDEX IF NOT EXISTS idx_books_title ON books(title)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_books_author ON books(author)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_books_genre ON books(genre)");
    
    return true;
}

void Database::insertSampleData()
{
    QVector<Book> sampleBooks = {
        Book("The Great Gatsby", "F. Scott Fitzgerald", "9780743273565", "Fiction", 1925),
        Book("To Kill a Mockingbird", "Harper Lee", "9780061120084", "Fiction", 1960),
        Book("1984", "George Orwell", "9780451524935", "Fiction", 1949),
        Book("Pride and Prejudice", "Jane Austen", "9780141439518", "Fiction", 1813),
        Book("The Catcher in the Rye", "J.D. Salinger", "9780316769174", "Fiction", 1951),
        Book("A Brief History of Time", "Stephen Hawking", "9780553380163", "Science", 1988),
        Book("Clean Code", "Robert C. Martin", "9780132350884", "Programming", 2008),
        Book("Design Patterns", "Gang of Four", "9780201633610", "Programming", 1994),
        Book("The Art of Computer Programming", "Donald Knuth", "9780321751041", "Programming", 1968),
        Book("Introduction to Algorithms", "Thomas H. Cormen", "9780262033848", "Programming", 2009)
    };
    
    for (const Book &book : sampleBooks) {
        addBook(book);
    }
}

bool Database::addBook(const Book &book)
{
    if (bookExists(book.ISBN)) {
        return false; // Book already exists
    }
    
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO books (isbn, title, author, genre, year, checked_out)
        VALUES (?, ?, ?, ?, ?, ?)
    )");
    
    query.addBindValue(book.ISBN);
    query.addBindValue(book.title);
    query.addBindValue(book.author);
    query.addBindValue(book.genre);
    query.addBindValue(book.year);
    query.addBindValue(book.checkedOut);
    
    if (!query.exec()) {
        qDebug() << "Failed to add book:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool Database::updateBook(const QString &isbn, const Book &book)
{
    QSqlQuery query;
    query.prepare(R"(
        UPDATE books 
        SET title = ?, author = ?, genre = ?, year = ?, checked_out = ?, updated_at = CURRENT_TIMESTAMP
        WHERE isbn = ?
    )");
    
    query.addBindValue(book.title);
    query.addBindValue(book.author);
    query.addBindValue(book.genre);
    query.addBindValue(book.year);
    query.addBindValue(book.checkedOut);
    query.addBindValue(isbn);
    
    if (!query.exec()) {
        qDebug() << "Failed to update book:" << query.lastError().text();
        return false;
    }
    
    return query.numRowsAffected() > 0;
}

bool Database::removeBook(const QString &isbn)
{
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE isbn = ?");
    query.addBindValue(isbn);
    
    if (!query.exec()) {
        qDebug() << "Failed to remove book:" << query.lastError().text();
        return false;
    }
    
    return query.numRowsAffected() > 0;
}

QVector<Book> Database::getAllBooks()
{
    QVector<Book> books;
    QSqlQuery query("SELECT isbn, title, author, genre, year, checked_out FROM books ORDER BY title");
    
    while (query.next()) {
        Book book;
        book.ISBN = query.value(0).toString();
        book.title = query.value(1).toString();
        book.author = query.value(2).toString();
        book.genre = query.value(3).toString();
        book.year = query.value(4).toInt();
        book.checkedOut = query.value(5).toBool();
        books.append(book);
    }
    
    return books;
}

QVector<Book> Database::searchBooks(const QString &query)
{
    QVector<Book> books;
    QSqlQuery sqlQuery;
    
    QString searchSQL = R"(
        SELECT isbn, title, author, genre, year, checked_out 
        FROM books 
        WHERE title LIKE ? OR author LIKE ? OR isbn LIKE ? OR genre LIKE ?
        ORDER BY title
    )";
    
    sqlQuery.prepare(searchSQL);
    QString searchPattern = "%" + query + "%";
    sqlQuery.addBindValue(searchPattern);
    sqlQuery.addBindValue(searchPattern);
    sqlQuery.addBindValue(searchPattern);
    sqlQuery.addBindValue(searchPattern);
    
    if (!sqlQuery.exec()) {
        qDebug() << "Search failed:" << sqlQuery.lastError().text();
        return books;
    }
    
    while (sqlQuery.next()) {
        Book book;
        book.ISBN = sqlQuery.value(0).toString();
        book.title = sqlQuery.value(1).toString();
        book.author = sqlQuery.value(2).toString();
        book.genre = sqlQuery.value(3).toString();
        book.year = sqlQuery.value(4).toInt();
        book.checkedOut = sqlQuery.value(5).toBool();
        books.append(book);
    }
    
    return books;
}

Book Database::getBookByISBN(const QString &isbn)
{
    Book book;
    QSqlQuery query;
    query.prepare("SELECT isbn, title, author, genre, year, checked_out FROM books WHERE isbn = ?");
    query.addBindValue(isbn);
    
    if (query.exec() && query.next()) {
        book.ISBN = query.value(0).toString();
        book.title = query.value(1).toString();
        book.author = query.value(2).toString();
        book.genre = query.value(3).toString();
        book.year = query.value(4).toInt();
        book.checkedOut = query.value(5).toBool();
    }
    
    return book;
}

bool Database::bookExists(const QString &isbn)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM books WHERE isbn = ?");
    query.addBindValue(isbn);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    
    return false;
}

int Database::getTotalBooks()
{
    QSqlQuery query("SELECT COUNT(*) FROM books");
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

int Database::getAvailableBooks()
{
    QSqlQuery query("SELECT COUNT(*) FROM books WHERE checked_out = 0");
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

int Database::getCheckedOutBooks()
{
    QSqlQuery query("SELECT COUNT(*) FROM books WHERE checked_out = 1");
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

double Database::getAvailabilityRate()
{
    int total = getTotalBooks();
    if (total == 0) return 0.0;
    
    int available = getAvailableBooks();
    return (double)available / total * 100.0;
}
