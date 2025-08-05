#include <iostream>
#include <string>
#include <vector>
#include "stock.hpp"
#include "api.hpp"

using namespace std;

void displayMenu() {
    cout << "\nStock Portfolio Tracker\n";
    cout << "1. Add Stock\n";
    cout << "2. View Portfolio\n";
    cout << "3. Exit\n";
    cout << "Enter choice: ";
}

int main() {
    vector<Stock> portfolio;
    loadPortfolio(portfolio); // Load from CSV

    while (true) {
        displayMenu();
        int choice;
        cin >> choice;
        cin.ignore(); // Clear newline

        if (choice == 1) {
            string symbol;
            int quantity;
            double purchasePrice;

            cout << "Enter stock symbol (e.g., AAPL): ";
            getline(cin, symbol);
            cout << "Enter quantity: ";
            cin >> quantity;
            cout << "Enter purchase price: ";
            cin >> purchasePrice;
            cin.ignore();

            if (quantity > 0 && purchasePrice >= 0) {
                portfolio.emplace_back(symbol, quantity, purchasePrice);
                cout << "Stock added successfully!\n";
            } else {
                cout << "Invalid input. Quantity and price must be non-negative.\n";
            }
        } else if (choice == 2) {
            if (portfolio.empty()) {
                cout << "Portfolio is empty.\n";
            } else {
                displayPortfolio(portfolio);
            }
        } else if (choice == 3) {
            savePortfolio(portfolio); // Save to CSV
            cout << "Portfolio saved. Exiting...\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}