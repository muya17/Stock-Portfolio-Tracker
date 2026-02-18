# Stock Portfolio Tracker

A C++ console application to manage a stock portfolio, fetch real-time stock prices using the Alpha Vantage API, and save/load portfolio data to/from a CSV file. Note: The project is not fully working as I am still resolving libcurl dependency issues on my local system. Contributions and suggestions are welcome!

## Features
- Add stocks with symbol, quantity, and purchase price.
- View portfolio with current prices, total value, and gain/loss.
- Save portfolio to portfolio.csv and load on startup.
- Cache stock prices for 15 minutes to reduce API calls.

## Prerequisites
- C++ Compiler: MSYS2 with g++ (e.g., mingw-w64-ucrt-x86_64-gcc).
- libcurl: For HTTP requests to the Alpha Vantage API (work in progress).
  - Option 1: Prebuilt libcurl at C:\curl-8.15.0_4-win64-mingw.
  - Option 2: Install via MSYS2: pacman -S mingw-w64-ucrt-x86_64-curl.
- Alpha Vantage API Key: Get a free key from https://www.alphavantage.co/support/#api-key.

## Setup
1. Clone the Repository:
   git clone https://github.com/your-username/Stock-Portfolio-Tracker.git
   cd Stock-Portfolio-Tracker

2. Set API Key:
   - Set the ALPHA_VANTAGE_API_KEY environment variable:
     - Windows (permanent): setx ALPHA_VANTAGE_API_KEY "your-api-key"
     - Windows (current session): set ALPHA_VANTAGE_API_KEY=your-api-key
     - Verify: echo %ALPHA_VANTAGE_API_KEY%

3. Install Dependencies:
   - Install MSYS2: Download from https://www.msys2.org/.
   - Update MSYS2 and install g++ and libcurl:
     pacman -Syu
     pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-curl

4. Compile:
   - With prebuilt libcurl (may have issues):
     C:/msys64/ucrt64/bin/g++.exe -g main.cpp -o portfolio_tracker -I C:/curl-8.15.0_4-win64-mingw/include -L C:/curl-8.15.0_4-win64-mingw/lib -lcurl -DCURL_STATICLIB -lws2_32 -lwldap32 -lwinmm
   - With MSYS2’s libcurl:
     C:/msys64/ucrt64/bin/g++.exe -g main.cpp -o portfolio_tracker -I C:/msys64/ucrt64/include -L C:/msys64/ucrt64/lib -lcurl

5. Run:
   .\portfolio_tracker.exe

## Usage
- Add Stock: Enter a stock symbol (e.g., AAPL), quantity, and purchase price.
- View Portfolio: Displays current prices, total value, and gain/loss (API functionality pending).
- Exit: Saves portfolio to portfolio.csv.

## Files
- main.cpp: Main program logic and menu.
- stock.hpp: Portfolio management and file I/O.
- api.hpp: Fetches stock prices using Alpha Vantage API.
- http.hpp: HTTP requests using libcurl.
- portfolio.csv: Generated file storing portfolio data (not tracked in Git).
- LICENSE: MIT License for the project.

## Notes
- The project is under development, with ongoing work to resolve libcurl issues causing "Empty response from API" errors.
- Contributions and suggestions are welcome via GitHub issues or pull requests.
- The program caches stock prices for 15 minutes to avoid API rate limits (5 calls/minute or 25/day for free Alpha Vantage accounts).

## Debugging
- The program includes debug output for API responses and file I/O.
- Use MSYS2’s libcurl if static linking fails.

## License
This project is licensed under the MIT License. See the LICENSE file for details.