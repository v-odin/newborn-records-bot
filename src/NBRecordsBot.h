#ifndef NBRECORDSBOT_H
#define NBRECORDSBOT_H

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <string>
#include <unordered_map>

#include <sqlite_modern_cpp.h>
#include <tgbot/Bot.h>

#include "RecordsPanel.h"
#include "Stats.h"
#include "Types.h"

namespace nbrecords {

class NBRecordsBot : public TgBot::Bot, public Handler<NBRecordsBot> {
public:
    NBRecordsBot(std::string, std::string);

    void run();
    sqlite::database& db();
    void addReplyHandler(const std::string&, MessageListener);
    void addCallbackHandler(const std::string&, CallbackQueryListener);

    // Handlers
    void onAnyMessage(Message);
    void onCallbackQuery(CallbackQuery);
    void onRecords(Message);
    void onStart(Message);
    void onDayReport(Message);
    void onPrevReport(CallbackQuery);
    void onNextReport(CallbackQuery);

private:
    sqlite::database d_db;
    std::unordered_map<std::string, MessageListener> d_replyHandlers;
    std::unordered_map<std::string, CallbackQueryListener> d_callbackHandlers;
    RecordsPanel d_recordsPanel;
    Stats d_stats;

    InlineKeyboard d_reportFullKeyboard;
    InlineKeyboard d_reportPrevKeyboard;

    void initDB();
    std::string extractReportDate(const std::string&);
    void updateReport(Message, std::int32_t);
};

} // namespace nbrecords
#endif // NBRECORDSBOT
