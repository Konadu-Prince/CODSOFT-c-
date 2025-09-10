#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

class UpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDialog(QWidget *parent = nullptr);
    void checkForUpdates();

private slots:
    void onUpdateCheckFinished();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished();
    void onDownloadError(QNetworkReply::NetworkError error);
    void downloadUpdate();
    void installUpdate();
    void skipUpdate();

private:
    void setupUI();
    void parseUpdateInfo(const QByteArray &data);
    bool isNewerVersion(const QString &remoteVersion);
    QString getCurrentVersion();
    
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_currentReply;
    
    // UI Components
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    QTextEdit *m_changelogText;
    QPushButton *m_downloadButton;
    QPushButton *m_installButton;
    QPushButton *m_skipButton;
    QPushButton *m_closeButton;
    
    // Update info
    QString m_latestVersion;
    QString m_downloadUrl;
    QString m_changelog;
    QString m_downloadedFilePath;
    bool m_updateAvailable;
};

#endif // UPDATEDIALOG_H
