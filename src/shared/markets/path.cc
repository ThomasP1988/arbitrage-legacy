#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <typeinfo>
#include <cstdio>
#include <map>

#include "shared/markets/state.h"
#include "shared/markets/config.h"

using namespace std;
/*
 * This function is made to find the path from a currency to another the same one 

 */
void AssociateMarketPaths(Markets &markets, StartingCurrencies &startingCurrencies)
{
    vector<array<Path, 5>> pathsInit;
    map<string, int> marketCount;

    PopulateMarketCount(marketCount, markets);

    vector<array<Path, 5>> paths = FindPaths(pathsInit, markets, startingCurrencies, 0, marketCount);
    cout << "final size paths :" << paths.size() << endl;

    auto PathCount = [](Path path) {
        if (path.isSet)
        {
            return true;
        }
        else
        {
            return false;
        }
    };

    // sort paths by shortest
    sort(paths.begin(), paths.end(), [PathCount](const array<Path, 5> &l, const array<Path, 5> &r) {
        int sizeL = count_if(l.begin(), l.end(), PathCount);
        int sizeR = count_if(r.begin(), r.end(), PathCount);
        return sizeL > sizeR;
    });

    // finds 5 paths by pairs, shortest is best

    map<string, int> CountByMarket;

    const int pathStart = paths.size();
    cout << "path size" << pathStart << endl;
    for (int i = pathStart; i >= 0; i--)
    {
        for (int j = 0; j < 5; j++)
        {
            if (paths[i][j].isSet)
            {
                if (CountByMarket.find(paths[i][j].market) == CountByMarket.end())
                {
                    CountByMarket[paths[i][j].market] = 0;
                }

                if (CountByMarket[paths[i][j].market] < 5)
                {

                    markets[paths[i][j].market]->paths[CountByMarket[paths[i][j].market]] = paths[i];
                    CountByMarket[paths[i][j].market]++;
                }
            }
        }
    }
}

void PopulateMarketCount(map<string, int> &marketCount, Markets &markets)
{
    for (const auto &[symb, mstate] : markets)
    {
        // cout << symb << endl;
        marketCount[symb] = 0;
    }
}

vector<array<Path, 5>> FindPaths(vector<array<Path, 5>> &paths, Markets &markets, StartingCurrencies &startingCurrencies, int step, map<string, int> &marketCount)
{
    cout << "market size :" << markets.size() << endl;
    cout << "starting currencies :" << startingCurrencies.size() << endl;
    cout << "step" << step << endl;
    cout << "paths size " << paths.size() << endl;

    const auto isFundCurrency = [startingCurrencies](string curr) -> bool {
        if (startingCurrencies.size() > 0)
        {
            if (startingCurrencies.count(curr) > 0 && startingCurrencies.at(curr))
            {
                return true;
            }
            return false;
        }
        else
        {
            return false;
        }
    };

    if (step == 0)
    {
        for (auto const &[marketSymb, mstate] : markets)
        {
            //   cout << "first step :" << mstate->market << endl;
            if (isFundCurrency(mstate->base) || isFundCurrency(mstate->quote))
            {

                // cout << "first step :" << isFundCurrency(mstate->base) << endl;
                string fiatFirstStep;
                string nextCurrency;
                int action;
                if (isFundCurrency(mstate->base))
                {
                    // cout << "base1 :" <<  mstate->base << endl;
                    // cout << "quote1 :" <<  mstate->quote << endl;
                    fiatFirstStep = mstate->base;
                    nextCurrency = mstate->quote;
                    action = 0;
                }
                if (isFundCurrency(mstate->quote))
                {
                    // cout << "base2 :" <<  mstate->base << endl;
                    // cout << "quote2 :" <<  mstate->quote << endl;
                    fiatFirstStep = mstate->quote;
                    nextCurrency = mstate->base;
                    action = 1;
                }

                array<Path, 5> newPath = {Path{
                    EMPTY,
                    EMPTY,
                    EMPTY,
                    EMPTY,
                    EMPTY,
                    2,
                    false}};

                newPath[step] = Path{
                    mstate->market,
                    mstate->base,
                    mstate->quote,
                    nextCurrency,
                    fiatFirstStep,
                    action,
                    true};
                paths.insert(paths.end(), newPath);
            }
        }
        step++;
        return FindPaths(paths, markets, startingCurrencies, step, marketCount);
    }
    else if (step < 4)
    {
        vector<array<Path, 5>> newPaths;

        const int pathStart = paths.size() - 1;
        for (int i = pathStart; i >= 0; i--)
        {

            if (paths[i][step - 1].isSet == false || (step > 1 && (isFundCurrency(paths[i][step - 1].base) || isFundCurrency(paths[i][step - 1].quote))))
            {
                newPaths.insert(newPaths.end(), paths[i]);
                continue;
            }

            for (const auto &[symb, mstate] : markets)
            {

                if (paths[i][step - 1].market.compare(symb) != 0 && (paths[i][step - 1].nextCurrency.compare(mstate->base) == 0 || paths[i][step - 1].nextCurrency.compare(mstate->quote) == 0))
                {

                    // if (limit > 0)
                    // {
                    //     if (marketCount[symb] >= limit)
                    //     {
                    //         continue;
                    //     }
                    //     else
                    //     {
                    //         marketCount[symb]++;
                    //     }
                    // }

                    // if fiat, check if its the same than the step one, if not continue
                    if (isFundCurrency(mstate->base) || isFundCurrency(mstate->quote))
                    {
                        string fiatCurrentStep;
                        if (isFundCurrency(mstate->base))
                        {
                            fiatCurrentStep = mstate->base;
                        }
                        else if (isFundCurrency(mstate->quote))
                        {
                            fiatCurrentStep = mstate->quote;
                        }

                        if (paths[i][0].originalFiat != fiatCurrentStep)
                        {
                            continue;
                        }
                    }

                    // define action
                    int action;
                    if (paths[i][step - 1].nextCurrency == mstate->base)
                    {
                        action = 0;
                    }
                    else if (paths[i][step - 1].nextCurrency == mstate->quote)
                    {
                        action = 1;
                    }

                    // calculate next currency
                    string nextCurrency = "";
                    if (paths[i][step - 1].nextCurrency == mstate->base)
                    {
                        nextCurrency = mstate->quote;
                    }
                    else if (paths[i][step - 1].nextCurrency == mstate->quote)
                    {
                        nextCurrency = mstate->base;
                    }
                    // copy original paths(5), add new path to it
                    array<Path, 5> newPath = paths[i];
                    newPath[step] = Path{
                        mstate->market,
                        mstate->base,
                        mstate->quote,
                        nextCurrency,
                        paths[i][0].originalFiat,
                        action,
                        true};
                    // if (step == 3)
                    // {
                    //     PrintPathDetails(newPath);
                    // }
                    newPaths.insert(newPaths.end(), newPath);
                    // cout << "EEE new paths size :" << newPaths.size() << endl;
                }
            }
        }
        cout << "finish" << endl;
        // PrintPaths(paths);
        return FindPaths(newPaths, markets, startingCurrencies, ++step, marketCount);
    }
    else if (step == 4)
    {
        vector<array<Path, 5>> newPaths;

        const int pathStart = paths.size() - 1;
        for (int i = pathStart; i >= 0; i--)
        {
            if (paths[i][step - 1].isSet == false || isFundCurrency(paths[i][step - 1].base) || isFundCurrency(paths[i][step - 1].quote))
            {
                newPaths.insert(newPaths.end(), paths[i]);
                continue;
            }

            string symb1 = paths[i][0].originalFiat + paths[i][step - 1].nextCurrency;
            string symb2 = paths[i][step - 1].nextCurrency + paths[i][0].originalFiat;
            string lastSymbol = "";

            if (markets.find(symb1) != markets.end())
            {
                lastSymbol = symb1;
            }
            else if (markets.find(symb2) != markets.end())
            {
                lastSymbol = symb2;
            }
            else
            {
                continue;
            }
            MarketState *symbol = markets[lastSymbol];
            int action;
            if (paths[i][step - 1].nextCurrency == symbol->base)
            {
                action = 0;
            }
            else if (paths[i][step - 1].nextCurrency == symbol->quote)
            {
                action = 1;
            }

            array<Path, 5> newPath = paths[i];
            newPath[step] = Path{
                symbol->market,
                symbol->base,
                symbol->quote,
                "",
                paths[i][0].originalFiat,
                action,
                true};
            newPaths.insert(newPaths.end(), newPath);
        }

        return newPaths;
    }
    return paths;
}

void PrintPathDetails(array<Path, 5> path)
{

    for (int i = 0; i < 4; i++)
    {

        cout << "step : " << i << " | ";
        cout << "isSet : " << path[i].isSet << " ---- ";
        if (path[i].isSet == true)
        {
            cout << "market : " << path[i].market << " ---- ";
            cout << "action : " << path[i].action << " | ";
            cout << "base : " << path[i].base << " | ";
            cout << "quote : " << path[i].quote << " | ";
            cout << "nextCurrency : " << path[i].nextCurrency << " | ";
            cout << "originalFiat : " << path[i].originalFiat << " | ";
        }

        cout << endl;
    }

    cout << endl;
}

void PrintPath(array<Path, 5> path)
{

    for (int i = 0; i < 5; i++)
    {
        if (path[i].isSet)
        {
            cout << path[i].market << " | ";
        }
        else
        {
            cout << path[i].isSet << " | ";
        }
    }

    cout << endl;
}

void PrintPaths(array<array<Path, 5>, 5> paths)
{
    for (array<array<Path, 5>, 5>::iterator it = paths.begin(); it != paths.end(); ++it)
    {
        PrintPath(*it);
    }
}

void PrintMarketsPath(Markets markets)
{
    for (const auto &[symb, market] : markets)
    {
        cout << " ----------------------------------- " << endl;
        cout << "market : " << market->market << endl;
        const int pathStart = market->paths.size() - 1;
        for (int i = pathStart; i >= 0; i--)
        {
            PrintPath(market->paths[i]);
        }
        cout << " ----------------------------------- " << endl;
    }
}

void PrintMarketCount(map<string, int> marketCount)
{

    for (const auto &[symb, count] : marketCount)
    {
        cout << "symb :" << symb << " | count :" << count << endl;
    }
}

void PrintArbitragePaths(array<ArbitragePath, 5> arbitragePaths)
{
    for (int i = 0; i < 5; i++)
    {
        cout << "market :" << arbitragePaths[i].Market << " | action :" << arbitragePaths[i].Action << endl;
        cout << "possession :" << arbitragePaths[i].Possession << endl;
        cout << "price :" << arbitragePaths[i].Price << endl;
    }
}
