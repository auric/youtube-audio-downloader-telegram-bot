#pragma once

#include "handler.h"

#include <QNetworkAccessManager>
#include <QObject>

#include <unordered_map>

#include "database.h"
#include "youtubefinder.h"

class Bot;
class Database;
class QNetworkAccessManager;

class SearchHandler : public QObject, public Handler
{
    Q_OBJECT
public:
    SearchHandler(Database &db, QNetworkAccessManager &manager, Bot &bot);

    void onMessage(const telegram::types::message &m) override;
    void onQuery(const telegram::types::callback_query &q) override;

private slots:
    void onReady(int64_t chatId, const QVector<Item> &items);

private:
    void enqeue(int64_t chatId, QString request);

private:
    Database &m_db;
    QNetworkAccessManager &m_manager;
    Bot &m_bot;
    std::unordered_map<int64_t, std::unique_ptr<YouTubeFinder>> m_finders;
};
