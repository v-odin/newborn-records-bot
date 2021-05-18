#include "RecordsPanel.h"

#include "NBRecordsBot.h"
#include "Utils.h"

namespace nbrecords {

RecordsPanel::RecordsPanel(NBRecordsBot* bot) :
    d_bot(bot)
{
    const std::string feedLeft = "🤱 left", feedRight = "🤱 right", lastFeed = "📋 feed";
    const std::string sleepStart = "😴 sleep", sleepStop = "☀️ wake up", lastSleep = "📋 sleep";
    const std::string diaper = "🚼 change diaper", lastDiaper = "📋 diaper";

    d_keyboard = utils::createReplyKeyboard({
        {feedLeft, feedRight},
        {sleepStart, sleepStop},
        {diaper},
        {lastFeed, lastSleep, lastDiaper}
    });
    // TODO: to make selecetive – so far does not work as expected
    // keyboard->selective = true;

    d_bot->addReplyHandler(feedLeft, asHandler(&RecordsPanel::onFeedLeft));
    d_bot->addReplyHandler(feedRight, asHandler(&RecordsPanel::onFeedRight));
    d_bot->addReplyHandler(lastFeed, asHandler(&RecordsPanel::onLastFeed));
    d_bot->addReplyHandler(sleepStart, asHandler(&RecordsPanel::onSleepStart));
    d_bot->addReplyHandler(sleepStop, asHandler(&RecordsPanel::onSleepStop));
    d_bot->addReplyHandler(lastSleep, asHandler(&RecordsPanel::onLastSleep));
    d_bot->addReplyHandler(diaper, asHandler(&RecordsPanel::onDiaper));
    d_bot->addReplyHandler(lastDiaper, asHandler(&RecordsPanel::onLastDiaper));
}

void RecordsPanel::show(std::int64_t chatId) {
    d_bot->getApi().sendMessage(chatId, "Choose an action", false, 0, d_keyboard);
}

// Handlers

void RecordsPanel::onFeedLeft(Message message) {
    feed(message, "left");
}

void RecordsPanel::onFeedRight(Message message) {
    feed(message, "right");
}

void RecordsPanel::feed(Message message, std::string side) {
    d_bot->db() << "insert into records (chat, name, arg, time) values(?, ?, ?, ?);"
        << message->chat->id << "feed" << side << message->date;

    std::stringstream reply;
    reply << "Feeding "<< side << " at " << utils::asClock(message->date);
    d_bot->getApi().sendMessage(message->chat->id, reply.str());
}

void RecordsPanel::onLastFeed(Message message) {
    d_bot->db() << "select * from records where chat = ? and name = ? order by time desc limit 1"
        << message->chat->id
        << "feed"
        >> [&](int chat, std::string name, std::string arg, std::int32_t time) {
                std::stringstream reply;
                arg[0] = toupper(arg[0]);
                reply << arg << " at " << utils::asClock(time) << std::endl
                    << utils::asElapsed(utils::now() - time) << " ago";
                d_bot->getApi().sendMessage(message->chat->id, reply.str());
            };
}

void RecordsPanel::onSleepStart(Message message) {
    sleep(message, "start");
}

void RecordsPanel::onSleepStop(Message message) {
    sleep(message, "stop");
}

void RecordsPanel::sleep(Message message, std::string action) {
    d_bot->db() << "insert into records (chat, name, arg, time) values(?, ?, ?, ?);"
        << message->chat->id << "sleep" << action << message->date;

    std::stringstream reply;
    action[0] = toupper(action[0]);
    reply << action << " sleeping"<< " at " << utils::asClock(message->date);
    d_bot->getApi().sendMessage(message->chat->id, reply.str());
}

void RecordsPanel::onLastSleep(Message message) {
    std::int32_t stop = 0, start = 0;
    d_bot->db() << "select time from records where chat = ? and name = ? and arg = ? order by time desc limit 1"
        << message->chat->id << "sleep" << "stop"
        >> stop;

    d_bot->db() << "select time from records where chat = ? and name = ? and arg = ? and time < ? ORDER by time desc limit 1"
        << message->chat->id << "sleep" << "start" << stop
        >> start;

    std::stringstream reply;
    reply << "Last sleep: " <<  utils::asClock(start) << " - " << utils::asClock(stop) << std::endl
        << "Total " << utils::asElapsed(stop - start) << std::endl;

    int currentStart = 0;
    try {
        d_bot->db() << "select time from records where chat = ? and name = ? and arg = ? and time > ? ORDER by time desc limit 1"
        << message->chat->id << "sleep" << "start" << stop
        >> currentStart;
    } catch (std::exception & e) {}

    const std::int32_t awake = currentStart ?  currentStart - stop : utils::now() - stop;
    reply << "Awake " << utils::asElapsed(awake);

    if (currentStart) {
        reply << std::endl << std::endl << "Current sleep: " << utils::asClock(currentStart) << " - now"<< std::endl
            << "Total " << utils::asElapsed(utils::now() - currentStart);
    }

    d_bot->getApi().sendMessage(message->chat->id, reply.str());
}

void RecordsPanel::onDiaper(Message message){
    d_bot->db() << "insert into records (chat, name, arg, time) values(?, ?, ?, ?);"
            << message->chat->id << "diaper" << nullptr << message->date;

    std::stringstream reply;
    reply << "Diaper change at " << utils::asClock(message->date);
    d_bot->getApi().sendMessage(message->chat->id, reply.str());
}

void RecordsPanel::onLastDiaper(Message message) {
    std::int32_t time = 0;
    d_bot->db() << "select time from records where chat = ? and name = ? order by time desc limit 1"
        << message->chat->id
        << "diaper"
        >> time;
    std::stringstream reply;
    reply << "Last diaper changed at " << utils::asClock(time)  << std::endl
        << utils::asElapsed(utils::now() - time) << " ago";
    d_bot->getApi().sendMessage(message->chat->id, reply.str());
}

} // namespace nbrecords