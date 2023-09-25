#pragma once

#include <QObject>

#include <future>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(int64_t chatId,
                        const QString &name,
                        const QString &videoId,
                        QObject *parent = nullptr);
    ~Downloader();

signals:
    void ready(qint64 chatId, const QString &path);

private:
    std::future<void> m_future;
    const int64_t m_chatId;
    const QString m_videoId;
};
