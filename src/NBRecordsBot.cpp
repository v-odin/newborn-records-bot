#include "NBRecordsBot.h"

#include "tgbot/types/InlineKeyboardMarkup.h"
#include "tgbot/net/TgLongPoll.h"

#include "Utils.h"

namespace nbrecords {

NBRecordsBot::NBRecordsBot(std::string token, std::string dbfile) :
    Bot(token),
    d_db(dbfile),
    d_recordsPanel(this),
    d_stats(d_db)
{
    initDB();
    getEvents().onCommand("start", asHandler(&NBRecordsBot::onStart));
    getEvents().onCommand("records", asHandler(&NBRecordsBot::onRecords));
    getEvents().onAnyMessage(asHandler(&NBRecordsBot::onAnyMessage));
    getEvents().onCallbackQuery(asHandler(&NBRecordsBot::onCallbackQuery));

    const std::string prev = "◀️", next = "▶️";
    d_reportFullKeyboard = utils::createInlineKeyboard({{prev, next}});
    d_reportPrevKeyboard = utils::createInlineKeyboard({{prev}});
    addCallbackHandler(prev, asHandler(&NBRecordsBot::onPrevReport));
    addCallbackHandler(next, asHandler(&NBRecordsBot::onNextReport));
    getEvents().onCommand("report", asHandler(&NBRecordsBot::onDayReport));
}

void NBRecordsBot::run() {
    try {
        getApi().deleteWebhook();

        TgBot::TgLongPoll longPoll(*this);
        while (true) {
            longPoll.start();
        }
    } catch (std::exception& e) {
        std::cerr << e.what();
    }
}

void NBRecordsBot::initDB() {
    d_db << "create table if not exists records ("
            "   chat int,"
            "   name text,"
            "   arg text,"
            "   time int"
            ");";
}

sqlite::database& NBRecordsBot::db() {
    return d_db;
}

void NBRecordsBot::addReplyHandler(const std::string& text, MessageListener handler) {
    d_replyHandlers[text] = handler;
}

void NBRecordsBot::addCallbackHandler(const std::string& query, CallbackQueryListener handler) {
    d_callbackHandlers[query] = handler;
}

// Handlers

void NBRecordsBot::onAnyMessage(Message message) {
    if (d_replyHandlers.count(message->text)) {
        try {
            d_replyHandlers[message->text](message);
        } catch (std::exception &e) {
            getApi().sendMessage(message->chat->id, std::string(e.what()));
        }
    }
}

void NBRecordsBot::onCallbackQuery(CallbackQuery query) {
    if (d_callbackHandlers.count(query->data)) {
        try {
            d_callbackHandlers[query->data](query);
        } catch (std::exception &e) {
            getApi().answerCallbackQuery(query->id, std::string(e.what()));
        }
    }
}

void NBRecordsBot::onRecords(Message message) {
    d_recordsPanel.show(message->chat->id);
}

void NBRecordsBot::onStart(Message message) {
    getApi().sendMessage(message->chat->id,
    "Hi! Congratulations on your newborn. I will help you to track essentials.\n"
    "/records promtps records panel \n"
    "/stats prompts stats panel");
}

void NBRecordsBot::onDayReport(Message message) {
    const std::string reply = d_stats.dayReport(message->chat->id, utils::now());
    getApi().sendMessage(message->chat->id, reply, false, 0, d_reportPrevKeyboard, "Markdown");
}

void NBRecordsBot::onPrevReport(CallbackQuery query) {
    const std::int32_t unixTime = utils::parseDate(extractReportDate(query->message->text));
    const std::int32_t prevDay = utils::prevDay(unixTime);
    updateReport(query->message, prevDay);
}

void NBRecordsBot::onNextReport(CallbackQuery query) {
    const std::int32_t unixTime = utils::parseDate(extractReportDate(query->message->text));
    const std::int32_t nextDay = utils::nextDay(unixTime);
    updateReport(query->message, nextDay);
}

std::string NBRecordsBot::extractReportDate(const std::string& message) {
    std::stringstream ss(message);
    std::string date;
    ss >> date >> date;
    return date;
}

void NBRecordsBot::updateReport(Message message, std::int32_t unixTime) {
    const std::string reply = d_stats.dayReport(message->chat->id, unixTime);
    const auto keyboard = utils::asDate(unixTime) == utils::asDate(utils::now()) ? d_reportPrevKeyboard : d_reportFullKeyboard;
    getApi().editMessageText(reply, message->chat->id, message->messageId, "", "Markdown", false, keyboard);
}

} //namespace nbrecords
