#include "bot.h"

#include "config.h"

#include <libtelegram/libtelegram.h>

Bot::Bot()
{
    m_sender = std::make_unique<telegram::sender>(cfg::botToken);
    m_listener = std::make_unique<telegram::listener::poll>(*m_sender);
    m_listener->set_num_threads(1);
}

Bot::~Bot()
{
    if (m_thread.joinable()) {
        m_listener->stop();
    }
    m_thread.join();
}

void Bot::run()
{
    if (m_thread.joinable()) {
        return;
    }

    m_thread = std::thread([this] { m_listener->run(); });
}

LockedPtr<std::mutex, telegram::sender *> Bot::getSender()
{
    return LockedPtr<std::mutex, telegram::sender *>(std::unique_lock(m_mutex), m_sender.get());
}

void Bot::set_callback_message(std::function<void(const telegram::types::message &)> f)
{
    getListener()->set_callback_message(std::move(f));
}

void Bot::set_callback_query_callback(std::function<void(const telegram::types::callback_query &)> f)
{
    getListener()->set_callback_query_callback(std::move(f));
}

LockedPtr<std::mutex, telegram::listener::poll *> Bot::getListener()
{
    return LockedPtr<std::mutex, telegram::listener::poll *>(std::unique_lock(m_mutex),
                                                             m_listener.get());
}
