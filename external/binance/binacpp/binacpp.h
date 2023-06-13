
/*
	Author: tensaix2j
	Date  : 2017/10/15
	
	C++ library for Binance API.
*/

#ifndef BINACPP_H
#define BINACPP_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <exception>

#include <curl/curl.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define BINANCE_HOST "https://api.binance.com"


class BinaCPP
{
	static std::string api_key;
	static std::string secret_key;
	static CURL *curl;

public:
	static void curl_api(std::string &url, std::string &result_json);
	static void curl_api_with_header(std::string &url, std::string &result_json, std::vector<std::string> &extra_http_header, std::string &post_data, std::string &action);
	static size_t curl_cb(void *content, size_t size, size_t nmemb, std::string *buffer);

	static void init(std::string &api_key, std::string &secret_key);
	static void cleanup();

	// Public API
	static void get_exchangeInfo(rapidjson::Document &json_result);
	static void get_serverTime(rapidjson::Document &json_result);

	static void get_allPrices(rapidjson::Document &json_result);
	static double get_price(const char *symbol);

	static void get_allBookTickers(rapidjson::Document &json_result);
	static void get_bookTicker(const char *symbol, rapidjson::Value &json_result);

	static void get_depth(const char *symbol, int limit, rapidjson::Document &json_result);
	static void get_aggTrades(const char *symbol, int fromId, time_t startTime, time_t endTime, int limit, rapidjson::Document &json_result);
	static void get_24hr(const char *symbol, rapidjson::Document &json_result);
	static void get_klines(const char *symbol, const char *interval, int limit, time_t startTime, time_t endTime, rapidjson::Document &json_result);

	// API + Secret keys required
	static void get_account(long recvWindow, rapidjson::Document &json_result);

	static void get_myTrades(
		const char *symbol,
		int limit,
		long fromId,
		long recvWindow,
		rapidjson::Document &json_result);

	static void get_openOrders(
		const char *symbol,
		long recvWindow,
		rapidjson::Document &json_result);

	static void get_allOrders(
		const char *symbol,
		long orderId,
		int limit,
		long recvWindow,
		rapidjson::Document &json_result);

	static void send_order(
		const char *symbol,
		const char *side,
		const char *type,
		const char *timeInForce,
		double quantity,
		double price,
		const char *newClientOrderId,
		double stopPrice,
		double icebergQty,
		long recvWindow,
		rapidjson::Document &json_result);

	static void get_order(
		const char *symbol,
		long orderId,
		const char *origClientOrderId,
		long recvWindow,
		rapidjson::Document &json_result);

	static void cancel_order(
		const char *symbol,
		long orderId,
		const char *origClientOrderId,
		const char *newClientOrderId,
		long recvWindow,
		rapidjson::Document &json_result);

	// API key required
	static void start_userDataStream(rapidjson::Document &json_result);
	static void keep_userDataStream(const char *listenKey);
	static void close_userDataStream(const char *listenKey);

	// WAPI
	static void withdraw(
		const char *asset,
		const char *address,
		const char *addressTag,
		double amount,
		const char *name,
		long recvWindow,
		rapidjson::Document &json_result);

	static void get_depositHistory(
		const char *asset,
		int status,
		long startTime,
		long endTime,
		long recvWindow,
		rapidjson::Document &json_result);

	static void get_withdrawHistory(
		const char *asset,
		int status,
		long startTime,
		long endTime,
		long recvWindow,
		rapidjson::Document &json_result);

	static void get_depositAddress(
		const char *asset,
		long recvWindow,
		rapidjson::Document &json_result);
};

#endif
