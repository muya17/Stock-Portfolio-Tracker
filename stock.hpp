#ifndef STOCK_HPP
#define STOCK_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <ctime>
#include "api.hpp"

using namespace std;

// Forward declaration to avoid include issues
double getStockPrice(const string& symbol, time_t& lastFetched);

// Transaction class for logging buy/sell actions
class Transaction {
public:
    string date;
    string action; // "BUY" or "SELL"
    string symbol;
    int quantity;
    double price;

    Transaction(const string& d, const string& a, const string& sym, int qty, double p)
        : date(d), action(a), symbol(sym), quantity(qty), price(p) {}
};

// Stock class for aggregated portfolio
class Stock {
public:
    string symbol;
    int quantity;
    double avgPurchasePrice;
    double currentPrice;
    time_t lastFetched;

    Stock(const string& sym, int qty, double avgPrice)
        : symbol(sym), quantity(qty), avgPurchasePrice(avgPrice), currentPrice(0.0), lastFetched(0) {}
};

// Get current date as string
string getCurrentDate() {
    time_t now = time(nullptr);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&now));
    return string(buffer);
}

// Log a transaction to transactions.csv
void logTransaction(const string& action, const string& symbol, int quantity, double price) {
    ofstream file("transactions.csv", ios::app);
    if (!file.is_open()) {
        // Create new file with header
        file.open("transactions.csv");
        if (!file.is_open()) {
            cout << "Error: Cannot create transactions.csv" << endl;
            return;
        }
        file << "Date,Action,Symbol,Quantity,Price\n";
    }
    
    string date = getCurrentDate();
    file << date << "," << action << "," << symbol << "," << quantity << "," << fixed << setprecision(2) << price << "\n";
    file.close();
    cout << "Transaction logged: " << action << " " << quantity << " " << symbol << " @ $" << price << endl;
}

// Load all transactions
vector<Transaction> loadTransactions() {
    vector<Transaction> transactions;
    ifstream file("transactions.csv");
    
    if (!file.is_open()) {
        cout << "No transactions.csv found, starting fresh" << endl;
        // Create the file with header
        ofstream newFile("transactions.csv");
        newFile << "Date,Action,Symbol,Quantity,Price\n";
        newFile.close();
        return transactions;
    }
    
    string line;
    getline(file, line); // Skip header
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string date, action, symbol, qtyStr, priceStr;
        
        try {
            getline(ss, date, ',');
            getline(ss, action, ',');
            getline(ss, symbol, ',');
            getline(ss, qtyStr, ',');
            getline(ss, priceStr);
            
            int quantity = stoi(qtyStr);
            double price = stod(priceStr);
            
            transactions.emplace_back(date, action, symbol, quantity, price);
        } catch (const exception& e) {
            cout << "Warning: Skipping invalid transaction line: " << line << endl;
        }
    }
    file.close();
    return transactions;
}

// Aggregate transactions into portfolio holdings
vector<Stock> aggregatePortfolio(const vector<Transaction>& transactions) {
    map<string, pair<int, double>> holdings; // symbol -> (quantity, total cost)
    
    for (const auto& txn : transactions) {
        if (txn.action == "BUY") {
            holdings[txn.symbol].first += txn.quantity;
            holdings[txn.symbol].second += txn.quantity * txn.price;
        } else if (txn.action == "SELL") {
            holdings[txn.symbol].first -= txn.quantity;
            // Note: Selling doesn't affect total cost basis proportionally
            // We keep original cost basis for remaining shares
        }
    }
    
    vector<Stock> portfolio;
    for (const auto& [symbol, data] : holdings) {
        int quantity = data.first;
        double totalCost = data.second;
        
        if (quantity > 0) {
            double avgPrice = totalCost / quantity;
            portfolio.emplace_back(symbol, quantity, avgPrice);
        }
    }
    
    return portfolio;
}

// Display portfolio with proper formatting
void displayPortfolio(vector<Stock>& portfolio) {
    if (portfolio.empty()) {
        cout << "\nPortfolio is empty. Buy some stocks first!\n";
        return;
    }
    
    double totalValue = 0.0;
    double totalCost = 0.0;
    double totalGainLoss = 0.0;

    cout << "\n" << string(85, '=') << "\n";
    cout << "                          STOCK PORTFOLIO\n";
    cout << string(85, '=') << "\n";
    cout << left << setw(8) << "Symbol" 
         << right << setw(10) << "Quantity" 
         << setw(15) << "Avg Price" 
         << setw(15) << "Current" 
         << setw(18) << "Value"
         << setw(18) << "Gain/Loss" << "\n";
    cout << string(85, '-') << "\n";
    
    for (auto& stock : portfolio) {
        double price = getStockPrice(stock.symbol, stock.lastFetched);
        if (price > 0) {
            stock.currentPrice = price;
            double value = price * stock.quantity;
            double cost = stock.avgPurchasePrice * stock.quantity;
            double gainLoss = value - cost;
            
            totalValue += value;
            totalCost += cost;
            totalGainLoss += gainLoss;

            cout << left << setw(8) << stock.symbol
                 << right << setw(10) << stock.quantity
                 << setw(15) << fixed << setprecision(2) << stock.avgPurchasePrice
                 << setw(15) << price
                 << setw(18) << value
                 << setw(18) << (gainLoss >= 0 ? "+" : "") << gainLoss << "\n";
        } else {
            cout << left << setw(8) << stock.symbol 
                 << right << setw(10) << stock.quantity
                 << "  (Error fetching price)\n";
        }
    }
    
    cout << string(85, '=') << "\n";
    cout << left << setw(48) << "TOTAL PORTFOLIO VALUE:" 
         << right << setw(18) << "$" << fixed << setprecision(2) << totalValue << "\n";
    cout << left << setw(48) << "TOTAL COST BASIS:" 
         << right << setw(18) << "$" << totalCost << "\n";
    cout << left << setw(48) << "TOTAL GAIN/LOSS:" 
         << right << setw(18) << (totalGainLoss >= 0 ? "+$" : "-$") 
         << abs(totalGainLoss) << "\n";
    
    if (totalCost > 0) {
        double returnPct = (totalGainLoss / totalCost) * 100;
        cout << left << setw(48) << "RETURN:" 
             << right << setw(18) << (returnPct >= 0 ? "+" : "") 
             << fixed << setprecision(2) << returnPct << "%\n";
    }
    cout << string(85, '=') << "\n";
}

#endif