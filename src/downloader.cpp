#include "downloader.h"

#include <QStandardPaths>

Downloader::Downloader(int64_t chatId, const QString &name, const QString &videoId, QObject *parent)
    : m_chatId(chatId)
    , m_videoId(videoId)
{
    m_future = std::async(std::launch::async, [this, name] {
        const auto paths = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
        const auto tempDir = paths.first();
        QString youtubeUrl = QStringLiteral("https://www.youtube.com/watch?v=") + m_videoId;
        const auto cmd = QStringLiteral("yt-dlp -x --audio-format aac -o \"%1/%2\" %3")
                             .arg(tempDir, name, youtubeUrl);
        std::system(cmd.toStdString().c_str());

        const QString path = QStringLiteral("%1/%2.m4a").arg(tempDir, name);

        emit ready(m_chatId, path);
    });
}

Downloader::~Downloader()
{
    m_future.get();
}
