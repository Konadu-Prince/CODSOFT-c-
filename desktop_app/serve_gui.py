#!/usr/bin/env python3

import http.server
import socketserver
import os
import webbrowser
import time

# Change to the directory where this script is located
os.chdir(os.path.dirname(os.path.abspath(__file__)))

PORT = 8081

class MyHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        super().end_headers()

def main():
    print("🚀 Starting Library Management System GUI Server...")
    print("=" * 50)
    
    # Check if web_gui.html exists
    if not os.path.exists('web_gui.html'):
        print("❌ Error: web_gui.html not found in current directory")
        print(f"Current directory: {os.getcwd()}")
        print("Files in directory:")
        for f in os.listdir('.'):
            print(f"  - {f}")
        return
    
    print(f"✅ Found web_gui.html in {os.getcwd()}")
    
    # Start the server
    with socketserver.TCPServer(("127.0.0.1", PORT), MyHTTPRequestHandler) as httpd:
        print(f"🌐 Server running at http://127.0.0.1:{PORT}/")
        print(f"📱 GUI available at: http://127.0.0.1:{PORT}/web_gui.html")
        print("=" * 50)
        print("✨ Features available:")
        print("   • Search and browse books")
        print("   • Add new books")
        print("   • Checkout/return books")
        print("   • View statistics")
        print("   • Modern responsive design")
        print("=" * 50)
        print("🛑 Press Ctrl+C to stop the server")
        print("")
        
        # Try to open browser automatically
        try:
            print("🌐 Opening browser automatically...")
            webbrowser.open(f'http://127.0.0.1:{PORT}/web_gui.html')
        except:
            print("💡 Please manually open your browser and go to the URL above")
        
        print("")
        print("🎉 Your Library Management System GUI is ready!")
        print("")
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\n🛑 Server stopped by user")
            print("👋 Thank you for using Library Management System!")

if __name__ == "__main__":
    main()
