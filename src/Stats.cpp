#include "Stats.h"

#include <sstream>

#include "Utils.h"

namespace nbrecords {

Stats::Stats(sqlite::database db) :
    d_db(db)
{}

std::string Stats::dayReport(std::int64_t chat, std::int32_t time) {
    std::stringstream result;
    time = utils::dayStart(time);

    try {
        //Sleep
        result << "😴 slept ";
        std::int32_t start = 0, total = 0;
        std::stringstream sleepReport;
        d_db << "select time from records where time < ? and name = 'sleep' and arg = 'start' and chat = ? order by time desc limit 1"
            << time
            << chat
            >> start;

        d_db << "select arg,time from records where time > ? and name = 'sleep' and chat = ? order by time asc"
            << time
            << chat
            >>[&](std::string action, std::int32_t eventTime) {
                if (action == "start") {
                    start = eventTime;
                } else {
                    total += eventTime-start;
                    sleepReport << utils::asClock(start) << " - " << utils::asClock(eventTime) << " | " << utils::asElapsed(eventTime - start) << std::endl;
                    start = 0;
                }
            };
        if (start) {
            sleepReport << utils::asClock(start) << " - now  " << " | " << utils::asElapsed(utils::now() - start) << std::endl;
        }
        result << utils::asElapsed(total);
        result << "``` "  << std::endl << sleepReport.str() << "```";

        //Feed
        result << std::endl << "🤱 fed ";
        int count = 0;
        std::stringstream feedReport;
        d_db << "select arg,time from records where time > ? and name = 'feed' and chat = ? order by time asc"
            << time
            << chat
            >>[&](std::string side, std::int32_t eventTime) {
                feedReport << utils::asClock(eventTime) << ": " << side << std::endl;
                ++count;
            };
        result << count << " times";
        result << "``` "  << std::endl << feedReport.str() << "```";

        //Diaper
        result << std::endl << "🚼 diaper ";
        count = 0;
        std::stringstream diaperReport;
        d_db << "select time from records where time > ? and name = 'diaper' and chat = ? order by time asc"
            << time
            << chat
            >>[&](std::int32_t eventTime) {
                diaperReport << utils::asClock(eventTime) << std::endl;
                ++count;
            };
        result << count << " times";
        result << "``` "  << std::endl << diaperReport.str() << "```";
    } catch (std::exception & e) {
        return e.what();
    }
    return result.str();
}

std::string Stats::lastDiaper(std::int64_t chat) {
    std::int32_t time = 0;
    d_db << "select time from records where chat = ? and name = ? order by time desc limit 1"
        << chat
        << "diaper"
        >> time;
    std::stringstream result;
    result << "Last diaper changed at " << utils::asClock(time)  << std::endl
        << utils::asElapsed(utils::now() - time) << " ago";
    return result.str();
}

std::string Stats::lastFeed(std::int64_t chat) {
    std::stringstream result;
    d_db << "select * from records where chat = ? and name = ? order by time desc limit 1"
        << chat
        << "feed"
        >> [&](int chat, std::string name, std::string arg, std::int32_t time) {
                arg[0] = toupper(arg[0]);
                result << arg << " at " << utils::asClock(time) << std::endl
                    << utils::asElapsed(utils::now() - time) << " ago";
            };
    return result.str();
}

std::string Stats::lastSleep(std::int64_t chat) {
    std::int32_t stop = 0, start = 0;
    d_db << "select time from records where chat = ? and name = ? and arg = ? order by time desc limit 1"
        << chat << "sleep" << "stop"
        >> stop;

    d_db << "select time from records where chat = ? and name = ? and arg = ? and time < ? ORDER by time desc limit 1"
        << chat << "sleep" << "start" << stop
        >> start;

    std::stringstream result;
    result << "Last sleep: " <<  utils::asClock(start) << " - " << utils::asClock(stop) << std::endl
        << "Total " << utils::asElapsed(stop - start) << std::endl;

    int currentStart = 0;
    try {
        d_db << "select time from records where chat = ? and name = ? and arg = ? and time > ? ORDER by time desc limit 1"
        << chat << "sleep" << "start" << stop
        >> currentStart;
    } catch (std::exception & e) {}

    const std::int32_t awake = currentStart ?  currentStart - stop : utils::now() - stop;
    result << "Awake " << utils::asElapsed(awake);

    if (currentStart) {
        result << std::endl << std::endl << "Current sleep: " << utils::asClock(currentStart) << " - now"<< std::endl
            << "Total " << utils::asElapsed(utils::now() - currentStart);
    }
    return result.str();
}

} // namespace nbrecords