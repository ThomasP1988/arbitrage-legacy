#include <cstdlib>
#include <unordered_map>
#include "shared/markets/state.h"
#include <iostream>

using namespace std;

void setMarketStateStruct()
{
}

void PopulateMarketBestPrices(Markets &markets, string symb, double ask, double bid)
{
    markets[symb]->bestBuyPrice = bid;
    markets[symb]->bestSellPrice = ask;
}

void PrintMarkets(Markets markets)
{
    for (const auto &[symb, market] : markets)
    {
        PrintMarket(*market);
    }
}

void PrintMarket(MarketState &market)
{
    cout << market.market << endl;
}