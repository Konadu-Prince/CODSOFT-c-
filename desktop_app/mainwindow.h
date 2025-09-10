#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QTabWidget>
#include <QStatusBar>
#include <QMenuBar>
#include <QAction>
#include <QProgressBar>
#include <QTimer>
#include "bookmodel.h"
#include "updatedialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addBook();
    void editBook();
    void deleteBook();
    void searchBooks();
    void checkoutBook();
    void returnBook();
    void refreshLibrary();
    void showStatistics();
    void checkForUpdates();
    void showAbout();
    void exportData();
    void importData();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void connectSignals();
    void updateStatistics();
    
    // UI Components
    QTabWidget *m_tabWidget;
    QTableView *m_bookTable;
    QLineEdit *m_searchEdit;
    QPushButton *m_searchButton;
    QPushButton *m_addButton;
    QPushButton *m_editButton;
    QPushButton *m_deleteButton;
    QPushButton *m_checkoutButton;
    QPushButton *m_returnButton;
    QPushButton *m_refreshButton;
    QPushButton *m_statsButton;
    
    // Statistics widgets
    QLabel *m_totalBooksLabel;
    QLabel *m_availableBooksLabel;
    QLabel *m_checkedOutBooksLabel;
    QLabel *m_availabilityRateLabel;
    
    // Model
    BookModel *m_bookModel;
    
    // Update system
    UpdateDialog *m_updateDialog;
    QTimer *m_updateTimer;
    
    // Status bar
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;
};

#endif // MAINWINDOW_H
