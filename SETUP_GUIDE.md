# Quick Setup Guide - Cross-Platform Configuration

## What Was Fixed

### 1. ✅ Path Issues - SOLVED
- **Before**: Used `./transactions.csv` (current directory)
- **After**: Uses `~/.stock-tracker/transactions.csv` (home directory)
- **Benefit**: Data persists regardless of where you run the app

### 2. ✅ API Key Configuration - SOLVED  
- **Before**: Required environment variable `ALPHA_VANTAGE_API_KEY`
- **After**: Multiple configuration options with priority chain
- **Benefit**: Easy to share API key between Windows and WSL

### 3. ✅ Cross-Platform Compatibility - CONFIRMED
- Works identically on Linux, WSL, and Windows (with WSL)
- No path conflicts between environments
- Optional data sharing between Windows and WSL

---

## Setting Up Your API Key

### Quick Method (Recommended)
```bash
cd ~/Documents/Stock-Portfolio-Tracker
./configure_api.sh
```

### Manual Method
```bash
# Create the config file
mkdir -p ~/.stock-tracker
cat > ~/.stock-tracker/config.txt << EOF
# Stock Portfolio Tracker Configuration
API_KEY=your-actual-api-key-here
EOF
```

### If You Already Have API Key in Windows
```bash
# Option A: Copy the config file from Windows to WSL
cp /mnt/c/Users/YourName/.stock-tracker/config.txt ~/.stock-tracker/

# Option B: Create a shared config (advanced)
# This makes WSL use the Windows config directly
rm -rf ~/.stock-tracker
ln -s /mnt/c/Users/YourName/.stock-tracker ~/.stock-tracker
```

---

## Running the Applications

### CLI Version
```bash
cd ~/Documents/Stock-Portfolio-Tracker
./portfolio_tracker
```

### Qt GUI Version
```bash
cd ~/Documents/Stock-Portfolio-Tracker
./portfolio_tracker_qt
```

Or from any directory:
```bash
~/Documents/Stock-Portfolio-Tracker/portfolio_tracker_qt
```

---

## API Key Priority

The app checks for your API key in this order:

1. **Environment variable**: `ALPHA_VANTAGE_API_KEY`
2. **Home config**: `~/.stock-tracker/config.txt` ← **Recommended**
3. **Project config**: `./config.txt`

If no API key is found:
- ✅ App still runs normally
- ✅ Can buy/sell stocks with manual prices
- ❌ Can't fetch live prices from API
- ⚠️  Shows one-time warning on startup

---

## Data Storage

### Transaction Data
- **Location**: `~/.stock-tracker/transactions.csv`
- **Format**: CSV with Date, Action, Symbol, Quantity, Price
- **Backup**: Just copy the entire `~/.stock-tracker/` directory

### Current Data
Your existing portfolio already has:
```csv
Date,Action,Symbol,Quantity,Price
2026-04-28,BUY,AAPL,20,59.01
```

---

## Cross-Platform Notes

### WSL + Windows Dual Setup

**Separate Data (Default)**:
- Windows: `C:\Users\YourName\.stock-tracker\`
- WSL: `~/.stock-tracker/` (inside WSL filesystem)
- Each environment has independent portfolio

**Shared Data (Advanced)**:
```bash
# In WSL, create symlink to Windows location
rm -rf ~/.stock-tracker
ln -s /mnt/c/Users/YourName/.stock-tracker ~/.stock-tracker
```

Now both environments share the same portfolio data!

---

## Troubleshooting

### "Warning: API key not configured"
- This is normal if you haven't set up the API key yet
- The app will still work, just can't fetch live prices
- Follow the setup steps above to configure it

### "Error: Cannot fetch price"
- Check that your API key is correct
- Verify internet connection
- Alpha Vantage has rate limits (5 API calls/minute for free tier)

### Data not persisting
- Check that `~/.stock-tracker/` directory exists
- Run: `ls -la ~/.stock-tracker/`
- Should see `transactions.csv` file

### Can't find my Windows API key
```bash
# In WSL, check if Windows config exists:
ls /mnt/c/Users/*/\.stock-tracker/config.txt

# Copy it to WSL:
cp /mnt/c/Users/YourName/.stock-tracker/config.txt ~/.stock-tracker/
```

---

## Summary

✅ **Paths**: Fixed - uses consistent home directory location
✅ **API Key**: Fixed - flexible config file system  
✅ **Cross-Platform**: Confirmed - works on Linux/WSL seamlessly
✅ **Data Persistence**: Fixed - no more lost transactions
✅ **Qt GUI**: Added - modern graphical interface
✅ **No Breaking Changes**: CLI version still works as before

**Your app is ready to use!** 🚀
