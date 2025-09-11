#!/bin/bash

echo "🚀 Library Management System GUI Launcher"
echo "=========================================="

# Kill any existing servers
pkill -f "python3 -m http.server" 2>/dev/null

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "📁 Working directory: $SCRIPT_DIR"
echo "📄 Files in directory:"
ls -la *.html 2>/dev/null || echo "No HTML files found"

# Check if index.html exists
if [ -f "index.html" ]; then
    echo "✅ Found index.html"
    HTML_FILE="index.html"
elif [ -f "web_gui.html" ]; then
    echo "✅ Found web_gui.html"
    HTML_FILE="web_gui.html"
else
    echo "❌ No HTML file found!"
    exit 1
fi

echo ""
echo "🌐 Starting web server..."
echo "📱 Your GUI will be available at:"
echo "   http://127.0.0.1:8081/$HTML_FILE"
echo "   http://localhost:8081/$HTML_FILE"
echo ""

# Start the server
python3 -m http.server 8081 --bind 127.0.0.1 &
SERVER_PID=$!

# Wait a moment for server to start
sleep 2

# Test if server is working
if curl -s -I "http://127.0.0.1:8081/$HTML_FILE" | grep -q "200 OK"; then
    echo "✅ Server is running successfully!"
    echo ""
    echo "🎯 Open your browser and go to:"
    echo "   http://127.0.0.1:8081/$HTML_FILE"
    echo ""
    echo "✨ Features available:"
    echo "   • Search and browse books"
    echo "   • Add new books"
    echo "   • Checkout/return books"
    echo "   • View statistics"
    echo "   • Modern responsive design"
    echo ""
    echo "🛑 To stop the server, press Ctrl+C"
    echo ""
    
    # Try to open browser
    if command -v xdg-open &> /dev/null; then
        echo "🌐 Opening browser automatically..."
        xdg-open "http://127.0.0.1:8081/$HTML_FILE" 2>/dev/null &
    fi
    
    echo "🎉 Your Library Management System GUI is ready!"
    echo ""
    
    # Wait for user to stop
    wait $SERVER_PID
else
    echo "❌ Server failed to start properly"
    kill $SERVER_PID 2>/dev/null
    exit 1
fi
