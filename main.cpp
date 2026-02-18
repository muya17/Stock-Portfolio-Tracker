#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include "stock.hpp"
#include "api.hpp"

using namespace std;

void displayMenu() {
    cout << "\n";
    cout << "==========================================\n";
    cout << "      Stock Portfolio Tracker\n";
    cout << "==========================================\n";
    cout << "1. Buy Stock\n";
    cout << "2. Sell Stock\n";
    cout << "3. View Portfolio\n";
    cout << "4. View Transaction History\n";
    cout << "5. Exit\n";
    cout << "==========================================\n";
    cout << "Enter choice: ";
}

int main() {
    // Load transactions and aggregate portfolio
    vector<Transaction> transactions = loadTransactions();
    vector<Stock> portfolio = aggregatePortfolio(transactions);
    
    cout << "Loaded " << transactions.size() << " transactions\n";
    cout << "Current holdings: " << portfolio.size() << " unique symbols\n";

    while (true) {
        displayMenu();
        int choice;
        cin >> choice;
        cin.ignore(); // Clear newline

        if (choice == 1) { // Buy Stock
            string symbol;
            int quantity;
            double price;

            cout << "\n=== BUY STOCK ===\n";
            cout << "Enter stock symbol (e.g., AAPL): ";
            getline(cin, symbol);
            
            // Validate and sanitize stock symbol
            if (symbol.empty() || symbol.length() > 5) {
                cout << "Invalid symbol. Must be 1-5 characters.\n";
                continue;
            }
            
            // Convert to uppercase
            for (char& c : symbol) {
                c = toupper(c);
            }
            
            // Check if symbol contains only letters
            bool validSymbol = true;
            for (char c : symbol) {
                if (!isalpha(c)) {
                    validSymbol = false;
                    break;
                }
            }
            if (!validSymbol) {
                cout << "Invalid symbol. Must contain only letters.\n";
                continue;
            }
            
            cout << "Enter quantity: ";
            cin >> quantity;
            cout << "Enter purchase price: $";
            cin >> price;
            cin.ignore();

            if (quantity > 0 && price > 0) {
                logTransaction("BUY", symbol, quantity, price);
                cout << "✓ Stock purchased successfully!\n";
                
                // Reload portfolio
                transactions = loadTransactions();
                portfolio = aggregatePortfolio(transactions);
            } else {
                cout << "Invalid input. Quantity and price must be positive.\n";
            }
            
        } else if (choice == 2) { // Sell Stock
            string symbol;
            int quantity;
            double price;

            cout << "\n=== SELL STOCK ===\n";
            cout << "Enter stock symbol (e.g., AAPL): ";
            getline(cin, symbol);
            
            // Convert to uppercase
            for (char& c : symbol) {
                c = toupper(c);
            }
            
            cout << "Enter quantity: ";
            cin >> quantity;
            cout << "Enter sale price: $";
            cin >> price;
            cin.ignore();

            if (quantity > 0 && price > 0) {
                // Check if user has enough shares
                bool found = false;
                for (const auto& stock : portfolio) {
                    if (stock.symbol == symbol) {
                        if (stock.quantity >= quantity) {
                            logTransaction("SELL", symbol, quantity, price);
                            cout << "✓ Stock sold successfully!\n";
                            
                            // Reload portfolio
                            transactions = loadTransactions();
                            portfolio = aggregatePortfolio(transactions);
                            found = true;
                        } else {
                            cout << "Error: You only have " << stock.quantity << " shares of " << symbol << "\n";
                            found = true;
                        }
                        break;
                    }
                }
                if (!found) {
                    cout << "Error: You don't own any shares of " << symbol << "\n";
                }
            } else {
                cout << "Invalid input. Quantity and price must be positive.\n";
            }
            
        } else if (choice == 3) { // View Portfolio
            portfolio = aggregatePortfolio(transactions);
            displayPortfolio(portfolio);
            
        } else if (choice == 4) { // View Transaction History
            if (transactions.empty()) {
                cout << "\nNo transactions yet.\n";
            } else {
                cout << "\n" << string(75, '=') << "\n";
                cout << "                    TRANSACTION HISTORY\n";
                cout << string(75, '=') << "\n";
                cout << left << setw(12) << "Date" 
                     << setw(8) << "Action"
                     << setw(8) << "Symbol"
                     << right << setw(10) << "Quantity"
                     << setw(15) << "Price"
                     << setw(18) << "Total" << "\n";
                cout << string(75, '-') << "\n";
                
                for (const auto& txn : transactions) {
                    double total = txn.quantity * txn.price;
                    cout << left << setw(12) << txn.date
                         << setw(8) << txn.action
                         << setw(8) << txn.symbol
                         << right << setw(10) << txn.quantity
                         << setw(15) << fixed << setprecision(2) << txn.price
                         << setw(18) << total << "\n";
                }
                cout << string(75, '=') << "\n";
            }
            
        } else if (choice == 5) { // Exit
            cout << "\nAll transactions saved. Exiting...\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}