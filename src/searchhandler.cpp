#include "searchhandler.h"

#include <libtelegram/sender.h>

#include "bot.h"

SearchHandler::SearchHandler(Database &db, QNetworkAccessManager &manager, Bot &bot)
    : m_db(db)
    , m_manager(manager)
    , m_bot(bot)
{
}

void SearchHandler::onMessage(const telegram::types::message &m)
{
    // TODO: verify is command?

    // use Qt::QueuedConnection since bot runs in separate thread
    QMetaObject::invokeMethod(
        this,
        [this, m] { enqeue(m.chat.id, QString::fromUtf8(m.text->c_str(), m.text->size())); },
        Qt::QueuedConnection);

    std::string reply("searching for: " + *m.text + "...");
    m_bot.getSender()->send_message(m.chat.id, reply);
}

void SearchHandler::onQuery(const telegram::types::callback_query &q) {}

void SearchHandler::onReady(int64_t chatId, const QVector<Item> &items)
{
    for (const auto &i : items) {
        m_db.insert(chatId, i);
    }

    std::string res;
    // telegram::types::reply_markup::reply_keyboard_markup reply;
    telegram::types::reply_markup::inline_keyboard_markup reply;
    std::string resStr;
    using button = telegram::types::reply_markup::inline_keyboard_button;
    for (const auto &item : items) {
        auto str = item.m_name.toStdString();
        button replyBtn(str, button::type::BUTTON_CALLBACK);
        replyBtn.callback_data = item.m_id.toStdString();

        reply.keyboard_buttons.emplace_back();
        reply.keyboard_buttons.back().emplace_back(replyBtn);
        resStr += "\n" + str;
    }

    m_finders.erase(chatId);
    m_bot.getSender()->send_message(chatId, resStr, reply);
}

void SearchHandler::enqeue(int64_t chatId, QString request)
{
    auto f = std::make_unique<YouTubeFinder>(m_manager, chatId, std::move(request));
    connect(f.get(), &YouTubeFinder::ready, this, &SearchHandler::onReady);
    m_finders.emplace(chatId, std::move(f));
}
