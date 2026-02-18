# Stock Portfolio Tracker

A C++ console application to manage a stock portfolio with transaction logging, real-time stock prices via Alpha Vantage API, and comprehensive portfolio analytics.

## Features
- **Buy/Sell Tracking**: Log all stock transactions with dates
- **Transaction History**: View complete history of all trades
- **Portfolio Aggregation**: Automatic calculation of weighted average purchase prices
- **Real-time Prices**: Fetch current stock prices from Alpha Vantage API
- **Performance Metrics**: Track total value, profit/loss, and return percentage
- **Price Caching**: 15-minute cache to reduce API calls and avoid rate limits
- **Two-CSV System**: 
  - `transactions.csv` - Complete transaction log (buy/sell history)
  - Portfolio dynamically aggregated from transactions

## Prerequisites
- **C++ Compiler**: GCC with C++17 support
- **libcurl**: For HTTP requests to Alpha Vantage API
- **Alpha Vantage API Key**: Get a free key from https://www.alphavantage.co/support/#api-key

## Setup (WSL/Linux)

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/muya17/Stock-Portfolio-Tracker.git
   cd Stock-Portfolio-Tracker
   ```

2. **Install Dependencies**:
   ```bash
   # Option 1: Use the provided setup script
   chmod +x setup.sh
   ./setup.sh
   
   # Option 2: Manual installation
   sudo apt update
   sudo apt install -y build-essential libcurl4-openssl-dev
   ```

3. **Set API Key**:
   ```bash
   # Add to ~/.bashrc for persistence
   echo 'export ALPHA_VANTAGE_API_KEY="your-api-key-here"' >> ~/.bashrc
   source ~/.bashrc
   
   # Or set for current session only
   export ALPHA_VANTAGE_API_KEY="your-api-key-here"
   
   # Verify
   echo $ALPHA_VANTAGE_API_KEY
   ```

4. **Compile and Run**:
   ```bash
   # Compile
   make
   
   # Run the program
   ./portfolio_tracker
   
   # Or compile and run in one step
   make run
   
   # Clean build artifacts
   make clean
   ```

## Usage

### Main Menu:
1. **Buy Stock** - Log a stock purchase (symbol, quantity, price)
2. **Sell Stock** - Log a stock sale (validates you own enough shares)
3. **View Portfolio** - See aggregated holdings with live prices and analytics
4. **View Transaction History** - Complete log of all buy/sell transactions
5. **Exit** - Close the program (transactions auto-saved)

### Example Session:
```
==========================================
      Stock Portfolio Tracker
==========================================
1. Buy Stock
2. Sell Stock
3. View Portfolio
4. View Transaction History
5. Exit
==========================================
Enter choice: 1

=== BUY STOCK ===
Enter stock symbol (e.g., AAPL): AAPL
Enter quantity: 100
Enter purchase price: $150.50
Transaction logged: BUY 100 AAPL @ $150.5
✓ Stock purchased successfully!
```

### Portfolio View Example:
```
=====================================================================================
                          STOCK PORTFOLIO
=====================================================================================
Symbol    Quantity      Avg Price        Current              Value         Gain/Loss
-------------------------------------------------------------------------------------
AAPL           100         150.50         165.25          16525.00           +1475.00
TSLA            50         200.00         185.50           9275.00            -725.00
GOOGL           30         120.00         140.75           4222.50            +622.50
=====================================================================================
TOTAL PORTFOLIO VALUE:                                    $30022.50
TOTAL COST BASIS:                                         $28650.00
TOTAL GAIN/LOSS:                                           +$1372.50
RETURN:                                                        +4.79%
=====================================================================================
```

## Files
- **main.cpp**: Main program logic and menu system
- **stock.hpp**: Transaction logging, portfolio aggregation, and display
- **api.hpp**: Stock price fetching via Alpha Vantage API with caching
- **http.hpp**: HTTP requests using libcurl
- **Makefile**: Build automation for WSL/Linux
- **setup.sh**: Dependency checker and setup helper
- **transactions.csv**: Transaction log (auto-generated, not tracked in Git)

## How It Works

### Two-CSV System:
1. **transactions.csv** stores every buy/sell action:
   ```
   Date,Action,Symbol,Quantity,Price
   2026-02-19,BUY,AAPL,100,150.50
   2026-02-19,BUY,TSLA,50,200.00
   2026-02-20,SELL,AAPL,20,165.00
   ```

2. **Portfolio is dynamically aggregated** from transactions:
   - Combines duplicate symbols
   - Calculates weighted average purchase price
   - Tracks net position (buys - sells)

### Live Price Integration:
- Fetches current prices from Alpha Vantage API
- Caches prices for 15 minutes to respect rate limits
- Calculates gain/loss and return percentage

## Notes
- ✅ **Status**: Fully working on WSL/Linux
- Free Alpha Vantage accounts have rate limits: 5 calls/minute, 25/day
- The 15-minute cache minimizes API calls
- Transaction history provides complete audit trail
- Weighted average cost basis accounts for multiple purchases

## Contributing
Contributions and suggestions are welcome via GitHub issues or pull requests!

## License
This project is licensed under the MIT License. See the LICENSE file for details.
