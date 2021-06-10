#include "Utils.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "date/tz.h"

namespace nbrecords {
namespace utils {

using namespace std::chrono;

std::int32_t now() {
    return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

std::int32_t localTime(std::int32_t unixTime) {
    system_clock::time_point tp{seconds(unixTime)};
    auto local = date::make_zoned(date::current_zone(), tp).get_local_time();
    return date::floor<seconds>(local).time_since_epoch().count();
}

std::int32_t utcTime(std::int32_t unixTime) {
    auto local = make_zoned(date::current_zone(), date::local_seconds{seconds(unixTime)});
    return local.get_sys_time().time_since_epoch().count();
}

std::int32_t dayStart(std::int32_t unixTime) {
    const std::int32_t localDayStart = date::floor<date::days>(seconds(localTime(unixTime))).count() * 24 * 3600;
    return utcTime(localDayStart);
}

std::int32_t dayEnd(std::int32_t unixTime) {
    const std::int32_t localDayEnd = (date::floor<date::days>(seconds(localTime(unixTime))).count() + 1) * 24 * 3600;
    return utcTime(localDayEnd);
}

std::int32_t prevDay(std::int32_t unixTime) {
    return unixTime - 24 * 3600;
}

std::int32_t nextDay(std::int32_t unixTime) {
    return unixTime + 24 * 3600;
}

std::int32_t parseDate(const std::string& date, std::string format) {
    int32_t result = 0;
    std::stringstream ss(date);
    std::tm t = {};
    if (ss >> std::get_time(&t, format.c_str())) {
        t.tm_isdst = -1;
        result = std::mktime(&t);
    } else {
        throw("Could not parse the date: " + date);
    }
    return result;
}

std::string asClock(std::int32_t unixTime) {
    return format(unixTime, "%H:%M");
}

std::string asDate(std::int32_t unixTime) {
    return format(unixTime, "%d-%m-%Y");
}

std::string format(std::int32_t unixTime, std::string formatStr) {
    std::time_t time(unixTime);
    std::stringstream ss;
    ss << std::put_time(localtime(&time), formatStr.c_str());
    return ss.str();
}

std::string asElapsed(std::int32_t duration) {
    auto sec = seconds(duration);
    auto min = duration_cast<minutes>(sec);
    auto h = duration_cast<hours>(sec);

    std::stringstream ss;
    if (h.count()) {
        ss << h.count() << "h ";
    }
    ss << min.count() - h.count() * 60 << "min";
    return ss.str();
}

ReplyKeyboard createReplyKeyboard(const std::vector<std::vector<std::string>>& buttons) {
    auto keyboard = std::make_shared<TgBot::ReplyKeyboardMarkup>();
    for (const auto& names: buttons) {
        std::vector<TgBot::KeyboardButton::Ptr> buttonsRow;
        for (const auto& name: names) {
            auto button = std::make_shared<TgBot::KeyboardButton>();
            button->text = name;
            buttonsRow.push_back(button);
        }
        keyboard->keyboard.push_back(buttonsRow);
    }
    return keyboard;
}

InlineKeyboard createInlineKeyboard(const std::vector<std::vector<std::string>>& buttons) {
    auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
    for (const auto& names: buttons) {
        std::vector<TgBot::InlineKeyboardButton::Ptr> buttonsRow;
        for (const auto& name: names) {
            auto button = std::make_shared<TgBot::InlineKeyboardButton>();
            button->text = name;
            button->callbackData = name;
            buttonsRow.push_back(button);
        }
        keyboard->inlineKeyboard.push_back(buttonsRow);
    }
    return keyboard;
}

} // namespace utils
} // namespace nbrecords
