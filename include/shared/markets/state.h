#pragma once

#include <cstdlib>
#include <unordered_map>
#include <string>
#include <vector>
#include <map>
#include "./config.h"
#include "tbb/concurrent_unordered_map.h"
#include <mutex>
using namespace std;

const string EMPTY = "empty";

struct ArbitragePath
{
    string Market;
    int Action;
    double Price;
    double OperationPrice;
    double Possession;
    double Fees; 
};

struct Path
{
    string market;
    string base;
    string quote;
    string nextCurrency;
    string originalFiat;
    int action;
    bool isSet;
};

struct MarketState
{
    mutex mtx;
    double price;
    string market;
    tbb::concurrent_unordered_map<string, double> sell;
    tbb::concurrent_unordered_map<string, double> buy;
    double bestSellPrice;
    double bestSellQuantity;
    double bestBuyPrice;
    double bestBuyQuantity;
    int lastUpdate;
    string lastUpdateString;
    bool isReverse;
    string base;
    string quote;
    array<array<Path, 5>, 5> paths;
};

struct Fee {
	double maker;
	double taker;  
	double buyer;  
	double seller; 
};

typedef tbb::concurrent_unordered_map<string, MarketState *> Markets;

void PopulateMarketBestPrices(Markets &markets, string symb, double ask, double bid);

void setMarketStateStruct(Markets &markets);
void AssociateMarketPaths(Markets &markets, StartingCurrencies &startingCurrencies);

vector<array<Path, 5>> FindPaths(vector<array<Path, 5>> &paths, Markets &markets, StartingCurrencies &startingCurrencies, int step, map<string, int> &marketCount);
void PopulateMarketCount(map<string, int> &marketCount, Markets &markets);

void PrintPath(array<Path, 5> path);
void PrintPaths(array<array<Path, 5>, 5> paths);
void PrintPathDetails(array<Path, 5> path);
void PrintMarketsPath(Markets markets);
void PrintMarketCount(map<string, int> marketCount);
void PrintMarkets(Markets markets);
void PrintMarket(MarketState &market);
void PrintArbitragePaths(array<ArbitragePath, 5> arbitragePaths);
