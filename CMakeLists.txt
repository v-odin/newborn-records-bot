cmake_minimum_required(VERSION 2.8.4)
project(newborn-records)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(Boost_USE_MULTITHREADED ON)

find_path(TGBOT_INCLUDE_DIR tgbot/tgbot.h)

find_library(TGBOT_LIBRARY TgBot)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(src ${TGBOT_INCLUDE_DIR})

add_executable(newborn-records
    src/main.cpp
    src/NBRecordsBot.cpp
    src/RecordsPanel.cpp
    src/Stats.cpp
    src/Utils.cpp
)

target_link_libraries(newborn-records
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    ${TGBOT_LIBRARY}
    SQLite::SQLite3
    ${CMAKE_DL_LIBS}
)