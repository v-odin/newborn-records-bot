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

std::string asClock(std::int32_t unixTime, std::string format) {
    std::time_t time(unixTime);
    std::stringstream ss;
    ss << std::put_time(localtime(&time), format.c_str());
    return ss.str();
}

std::string asElapsed(std::int32_t duration) {
    using namespace std;
    auto sesonds = chrono::seconds(duration);
    auto minutes = chrono::duration_cast<chrono::minutes>(sesonds);
    auto hours = chrono::duration_cast<chrono::hours>(sesonds);

    stringstream ss;
    if (hours.count()) {
        ss << hours.count() << "h ";
    }
    ss << minutes.count() - hours.count() * 60 << "min";
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