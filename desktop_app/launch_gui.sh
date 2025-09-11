#!/bin/bash

echo "🚀 Launching Library Management System GUI..."
echo "=============================================="

# Check if server is already running
if netstat -tlnp 2>/dev/null | grep -q ":8081"; then
    echo "✅ Web server is already running on port 8081"
else
    echo "🌐 Starting web server on port 8081..."
    python3 -m http.server 8081 &
    sleep 2
fi

echo ""
echo "🎯 Your GUI Application is ready!"
echo "================================="
echo "📱 Open your web browser and go to:"
echo "   http://localhost:8081/web_gui.html"
echo ""
echo "🖥️  Or click this link to open automatically:"
echo "   http://127.0.0.1:8081/web_gui.html"
echo ""
echo "✨ Features available:"
echo "   • Search and browse books"
echo "   • Add new books"
echo "   • Checkout/return books"
echo "   • View statistics"
echo "   • Modern responsive design"
echo ""
echo "🛑 To stop the server, press Ctrl+C or run: pkill -f 'python3 -m http.server'"
echo ""

# Try to open the browser automatically
if command -v xdg-open &> /dev/null; then
    echo "🌐 Opening browser automatically..."
    xdg-open "http://localhost:8081/web_gui.html" 2>/dev/null &
elif command -v firefox &> /dev/null; then
    echo "🌐 Opening Firefox..."
    firefox "http://localhost:8081/web_gui.html" 2>/dev/null &
elif command -v google-chrome &> /dev/null; then
    echo "🌐 Opening Chrome..."
    google-chrome "http://localhost:8081/web_gui.html" 2>/dev/null &
else
    echo "💡 Please manually open your browser and go to the URL above"
fi

echo ""
echo "🎉 Enjoy your Library Management System GUI!"
