# 📈 Stock Portfolio Tracker

A C++ application to manage a stock portfolio with transaction logging, real-time stock prices via Alpha Vantage API, and comprehensive portfolio analytics. Available in both CLI and Qt GUI versions.

## ✨ Features

- **🖥️ Two Interface Options**: 
  - Command-line interface (CLI) for terminal users
  - Qt GUI with modern graphical interface
- **💰 Buy/Sell Tracking**: Log all stock transactions with dates
- **📜 Transaction History**: View complete history of all trades
- **📊 Portfolio Aggregation**: Automatic calculation of weighted average purchase prices
- **🔄 Real-time Prices**: Fetch current stock prices from Alpha Vantage API
- **📈 Performance Metrics**: Track total value, profit/loss, and return percentage
- **⚡ Price Caching**: 15-minute cache to reduce API calls and avoid rate limits
- **💾 Persistent Data Storage**: Transactions stored in `~/.stock-tracker/transactions.csv`
- **🎨 Color-Coded GUI**: Green/red indicators for gains/losses in Qt version

## 📋 Prerequisites

- **⚙️ C++ Compiler**: GCC with C++17 support
- **🌐 libcurl**: For HTTP requests to Alpha Vantage API
- **🖼️ Qt5** (optional): Only required for GUI version
- **🔑 Alpha Vantage API Key**: Get a free key from https://www.alphavantage.co/support/#api-key

## 🚀 Setup (WSL/Linux)

1. **📥 Clone the Repository**:
   ```bash
   git clone https://github.com/muya17/Stock-Portfolio-Tracker.git
   cd Stock-Portfolio-Tracker
   ```

2. **📦 Install Dependencies**:
   ```bash
   # Install all dependencies (including Qt5 for GUI)
   make install-deps
   
   # Or install manually:
   sudo apt update
   sudo apt install -y build-essential libcurl4-openssl-dev qtbase5-dev qt5-qmake
   ```

3. **🔑 Set API Key** (Choose one method):
   
   **Method 1 - Config File (Recommended for cross-platform use)**:
   ```bash
   # Create config in home directory (works on both Linux and WSL)
   mkdir -p ~/.stock-tracker
   echo "API_KEY=your-api-key-here" > ~/.stock-tracker/config.txt
   
   # Or copy the example config file
   cp config.txt.example ~/.stock-tracker/config.txt
   # Then edit with your API key: nano ~/.stock-tracker/config.txt
   ```
   
   **Method 2 - Environment Variable (Linux/WSL)**:
   ```bash
   # Add to ~/.bashrc for persistence
   echo 'export ALPHA_VANTAGE_API_KEY="your-api-key-here"' >> ~/.bashrc
   source ~/.bashrc
   
   # Or set for current session only
   export ALPHA_VANTAGE_API_KEY="your-api-key-here"
   ```
   
   **🔍 API Key Priority**: The app checks in this order:
   1. `ALPHA_VANTAGE_API_KEY` environment variable
   2. `~/.stock-tracker/config.txt`
   3. `config.txt` in project directory
   
   **💡 Note**: The app works without an API key, but live stock prices won't be available.
   Get a free API key from: https://www.alphavantage.co/support/#api-key

4. **🔨 Compile and Run**:
   ```bash
   # Compile both versions
   make
   
   # Or compile individually:
   make cli     # CLI version only
   make gui     # Qt GUI version only
   
   # Run CLI version
   ./portfolio_tracker
   # Or: make run-cli
   
   # Run Qt GUI version
   ./portfolio_tracker_qt
   # Or: make run-gui
   
   # Clean build artifacts
   make clean
   ```

## 📖 Usage

### 💻 CLI Version

#### Main Menu:
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

### 🖼️ Qt GUI Version

The Qt GUI provides a modern graphical interface with the following features:

- **Interactive Table View**: Portfolio displayed in an easy-to-read table with sortable columns
- **Color-Coded Indicators**: Green for gains, red for losses
- **Dialog-Based Input**: User-friendly input dialogs for buy/sell transactions
- **Real-Time Summary**: Live portfolio value, cost basis, total gain/loss, and return percentage
- **Transaction History Window**: Pop-up window showing complete transaction log
- **Refresh Button**: Manually refresh stock prices on demand
- **Validation**: Built-in validation for stock symbols and quantities

#### GUI Features:
- **Buy Stock**: Opens dialog to enter symbol, quantity, and price
- **Sell Stock**: Validates available shares before allowing sale
- **Refresh Prices**: Updates all stock prices from API
- **Transaction History**: Shows detailed transaction log in a separate window
- **Portfolio Summary**: Real-time display of:
  - Total Portfolio Value
  - Total Cost Basis
  - Total Gain/Loss (color-coded)
  - Return Percentage (color-coded)

## 💾 Data Storage

### 📂 Path Configuration:
Transactions are now stored in a consistent location: `~/.stock-tracker/transactions.csv`

This ensures:
- Data persists regardless of where you run the application
- Automatic directory creation on first run
- No conflicts with multiple project copies
- Easy backup and migration (just copy `~/.stock-tracker/`)

### 🔄 Cross-Platform Usage (Windows + WSL):
If you're using both Windows and WSL on the same machine:

**Option 1 - Separate Data** (Default):
- Windows version stores data in `C:\Users\YourName\.stock-tracker\`
- WSL/Linux stores data in `~/.stock-tracker/` (inside WSL filesystem)
- Each environment maintains its own portfolio

**Option 2 - Shared Data** (Advanced):
Create a symlink in WSL to point to your Windows data:
```bash
# Remove WSL data directory if it exists
rm -rf ~/.stock-tracker

# Create symlink to Windows directory
ln -s /mnt/c/Users/YourName/.stock-tracker ~/.stock-tracker
```

**🔑 API Key Sharing**:
The config file approach makes it easy to share API keys:
```bash
# In Windows: C:\Users\YourName\.stock-tracker\config.txt
# In WSL: ~/.stock-tracker/config.txt
# Both should contain: API_KEY=your-api-key-here
```

## 📁 Files

### Core Files:
- **main.cpp**: CLI program entry point with menu system
- **main_qt.cpp**: Qt GUI entry point
- **mainwindow.h**: Qt main window header
- **mainwindow.cpp**: Qt main window implementation
- **stock.hpp**: Transaction logging, portfolio aggregation, and display
- **api.hpp**: Stock price fetching via Alpha Vantage API with caching
- **http.hpp**: HTTP requests using libcurl
- **config.txt.example**: Example configuration file for API key

### Build Files:
- **Makefile**: Build automation for both CLI and Qt versions
- **setup.sh**: Dependency checker and setup helper

### Data Files (auto-generated):
- **~/.stock-tracker/transactions.csv**: Transaction log (not tracked in Git)

## ⚙️ How It Works

### 📝 Transaction Storage:
1. **~/.stock-tracker/transactions.csv** stores every buy/sell action:
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

### 🔄 Live Price Integration:
- Fetches current prices from Alpha Vantage API
- Caches prices for 15 minutes to respect rate limits
- Calculates gain/loss and return percentage

## 📝 Notes
- ✅ **Status**: Fully working on WSL/Linux
- Free Alpha Vantage accounts have rate limits: 5 calls/minute, 25/day
- The 15-minute cache minimizes API calls
- Transaction history provides complete audit trail
- Weighted average cost basis accounts for multiple purchases

## Contributing
Contributions and suggestions are welcome via GitHub issues or pull requests!

## License
This project is licensed under the MIT License. See the LICENSE file for details.

