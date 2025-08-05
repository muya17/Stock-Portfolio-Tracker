#ifndef STOCK_HPP
#define STOCK_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "api.hpp"

using namespace std;

// Forward declaration to avoid include issues
double getStockPrice(const string& symbol, time_t& lastFetched);

class Stock {
public:
    string symbol;
    int quantity;
    double purchasePrice;
    double currentPrice;
    time_t lastFetched;

    Stock(const string& sym, int qty, double price)
        : symbol(sym), quantity(qty), purchasePrice(price), currentPrice(0.0), lastFetched(0) {}
};

void savePortfolio(const vector<Stock>& portfolio) {
    ofstream file("portfolio.csv");
    if (!file.is_open()) {
        cout << "Error: Cannot open portfolio.csv for writing. Check permissions." << endl;
        return;
    }
    cout << "Saving portfolio to portfolio.csv" << endl;
    file << "Symbol,Quantity,PurchasePrice\n";
    for (const auto& stock : portfolio) {
        file << stock.symbol << "," << stock.quantity << "," << stock.purchasePrice << "\n";
        cout << "Wrote: " << stock.symbol << "," << stock.quantity << "," << stock.purchasePrice << endl;
    }
    file.close();
    cout << "Portfolio saved successfully" << endl;
}

void loadPortfolio(vector<Stock>& portfolio) {
    ifstream file("portfolio.csv");
    if (!file.is_open()) {
        cout << "No portfolio.csv found, starting with empty portfolio" << endl;
        return;
    }
    cout << "Loading portfolio from portfolio.csv" << endl;
    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string symbol;
        int quantity;
        double purchasePrice;
        string temp;

        getline(ss, symbol, ',');
        getline(ss, temp, ',');
        quantity = stoi(temp);
        getline(ss, temp);
        purchasePrice = stod(temp);

        portfolio.emplace_back(symbol, quantity, purchasePrice);
        cout << "Loaded: " << symbol << "," << quantity << "," << purchasePrice << endl;
    }
    file.close();
}

void displayPortfolio(vector<Stock>& portfolio) {
    double totalValue = 0.0;
    double totalGainLoss = 0.0;

    cout << "\nPortfolio:\n";
    cout << "Symbol\tQuantity\tPurchase Price\tCurrent Price\tGain/Loss\n";
    for (auto& stock : portfolio) {
        double price = getStockPrice(stock.symbol, stock.lastFetched);
        if (price > 0) {
            stock.currentPrice = price;
            double gainLoss = (price - stock.purchasePrice) * stock.quantity;
            totalValue += price * stock.quantity;
            totalGainLoss += gainLoss;

            cout << stock.symbol << "\t" << stock.quantity << "\t\t" << stock.purchasePrice
                 << "\t\t" << price << "\t\t" << gainLoss << "\n";
        } else {
            cout << stock.symbol << "\tError fetching price\n";
        }
    }
    cout << "\nTotal Portfolio Value: $" << totalValue << "\n";
    cout << "Total Gain/Loss: $" << totalGainLoss << "\n";
}

#endif