#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "Types.h"

namespace nbrecords {
namespace utils {

// Time
std::int32_t now();
std::int32_t localTime(std::int32_t);
std::int32_t utcTime(std::int32_t);
std::int32_t dayStart(std::int32_t);
std::int32_t dayEnd(std::int32_t);
std::int32_t prevDay(std::int32_t);
std::int32_t nextDay(std::int32_t);
std::int32_t parseDate(const std::string&, std::string format = "%d-%m-%Y");
std::string asClock(std::int32_t);
std::string asDate(std::int32_t);
std::string format(std::int32_t, std::string);
std::string asElapsed(std::int32_t);

// Telegram
ReplyKeyboard createReplyKeyboard(const std::vector<std::vector<std::string>>&);
InlineKeyboard createInlineKeyboard(const std::vector<std::vector<std::string>>&);

} // namespace utils
} // namespace nbrecords
#endif //UTILS_H
