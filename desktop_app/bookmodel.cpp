#include "bookmodel.h"
#include "database.h"
#include <QColor>

BookModel::BookModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    refreshData();
}

int BookModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_books.size();
}

int BookModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 6; // Title, Author, ISBN, Genre, Year, Status
}

QVariant BookModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_books.size()) {
        return QVariant();
    }
    
    const Book &book = m_books[index.row()];
    
    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return book.title;
        case 1: return book.author;
        case 2: return book.ISBN;
        case 3: return book.genre;
        case 4: return book.year;
        case 5: return book.checkedOut ? "Checked Out" : "Available";
        }
        break;
        
    case Qt::TextAlignmentRole:
        if (index.column() == 4) { // Year column
            return Qt::AlignCenter;
        }
        break;
        
    case Qt::BackgroundRole:
        if (book.checkedOut) {
            return QColor(53, 53, 53); // Darker background for checked out books
        }
        break;
        
    case Qt::ForegroundRole:
        if (book.checkedOut) {
            return QColor(220, 53, 69); // Red text for checked out books
        }
        break;
        
    case Qt::ToolTipRole:
        return QString("Title: %1\nAuthor: %2\nISBN: %3\nGenre: %4\nYear: %5\nStatus: %6")
               .arg(book.title, book.author, book.ISBN, book.genre)
               .arg(book.year)
               .arg(book.checkedOut ? "Checked Out" : "Available");
    }
    
    return QVariant();
}

QVariant BookModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return "Title";
        case 1: return "Author";
        case 2: return "ISBN";
        case 3: return "Genre";
        case 4: return "Year";
        case 5: return "Status";
        }
    }
    
    return QVariant();
}

bool BookModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }
    
    Book &book = m_books[index.row()];
    
    switch (index.column()) {
    case 0: book.title = value.toString(); break;
    case 1: book.author = value.toString(); break;
    case 2: book.ISBN = value.toString(); break;
    case 3: book.genre = value.toString(); break;
    case 4: book.year = value.toInt(); break;
    case 5: book.checkedOut = value.toBool(); break;
    default: return false;
    }
    
    emit dataChanged(index, index, {role});
    return true;
}

Qt::ItemFlags BookModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    
    // Make certain columns editable
    if (index.column() == 0 || index.column() == 1 || index.column() == 3 || index.column() == 4) {
        flags |= Qt::ItemIsEditable;
    }
    
    return flags;
}

void BookModel::refreshData()
{
    beginResetModel();
    m_books = Database::instance().getAllBooks();
    endResetModel();
}

void BookModel::searchBooks(const QString &query)
{
    beginResetModel();
    if (query.isEmpty()) {
        m_books = Database::instance().getAllBooks();
    } else {
        m_books = Database::instance().searchBooks(query);
    }
    m_searchQuery = query;
    endResetModel();
}

Book BookModel::getBookAt(int row) const
{
    if (row >= 0 && row < m_books.size()) {
        return m_books[row];
    }
    return Book();
}
