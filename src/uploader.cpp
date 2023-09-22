#include "uploader.h"

#include <QFile>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

Uploader::Uploader(
    QNetworkAccessManager &manager, QString botToken, int64_t chatId, QString path, QObject *parent)
    : QObject{parent}
{
    // Replace with the path to your sound file
    QString soundFilePath = path;

    // Read the sound file
    m_audioFile = std::make_unique<QFile>(soundFilePath);
    if (!m_audioFile->open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open sound file";
    }

    // Construct the API URL
    QUrl apiUrl("https://api.telegram.org/bot" + botToken + "/sendAudio");

    // Create a multi-part request
    m_multiPart = std::make_unique<QHttpMultiPart>(QHttpMultiPart::FormDataType);

    // Add the chat ID to the request parameters
    QUrlQuery query;
    query.addQueryItem("chat_id", QString::number(chatId));
    apiUrl.setQuery(query);

    // Add the sound file to the request
    m_soundPart = std::make_unique<QHttpPart>();
    m_soundPart->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("audio/mpeg"));
    m_soundPart->setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant("form-data; name=\"audio\"; filename=\""
                                    + m_audioFile->fileName() + "\""));
    m_soundPart->setBodyDevice(m_audioFile.get());
    m_multiPart->append(*m_soundPart);

    // Send the request
    QNetworkReply *reply = manager.post(QNetworkRequest(apiUrl), m_multiPart.get());

    // Handle the response
    QObject::connect(reply, &QNetworkReply::finished, [reply, path]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            // Handle the response data here
            qDebug() << "Response:" << responseData;
        } else {
            qDebug() << "Request failed:" << reply->errorString();
        }

        // Clean up
        reply->deleteLater();
        QFile::remove(path);
    });
}

Uploader::~Uploader()
{
    qDebug() << "Uploader destruction";
}
