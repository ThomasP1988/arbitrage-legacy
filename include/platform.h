#pragma once
#include "rapidjson/document.h"
#include "shared/markets/state.h"

struct BookTicker
{
   int id;                 // u
   std::string symbol;     // s
   double bestBidPrice;    // b
   double bestBidQuantity; // B
   double bestAskPrice;    // a
   double bestAskQuantity; // A
};

typedef std::function<void(BookTicker)> CallbackFunction;

class Platform
{
public:
   virtual void ListMarkets(Markets &markets) = 0; // = 0 makes it pure virtual, prevents bug as its abstract
   virtual void SetAllMarketsBestPrices(Markets &markets) = 0;
   virtual void GetAccount(Fee &fees) = 0;
   virtual void WsBookTicker(CallbackFunction cb,  std::function<void()> &stop) = 0;
   virtual void Buy(std::string symbol, double quantity, double price) = 0;
   virtual ~Platform() {}
};
