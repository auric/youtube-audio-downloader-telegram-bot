#include "resolver.h"

#include <libtelegram/types/types.h>

#include "bot.h"

Resolver::Resolver(Bot &bot, QObject *parent)
    : m_bot(bot)
{
    m_bot.set_callback_message([&](const telegram::types::message &message) {
        for (const auto &h : m_handlers) {
            h->onMessage(message);
        }
    });

    m_bot.set_callback_query_callback([this](const telegram::types::callback_query &q) {
        for (const auto &h : m_handlers) {
            h->onQuery(q);
        }
    });
    m_bot.run();
}

Resolver::~Resolver() {}

void Resolver::appendHandler(HandlerPtr h)
{
    m_handlers.push_back(std::move(h));
}
