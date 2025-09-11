#!/bin/bash

echo "🚀 Library Management System GUI Launcher"
echo "=========================================="

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
HTML_FILE="$SCRIPT_DIR/index.html"

echo "📁 Working directory: $SCRIPT_DIR"
echo "📄 HTML file: $HTML_FILE"

# Check if HTML file exists
if [ ! -f "$HTML_FILE" ]; then
    echo "❌ Error: index.html not found in $SCRIPT_DIR"
    exit 1
fi

echo "✅ Found HTML file"
echo ""
echo "🎯 Opening Library Management System GUI..."
echo ""

# Try to open the HTML file in the default browser
if command -v xdg-open &> /dev/null; then
    echo "🌐 Opening with xdg-open..."
    xdg-open "$HTML_FILE" &
elif command -v firefox &> /dev/null; then
    echo "🌐 Opening with Firefox..."
    firefox "$HTML_FILE" &
elif command -v google-chrome &> /dev/null; then
    echo "🌐 Opening with Chrome..."
    google-chrome "$HTML_FILE" &
elif command -v chromium-browser &> /dev/null; then
    echo "🌐 Opening with Chromium..."
    chromium-browser "$HTML_FILE" &
else
    echo "💡 Please manually open your browser and navigate to:"
    echo "   file://$HTML_FILE"
    echo ""
    echo "Or copy this path and paste it in your browser's address bar:"
    echo "   file://$HTML_FILE"
fi

echo ""
echo "✨ Features available:"
echo "   • Search and browse books"
echo "   • Add new books"
echo "   • Checkout/return books"
echo "   • View statistics"
echo "   • Modern responsive design"
echo ""
echo "🎉 Your Library Management System GUI is ready!"
echo ""
echo "📝 Note: This is a local file, so all data is stored in your browser's memory."
echo "   To save data permanently, you would need the full desktop application."
