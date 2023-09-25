#pragma once

#include <functional>
#include <memory>
#include <mutex>

#include <libtelegram/types/types.h>

#include "lockedptr.h"

namespace telegram {

class sender;

namespace listener {

class poll;

}

} // namespace telegram

class Bot
{
public:
    Bot();
    ~Bot();

    void run();

    LockedPtr<std::mutex, telegram::sender *> getSender();

    // due to bad designed telegram::listener::base<T> we unable to include
    // libtelegram/listener.h more then once in the project
    void set_callback_message(std::function<void(const telegram::types::message &)> f);
    void set_callback_query_callback(std::function<void(const telegram::types::callback_query &)> f);

private:
    LockedPtr<std::mutex, telegram::listener::poll *> getListener();

private:
    std::mutex m_mutex;
    std::thread m_thread;
    std::unique_ptr<telegram::sender> m_sender;
    std::unique_ptr<telegram::listener::poll> m_listener;
};
