#pragma once

#include <QHash>
#include <QString>

#include <unordered_map>

struct Item {
    QString m_name;
    QString m_id;
};

using Items = QHash<QString, Item>;
