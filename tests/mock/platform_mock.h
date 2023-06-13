#pragma once
#include "gmock/gmock.h" 
#include "platform.h"
#include "shared/markets/state.h"

class MockPlatform : public Platform {
 public:
  MOCK_METHOD(void, ListMarkets, (Markets &markets), (override));
  MOCK_METHOD(void, SetAllMarketsBestPrices, (Markets &markets), (override));
  MOCK_METHOD(void, GetAccount, (Fee &fees), (override));
  MOCK_METHOD(void, WsBookTicker, (CallbackFunction cb,  std::function<void()> &stop), (override));
  MOCK_METHOD(void, Buy, (std::string symbol, double quantity, double price), (override));
};
