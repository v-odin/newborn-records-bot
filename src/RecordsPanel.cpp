#include "RecordsPanel.h"

#include "NBRecordsBot.h"
#include "Utils.h"

namespace nbrecords
{

    RecordsPanel::RecordsPanel(NBRecordsBot *bot) : d_bot(bot),
                                                    d_stats(bot->db())
    {
        //TODO: customizable panel
        // const std::string feedLeft = "🤱 left", feedSpoon = "🥄 spoon", feedRight = "🤱 right", lastFeed = "📋 feed";
        const std::string feedSpoon = "🥄 spoon", feedBottle = "🍼 bottle", lastFeed = "📋 feed";
        const std::string sleepStart = "😴 sleep", sleepStop = "☀️ wake up", lastSleep = "📋 sleep";
        const std::string diaper = "🚼 change diaper", lastDiaper = "📋 diaper";

        d_keyboard = utils::createReplyKeyboard({{feedSpoon, feedBottle},
                                                 {sleepStart, sleepStop},
                                                 {diaper},
                                                 {lastFeed, lastSleep, lastDiaper}});
        // TODO: to make selecetive – so far does not work as expected
        // keyboard->selective = true;

        const std::string del = "❌";
        d_panelResponseKeyboard = utils::createInlineKeyboard({{del}});

        d_bot->addReplyHandler(feedSpoon, asHandler(&RecordsPanel::onFeedSpoon));
        d_bot->addReplyHandler(feedBottle, asHandler(&RecordsPanel::onFeedRight));
        d_bot->addReplyHandler(lastFeed, asHandler(&RecordsPanel::onLastFeed));
        d_bot->addReplyHandler(sleepStart, asHandler(&RecordsPanel::onSleepStart));
        d_bot->addReplyHandler(sleepStop, asHandler(&RecordsPanel::onSleepStop));
        d_bot->addReplyHandler(lastSleep, asHandler(&RecordsPanel::onLastSleep));
        d_bot->addReplyHandler(diaper, asHandler(&RecordsPanel::onDiaper));
        d_bot->addReplyHandler(lastDiaper, asHandler(&RecordsPanel::onLastDiaper));
        d_bot->addCallbackHandler(del, asHandler(&RecordsPanel::onLastDeleteRecord));
    }

    void RecordsPanel::show(std::int64_t chatId)
    {
        d_bot->getApi().sendMessage(chatId, "Choose an action", false, 0, d_keyboard);
    }

    // Handlers

    void RecordsPanel::onFeedLeft(Message message)
    {
        feed(message, "left");
    }

    void RecordsPanel::onFeedSpoon(Message message)
    {
        feed(message, "spoon");
    }

    void RecordsPanel::onFeedBottle(Message message)
    {
        feed(message, "bottle");
    }

    void RecordsPanel::onFeedRight(Message message)
    {
        feed(message, "right");
    }

    void RecordsPanel::feed(Message message, std::string side)
    {
        std::stringstream reply;
        reply << "Feeding " << side << " at " << utils::asClock(message->date);
        const auto replyMsg = d_bot->getApi().sendMessage(message->chat->id, reply.str(), false, 0, d_panelResponseKeyboard, "Markdown");

        d_bot->db() << "insert into records (chat, name, arg, time, msgId) values(?, ?, ?, ?, ?);"
                    << message->chat->id << "feed" << side << message->date << replyMsg->messageId;
    }

    void RecordsPanel::onLastFeed(Message message)
    {
        d_bot->getApi().sendMessage(message->chat->id, d_stats.lastFeed(message->chat->id));
    }

    void RecordsPanel::onSleepStart(Message message)
    {
        sleep(message, "start");
    }

    void RecordsPanel::onSleepStop(Message message)
    {
        sleep(message, "stop");
    }

    void RecordsPanel::sleep(Message message, std::string action)
    {

        std::stringstream reply;
        action[0] = toupper(action[0]);
        reply << action << " sleeping"
              << " at " << utils::asClock(message->date);
        const auto replyMsg = d_bot->getApi().sendMessage(message->chat->id, reply.str(), false, 0, d_panelResponseKeyboard, "Markdown");

        d_bot->db() << "insert into records (chat, name, arg, time, msgId) values(?, ?, ?, ?, ?);"
                    << message->chat->id << "sleep" << action << message->date << replyMsg->messageId;
    }

    void RecordsPanel::onLastSleep(Message message)
    {
        d_bot->getApi().sendMessage(message->chat->id, d_stats.lastSleep(message->chat->id));
    }

    void RecordsPanel::onDiaper(Message message)
    {

        std::stringstream reply;
        reply << "Diaper change at " << utils::asClock(message->date);
        const auto replyMsg = d_bot->getApi().sendMessage(message->chat->id, reply.str(), false, 0, d_panelResponseKeyboard, "Markdown");

        d_bot->db() << "insert into records (chat, name, arg, time, msgId) values(?, ?, ?, ?, ?);"
                    << message->chat->id << "diaper" << nullptr << message->date << replyMsg->messageId;
    }

    void RecordsPanel::onLastDiaper(Message message)
    {
        d_bot->getApi().sendMessage(message->chat->id, d_stats.lastDiaper(message->chat->id));
    }

    void RecordsPanel::onLastDeleteRecord(CallbackQuery query)
    {
        const auto msgId = query->message->messageId;
        d_bot->db() << "delete from records where msgId = ?" << msgId;
        d_bot->getApi().deleteMessage(query->message->chat->id, msgId);
    }
} // namespace nbrecords
