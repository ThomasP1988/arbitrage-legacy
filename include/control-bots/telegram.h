#include <string>
#include <tgbot/tgbot.h>
#include "shared/control-bot.h"

// this class is useful for using abstract class only, then i can mock it for test, otherwise its hell
class Telegram : public ControlBot
{
public:
    explicit Telegram(std::string token) : _bot(TgBot::Bot(token)), _longPoll(TgBot::TgLongPoll(this->_bot)) {};

    inline bool setMyCommands(const std::vector<TgBot::BotCommand::Ptr> &commands)
    {
        return this->_bot.getApi().setMyCommands(commands);
    }
    inline TgBot::Message::Ptr sendMessage(std::int64_t chatId, const string &text)
    {
        return this->_bot.getApi().sendMessage(chatId, text);
    }
    inline bool deleteWebhook()
    {
        return this->_bot.getApi().deleteWebhook();
    }

    inline TgBot::User::Ptr getMe()
    {
        return this->_bot.getApi().getMe();
    }

    inline void onAnyMessage(const TgBot::EventBroadcaster::MessageListener &listener)
    {
        this->_bot.getEvents().onAnyMessage(listener);
    }
    inline void onCommand(const std::string &commandName, const TgBot::EventBroadcaster::MessageListener &listener)
    {
        this->_bot.getEvents().onCommand(commandName, listener);
    }



    inline void longPoll()
    {
      
        this->_longPoll.start();
    }

private:
    TgBot::Bot _bot;
    TgBot::TgLongPoll _longPoll;
};