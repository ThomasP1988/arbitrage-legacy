#include <cstdlib>
#include <string>
#include "platforms/binance.h"
#include "binacpp/binacpp.h"
#include "binacpp/binacpp_utils.h"
#include "rapidjson/document.h"
#include "utils/double.h"
#include "utils/time.h"
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <daw/json/daw_json_link.h>
#include <atomic>
#include <curl/curl.h>

using namespace rapidjson;
using namespace std;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

string BinancePlatform::apiKey = "";
string BinancePlatform::secretKey = "";
CURL *BinancePlatform::curl = curl_easy_init();

BinancePlatform::BinancePlatform()
{
    BinaCPP::init(secretKey, apiKey);
    this->host = BINANCE_HOST;
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

void BinancePlatform::ListMarkets(Markets &markets)
{
    Document marketsJson;
    BinaCPP::get_exchangeInfo(marketsJson);
    const Value &symbols = marketsJson["symbols"];
    assert(symbols.IsArray()); // attributes is an array
    for (Value::ConstValueIterator itr = symbols.Begin(); itr != symbols.End(); ++itr)
    {
        const Value &symbol = *itr;
        assert(symbol.IsObject()); // each attribute is an object
        MarketState *mstate = new MarketState();
        mstate->bestBuyPrice = 0;
        mstate->bestSellPrice = 0;
        mstate->paths = {{Path{
            EMPTY,
            EMPTY,
            EMPTY,
            EMPTY,
            EMPTY,
            2,
            false}}};
        for (Value::ConstMemberIterator itr2 = symbol.MemberBegin(); itr2 != symbol.MemberEnd(); ++itr2)
        {
            if (itr2->value.IsString())
            {
                if (strcmp(itr2->name.GetString(), "symbol") == 0)
                {
                    mstate->market = itr2->value.GetString();
                }
                if (strcmp(itr2->name.GetString(), "baseAsset") == 0)
                {
                    mstate->base = itr2->value.GetString();
                }
                if (strcmp(itr2->name.GetString(), "quoteAsset") == 0)
                {
                    mstate->quote = itr2->value.GetString();
                }
            }
        }
        auto keySymbol = (string(mstate->base) + string(mstate->quote));
        markets[keySymbol] = mstate;
    }
}

void BinancePlatform::SetAllMarketsBestPrices(Markets &markets)
{
    Document marketsPricesJson;
    BinaCPP::get_allBookTickers(marketsPricesJson);
    for (Value::ConstValueIterator itr = marketsPricesJson.Begin(); itr != marketsPricesJson.End(); ++itr)
    {

        const Value &bookTickerJson = *itr;
        assert(bookTickerJson.IsObject()); // each attribute is an object
        string symbol = "";
        for (Value::ConstMemberIterator itr2 = bookTickerJson.MemberBegin(); itr2 != bookTickerJson.MemberEnd(); ++itr2)
        {
            if (itr2->value.IsString())
            {
                if (strcmp(itr2->name.GetString(), "symbol") == 0)
                {
                    symbol = itr2->value.GetString();
                }
                if (strcmp(itr2->name.GetString(), "bidPrice") == 0)
                {
                    markets[symbol]->bestBuyPrice = stod(itr2->value.GetString());
                }
                if (strcmp(itr2->name.GetString(), "bidQty") == 0)
                {
                    markets[symbol]->bestBuyQuantity = stod(itr2->value.GetString());
                }
                if (strcmp(itr2->name.GetString(), "askPrice") == 0)
                {
                    markets[symbol]->bestSellPrice = stod(itr2->value.GetString());
                }
                if (strcmp(itr2->name.GetString(), "askQty") == 0)
                {
                    markets[symbol]->bestSellQuantity = stod(itr2->value.GetString());
                }
            }
        }
    }
}

void BinancePlatform::GetAccount(Fee &fees)
{
    Document account;
    BinaCPP::get_account(5000, account);

    fees.taker = 7.5 * 0.01;

    for (Value::ConstMemberIterator itr2 = account.MemberBegin(); itr2 != account.MemberEnd(); ++itr2)
    {
        if (itr2->value.IsInt())
        {
            if (strcmp(itr2->name.GetString(), "makerCommission") == 0)
            {
                fees.maker = double(itr2->value.GetInt()) / 1000;
            }
            if (strcmp(itr2->name.GetString(), "takerCommission") == 0)
            {
                fees.taker = double(itr2->value.GetInt()) / 1000;
            }
            if (strcmp(itr2->name.GetString(), "buyerCommission") == 0)
            {
                fees.buyer = double(itr2->value.GetInt()) / 1000;
            }
            if (strcmp(itr2->name.GetString(), "sellerCommission") == 0)
            {
                fees.seller = double(itr2->value.GetInt()) / 1000;
            }
        }
    }
}

// std::function<void()> BinancePlatform::WsBookTicker(CallbackFunction cb)
void BinancePlatform::WsBookTicker(CallbackFunction cb, std::function<void()> &stop)
{
    try
    {
        cout << "4" << endl;
        // Check command line arguments.
        auto const host = "stream.binance.com";
        auto const port = "9443";
        auto const rpcJson = R"({ "method": "SUBSCRIBE", "params": ["!bookTicker"], "id": 1 })";

        // The io_context is required for all I/O
        net::io_context ioc;

        // The SSL context is required, and holds certificates
        ssl::context ctx{ssl::context::tlsv12_client};

        // This holds the root certificate used for verification
        //load_root_certificates(ctx);

        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        net::connect(ws.next_layer().next_layer(), results.begin(), results.end());

        // Perform the SSL handshake
        ws.next_layer().handshake(ssl::stream_base::client);

        // Set a decorator to change the User-Agent of the handshake
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::request_type &req) {
                req.set(http::field::user_agent,
                        std::string(BOOST_BEAST_VERSION_STRING) +
                            " websocket-client-coro");
            }));

        // Perform the websocket handshake
        ws.handshake(host, "/ws");

        // Our message in this case should be stringified JSON-RPC request
        ws.write(net::buffer(std::string(rpcJson)));

        // This buffer will hold the incoming message
        beast::flat_buffer buffer;
        boost::beast::error_code ec;
        std::atomic_flag read = ATOMIC_FLAG_INIT;

        stop = [&ws, &read]() {
            cout << "stop binance" << endl;
            read.clear(std::memory_order_release);
        };

        read.test_and_set(std::memory_order_acquire);
        // Read a message into our buffer
        while (read.test_and_set(std::memory_order_acquire))
        {
            // The make_printable() function helps print a ConstBufferSequence
            // auto len = ws.read(buffer, ec);
            ws.read(buffer, ec);

            if (ec == websocket::error::closed)
            {
                break;
            }

            string result = std::string(boost::asio::buffer_cast<char const *>(
                                            beast::buffers_front(buffer.data())),
                                        boost::asio::buffer_size(buffer.data()));

            // do something with the frame I just received
            try
            {

                cb(daw::json::from_json<BookTicker>(result));
            }
            catch (daw::json::json_exception const &je)
            {
                cout << result << endl;
                cout << je.reason() << endl;
            }
            buffer.consume(buffer.size()); // or buffer.consume(dbuf.size()); clear the websocket buffer
        }
        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void BinancePlatform::Buy(string symbol, double quantity, double price)
{

    string data = "symbol=" + symbol + "&newOrderRespType=RESULT&side=BUY&type=LIMIT&timeInForce=IOC&quantity=" + to_string(quantity) + "&price=" + to_string(price) + "&timestamp=" + getTimestampStr();
    string signature = hmac_sha256(secretKey.c_str(), data.c_str());
    data.append("&signature=" + signature);
    cout << "secretKey : " << secretKey << endl;
    cout << "data : " << data << endl;
    string str_result;
    curl_easy_setopt(BinancePlatform::curl, CURLOPT_URL, "https://api.binance.com/api/v3/order/test?");
    curl_easy_setopt(BinancePlatform::curl, CURLOPT_WRITEFUNCTION, BinaCPP::curl_cb);
    curl_easy_setopt(BinancePlatform::curl, CURLOPT_WRITEDATA, &str_result);
    curl_easy_setopt(BinancePlatform::curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(BinancePlatform::curl, CURLOPT_ENCODING, "gzip");
    curl_easy_setopt(BinancePlatform::curl, CURLOPT_POSTFIELDS, data.c_str());

    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, ("X-MBX-APIKEY: " + apiKey).c_str());
    curl_easy_setopt(BinancePlatform::curl, CURLOPT_HTTPHEADER, chunk);

    CURLcode res;
    res = curl_easy_perform(BinancePlatform::curl);
    cout << "res" << res << endl;
    cout << "str_result" << str_result << endl;
}
