#!/usr/bin/env python3

import http.server
import socketserver
import os
import webbrowser
import time

# Change to the directory where this script is located
os.chdir(os.path.dirname(os.path.abspath(__file__)))

PORT = 8083

class MyHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/' or self.path == '/index.html':
            self.path = '/index.html'
        return super().do_GET()

def main():
    print("🚀 Library Management System GUI Server")
    print("=" * 50)
    
    # Check if index.html exists
    if not os.path.exists('index.html'):
        print("❌ Error: index.html not found")
        return
    
    print(f"✅ Found index.html in {os.getcwd()}")
    
    # Start the server
    with socketserver.TCPServer(("127.0.0.1", PORT), MyHTTPRequestHandler) as httpd:
        print(f"🌐 Server running at http://127.0.0.1:{PORT}/")
        print(f"📱 GUI available at: http://127.0.0.1:{PORT}/index.html")
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
            webbrowser.open(f'http://127.0.0.1:{PORT}/index.html')
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
