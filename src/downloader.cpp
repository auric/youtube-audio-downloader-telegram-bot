#include "downloader.h"

#include <QStandardPaths>

Downloader::Downloader(int64_t chatId, QString videoId, QObject *parent)
    : m_chatId(chatId)
    , m_videoId(videoId)
{
    m_thread = std::thread([this] {
        const auto tempDir = QStandardPaths::standardLocations(QStandardPaths::TempLocation).first();
        QString youtubeUrl = QStringLiteral("https://www.youtube.com/watch?v=") + m_videoId;
        const auto cmd = QStringLiteral(
                             "yt-dlp -x --audio-format aac --audio-quality 0 -o %1/%2 %3")
                             .arg(tempDir, m_videoId, youtubeUrl);
        std::system(cmd.toStdString().c_str());

        const QString path = QStringLiteral("%1/%2.m4a").arg(tempDir, m_videoId);

        emit ready(m_chatId, path);
    });
}

Downloader::~Downloader()
{
    m_thread.join();
}
