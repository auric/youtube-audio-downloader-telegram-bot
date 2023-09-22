#include "resolver.h"

#include <QNetworkAccessManager>

#include <SDKDDKVer.h>
#include <WinSDKVer.h>

#include <libtelegram/libtelegram.h>

#include "uploader.h"

static const std::string token; // use your telegram token

Resolver::Resolver(QObject *parent)
{
    m_sender = std::make_unique<telegram::sender>(token);
    m_listener = std::make_unique<telegram::listener::poll>(*m_sender);
    m_listener->set_num_threads(1);

    m_listener->set_callback_message([&](const telegram::types::message &message) {
        // TODO: verify is command?
        QMetaObject::invokeMethod(
            this,
            [this, message] {
                enqeue(message.chat.id,
                       QString::fromUtf8(message.text->c_str(), message.text->size()));
            },
            Qt::QueuedConnection);

        const std::string reply("searching for: " + *message.text + "...");
        m_sender->send_message(message.chat.id, reply);
    });

    m_listener->set_callback_query_callback([this](const telegram::types::callback_query &q) {
        auto chatId = q.from.id;
        auto data = q.data;
        QMetaObject::invokeMethod(
            this,
            [this, chatId, data] {
                enqeueDownload(chatId, QString::fromUtf8(data->c_str(), data->size()));
            },
            Qt::QueuedConnection);
    });

    m_thread = std::thread([&] { m_listener->run(); });
    m_manager = std::make_unique<QNetworkAccessManager>();
}

Resolver::~Resolver() {}

void Resolver::onReady(int64_t chatId, const QVector<Item> &items)
{
    std::unique_lock lock(m_pollMutex);

    std::string res;
    // telegram::types::reply_markup::reply_keyboard_markup reply;
    telegram::types::reply_markup::inline_keyboard_markup reply;
    std::string resStr;
    using button = telegram::types::reply_markup::inline_keyboard_button;
    reply.keyboard_buttons.emplace_back();
    for (const auto &item : items) {
        auto str = item.m_name.toStdString();
        button replyBtn(str, button::type::BUTTON_CALLBACK);
        replyBtn.callback_data = item.m_id.toStdString();

        reply.keyboard_buttons.back().emplace_back(replyBtn);
        resStr += "\n" + str;
    }

    m_sender->send_message(chatId, resStr, reply);
}

void Resolver::onDownloaded(int64_t chatId, const QString &path)
{
    auto it = m_downloaders.find(chatId);
    m_downloaders.erase(it);
    auto u = std::make_unique<Uploader>(*m_manager,
                                        QString::fromUtf8(token.c_str(), token.size()),
                                        chatId,
                                        path);

    m_uploaders.emplace(chatId, std::move(u));
}

void Resolver::enqeue(int64_t chatId, QString request)
{
    auto f = std::make_unique<YouTubeFinder>(*m_manager, chatId, std::move(request));
    connect(f.get(), &YouTubeFinder::ready, this, &Resolver::onReady);
    std::unique_lock lock(m_pollMutex);
    m_finders.emplace(chatId, std::move(f));
}

void Resolver::enqeueDownload(int64_t chatId, QString videoId)
{
    auto d = std::make_unique<Downloader>(chatId, videoId);
    connect(d.get(), &Downloader::ready, this, &Resolver::onDownloaded);

    m_downloaders.emplace(chatId, std::move(d));
}
