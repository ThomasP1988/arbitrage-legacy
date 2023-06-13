#include <Disruptor/IEventHandler.h>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include "platforms/binance.h"
#include "shared/markets/state.h"
#include "tbb/concurrent_unordered_map.h"
#include "shared/control-bot.h"
#include "platform.h"

struct ComparePriceEventHandler : Disruptor::IEventHandler<BookTicker>
{
    explicit ComparePriceEventHandler(Platform *p, Markets &m, Fee &f, ControlBot &b, std::int32_t u) : platform(p), markets(m), fees(f), bot(b), userId(u)
    {
    }

    void onEvent(BookTicker &event, int64_t, bool) override
    {

        // std::cout << "BookTicker: " << event.symbol << std::endl;

        std::time_t result = std::time(nullptr);
        std::cout << std::asctime(std::localtime(&result))
                  << result << " seconds since the Epoch\n";

        markets[event.symbol]->mtx.lock();
        // populate prices in the heap
        PopulateMarketBestPrices(markets, event.symbol, event.bestAskPrice, event.bestBidPrice);
        mutexes[event.symbol] = true;
        // compare prices
        for (int i = 0; i < 5; i++) // 5 = number of paths collections
        {
            double spent = 500;
            double possession = spent;
            int maxVolumeIndex = -1;
            double maxVolume;
            double totalFees = 0;
            double result;
            double aggregateRatioAfterMaxVolume = 0;

            for (int j = 0; j < 5; j++)
            {

                if (markets[event.symbol]->paths[i][j].isSet)
                {
                    // we block the values of our pairs, if it gets updated in the meantime, it can lead to dangerous operation
                    if (!mutexes[markets[event.symbol]->paths[i][j].market])
                    {
                        markets[markets[event.symbol]->paths[i][j].market]->mtx.lock();
                        mutexes[markets[event.symbol]->paths[i][j].market] = true;
                    }

                    arbitragePaths[j].Market = markets[event.symbol]->paths[i][j].market;
                    arbitragePaths[j].Action = markets[event.symbol]->paths[i][j].action;

                    if (markets[event.symbol]->paths[i][j].action == 1) // sell
                    {
                        arbitragePaths[j].Price = markets[arbitragePaths[j].Market]->bestSellPrice;
                        if (possession > markets[arbitragePaths[j].Market]->bestSellQuantity) // as we sell we just need to compare the quantity e.g. you have 4 bitcoins to sell, you compare it with the quantity only
                        {
                            maxVolumeIndex = j;
                            maxVolume = markets[arbitragePaths[j].Market]->bestSellQuantity;
                            possession = markets[arbitragePaths[j].Market]->bestSellQuantity;
                            aggregateRatioAfterMaxVolume = 1 / markets[arbitragePaths[j].Market]->bestSellPrice;
                            totalFees = 0;
                        }
                        else if (aggregateRatioAfterMaxVolume != 0)
                        {
                            aggregateRatioAfterMaxVolume = aggregateRatioAfterMaxVolume / markets[arbitragePaths[j].Market]->bestSellPrice;
                        }

                        possession = possession / markets[arbitragePaths[j].Market]->bestSellPrice;

                        // fees have to be calculated, then the total has to be converted and added to calculated fees
                        double feesStep = (possession * fees.buyer) + (possession * fees.taker);
                        arbitragePaths[j].Fees = feesStep;
                        totalFees = (totalFees / markets[arbitragePaths[j].Market]->bestSellPrice) + feesStep;
                    }
                    else // buy
                    {

                        if (possession > (markets[arbitragePaths[j].Market]->bestBuyQuantity * markets[arbitragePaths[j].Market]->bestBuyPrice)) // as we buy we need to multiply quantity and price
                        {
                            maxVolumeIndex = j;
                            maxVolume = markets[arbitragePaths[j].Market]->bestBuyQuantity;
                            possession = markets[arbitragePaths[j].Market]->bestBuyQuantity;
                            aggregateRatioAfterMaxVolume = 1 * markets[arbitragePaths[j].Market]->bestBuyPrice;
                            totalFees = 0;
                        }
                        else if (aggregateRatioAfterMaxVolume != 0)
                        {
                            aggregateRatioAfterMaxVolume = aggregateRatioAfterMaxVolume * markets[arbitragePaths[j].Market]->bestBuyPrice;
                        }

                        possession = possession * markets[arbitragePaths[j].Market]->bestBuyPrice;
                        arbitragePaths[j].Price = markets[arbitragePaths[j].Market]->bestBuyPrice;

                        double feesStep = (possession * fees.seller) + (possession * fees.taker);
                        arbitragePaths[j].Fees = feesStep;
                        totalFees = (totalFees * markets[arbitragePaths[j].Market]->bestBuyPrice) + feesStep;
                    }
                    result = possession;
                    arbitragePaths[j].Possession = possession;
                    cout << "market  : " << arbitragePaths[j].Market;
                    cout << " | price : " << arbitragePaths[j].Price << endl;
                    cout << " | quantity : " << arbitragePaths[j].Possession << endl;
                }
                else
                {
                    if (j == 0)
                    {
                        return;
                    }
                    break;
                }
            }

            if (maxVolumeIndex >= 0)
            {

                // re calcultate up this position

                for (int i = maxVolumeIndex; i > 0; i--)
                {

                    arbitragePaths[i - 1].Possession = maxVolume;
                    if (arbitragePaths[i - 1].Action == 1)
                    { // sell
                        maxVolume = maxVolume * markets[arbitragePaths[i - 1].Market]->bestSellPrice;
                        // calculate aggregate
                        aggregateRatioAfterMaxVolume = aggregateRatioAfterMaxVolume / markets[arbitragePaths[i - 1].Market]->bestSellPrice;
                        // fees

                        // totalFees -= arbitragePaths[i - 1].Fees * aggregateRatioAfterMaxVolume;
                        double feesStep = (maxVolume * fees.buyer) + (maxVolume * fees.taker);
                        arbitragePaths[i - 1].Fees = feesStep;
                        totalFees += feesStep * aggregateRatioAfterMaxVolume;
                    }
                    else
                    { // buy
                        maxVolume = maxVolume / markets[arbitragePaths[i - 1].Market]->bestBuyPrice;
                        // calculate aggregate
                        aggregateRatioAfterMaxVolume = aggregateRatioAfterMaxVolume * markets[arbitragePaths[i - 1].Market]->bestBuyPrice;
                        double feesStep = (maxVolume * fees.seller) + (maxVolume * fees.taker);
                        arbitragePaths[i - 1].Fees = feesStep;
                        totalFees += feesStep * aggregateRatioAfterMaxVolume;
                    }
                }
                spent = maxVolume;
            }

            // unlock all mutexes
            for (const auto &[symb, value] : mutexes)
            {
                if (value)
                {
                    markets[symb]->mtx.unlock();
                }
                mutexes[symb] = false;
            }
            mutexes.empty();

            // take actions
            cout << "totalFees : " << totalFees << endl;
            cout << "result : " << result << endl;
            cout << "spent : " << spent << endl;
            double totalCost = totalFees + spent;
            if ((totalCost < result || true) && result > 10)
            {
                cout << "----- ARBITRAGE ----" << endl;
                cout << "investment : " << totalCost << endl;
                cout << "result : " << result << endl;
                cout << "difference : " << result - totalCost << endl;
                // bot.sendMessage(userId, "bijour");

                for (int j = 0; j < 5; j++)
                {

                    if (arbitragePaths[j].Action == 1)
                    {
                        platform->Buy(arbitragePaths[j].Market, arbitragePaths[j].Possession, arbitragePaths[j].Price);
                    }
                    else
                    {
                    }
                }
            }
        }

        // m_allDone.notify_all();
    }

    void waitEndOfProcessing()
    {
        std::unique_lock<decltype(m_mutex)> lk(m_mutex);
        m_allDone.wait(lk);
    }

private:
    array<ArbitragePath, 5> arbitragePaths;
    std::mutex m_mutex;
    tbb::concurrent_unordered_map<string, bool> mutexes;
    std::condition_variable m_allDone;
    Platform *platform;
    Markets &markets;
    Fee &fees;
    ControlBot &bot;
    std::int32_t userId;
};
