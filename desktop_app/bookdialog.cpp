#include "bookdialog.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QDate>

BookDialog::BookDialog(QWidget *parent, const Book &book)
    : QDialog(parent)
    , m_originalBook(book)
    , m_isEdit(!book.ISBN.isEmpty())
{
    setupUI();
    setWindowTitle(m_isEdit ? "Edit Book" : "Add New Book");
    setModal(true);
    resize(400, 300);
    
    if (m_isEdit) {
        m_titleEdit->setText(book.title);
        m_authorEdit->setText(book.author);
        m_isbnEdit->setText(book.ISBN);
        m_isbnEdit->setEnabled(false); // Don't allow ISBN changes
        m_genreCombo->setCurrentText(book.genre);
        m_yearSpinBox->setValue(book.year);
    }
}

void BookDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Form layout
    QFormLayout *formLayout = new QFormLayout();
    
    // Title
    m_titleEdit = new QLineEdit();
    m_titleEdit->setPlaceholderText("Enter book title");
    m_titleEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid #555; border-radius: 4px; }");
    formLayout->addRow("Title:", m_titleEdit);
    
    // Author
    m_authorEdit = new QLineEdit();
    m_authorEdit->setPlaceholderText("Enter author name");
    m_authorEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid #555; border-radius: 4px; }");
    formLayout->addRow("Author:", m_authorEdit);
    
    // ISBN
    m_isbnEdit = new QLineEdit();
    m_isbnEdit->setPlaceholderText("Enter ISBN (10 or 13 digits)");
    m_isbnEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid #555; border-radius: 4px; }");
    formLayout->addRow("ISBN:", m_isbnEdit);
    
    // Genre
    m_genreCombo = new QComboBox();
    m_genreCombo->setEditable(true);
    m_genreCombo->addItems({
        "Fiction", "Non-Fiction", "Science", "Programming", "History",
        "Biography", "Mystery", "Romance", "Fantasy", "Science Fiction",
        "Thriller", "Horror", "Poetry", "Drama", "Comedy", "Other"
    });
    m_genreCombo->setStyleSheet("QComboBox { padding: 8px; border: 1px solid #555; border-radius: 4px; }");
    formLayout->addRow("Genre:", m_genreCombo);
    
    // Year
    m_yearSpinBox = new QSpinBox();
    m_yearSpinBox->setRange(1000, QDate::currentDate().year());
    m_yearSpinBox->setValue(QDate::currentDate().year());
    m_yearSpinBox->setStyleSheet("QSpinBox { padding: 8px; border: 1px solid #555; border-radius: 4px; }");
    formLayout->addRow("Year:", m_yearSpinBox);
    
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch();
    
    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_okButton = new QPushButton(m_isEdit ? "Update" : "Add");
    m_okButton->setStyleSheet("QPushButton { background-color: #28a745; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    m_cancelButton = new QPushButton("Cancel");
    m_cancelButton->setStyleSheet("QPushButton { background-color: #6c757d; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(m_okButton, &QPushButton::clicked, this, &BookDialog::accept);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_titleEdit, &QLineEdit::textChanged, this, &BookDialog::validateInput);
    connect(m_authorEdit, &QLineEdit::textChanged, this, &BookDialog::validateInput);
    connect(m_isbnEdit, &QLineEdit::textChanged, this, &BookDialog::validateInput);
    
    // Initial validation
    validateInput();
}

Book BookDialog::getBook() const
{
    Book book;
    book.title = m_titleEdit->text().trimmed();
    book.author = m_authorEdit->text().trimmed();
    book.ISBN = m_isbnEdit->text().trimmed();
    book.genre = m_genreCombo->currentText().trimmed();
    book.year = m_yearSpinBox->value();
    book.checkedOut = m_originalBook.checkedOut; // Preserve checkout status
    return book;
}

void BookDialog::validateInput()
{
    bool isValid = true;
    
    // Check if all required fields are filled
    if (m_titleEdit->text().trimmed().isEmpty()) {
        isValid = false;
    }
    
    if (m_authorEdit->text().trimmed().isEmpty()) {
        isValid = false;
    }
    
    if (m_isbnEdit->text().trimmed().isEmpty()) {
        isValid = false;
    } else if (!validateISBN(m_isbnEdit->text().trimmed())) {
        isValid = false;
    }
    
    if (m_genreCombo->currentText().trimmed().isEmpty()) {
        isValid = false;
    }
    
    m_okButton->setEnabled(isValid);
}

bool BookDialog::validateISBN(const QString &isbn)
{
    // Remove any non-digit characters
    QString cleanISBN = isbn;
    cleanISBN.remove(QRegularExpression("[^0-9]"));
    
    // Check length (10 or 13 digits)
    if (cleanISBN.length() != 10 && cleanISBN.length() != 13) {
        return false;
    }
    
    // Basic ISBN validation
    if (cleanISBN.length() == 10) {
        // ISBN-10 validation
        int sum = 0;
        for (int i = 0; i < 9; ++i) {
            sum += cleanISBN[i].digitValue() * (10 - i);
        }
        int checkDigit = (11 - (sum % 11)) % 11;
        return checkDigit == cleanISBN[9].digitValue();
    } else {
        // ISBN-13 validation
        int sum = 0;
        for (int i = 0; i < 12; ++i) {
            int digit = cleanISBN[i].digitValue();
            sum += (i % 2 == 0) ? digit : digit * 3;
        }
        int checkDigit = (10 - (sum % 10)) % 10;
        return checkDigit == cleanISBN[12].digitValue();
    }
}

void BookDialog::accept()
{
    // Final validation
    if (m_titleEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a book title.");
        m_titleEdit->setFocus();
        return;
    }
    
    if (m_authorEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter an author name.");
        m_authorEdit->setFocus();
        return;
    }
    
    if (m_isbnEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter an ISBN.");
        m_isbnEdit->setFocus();
        return;
    }
    
    if (!validateISBN(m_isbnEdit->text().trimmed())) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid ISBN (10 or 13 digits).");
        m_isbnEdit->setFocus();
        return;
    }
    
    if (m_genreCombo->currentText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please select or enter a genre.");
        m_genreCombo->setFocus();
        return;
    }
    
    // Check for duplicate ISBN (only for new books)
    if (!m_isEdit && Database::instance().bookExists(m_isbnEdit->text().trimmed())) {
        QMessageBox::warning(this, "Duplicate ISBN", "A book with this ISBN already exists.");
        m_isbnEdit->setFocus();
        return;
    }
    
    QDialog::accept();
}
