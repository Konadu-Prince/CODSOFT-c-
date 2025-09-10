#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

struct Book {
    QString title;
    QString author;
    QString ISBN;
    QString genre;
    int year;
    bool checkedOut;
    
    Book() : year(0), checkedOut(false) {}
    Book(const QString &t, const QString &a, const QString &i, const QString &g, int y, bool co = false)
        : title(t), author(a), ISBN(i), genre(g), year(y), checkedOut(co) {}
};

class Database : public QObject
{
    Q_OBJECT

public:
    static Database& instance();
    
    bool initialize();
    bool addBook(const Book &book);
    bool updateBook(const QString &isbn, const Book &book);
    bool removeBook(const QString &isbn);
    QVector<Book> getAllBooks();
    QVector<Book> searchBooks(const QString &query);
    Book getBookByISBN(const QString &isbn);
    bool bookExists(const QString &isbn);
    
    // Statistics
    int getTotalBooks();
    int getAvailableBooks();
    int getCheckedOutBooks();
    double getAvailabilityRate();

private:
    Database() = default;
    ~Database() = default;
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    
    QSqlDatabase m_database;
    bool createTables();
    void insertSampleData();
};

#endif // DATABASE_H
