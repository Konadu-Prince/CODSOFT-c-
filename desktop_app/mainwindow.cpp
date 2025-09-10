#include "mainwindow.h"
#include "bookdialog.h"
#include "database.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHeaderView>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_bookModel(new BookModel(this))
    , m_updateDialog(nullptr)
    , m_updateTimer(new QTimer(this))
{
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    connectSignals();
    
    // Set window properties
    setWindowTitle("Library Management System v1.0.0");
    setMinimumSize(1000, 700);
    resize(1200, 800);
    
    // Center window on screen
    setGeometry(
        QApplication::desktop()->screen()->rect().center().x() - width()/2,
        QApplication::desktop()->screen()->rect().center().y() - height()/2,
        width(),
        height()
    );
    
    // Auto-check for updates every 24 hours
    m_updateTimer->setInterval(24 * 60 * 60 * 1000); // 24 hours
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::checkForUpdates);
    m_updateTimer->start();
    
    // Initial update check
    QTimer::singleShot(5000, this, &MainWindow::checkForUpdates);
    
    updateStatistics();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Search bar
    QHBoxLayout *searchLayout = new QHBoxLayout();
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("Search books by title, author, ISBN, or genre...");
    m_searchButton = new QPushButton("Search");
    m_searchButton->setStyleSheet("QPushButton { background-color: #2a82da; color: white; border: none; padding: 8px 16px; border-radius: 4px; }");
    
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(m_searchButton);
    
    // Book table
    m_bookTable = new QTableView();
    m_bookTable->setModel(m_bookModel);
    m_bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_bookTable->setAlternatingRowColors(true);
    m_bookTable->setSortingEnabled(true);
    m_bookTable->horizontalHeader()->setStretchLastSection(true);
    m_bookTable->setStyleSheet(
        "QTableView { gridline-color: #404040; }"
        "QTableView::item:selected { background-color: #2a82da; }"
    );
    
    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_addButton = new QPushButton("Add Book");
    m_editButton = new QPushButton("Edit Book");
    m_deleteButton = new QPushButton("Delete Book");
    m_checkoutButton = new QPushButton("Checkout");
    m_returnButton = new QPushButton("Return");
    m_refreshButton = new QPushButton("Refresh");
    m_statsButton = new QPushButton("Statistics");
    
    // Style buttons
    QString buttonStyle = "QPushButton { padding: 8px 16px; border-radius: 4px; border: 1px solid #555; }";
    m_addButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #28a745; color: white; }");
    m_editButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #ffc107; color: black; }");
    m_deleteButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #dc3545; color: white; }");
    m_checkoutButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #17a2b8; color: white; }");
    m_returnButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #6f42c1; color: white; }");
    m_refreshButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #6c757d; color: white; }");
    m_statsButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #fd7e14; color: white; }");
    
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addWidget(m_checkoutButton);
    buttonLayout->addWidget(m_returnButton);
    buttonLayout->addWidget(m_refreshButton);
    buttonLayout->addWidget(m_statsButton);
    buttonLayout->addStretch();
    
    // Statistics panel
    QGroupBox *statsGroup = new QGroupBox("Library Statistics");
    QGridLayout *statsLayout = new QGridLayout(statsGroup);
    
    m_totalBooksLabel = new QLabel("Total Books: 0");
    m_availableBooksLabel = new QLabel("Available: 0");
    m_checkedOutBooksLabel = new QLabel("Checked Out: 0");
    m_availabilityRateLabel = new QLabel("Availability: 0%");
    
    statsLayout->addWidget(m_totalBooksLabel, 0, 0);
    statsLayout->addWidget(m_availableBooksLabel, 0, 1);
    statsLayout->addWidget(m_checkedOutBooksLabel, 1, 0);
    statsLayout->addWidget(m_availabilityRateLabel, 1, 1);
    
    // Assemble main layout
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(m_bookTable, 1);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(statsGroup);
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();
    
    // File menu
    QMenu *fileMenu = menuBar->addMenu("&File");
    QAction *importAction = fileMenu->addAction("&Import Books...");
    QAction *exportAction = fileMenu->addAction("&Export Books...");
    fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction("E&xit");
    
    connect(importAction, &QAction::triggered, this, &MainWindow::importData);
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportData);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Tools menu
    QMenu *toolsMenu = menuBar->addMenu("&Tools");
    QAction *checkUpdatesAction = toolsMenu->addAction("Check for &Updates");
    QAction *refreshAction = toolsMenu->addAction("&Refresh Library");
    
    connect(checkUpdatesAction, &QAction::triggered, this, &MainWindow::checkForUpdates);
    connect(refreshAction, &QAction::triggered, this, &MainWindow::refreshLibrary);
    
    // Help menu
    QMenu *helpMenu = menuBar->addMenu("&Help");
    QAction *aboutAction = helpMenu->addAction("&About");
    
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::setupToolBar()
{
    QToolBar *toolBar = addToolBar("Main Toolbar");
    toolBar->addAction("Add Book", this, &MainWindow::addBook);
    toolBar->addAction("Search", this, &MainWindow::searchBooks);
    toolBar->addSeparator();
    toolBar->addAction("Check Updates", this, &MainWindow::checkForUpdates);
}

void MainWindow::setupStatusBar()
{
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    m_statusLabel = new QLabel("Ready");
    
    statusBar()->addWidget(m_statusLabel, 1);
    statusBar()->addPermanentWidget(m_progressBar);
}

void MainWindow::connectSignals()
{
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::addBook);
    connect(m_editButton, &QPushButton::clicked, this, &MainWindow::editBook);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::deleteBook);
    connect(m_checkoutButton, &QPushButton::clicked, this, &MainWindow::checkoutBook);
    connect(m_returnButton, &QPushButton::clicked, this, &MainWindow::returnBook);
    connect(m_refreshButton, &QPushButton::clicked, this, &MainWindow::refreshLibrary);
    connect(m_statsButton, &QPushButton::clicked, this, &MainWindow::showStatistics);
    connect(m_searchButton, &QPushButton::clicked, this, &MainWindow::searchBooks);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MainWindow::searchBooks);
}

void MainWindow::addBook()
{
    BookDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Book book = dialog.getBook();
        if (Database::instance().addBook(book)) {
            m_bookModel->refreshData();
            updateStatistics();
            m_statusLabel->setText("Book added successfully");
        } else {
            QMessageBox::warning(this, "Error", "Failed to add book. ISBN might already exist.");
        }
    }
}

void MainWindow::editBook()
{
    QModelIndexList selection = m_bookTable->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::information(this, "No Selection", "Please select a book to edit.");
        return;
    }
    
    int row = selection.first().row();
    Book book = m_bookModel->getBookAt(row);
    
    BookDialog dialog(this, book);
    if (dialog.exec() == QDialog::Accepted) {
        Book updatedBook = dialog.getBook();
        if (Database::instance().updateBook(book.ISBN, updatedBook)) {
            m_bookModel->refreshData();
            updateStatistics();
            m_statusLabel->setText("Book updated successfully");
        } else {
            QMessageBox::warning(this, "Error", "Failed to update book.");
        }
    }
}

void MainWindow::deleteBook()
{
    QModelIndexList selection = m_bookTable->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::information(this, "No Selection", "Please select a book to delete.");
        return;
    }
    
    int row = selection.first().row();
    Book book = m_bookModel->getBookAt(row);
    
    int ret = QMessageBox::question(this, "Confirm Delete",
                                   QString("Are you sure you want to delete '%1' by %2?")
                                   .arg(book.title, book.author),
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        if (Database::instance().removeBook(book.ISBN)) {
            m_bookModel->refreshData();
            updateStatistics();
            m_statusLabel->setText("Book deleted successfully");
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete book.");
        }
    }
}

void MainWindow::searchBooks()
{
    QString query = m_searchEdit->text().trimmed();
    if (query.isEmpty()) {
        m_bookModel->refreshData();
    } else {
        m_bookModel->searchBooks(query);
    }
    updateStatistics();
}

void MainWindow::checkoutBook()
{
    QModelIndexList selection = m_bookTable->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::information(this, "No Selection", "Please select a book to checkout.");
        return;
    }
    
    int row = selection.first().row();
    Book book = m_bookModel->getBookAt(row);
    
    if (book.checkedOut) {
        QMessageBox::information(this, "Already Checked Out", "This book is already checked out.");
        return;
    }
    
    bool ok;
    QString borrower = QInputDialog::getText(this, "Checkout Book",
                                           "Enter borrower name:",
                                           QLineEdit::Normal, "", &ok);
    
    if (ok && !borrower.isEmpty()) {
        book.checkedOut = true;
        if (Database::instance().updateBook(book.ISBN, book)) {
            m_bookModel->refreshData();
            updateStatistics();
            m_statusLabel->setText(QString("Book checked out to %1").arg(borrower));
        }
    }
}

void MainWindow::returnBook()
{
    QModelIndexList selection = m_bookTable->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::information(this, "No Selection", "Please select a book to return.");
        return;
    }
    
    int row = selection.first().row();
    Book book = m_bookModel->getBookAt(row);
    
    if (!book.checkedOut) {
        QMessageBox::information(this, "Not Checked Out", "This book is not checked out.");
        return;
    }
    
    book.checkedOut = false;
    if (Database::instance().updateBook(book.ISBN, book)) {
        m_bookModel->refreshData();
        updateStatistics();
        m_statusLabel->setText("Book returned successfully");
    }
}

void MainWindow::refreshLibrary()
{
    m_bookModel->refreshData();
    updateStatistics();
    m_statusLabel->setText("Library refreshed");
}

void MainWindow::showStatistics()
{
    QVector<Book> books = Database::instance().getAllBooks();
    int total = books.size();
    int available = 0;
    int checkedOut = 0;
    
    for (const Book &book : books) {
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
        "Availability Rate: %4%\n\n"
        "Most Popular Genre: %5"
    ).arg(total).arg(available).arg(checkedOut)
     .arg(QString::number(availabilityRate, 'f', 1))
     .arg("Fiction"); // TODO: Calculate actual most popular genre
    
    QMessageBox::information(this, "Library Statistics", stats);
}

void MainWindow::checkForUpdates()
{
    if (!m_updateDialog) {
        m_updateDialog = new UpdateDialog(this);
    }
    m_updateDialog->checkForUpdates();
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "About Library Management System",
                      "Library Management System v1.0.0\n\n"
                      "A modern desktop application for managing library collections.\n\n"
                      "Developed as part of CODSOFT C++ Internship Program\n"
                      "© 2024 CODSOFT. All rights reserved.");
}

void MainWindow::exportData()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                   "Export Books",
                                                   "books_export.json",
                                                   "JSON Files (*.json)");
    
    if (!fileName.isEmpty()) {
        QVector<Book> books = Database::instance().getAllBooks();
        QJsonArray jsonArray;
        
        for (const Book &book : books) {
            QJsonObject jsonBook;
            jsonBook["title"] = book.title;
            jsonBook["author"] = book.author;
            jsonBook["ISBN"] = book.ISBN;
            jsonBook["genre"] = book.genre;
            jsonBook["year"] = book.year;
            jsonBook["checkedOut"] = book.checkedOut;
            jsonArray.append(jsonBook);
        }
        
        QJsonDocument doc(jsonArray);
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
            m_statusLabel->setText("Data exported successfully");
        } else {
            QMessageBox::warning(this, "Export Error", "Failed to export data.");
        }
    }
}

void MainWindow::importData()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Import Books",
                                                   "",
                                                   "JSON Files (*.json)");
    
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            QJsonArray jsonArray = doc.array();
            
            int imported = 0;
            for (const QJsonValue &value : jsonArray) {
                QJsonObject jsonBook = value.toObject();
                Book book;
                book.title = jsonBook["title"].toString();
                book.author = jsonBook["author"].toString();
                book.ISBN = jsonBook["ISBN"].toString();
                book.genre = jsonBook["genre"].toString();
                book.year = jsonBook["year"].toInt();
                book.checkedOut = jsonBook["checkedOut"].toBool();
                
                if (Database::instance().addBook(book)) {
                    imported++;
                }
            }
            
            m_bookModel->refreshData();
            updateStatistics();
            m_statusLabel->setText(QString("Imported %1 books").arg(imported));
        } else {
            QMessageBox::warning(this, "Import Error", "Failed to open file.");
        }
    }
}

void MainWindow::updateStatistics()
{
    QVector<Book> books = Database::instance().getAllBooks();
    int total = books.size();
    int available = 0;
    int checkedOut = 0;
    
    for (const Book &book : books) {
        if (book.checkedOut) {
            checkedOut++;
        } else {
            available++;
        }
    }
    
    double availabilityRate = total > 0 ? (double)available / total * 100 : 0;
    
    m_totalBooksLabel->setText(QString("Total Books: %1").arg(total));
    m_availableBooksLabel->setText(QString("Available: %1").arg(available));
    m_checkedOutBooksLabel->setText(QString("Checked Out: %1").arg(checkedOut));
    m_availabilityRateLabel->setText(QString("Availability: %1%").arg(QString::number(availabilityRate, 'f', 1)));
}
