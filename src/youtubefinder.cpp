#include "youtubefinder.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSslError>
#include <QtDebug>

#include <regex>

#include "config.h"
#include "item.h"
#include "scopedresource.h"

namespace {

std::string trim(std::string s)
{
    static std::string whitespaces = " \t\n\r\f\v";
    size_t left = s.find_first_not_of(whitespaces);
    if (left == std::string::npos) {
        return "";
    }
    size_t right = s.find_last_not_of(whitespaces);
    return s.substr(left, right - left + 1);
}

std::string fixFilename(std::string filename)
{
    static std::string validChars = R"raw(a-zA-Z0-9._ ()\-)raw";
    // Create a regex that matches any character that is not valid
    static std::regex re("[^" + validChars + "]");
    std::string fixedFilename = std::regex_replace(filename, re, " ");
    return fixedFilename;
}

} // namespace

YouTubeFinder::YouTubeFinder(QNetworkAccessManager &manager, int64_t chatId, QString query)
    : m_manager(manager)
    , m_chatId(chatId)
{
    QString host = "https://youtube.googleapis.com";
    auto path = QStringLiteral("/youtube/v3/"
                               "search?part=snippet&key=%1&maxResults=%2&q=%3")
                    .arg(QString::fromStdString(cfg::youtubeToken))
                    .arg(m_maxRes)
                    .arg(query);
    QNetworkRequest req(QUrl(host + path));
    connect(&m_manager, &QNetworkAccessManager::finished, this, &YouTubeFinder::onFinished);
    connect(&m_manager, &QNetworkAccessManager::sslErrors, this, &YouTubeFinder::onSslErrors);
    m_manager.get(req);
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
        title = QString::fromStdString(trim(fixFilename(title.toStdString())));
        m_items.push_back({ std::move(title), std::move(id) });
    }
    reply->deleteLater();
    emit ready(m_chatId, m_items);
}
