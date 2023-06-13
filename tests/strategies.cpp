#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>
#include <stdlib.h>
#include <tuple>
#include <string>

#include "mock/platform_mock.h"
#include "mock/tgbot_mock.h"
#include "shared/markets/state.h"
#include "shared/markets/config.h"
#include "strategies/simple_strategie.h"

#include <tgbot/EventBroadcaster.h>

// This is all that is needed to compile a test-runner executable.
// More tests can be added here, or in a new tests/*.cpp file.
using namespace std;
using ::testing::_;
using ::testing::Assign;
using ::testing::ByRef;
using ::testing::Const;
using ::testing::DoAll;
using ::testing::InitGoogleMock;
using ::testing::InitGoogleTest;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;

class Strategies : public ::testing::Test
{
protected:
    void SetUp(void)
    {

        auto generateMarketState = [](string base, string quote,
                                      double buyPrice, double buyQuantity, double sellPrice, double sellQuantity) -> tuple<MarketState *, string> {
            MarketState *mstate = new MarketState();

            mstate->bestBuyPrice = buyPrice;
            mstate->bestBuyQuantity = buyQuantity;
            mstate->bestSellPrice = sellPrice;
            mstate->bestSellQuantity = sellQuantity;
            mstate->paths = {{Path{
                EMPTY,
                EMPTY,
                EMPTY,
                EMPTY,
                EMPTY,
                2,
                false}}};

            mstate->base = base;
            mstate->quote = quote;
            auto keySymbol = mstate->base + mstate->quote;
            mstate->market = keySymbol;
            return make_tuple(mstate, keySymbol);
        };

        auto [mstate, keySymbol] = generateMarketState("TOM", "CAR", 2, 4, 2, 2);
        markets[keySymbol] = mstate;

        tie(mstate, keySymbol) = generateMarketState("CAR", "OIG", 2, 2, 2, 1);
        markets[keySymbol] = mstate;

        tie(mstate, keySymbol) = generateMarketState("OIG", "AVO", 0.5, 4, 0.5, 1);
        markets[keySymbol] = mstate;

        tie(mstate, keySymbol) = generateMarketState("AVO", "TOM", 0.8, 4, 0.5, 4);
        markets[keySymbol] = mstate;

        fees = {
            0.0001, // maker
            0.0001, //taker
            0,       //buyer
            0,       //seller
        };
    }

    void TearDown(void)
    {
    }

    Markets markets;
    Fee fees;
};

// ACTION_P(AssignMarkets, param) { *static_cast<Markets *>(arg0) = param; }

void MimickPlatformWsTicker(CallbackFunction cb, std::function<void()> &stop)
{

    BookTicker bt = {
        2355,     // id
        "TOMCAR", // symbol
        2,       // bestBidPrice
        2,        // bestBidQuantity
        2,     // bestAskPrice
        2         // bestAskQuantity
    };

    cb(bt);

    stop = []() {
        cout << "stop" << endl;
    };
}

void MimickTgBotOnCommand(const std::string &commandName, const TgBot::EventBroadcaster::MessageListener &listener)
{
    cout << "OnCommand" << endl;
    if (commandName == "start")
    {
        cout << "start" << endl;
        auto message(make_shared<TgBot::Message>());
        auto user(make_shared<TgBot::User>());
        user->id = 1;
        message->from = user;
        listener(message);
    }
}

TEST_F(Strategies, SimpleStrategie)
{

    PrintMarkets(markets);

    MockPlatform platform;

    EXPECT_CALL(platform, ListMarkets(_))
        .WillRepeatedly(DoAll(Invoke([]() {
                                  cout << "ici" << endl;
                              }),
                              SetArgReferee<0>(markets), Return()));

    EXPECT_CALL(platform, SetAllMarketsBestPrices(_)).WillRepeatedly(DoAll(Return()));

    EXPECT_CALL(platform, GetAccount(_))
        .WillRepeatedly(DoAll(SetArgReferee<0>(fees), Return()));

    EXPECT_CALL(platform, WsBookTicker(_, _))
        .WillRepeatedly(Invoke(MimickPlatformWsTicker));

    MockTgBot mockBot;

    EXPECT_CALL(mockBot, setMyCommands(_))
        .WillRepeatedly(DoAll(Return(true)));

    EXPECT_CALL(mockBot, sendMessage(_, _))
        .WillRepeatedly(DoAll(Return(TgBot::Message::Ptr{})));

    EXPECT_CALL(mockBot, deleteWebhook())
        .WillRepeatedly(DoAll(Return(true)));

    auto user(make_shared<TgBot::User>());
    user->id = 895329659;
    user->username = "test";
    EXPECT_CALL(mockBot, getMe())
        .WillRepeatedly(DoAll(Return(user)));

    EXPECT_CALL(mockBot, onAnyMessage(_))
        .WillRepeatedly(DoAll(Return()));
        
    EXPECT_CALL(mockBot, longPoll())
        .WillRepeatedly(DoAll(Return()));

    EXPECT_CALL(mockBot, onCommand(_, _))
        .WillRepeatedly(DoAll(Invoke(MimickTgBotOnCommand)));

    const int32_t telegramUserId = 1;
    StartingCurrencies startingCurrencies = {{"TOM", true}};

    SimpleStragie(&platform, mockBot, telegramUserId, startingCurrencies);
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    InitGoogleMock(&argc, argv);
    auto result = RUN_ALL_TESTS();
    cout << result << endl;
    return 0;
}
