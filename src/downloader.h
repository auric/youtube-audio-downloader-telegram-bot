#pragma once

#include <QObject>

#include <future>

#include "item.h"

class Downloader : public QObject {
    Q_OBJECT
public:
    explicit Downloader(int64_t chatId,
        const Item &item,
        QObject *parent = nullptr);
    ~Downloader();

signals:
    void ready(qint64 chatId, const QString &path);

private:
    std::future<void> m_future;
    const int64_t m_chatId;
    const Item m_item;
};
