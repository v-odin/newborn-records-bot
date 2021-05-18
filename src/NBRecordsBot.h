#ifndef NBRECORDSBOT_H
#define NBRECORDSBOT_H

#include <string>
#include <unordered_map>

#include <sqlite_modern_cpp.h>
#include <tgbot/Bot.h>

#include "RecordsPanel.h"
#include "Types.h"

namespace nbrecords {

class NBRecordsBot : public TgBot::Bot, public Handler<NBRecordsBot> {
public:
    NBRecordsBot(std::string, std::string);

    void start();
    sqlite::database& db();
    void addReplyHandler(const std::string&, MessageListener);

    // Handlers
    void onAnyMessage(Message);
    void onRecords(Message);
    void onStart(Message);
    void onStats(Message);

private:
    sqlite::database d_db;
    std::unordered_map<std::string, MessageListener> d_handlers;
    RecordsPanel d_recordsPanel;

    void initDB();
};

} // namespace nbrecords
#endif // NBRECORDSBOT