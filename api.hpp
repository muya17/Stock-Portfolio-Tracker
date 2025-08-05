#ifndef API_HPP
#define API_HPP

#include <string>
#include <ctime>
#include <map>
#include <iostream>
#include <cstdlib>
#include "http.hpp"

using namespace std;

const string API_KEY = []() {
    const char* key = std::getenv("ALPHA_VANTAGE_API_KEY");
    if (key == nullptr) {
        cout << "Error: ALPHA_VANTAGE_API_KEY environment variable not set." << endl;
        return string("");
    }
    return string(key);
}();
const int CACHE_DURATION = 15 * 60; // 15 minutes in seconds
map<string, pair<double, time_t>> priceCache; // Cache for stock prices

double parseStockPrice(const string& json) {
    cout << "API Response: " << json << endl; // Debug: Print raw response
    string priceKey = "\"05. price\": \"";
    size_t start = json.find(priceKey);
    if (start == string::npos) {
        cout << "Error: Price key not found in response" << endl;
        return -1.0;
    }
    start += priceKey.length();
    size_t end = json.find("\"", start);
    if (end == string::npos) {
        cout << "Error: Invalid price format in response" << endl;
        return -1.0;
    }
    try {
        return stod(json.substr(start, end - start));
    } catch (...) {
        cout << "Error: Failed to parse price" << endl;
        return -1.0;
    }
}

double getStockPrice(const string& symbol, time_t& lastFetched) {
    time_t now = time(nullptr);
    auto cached = priceCache.find(symbol);
    
    // Check cache
    if (cached != priceCache.end() && (now - cached->second.second) < CACHE_DURATION) {
        lastFetched = cached->second.second;
        return cached->second.first;
    }

    // Check if API key is valid
    if (API_KEY.empty()) {
        cout << "Error: Cannot fetch price, API key is missing" << endl;
        return -1.0;
    }

    // Fetch from API
    string url = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + symbol + "&apikey=" + API_KEY;
    cout << "Fetching URL: " << url << endl; // Debug: Print URL
    string response = httpGet(url);
    if (response.empty()) {
        cout << "Error: Empty response from API" << endl;
        return -1.0;
    }
    double price = parseStockPrice(response);

    if (price > 0) {
        priceCache[symbol] = {price, now};
        lastFetched = now;
    }
    return price;
}

#endif