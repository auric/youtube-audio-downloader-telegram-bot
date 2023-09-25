#pragma once

#include <memory>

namespace telegram::types {

struct message;
struct callback_query;

} // namespace telegram::types

struct Handler
{
    virtual ~Handler() = default;

    virtual void onMessage(const telegram::types::message &msg) = 0;
    virtual void onQuery(const telegram::types::callback_query &q) = 0;
};

using HandlerPtr = std::unique_ptr<Handler>;
