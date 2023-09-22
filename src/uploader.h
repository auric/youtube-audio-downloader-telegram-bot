#pragma once

#include <QObject>

#include <memory>

class QNetworkAccessManager;
class QFile;
class QHttpPart;
class QHttpMultiPart;

class Uploader : public QObject
{
    Q_OBJECT
public:
    explicit Uploader(QNetworkAccessManager &manager,
                      QString botToken,
                      int64_t chatId,
                      QString path,
                      QObject *parent = nullptr);
    ~Uploader();

private:
    std::unique_ptr<QFile> m_audioFile;
    std::unique_ptr<QHttpPart> m_soundPart;
    std::unique_ptr<QHttpMultiPart> m_multiPart;
};
