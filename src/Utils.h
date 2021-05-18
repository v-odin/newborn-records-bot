#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "Types.h"

namespace nbrecords {
namespace utils {

std::int32_t now();
std::string asClock(std::int32_t, std::string format = "%H:%M");
std::string asElapsed(std::int32_t);
ReplyKeyboard createReplyKeyboard(const std::vector<std::vector<std::string>>&);

} // namespace utils
} // namespace nbrecords
#endif //UTILS_H