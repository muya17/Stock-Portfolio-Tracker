#!/bin/bash
# Helper script to configure API key for Stock Portfolio Tracker

echo "=========================================="
echo "Stock Portfolio Tracker - API Key Setup"
echo "=========================================="
echo ""

# Create .stock-tracker directory if it doesn't exist
STOCK_DIR="$HOME/.stock-tracker"
CONFIG_FILE="$STOCK_DIR/config.txt"

mkdir -p "$STOCK_DIR"

echo "Current API key status:"
if [ -f "$CONFIG_FILE" ] && grep -q "API_KEY=" "$CONFIG_FILE" && ! grep -q "API_KEY=your-api-key-here" "$CONFIG_FILE"; then
    echo "✓ API key is configured in $CONFIG_FILE"
    echo ""
    echo "Would you like to update it? (y/n)"
    read -r UPDATE
    if [ "$UPDATE" != "y" ]; then
        echo "Exiting without changes."
        exit 0
    fi
else
    echo "✗ No API key configured"
    echo ""
fi

# Get API key from user
echo "Enter your Alpha Vantage API key:"
echo "(Get a free key from: https://www.alphavantage.co/support/#api-key)"
echo ""
read -r API_KEY

if [ -z "$API_KEY" ]; then
    echo "Error: API key cannot be empty"
    exit 1
fi

# Write config file
cat > "$CONFIG_FILE" << EOF
# Stock Portfolio Tracker Configuration
# Generated on $(date)

# Alpha Vantage API Key
API_KEY=$API_KEY

# The app will check for API key in this order:
# 1. ALPHA_VANTAGE_API_KEY environment variable
# 2. ~/.stock-tracker/config.txt (this file)
# 3. config.txt (in project directory)
EOF

echo ""
echo "✓ API key saved to: $CONFIG_FILE"
echo "✓ Configuration complete!"
echo ""
echo "You can now run:"
echo "  ./portfolio_tracker     (CLI version)"
echo "  ./portfolio_tracker_qt  (Qt GUI version)"
