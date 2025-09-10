#ifndef BOOKDIALOG_H
#define BOOKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "database.h"

class BookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookDialog(QWidget *parent = nullptr, const Book &book = Book());

    Book getBook() const;

private slots:
    void validateInput();
    void accept() override;

private:
    void setupUI();
    bool validateISBN(const QString &isbn);
    
    QLineEdit *m_titleEdit;
    QLineEdit *m_authorEdit;
    QLineEdit *m_isbnEdit;
    QComboBox *m_genreCombo;
    QSpinBox *m_yearSpinBox;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
    
    Book m_originalBook;
    bool m_isEdit;
};

#endif // BOOKDIALOG_H
