#pragma once

#include <QNetworkAccessManager>

#include "downloader.h"
#include "handler.h"
#include "uploader.h"

class Bot;
class Database;
class QNetworkAccessManager;

class DownloadHandler : public QObject, public Handler
{
    Q_OBJECT
public:
    DownloadHandler(Database &db, QNetworkAccessManager &manager, Bot &bot);

    void onMessage(const telegram::types::message &m) override;
    void onQuery(const telegram::types::callback_query &q) override;

private slots:
    void onDownloaded(int64_t chatId, const QString &path);

private:
    void enqeueDownload(int64_t chatId, QString videoId);

private:
    Database &m_db;
    QNetworkAccessManager &m_manager;
    Bot &m_bot;
    std::unordered_map<int64_t, std::unique_ptr<Downloader>> m_downloaders;
    std::unordered_map<int64_t, std::unique_ptr<Uploader>> m_uploaders;
};
