#include <csignal>
#include <stdlib.h>

#include "NBRecordsBot.h"

#include "Utils.h"

int main() {
    const char* token = getenv("TOKEN");
    if (!token) {
        std::cerr << "Please provide TOKEN as an environment variable" << std::endl;
        return 1;
    }
    nbrecords::NBRecordsBot bot(token, "dbfile.db");
    bot.start();
    return 0;
}