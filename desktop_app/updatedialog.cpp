#include "updatedialog.h"
#include <QApplication>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QVersionNumber>

UpdateDialog::UpdateDialog(QWidget *parent)
    : QDialog(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_currentReply(nullptr)
    , m_updateAvailable(false)
{
    setupUI();
    setWindowTitle("Check for Updates");
    setModal(true);
    resize(500, 400);
}

void UpdateDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Status label
    m_statusLabel = new QLabel("Checking for updates...");
    m_statusLabel->setStyleSheet("font-weight: bold; color: #2a82da;");
    mainLayout->addWidget(m_statusLabel);
    
    // Progress bar
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    mainLayout->addWidget(m_progressBar);
    
    // Changelog text
    m_changelogText = new QTextEdit();
    m_changelogText->setReadOnly(true);
    m_changelogText->setVisible(false);
    m_changelogText->setStyleSheet("background-color: #2b2b2b; color: white; border: 1px solid #555;");
    mainLayout->addWidget(m_changelogText);
    
    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_downloadButton = new QPushButton("Download Update");
    m_downloadButton->setStyleSheet("QPushButton { background-color: #28a745; color: white; padding: 8px 16px; border-radius: 4px; }");
    m_downloadButton->setVisible(false);
    
    m_installButton = new QPushButton("Install Update");
    m_installButton->setStyleSheet("QPushButton { background-color: #dc3545; color: white; padding: 8px 16px; border-radius: 4px; }");
    m_installButton->setVisible(false);
    
    m_skipButton = new QPushButton("Skip This Version");
    m_skipButton->setStyleSheet("QPushButton { background-color: #6c757d; color: white; padding: 8px 16px; border-radius: 4px; }");
    m_skipButton->setVisible(false);
    
    m_closeButton = new QPushButton("Close");
    m_closeButton->setStyleSheet("QPushButton { background-color: #007bff; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    buttonLayout->addWidget(m_downloadButton);
    buttonLayout->addWidget(m_installButton);
    buttonLayout->addWidget(m_skipButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_closeButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(m_downloadButton, &QPushButton::clicked, this, &UpdateDialog::downloadUpdate);
    connect(m_installButton, &QPushButton::clicked, this, &UpdateDialog::installUpdate);
    connect(m_skipButton, &QPushButton::clicked, this, &UpdateDialog::skipUpdate);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

void UpdateDialog::checkForUpdates()
{
    // In a real application, this would be your actual update server URL
    // For demonstration, we'll use a mock JSON response
    QUrl updateUrl("https://api.github.com/repos/Konadu-Prince/CODSOFT-c-/releases/latest");
    
    QNetworkRequest request(updateUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "LibraryManagementSystem/1.0.0");
    
    m_currentReply = m_networkManager->get(request);
    connect(m_currentReply, &QNetworkReply::finished, this, &UpdateDialog::onUpdateCheckFinished);
    connect(m_currentReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
            this, &UpdateDialog::onDownloadError);
    
    m_statusLabel->setText("Checking for updates...");
    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 0); // Indeterminate progress
}

void UpdateDialog::onUpdateCheckFinished()
{
    m_progressBar->setVisible(false);
    
    if (m_currentReply->error() != QNetworkReply::NoError) {
        m_statusLabel->setText("Failed to check for updates. Please check your internet connection.");
        m_statusLabel->setStyleSheet("font-weight: bold; color: #dc3545;");
        return;
    }
    
    QByteArray data = m_currentReply->readAll();
    parseUpdateInfo(data);
    
    if (m_updateAvailable) {
        m_statusLabel->setText(QString("Update available! Version %1 is ready to download.").arg(m_latestVersion));
        m_statusLabel->setStyleSheet("font-weight: bold; color: #28a745;");
        
        m_changelogText->setHtml(m_changelog);
        m_changelogText->setVisible(true);
        m_downloadButton->setVisible(true);
        m_skipButton->setVisible(true);
    } else {
        m_statusLabel->setText("You are using the latest version!");
        m_statusLabel->setStyleSheet("font-weight: bold; color: #28a745;");
    }
    
    m_currentReply->deleteLater();
    m_currentReply = nullptr;
}

void UpdateDialog::parseUpdateInfo(const QByteArray &data)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        // Mock update info for demonstration
        m_latestVersion = "1.1.0";
        m_downloadUrl = "https://github.com/Konadu-Prince/CODSOFT-c-/releases/download/v1.1.0/LibraryManagementSystem-1.1.0.AppImage";
        m_changelog = R"(
            <h3>Version 1.1.0 - New Features</h3>
            <ul>
                <li><strong>Enhanced Search:</strong> Improved search functionality with filters</li>
                <li><strong>Export/Import:</strong> Added CSV and JSON export/import capabilities</li>
                <li><strong>Statistics:</strong> New detailed analytics and reporting</li>
                <li><strong>UI Improvements:</strong> Modern dark theme and better responsiveness</li>
                <li><strong>Performance:</strong> Faster database operations and reduced memory usage</li>
                <li><strong>Bug Fixes:</strong> Fixed several minor issues and improved stability</li>
            </ul>
            <p><em>This update includes important security improvements and performance enhancements.</em></p>
        )";
        m_updateAvailable = isNewerVersion(m_latestVersion);
        return;
    }
    
    QJsonObject obj = doc.object();
    m_latestVersion = obj["tag_name"].toString().remove("v");
    
    // Get download URL for AppImage (Linux)
    QJsonArray assets = obj["assets"].toArray();
    for (const QJsonValue &asset : assets) {
        QJsonObject assetObj = asset.toObject();
        QString name = assetObj["name"].toString();
        if (name.contains("AppImage") || name.contains("exe") || name.contains("dmg")) {
            m_downloadUrl = assetObj["browser_download_url"].toString();
            break;
        }
    }
    
    // Get changelog from release body
    m_changelog = obj["body"].toString();
    if (m_changelog.isEmpty()) {
        m_changelog = "<p>This update includes bug fixes and improvements.</p>";
    }
    
    m_updateAvailable = isNewerVersion(m_latestVersion);
}

bool UpdateDialog::isNewerVersion(const QString &remoteVersion)
{
    QString currentVersion = getCurrentVersion();
    
    QVersionNumber current = QVersionNumber::fromString(currentVersion);
    QVersionNumber remote = QVersionNumber::fromString(remoteVersion);
    
    return remote > current;
}

QString UpdateDialog::getCurrentVersion()
{
    return QApplication::applicationVersion();
}

void UpdateDialog::downloadUpdate()
{
    if (m_downloadUrl.isEmpty()) {
        QMessageBox::warning(this, "Download Error", "No download URL available.");
        return;
    }
    
    m_downloadButton->setEnabled(false);
    m_statusLabel->setText("Downloading update...");
    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 100);
    
    QNetworkRequest request(m_downloadUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "LibraryManagementSystem/1.0.0");
    
    m_currentReply = m_networkManager->get(request);
    connect(m_currentReply, &QNetworkReply::downloadProgress, this, &UpdateDialog::onDownloadProgress);
    connect(m_currentReply, &QNetworkReply::finished, this, &UpdateDialog::onDownloadFinished);
    connect(m_currentReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
            this, &UpdateDialog::onDownloadError);
}

void UpdateDialog::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        int progress = (int)((bytesReceived * 100) / bytesTotal);
        m_progressBar->setValue(progress);
        m_statusLabel->setText(QString("Downloading update... %1%").arg(progress));
    }
}

void UpdateDialog::onDownloadFinished()
{
    m_progressBar->setVisible(false);
    
    if (m_currentReply->error() != QNetworkReply::NoError) {
        m_statusLabel->setText("Download failed. Please try again later.");
        m_statusLabel->setStyleSheet("font-weight: bold; color: #dc3545;");
        m_downloadButton->setEnabled(true);
        return;
    }
    
    // Save downloaded file
    QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString fileName = QString("LibraryManagementSystem-%1").arg(m_latestVersion);
    
    if (m_downloadUrl.contains("AppImage")) {
        fileName += ".AppImage";
    } else if (m_downloadUrl.contains("exe")) {
        fileName += ".exe";
    } else if (m_downloadUrl.contains("dmg")) {
        fileName += ".dmg";
    }
    
    m_downloadedFilePath = QDir(downloadsPath).filePath(fileName);
    
    QFile file(m_downloadedFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(m_currentReply->readAll());
        file.close();
        
        m_statusLabel->setText("Download completed! Ready to install.");
        m_statusLabel->setStyleSheet("font-weight: bold; color: #28a745;");
        m_installButton->setVisible(true);
        m_downloadButton->setVisible(false);
    } else {
        m_statusLabel->setText("Failed to save downloaded file.");
        m_statusLabel->setStyleSheet("font-weight: bold; color: #dc3545;");
        m_downloadButton->setEnabled(true);
    }
    
    m_currentReply->deleteLater();
    m_currentReply = nullptr;
}

void UpdateDialog::onDownloadError(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)
    m_progressBar->setVisible(false);
    m_statusLabel->setText("Network error occurred. Please check your internet connection.");
    m_statusLabel->setStyleSheet("font-weight: bold; color: #dc3545;");
    m_downloadButton->setEnabled(true);
    
    if (m_currentReply) {
        m_currentReply->deleteLater();
        m_currentReply = nullptr;
    }
}

void UpdateDialog::installUpdate()
{
    if (m_downloadedFilePath.isEmpty() || !QFile::exists(m_downloadedFilePath)) {
        QMessageBox::warning(this, "Installation Error", "Downloaded file not found.");
        return;
    }
    
    int ret = QMessageBox::question(this, "Install Update",
                                   "The application will close to install the update. Continue?",
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        // Make file executable (for AppImage)
        QFile::setPermissions(m_downloadedFilePath, QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
        
        // Launch installer
        QProcess::startDetached(m_downloadedFilePath);
        
        // Close application
        QApplication::quit();
    }
}

void UpdateDialog::skipUpdate()
{
    // In a real application, you might save this version to skip
    // For now, just close the dialog
    accept();
}
