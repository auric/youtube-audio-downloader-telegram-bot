#pragma once

#include <QObject>

#include "handler.h"

class Bot;

class Resolver : public QObject
{
    Q_OBJECT
public:
    explicit Resolver(Bot &bot, QObject *parent = nullptr);
    ~Resolver();

    void appendHandler(HandlerPtr h);

private:
    Bot &m_bot;
    std::vector<HandlerPtr> m_handlers;
};
