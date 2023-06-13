#pragma once
#include <string>
#include <vector>
#include <tgbot/tgbot.h>

class ControlBot
{
public:
   virtual bool setMyCommands(const std::vector<TgBot::BotCommand::Ptr> & commands) = 0; // = 0 makes it pure virtual, prevents bug as its abstract
   virtual TgBot::Message::Ptr sendMessage(std::int64_t chatId, const std::string &text) = 0;
   virtual bool deleteWebhook() = 0;
   virtual TgBot::User::Ptr getMe() = 0;
   virtual void onAnyMessage(const TgBot::EventBroadcaster::MessageListener &listener) = 0;
   virtual void onCommand(const std::string &commandName, const TgBot::EventBroadcaster::MessageListener &listener) = 0;
   virtual void longPoll() = 0;
   virtual ~ControlBot() {}
};