#include "Utils.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace nbrecords {
namespace utils {

std::int32_t now() {
    using namespace std::chrono;
    return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

std::int32_t dayStart(std::int32_t time) {
    using namespace std;
    using namespace std::chrono;
    using days = duration<int, ratio_multiply<hours::period, ratio<24> >::type>;
    return duration_cast<days>(seconds(time)).count() * 24 * 3600;
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
    using namespace std;
    using namespace std::chrono;
    auto sec = seconds(duration);
    auto min = duration_cast<minutes>(sec);
    auto h = duration_cast<hours>(sec);

    stringstream ss;
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

} // namespace utils
} // namespace nbrecords
