#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "Types.h"

namespace nbrecords {
namespace utils {

std::int32_t now();
std::int32_t dayStart(std::int32_t);
std::string asClock(std::int32_t);
std::string asDate(std::int32_t);
std::string format(std::int32_t, std::string);
std::string asElapsed(std::int32_t);
ReplyKeyboard createReplyKeyboard(const std::vector<std::vector<std::string>>&);

} // namespace utils
} // namespace nbrecords
#endif //UTILS_H
