#ifndef STATS_H
#define STATS_H

#include <sqlite_modern_cpp.h>

#include "Handler.h"
#include "Types.h"

namespace nbrecords {

class Stats
{
public:
    Stats(sqlite::database);

    std::string dayReport(std::int64_t, std::int32_t);
    std::string lastDiaper(std::int64_t);
    std::string lastFeed(std::int64_t);
    std::string lastSleep(std::int64_t);

private:
    sqlite::database d_db;
};

} // namespace nbrecords
#endif // STATS_H