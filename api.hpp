#ifndef API_HPP
#define API_HPP

#include <string>
#include <cstring>
#include <ctime>
#include <map>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include "http.hpp"

using namespace std;

// Get home directory
inline string getHomeDirectory() {
    const char* homeDir = getenv("HOME");
    if (homeDir == nullptr) {
        struct passwd* pw = getpwuid(getuid());
        if (pw != nullptr) {
            homeDir = pw->pw_dir;
        }
    }
    return homeDir ? string(homeDir) : ".";
}

// Load API key from multiple sources
const string API_KEY = []() {
    // 1. Try environment variable first
    const char* envKey = std::getenv("ALPHA_VANTAGE_API_KEY");
    if (envKey != nullptr && strlen(envKey) > 0) {
        return string(envKey);
    }
    
    // 2. Try config file in ~/.stock-tracker/config.txt
    string homeDir = getHomeDirectory();
    string configPath = homeDir + "/.stock-tracker/config.txt";
    ifstream configFile(configPath);
    if (configFile.is_open()) {
        string line;
        while (getline(configFile, line)) {
            // Skip comments and empty lines
            if (line.empty() || line[0] == '#') continue;
            
            // Look for API_KEY=value
            size_t pos = line.find("API_KEY=");
            if (pos != string::npos) {
                string key = line.substr(pos + 8);
                // Trim whitespace
                key.erase(0, key.find_first_not_of(" \t\r\n"));
                key.erase(key.find_last_not_of(" \t\r\n") + 1);
                if (!key.empty()) {
                    configFile.close();
                    return key;
                }
            }
        }
        configFile.close();
    }
    
    // 3. Try config file in current directory
    configFile.open("config.txt");
    if (configFile.is_open()) {
        string line;
        while (getline(configFile, line)) {
            if (line.empty() || line[0] == '#') continue;
            size_t pos = line.find("API_KEY=");
            if (pos != string::npos) {
                string key = line.substr(pos + 8);
                key.erase(0, key.find_first_not_of(" \t\r\n"));
                key.erase(key.find_last_not_of(" \t\r\n") + 1);
                if (!key.empty()) {
                    configFile.close();
                    return key;
                }
            }
        }
        configFile.close();
    }
    
    // No API key found - app can still work without it
    return string("");
}();
const int CACHE_DURATION = 15 * 60; // 15 minutes in seconds
inline map<string, pair<double, time_t>>& getPriceCache() {
    static map<string, pair<double, time_t>> priceCache;
    return priceCache;
}

inline double parseStockPrice(const string& json) {
    cout << "API Response: " << json << endl; // Debug: Print raw response
    
    // Check for API error responses
    if (json.find("Error Message") != string::npos) {
        cout << "Error: API returned error message" << endl;
        return -1.0;
    }
    if (json.find("Invalid API call") != string::npos) {
        cout << "Error: Invalid API call" << endl;
        return -1.0;
    }
    if (json.find("Information") != string::npos && json.find("rate limit") != string::npos) {
        cout << "Error: API rate limit exceeded. Please wait before requesting again." << endl;
        return -1.0;
    }
    
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
        double price = stod(json.substr(start, end - start));
        if (price <= 0) {
            cout << "Error: Invalid price value (must be positive)" << endl;
            return -1.0;
        }
        return price;
    } catch (const exception& e) {
        cout << "Error: Failed to parse price - " << e.what() << endl;
        return -1.0;
    }
}

inline double getStockPrice(const string& symbol, time_t& lastFetched) {
    auto& priceCache = getPriceCache();
    time_t now = time(nullptr);
    auto cached = priceCache.find(symbol);
    
    // Check cache
    if (cached != priceCache.end() && (now - cached->second.second) < CACHE_DURATION) {
        lastFetched = cached->second.second;
        return cached->second.first;
    }

    // Check if API key is valid - show warning only once
    if (API_KEY.empty()) {
        static bool warningShown = false;
        if (!warningShown) {
            cout << "Warning: API key not configured. Live prices unavailable." << endl;
            cout << "To enable live prices, set API_KEY in ~/.stock-tracker/config.txt" << endl;
            warningShown = true;
        }
        return -1.0;
    }

    // Fetch from API
    string url = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + symbol + "&apikey=" + API_KEY;
    cout << "Fetching price for " << symbol << "..." << endl;
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