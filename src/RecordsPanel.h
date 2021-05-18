#ifndef RECORDSPANEL_H
#define RECORDSPANEL_H

#include "Handler.h"
#include "Types.h"

namespace nbrecords {

class NBRecordsBot;
class RecordsPanel : public Handler<RecordsPanel>
{
public:
    RecordsPanel(NBRecordsBot*);

    void show(std::int64_t);

    // Handlers
    void onFeedLeft(Message);
    void onFeedRight(Message);
    void onLastFeed(Message);
    void onSleepStart(Message);
    void onSleepStop(Message);
    void onLastSleep(Message);
    void onDiaper(Message);
    void onLastDiaper(Message);

private:
    NBRecordsBot* d_bot;
    ReplyKeyboard d_keyboard;

    void feed(Message, std::string);
    void sleep(Message, std::string);
};

} // namespace nbrecords
#endif // RECORDSPANEL_H