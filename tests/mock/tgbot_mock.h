#pragma once
#include "gmock/gmock.h"
#include <tgbot/tgbot.h>
#include "shared/control-bot.h"
#include <vector>
#include <string>

class MockTgBot : public ControlBot
{
public:
    MOCK_METHOD(bool, setMyCommands, (const std::vector<TgBot::BotCommand::Ptr> & commands));
    MOCK_METHOD(TgBot::Message::Ptr, sendMessage, (std::int64_t chatId, const std::string &text));
    MOCK_METHOD(TgBot::User::Ptr, getMe, ());
    MOCK_METHOD(bool, deleteWebhook, ());
    MOCK_METHOD(void, onAnyMessage, (const TgBot::EventBroadcaster::MessageListener &listener));
    MOCK_METHOD(void, onCommand, (const std::string &commandName, const TgBot::EventBroadcaster::MessageListener &listener));
    MOCK_METHOD(void, longPoll, ());
};