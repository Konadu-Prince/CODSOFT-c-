#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QListWidget>
#include <QGroupBox>
#include <QDesktopWidget>
#include <iostream>
#include <vector>
#include <string>

// Simple Book structure
struct Book {
    std::string title;
    std::string author;
    std::string ISBN;
    std::string genre;
    int year;
    bool checkedOut;
    
    Book(const std::string& t = "", const std::string& a = "", const std::string& i = "", 
         const std::string& g = "", int y = 0, bool co = false)
        : title(t), author(a), ISBN(i), genre(g), year(y), checkedOut(co) {}
};

class SimpleLibraryGUI : public QWidget {
    Q_OBJECT

private:
    std::vector<Book> books;
    QListWidget* bookList;
    QLineEdit* searchEdit;
    QTextEdit* detailsText;
    QLabel* statusLabel;
    
public:
    SimpleLibraryGUI(QWidget* parent = nullptr) : QWidget(parent) {
        setupUI();
        loadSampleData();
        updateBookList();
    }
    
private slots:
    void searchBooks() {
        QString query = searchEdit->text().toLower();
        bookList->clear();
        
        for (size_t i = 0; i < books.size(); ++i) {
            const Book& book = books[i];
            QString bookText = QString::fromStdString(book.title + " by " + book.author);
            
            if (query.isEmpty() || 
                bookText.toLower().contains(query) ||
                QString::fromStdString(book.genre).toLower().contains(query) ||
                QString::fromStdString(book.ISBN).contains(query)) {
                
                QListWidgetItem* item = new QListWidgetItem(bookText);
                item->setData(Qt::UserRole, static_cast<int>(i));
                bookList->addItem(item);
            }
        }
        
        statusLabel->setText(QString("Found %1 books").arg(bookList->count()));
    }
    
    void showBookDetails(QListWidgetItem* item) {
        if (!item) return;
        
        int index = item->data(Qt::UserRole).toInt();
        if (index >= 0 && index < static_cast<int>(books.size())) {
            const Book& book = books[index];
            
            QString details = QString(
                "Title: %1\n"
                "Author: %2\n"
                "ISBN: %3\n"
                "Genre: %4\n"
                "Year: %5\n"
                "Status: %6"
            ).arg(QString::fromStdString(book.title))
             .arg(QString::fromStdString(book.author))
             .arg(QString::fromStdString(book.ISBN))
             .arg(QString::fromStdString(book.genre))
             .arg(book.year)
             .arg(book.checkedOut ? "Checked Out" : "Available");
            
            detailsText->setPlainText(details);
        }
    }
    
    void addBook() {
        bool ok;
        QString title = QInputDialog::getText(this, "Add Book", "Enter book title:", QLineEdit::Normal, "", &ok);
        if (!ok || title.isEmpty()) return;
        
        QString author = QInputDialog::getText(this, "Add Book", "Enter author:", QLineEdit::Normal, "", &ok);
        if (!ok || author.isEmpty()) return;
        
        QString ISBN = QInputDialog::getText(this, "Add Book", "Enter ISBN:", QLineEdit::Normal, "", &ok);
        if (!ok || ISBN.isEmpty()) return;
        
        QString genre = QInputDialog::getText(this, "Add Book", "Enter genre:", QLineEdit::Normal, "", &ok);
        if (!ok || genre.isEmpty()) return;
        
        int year = QInputDialog::getInt(this, "Add Book", "Enter year:", 2024, 1000, 3000, 1, &ok);
        if (!ok) return;
        
        books.emplace_back(title.toStdString(), author.toStdString(), ISBN.toStdString(), 
                          genre.toStdString(), year, false);
        
        updateBookList();
        QMessageBox::information(this, "Success", "Book added successfully!");
    }
    
    void showStatistics() {
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
        
        QString stats = QString(
            "Library Statistics:\n\n"
            "Total Books: %1\n"
            "Available: %2\n"
            "Checked Out: %3\n"
            "Availability Rate: %4%"
        ).arg(total).arg(available).arg(checkedOut).arg(availabilityRate, 0, 'f', 1);
        
        QMessageBox::information(this, "Library Statistics", stats);
    }
    
private:
    void setupUI() {
        setWindowTitle("Library Management System - Desktop GUI");
        setMinimumSize(800, 600);
        resize(1000, 700);
        
        // Center window
        QDesktopWidget* desktop = QApplication::desktop();
        setGeometry(
            desktop->screen()->rect().center().x() - width()/2,
            desktop->screen()->rect().center().y() - height()/2,
            width(),
            height()
        );
        
        // Main layout
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        
        // Title
        QLabel* titleLabel = new QLabel("🏛️ Library Management System");
        titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2a82da; margin: 10px;");
        titleLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(titleLabel);
        
        // Search section
        QHBoxLayout* searchLayout = new QHBoxLayout();
        searchEdit = new QLineEdit();
        searchEdit->setPlaceholderText("Search books by title, author, genre, or ISBN...");
        QPushButton* searchButton = new QPushButton("Search");
        searchButton->setStyleSheet("QPushButton { background-color: #2a82da; color: white; padding: 8px 16px; border-radius: 4px; }");
        
        searchLayout->addWidget(searchEdit);
        searchLayout->addWidget(searchButton);
        mainLayout->addLayout(searchLayout);
        
        // Main content area
        QHBoxLayout* contentLayout = new QHBoxLayout();
        
        // Book list
        QGroupBox* listGroup = new QGroupBox("Books");
        QVBoxLayout* listLayout = new QVBoxLayout(listGroup);
        bookList = new QListWidget();
        bookList->setStyleSheet("QListWidget { border: 1px solid #ccc; border-radius: 4px; }");
        listLayout->addWidget(bookList);
        contentLayout->addWidget(listGroup, 2);
        
        // Book details
        QGroupBox* detailsGroup = new QGroupBox("Book Details");
        QVBoxLayout* detailsLayout = new QVBoxLayout(detailsGroup);
        detailsText = new QTextEdit();
        detailsText->setReadOnly(true);
        detailsText->setStyleSheet("QTextEdit { border: 1px solid #ccc; border-radius: 4px; background-color: #f9f9f9; }");
        detailsLayout->addWidget(detailsText);
        contentLayout->addWidget(detailsGroup, 1);
        
        mainLayout->addLayout(contentLayout, 1);
        
        // Buttons
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        QPushButton* addButton = new QPushButton("Add Book");
        addButton->setStyleSheet("QPushButton { background-color: #28a745; color: white; padding: 8px 16px; border-radius: 4px; }");
        
        QPushButton* statsButton = new QPushButton("Statistics");
        statsButton->setStyleSheet("QPushButton { background-color: #ffc107; color: black; padding: 8px 16px; border-radius: 4px; }");
        
        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(statsButton);
        buttonLayout->addStretch();
        mainLayout->addLayout(buttonLayout);
        
        // Status bar
        statusLabel = new QLabel("Ready");
        statusLabel->setStyleSheet("color: #666; padding: 5px;");
        mainLayout->addWidget(statusLabel);
        
        // Connect signals
        connect(searchButton, &QPushButton::clicked, this, &SimpleLibraryGUI::searchBooks);
        connect(searchEdit, &QLineEdit::returnPressed, this, &SimpleLibraryGUI::searchBooks);
        connect(bookList, &QListWidget::itemClicked, this, &SimpleLibraryGUI::showBookDetails);
        connect(addButton, &QPushButton::clicked, this, &SimpleLibraryGUI::addBook);
        connect(statsButton, &QPushButton::clicked, this, &SimpleLibraryGUI::showStatistics);
    }
    
    void loadSampleData() {
        books.emplace_back("The Great Gatsby", "F. Scott Fitzgerald", "9780743273565", "Fiction", 1925);
        books.emplace_back("To Kill a Mockingbird", "Harper Lee", "9780061120084", "Fiction", 1960);
        books.emplace_back("1984", "George Orwell", "9780451524935", "Dystopian", 1949);
        books.emplace_back("Clean Code", "Robert C. Martin", "9780132350884", "Programming", 2008);
        books.emplace_back("Design Patterns", "Gang of Four", "9780201633610", "Programming", 1994);
        books.emplace_back("Pride and Prejudice", "Jane Austen", "9780141439518", "Romance", 1813);
        books.emplace_back("The Catcher in the Rye", "J.D. Salinger", "9780316769174", "Fiction", 1951, true);
        books.emplace_back("Introduction to Algorithms", "Thomas H. Cormen", "9780262033848", "Computer Science", 2009);
    }
    
    void updateBookList() {
        bookList->clear();
        for (size_t i = 0; i < books.size(); ++i) {
            const Book& book = books[i];
            QString bookText = QString::fromStdString(book.title + " by " + book.author);
            QListWidgetItem* item = new QListWidgetItem(bookText);
            item->setData(Qt::UserRole, static_cast<int>(i));
            bookList->addItem(item);
        }
        statusLabel->setText(QString("Total books: %1").arg(books.size()));
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Library Management System");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("CODSOFT");
    
    // Create and show main window
    SimpleLibraryGUI window;
    window.show();
    
    return app.exec();
}

#include "simple_gui.moc"
