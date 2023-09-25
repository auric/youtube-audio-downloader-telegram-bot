#include "database.h"

Database::Database(QObject *parent)
    : QObject{parent}
{
}

void Database::insert(int64_t chatId, const Item &item)
{
    m_items[chatId].insert(item.m_id, item);
}

void Database::remove(int64_t chatId)
{
    m_items.erase(chatId);
}

Item Database::get(int64_t chatId, const QString &videoId) const
{
    auto it = m_items.find(chatId);
    if (it == m_items.end()) {
        return {};
    }
    auto video = it->second.find(videoId);
    if (video == it->second.end()) {
        return {};
    }
    return *video;
}
