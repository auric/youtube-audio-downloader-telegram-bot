#include "youtubefinder.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSslError>
#include <QtDebug>

#include "item.h"

YouTubeFinder::YouTubeFinder(QNetworkAccessManager &manager, int64_t chatId, QString query)
    : m_manager(manager)
    , m_chatId(chatId)
{
    QString host = "https://youtube.googleapis.com";
    //QString host = "https://web.telegram.org";
    //QString port = "443";
    auto path
        = QStringLiteral(
              "/youtube/v3/"
              "search?part=snippet&key=AIzaSyBK50cAp_Q8YkUptzqRTVpZRklWP108Kzc&maxResults=%1&q=%2")
              .arg(m_maxRes)
              .arg(query);
    QNetworkRequest req(QUrl(host + path));
    connect(&m_manager, &QNetworkAccessManager::finished, this, &YouTubeFinder::onFinished);
    connect(&m_manager, &QNetworkAccessManager::sslErrors, this, &YouTubeFinder::onSslErrors);
    auto reply = m_manager.get(req);
    connect(reply, &QNetworkReply::errorOccurred, this, [](auto err) {
        qDebug() << "Error:" << err;
    });
    //connect(reply, &QNetworkReply::finished, this, [this, reply] { onFinished(reply); });
}

QVector<Item> YouTubeFinder::items() const
{
    return m_items;
}

void YouTubeFinder::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    for (const auto &e : errors) {
        qDebug() << e.errorString();
    }
}

void YouTubeFinder::onFinished(QNetworkReply *reply)
{
    auto doc = QJsonDocument::fromJson(reply->readAll());
    auto obj = doc.object();

    auto items = obj["items"].toArray();
    for (const auto &item : items) {
        auto itemObj = item.toObject();
        auto id = itemObj["id"].toObject()["videoId"].toString();
        if (id.isEmpty()) {
            continue;
        }
        auto title = itemObj["snippet"].toObject()["title"].toString();
        m_items.push_back({std::move(title), std::move(id)});
    }
    reply->deleteLater();
    emit ready(m_chatId, m_items);
}