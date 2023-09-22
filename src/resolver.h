#pragma once

#include <QObject>

#include <mutex>
#include <thread>

#include "downloader.h"
#include "item.h"
#include "uploader.h"
#include "youtubefinder.h"

namespace telegram {

class sender;

namespace listener {
class poll;
}

} // namespace telegram

class Resolver : public QObject
{
    Q_OBJECT
public:
    explicit Resolver(QObject *parent = nullptr);
    ~Resolver();

private slots:
    void onReady(int64_t chatId, const QVector<Item> &items);
    void onDownloaded(int64_t chatId, const QString &path);

private:
    void enqeue(int64_t chatId, QString request);
    void enqeueDownload(int64_t chatId, QString videoId);

private:
    std::mutex m_pollMutex;
    std::thread m_thread;
    std::unique_ptr<telegram::sender> m_sender;
    std::unique_ptr<telegram::listener::poll> m_listener;
    std::unique_ptr<QNetworkAccessManager> m_manager;
    std::unordered_map<int64_t, std::unique_ptr<YouTubeFinder>> m_finders;
    std::unordered_map<int64_t, std::unique_ptr<Downloader>> m_downloaders;
    std::unordered_map<int64_t, std::unique_ptr<Uploader>> m_uploaders;
};
