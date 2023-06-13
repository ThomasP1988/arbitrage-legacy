// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.

#define RAPIDJSON_HAS_STDSTRING 1

#include <iostream>
#include <stdlib.h>

#include <cstdlib>
#include <iostream>
#include <string>

#include "platforms/binance.h"
#include "strategies/simple_strategie.h"
#include "shared/markets/config.h"
#include "utils/time.h"
#include "control-bots/telegram.h"
#include <tgbot/tgbot.h>

using namespace TgBot;
using namespace std;

std::string get_compile_version()
{
    char buffer[sizeof(BOOST_PLATFORM) + sizeof(BOOST_COMPILER) + sizeof(__DATE__) + 5];
    sprintf(buffer, "[%s/%s](%s)", BOOST_PLATFORM, BOOST_COMPILER, __DATE__);
    std::string compileinfo(buffer);
    return compileinfo;
}

// Sends a WebSocket message and prints the response
int main()
{
    // Bot bot("1328203198:AAGtvukCVEQvhwuLH0yC-YX4h3V-gzM-O-8");
    getTimestampStr();
    auto bot = Telegram("");
    const int32_t telegramUserId = 1;
    auto platform = new BinancePlatform{};
    cout << get_compile_version() << endl;
    cout << __cplusplus << endl;
    StartingCurrencies startingCurrencies = {{"EUR", true}};
    return SimpleStragie(platform, bot, telegramUserId, startingCurrencies);
}
