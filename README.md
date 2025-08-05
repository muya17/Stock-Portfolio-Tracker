# Stock Portfolio Tracker

A C++ console application to manage a stock portfolio, fetch real-time stock prices using the Alpha Vantage API, and save/load portfolio data to/from a CSV file.

## Features
- Add stocks with symbol, quantity, and purchase price.
- View portfolio with current prices, total value, and gain/loss.
- Save portfolio to `portfolio.csv` and load on startup.
- Cache stock prices for 15 minutes to reduce API calls.

## Prerequisites
- **C++ Compiler**: MSYS2 with `g++` (e.g., `mingw-w64-ucrt-x86_64-gcc`).
- **libcurl**: For HTTP requests to the Alpha Vantage API.
  - Option 1: Install prebuilt `libcurl` at `C:\curl-8.15.0_4-win64-mingw`.
  - Option 2: Install via MSYS2: `pacman -S mingw-w64-ucrt-x86_64-curl`.
- **Alpha Vantage API Key**: Get a free key from [Alpha Vantage](https://www.alphavantage.co/support/#api-key).

## Setup
1. **Clone the Repository**:
   ```bash
   git clone <your-repo-url>
   cd Stock-Portfolio-Tracker