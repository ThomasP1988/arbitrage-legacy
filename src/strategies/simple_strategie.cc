#include <iostream>
#include <stdlib.h>
#include <typeinfo>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

#include "platforms/binance.h"
#include "shared/markets/state.h"
#include "shared/markets/config.h"

#include <mutex>
#include <memory>
#include "disruptors/binance_compare_price.h"
#include <Disruptor/Disruptor.h>
#include <Disruptor/ThreadPerTaskScheduler.h>
#include <boost/beast/websocket.hpp>
#include <atomic>

#include <tgbot/tgbot.h>
#include "shared/control-bot.h"

using namespace rapidjson;
using namespace TgBot;
using namespace std;

int SimpleStragie(Platform *platform, ControlBot &bot, const int32_t telegramUserId, StartingCurrencies &startingCurrencies)
{

    Markets markets;
    Fee fees;

    try
    {
        platform->ListMarkets(markets);
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        platform->GetAccount(fees);
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    AssociateMarketPaths(markets, startingCurrencies);

    // StringBuffer buffer;
    // Writer<StringBuffer> writer(buffer);
    // marketsJson.Accept(writer);

    // std::cout << buffer.GetString() << std::endl;
    // return 0;

    cout << "fees.buyer :" << fees.buyer << endl;
    cout << "fees.seller :" << fees.seller << endl;
    cout << "fees.taker" << fees.taker << endl;
    cout << "fees.maker" << fees.maker << endl;

    cout << "INIT Telegram bot Commands" << endl;
    vector<BotCommand::Ptr> commands;

    BotCommand::Ptr cmdStart(new BotCommand);
    cmdStart->command = "start";
    cmdStart->description = "start arbitrage";
    commands.push_back(cmdStart);

    BotCommand::Ptr cmdStop(new BotCommand);
    cmdStop->command = "stop";
    cmdStop->description = "stop arbitrage";
    commands.push_back(cmdStop);
    cout << "Setting telegram command" << endl;
    bot.setMyCommands(commands);
    cout << "END INIT Telegram bot Commands" << endl;
    try
    {

        std::shared_ptr<Disruptor::disruptor<BookTicker>> disruptor;
        std::shared_ptr<Disruptor::ThreadPerTaskScheduler> taskScheduler;
        std::shared_ptr<Disruptor::RingBuffer<BookTicker>> ringBuffer;
        function<void(BookTicker)> cb;
        function<void()> stop = []() {
            cout << "stop function was not set properly" << endl;
        };

        auto unknownUserMessage = [&bot, &telegramUserId](Message::Ptr message) {
            bot.sendMessage(telegramUserId, "Unknown user tried to send a message text: " + message->text + ", id:" + to_string(message->from->id) + ", firstname:" + message->from->firstName + ", lastname:" + message->from->lastName);
        };

        bot.onAnyMessage([&bot, &unknownUserMessage, &telegramUserId](Message::Ptr message) {
            printf("User wrote %s\n", message->text.c_str());
            if (message->from->id != telegramUserId)
            {
                unknownUserMessage(message);
                return;
            }

            if (StringTools::startsWith(message->text, "/start"))
            {
                return;
            }
        });

        std::thread startServer;
        auto const RingBufferSize = 1024;
        bot.onCommand("start", [&bot, &disruptor, &taskScheduler, &ringBuffer, &RingBufferSize, &platform, &markets, &fees, &unknownUserMessage, &cb, &stop, &startServer, &telegramUserId](Message::Ptr message) {
            cout << "start command received" << endl;
            cout << "message username:" << message->from->id << endl;
            if (message->from->id != telegramUserId)
            {
                cout << "Unknown user : " << message->from->id << endl;
                unknownUserMessage(message);
                return;
            }
            // START DISRUPTOR

            auto eventFactory = []() { return BookTicker{}; };
            taskScheduler = std::make_shared<Disruptor::ThreadPerTaskScheduler>();
            disruptor = std::make_shared<Disruptor::disruptor<BookTicker>>(eventFactory, RingBufferSize, taskScheduler);
            ringBuffer = disruptor->ringBuffer();
            auto comparePriceEventHandler = std::make_shared<ComparePriceEventHandler>(platform, markets, fees, bot, telegramUserId);

            disruptor->handleEventsWith(comparePriceEventHandler);
            disruptor->start();
            taskScheduler->start(1);
            // END DISRUPTOR

            // POPULATE MARKETS WITH CORRECT PRICES
            try
            {
                platform->SetAllMarketsBestPrices(markets);
            }
            catch (std::exception const &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
                return;
            }
            // END POPULATE
            cout << "1" << endl;
            // call back to handle data from platform websocket
            cb = [&ringBuffer](BookTicker bt) {
                auto nextSequence = ringBuffer->next();
                (*ringBuffer)[nextSequence] = bt;
                ringBuffer->publish(nextSequence);
            };
            cout << "2" << endl;
            bot.sendMessage(telegramUserId, "Server started");
            startServer = std::thread([&platform, &cb, &stop]() {
                cout << "3" << endl;
                platform->WsBookTicker(cb, stop);
            });
        });

        bot.onCommand("stop", [&disruptor, &taskScheduler, &stop, &unknownUserMessage, &bot, &startServer, &telegramUserId](Message::Ptr message) {
            if (message->from->id != telegramUserId)
            {
                unknownUserMessage(message);
                return;
            }
            if (disruptor)
            {
                disruptor->shutdown();
                taskScheduler->stop();
            }
            stop();
            if (startServer.joinable())
            {
                startServer.join();
            }
            bot.sendMessage(telegramUserId, "Server stopped");
        });

        // Wait for the end of execution and shutdown
        // comparePriceEventHandler->waitEndOfProcessing();

        // If we get here then the connection is closed gracefully
        try
        {
            printf("Bot username: %s\n", bot.getMe()->username.c_str());
            bot.deleteWebhook();
            printf("Starting long poll\n");
            while (true)
            {
                bot.longPoll();
            }
            printf("finishing long poll\n");
        }
        catch (exception &e)
        {
            printf("error: %s\n", e.what());
        }
        // disruptor->shutdown();
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}