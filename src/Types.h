#ifndef TYPES_H
#define TYPES_H

#include "tgbot/EventBroadcaster.h"
#include "tgbot/types/InlineKeyboardMarkup.h"
#include "tgbot/types/Message.h"
#include "tgbot/types/ReplyKeyboardMarkup.h"

namespace nbrecords {
    using Message = TgBot::Message::Ptr;
    using MessageListener = TgBot::EventBroadcaster::MessageListener;
    using CallbackQueryListener = TgBot::EventBroadcaster::CallbackQueryListener;
    using CallbackQuery = TgBot::CallbackQuery::Ptr;
    using ReplyKeyboard = TgBot::ReplyKeyboardMarkup::Ptr;
    using InlineKeyboard = TgBot::InlineKeyboardMarkup::Ptr;
} // namespace nbrecords

#endif // TYPES_H
