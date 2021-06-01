#include "NBRecordsBot.h"

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
    getEvents().onCommand("report", asHandler(&NBRecordsBot::onDayReport));
    getEvents().onAnyMessage(asHandler(&NBRecordsBot::onAnyMessage));
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
    d_handlers[text] = handler;
}

// Handlers

void NBRecordsBot::onAnyMessage(Message message) {
    if (d_handlers.count(message->text)) {
        try {
            d_handlers[message->text](message);
        } catch (std::exception &e) {
            getApi().sendMessage(message->chat->id, "Fail: " + std::string(e.what()));
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
    std::stringstream reply;
    reply << "📊 " << utils::asDate(utils::now()) << std::endl << std::endl;
    reply << d_stats.dayReport(message->chat->id, utils::now());
    getApi().sendMessage(message->chat->id, reply.str(), false, 0, nullptr, "Markdown");
}

} //namespace nbrecords
