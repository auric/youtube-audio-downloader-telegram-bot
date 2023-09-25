#pragma once

#include <QNetworkAccessManager>
#include <QVector>

#include "item.h"

class YouTubeFinder : public QObject
{
    Q_OBJECT
public:
    YouTubeFinder(QNetworkAccessManager &m_manager, int64_t chatId, QString requestedQuery);

    QVector<Item> items() const;

signals:
    void ready(int64_t chatId, const QVector<Item> &items);

private slots:
    void onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void onFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager &m_manager;
    const int64_t m_chatId;
    const int m_maxRes = 5;
    QVector<Item> m_items;
};
