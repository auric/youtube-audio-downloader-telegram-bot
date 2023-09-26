#include "downloadhandler.h"

#include <libtelegram/sender.h>
#include <libtelegram/types/types.h>

#include "bot.h"
#include "database.h"

DownloadHandler::DownloadHandler(Database &db, QNetworkAccessManager &manager, Bot &bot)
    : m_db(db)
    , m_manager(manager)
    , m_bot(bot)
{
}

void DownloadHandler::onMessage(const telegram::types::message &m)
{
}

void DownloadHandler::onQuery(const telegram::types::callback_query &q)
{
    int64_t chatId = q.from.id;
    auto data = q.data;
    QMetaObject::invokeMethod(
        this,
        [this, chatId, data] {
            enqeueDownload(chatId, QString::fromStdString(*data));
        },
        Qt::QueuedConnection);

    m_bot.getSender()->send_message(chatId, "Preparing audio");
}

void DownloadHandler::onDownloaded(int64_t chatId, const QString &path)
{
    auto it = m_downloaders.find(chatId);
    m_downloaders.erase(it);
    m_uploaders.erase(chatId);
    auto u = std::make_unique<Uploader>(m_manager, chatId, path);

    m_uploaders.emplace(chatId, std::move(u));
}

void DownloadHandler::enqeueDownload(int64_t chatId, QString videoId)
{
    auto item = m_db.get(chatId, videoId);
    auto d = std::make_unique<Downloader>(chatId, item);
    connect(d.get(), &Downloader::ready, this, &DownloadHandler::onDownloaded);

    m_downloaders.emplace(chatId, std::move(d));
}
