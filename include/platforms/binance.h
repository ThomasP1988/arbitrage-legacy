#pragma once

#include <cstdlib>
#include <string>
#include "rapidjson/document.h"
#include "platform.h"
#include "shared/markets/state.h"
#include <daw/json/daw_json_link.h>
#include <curl/curl.h>

class BinancePlatform : public Platform
{       // The class
public: // Access specifier
  BinancePlatform();
  void ListMarkets(Markets &markets);
  void SetAllMarketsBestPrices(Markets &markets);
  void GetAccount(Fee &fees);
  void WsBookTicker(CallbackFunction cb, std::function<void()> &stop);
  void Buy(std::string symbol, double quantity, double price);
private:
  static std::string secretKey;
  static std::string apiKey;
  static CURL *curl;
  std::string url;
  std::string host;
};

struct MarketUpdate
{
  std::string type;     // e
  int time;             // E
  std::string symbol;   // s
  int tradeId;          // a
  std::string price;    // p
  std::string quantity; // q
  int firstTradeId;     // f
  int lastTradeId;      // l
  int timestamp;        // T
  int isMaker;          // m
};

struct BinanceBookTicker : BookTicker
{
};

namespace daw::json
{
  template <>
  struct json_data_contract<MarketUpdate>
  {
    static constexpr char const e[] = "e";
    static constexpr char const E[] = "E";
    static constexpr char const s[] = "s";
    static constexpr char const a[] = "a";
    static constexpr char const p[] = "p";
    static constexpr char const q[] = "q";
    static constexpr char const f[] = "f";
    static constexpr char const l[] = "l";
    static constexpr char const T[] = "T";
    static constexpr char const m[] = "m";

    using type = json_member_list<
        json_string<e>,
        json_number<E, int>,
        json_string<s>,
        json_number<a, int>,
        json_string<p>,
        json_string<q>,
        json_number<f, int>,
        json_number<l, int>,
        json_number<T, int>,
        json_number<m, int>>;
  };

  template <>
  struct json_data_contract<BookTicker>
  {
    static constexpr char const u[] = "u";
    static constexpr char const s[] = "s";
    static constexpr char const b[] = "b";
    static constexpr char const B[] = "B";
    static constexpr char const a[] = "a";
    static constexpr char const A[] = "A";

    using type = json_member_list<
        json_number<u, int>,
        json_string<s>,
        json_number<b, double, LiteralAsStringOpt::Always>,
        json_number<B, double, LiteralAsStringOpt::Always>,
        json_number<a, double, LiteralAsStringOpt::Always>,
        json_number<A, double, LiteralAsStringOpt::Always>>;
  };

} // namespace daw::json
