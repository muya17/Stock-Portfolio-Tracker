#!/bin/bash
# Setup script for Stock Portfolio Tracker in WSL

echo "Stock Portfolio Tracker - WSL Setup"
echo "===================================="
echo ""

# Check for libcurl
if ! ldconfig -p | grep -q libcurl; then
    echo "❌ libcurl not found. Installing dependencies..."
    sudo apt update
    sudo apt install -y build-essential libcurl4-openssl-dev
else
    echo "✓ libcurl is installed"
fi

# Check for g++
if ! command -v g++ &> /dev/null; then
    echo "❌ g++ not found. Installing..."
    sudo apt install -y build-essential
else
    echo "✓ g++ is installed"
fi

# Check for API key
if [ -z "$ALPHA_VANTAGE_API_KEY" ]; then
    echo ""
    echo "❌ ALPHA_VANTAGE_API_KEY environment variable is NOT set"
    echo ""
    echo "To set your API key:"
    echo "1. Get a free key from: https://www.alphavantage.co/support/#api-key"
    echo "2. Add to your ~/.bashrc:"
    echo "   echo 'export ALPHA_VANTAGE_API_KEY=\"your-api-key-here\"' >> ~/.bashrc"
    echo "   source ~/.bashrc"
    echo ""
    echo "Or set it temporarily for this session:"
    echo "   export ALPHA_VANTAGE_API_KEY=\"your-api-key-here\""
    echo ""
else
    echo "✓ ALPHA_VANTAGE_API_KEY is set: ${ALPHA_VANTAGE_API_KEY:0:8}..."
fi

echo ""
echo "Setup complete! Next steps:"
echo "1. Set your API key (if not already set)"
echo "2. Run: make"
echo "3. Run: ./portfolio_tracker"
