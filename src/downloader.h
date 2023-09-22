#pragma once

#include <QObject>

#include <thread>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(int64_t chatId, QString videoId, QObject *parent = nullptr);
    ~Downloader();

signals:
    void ready(qint64 chatId, const QString &path);

private:
    std::thread m_thread;
    const int64_t m_chatId;
    const QString m_videoId;
};
