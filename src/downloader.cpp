#include "downloader.h"

#include <QStandardPaths>

Downloader::Downloader(int64_t chatId, const Item &item, QObject *parent)
    : m_chatId(chatId)
    , m_item(item)
{
    m_future = std::async(std::launch::async, [this] {
        const auto paths = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
        const auto tempDir = paths.first();
        QString youtubeUrl = QStringLiteral("https://www.youtube.com/watch?v=") + m_item.m_id;
        const auto cmd = QStringLiteral("yt-dlp -x -f bestaudio[ext=m4a] --add-metadata \"%1/%2\" %3")
                             .arg(tempDir, m_item.m_name, youtubeUrl);
        std::system(cmd.toStdString().c_str());

        const QString path = QStringLiteral("%1/%2.m4a").arg(tempDir, m_item.m_name);

        emit ready(m_chatId, path);
    });
}

Downloader::~Downloader()
{
    m_future.get();
}
