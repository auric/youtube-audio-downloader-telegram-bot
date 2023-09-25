#pragma once

#include <QObject>

#include "item.h"

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);

    void insert(int64_t chatId, const Item &item);
    void remove(int64_t chatId);

    Item get(int64_t chatId, const QString &videoId) const;

private:
    std::unordered_map<int64_t, Items> m_items;
};
