#!/usr/bin/env python3
"""
Simple script to create an application icon for the Library Management System
This creates a basic icon that can be used for the desktop application
"""

from PIL import Image, ImageDraw, ImageFont
import os

def create_icon():
    # Create a 256x256 image with a dark background
    size = 256
    img = Image.new('RGBA', (size, size), (53, 53, 53, 255))
    draw = ImageDraw.Draw(img)
    
    # Draw a book icon
    # Book cover (rectangle)
    book_x = size // 4
    book_y = size // 4
    book_width = size // 2
    book_height = size // 2
    
    # Book cover
    draw.rectangle([book_x, book_y, book_x + book_width, book_y + book_height], 
                   fill=(42, 130, 218, 255), outline=(255, 255, 255, 255), width=3)
    
    # Book pages (white lines)
    for i in range(5):
        line_y = book_y + 20 + i * 15
        draw.line([book_x + 10, line_y, book_x + book_width - 10, line_y], 
                  fill=(255, 255, 255, 255), width=2)
    
    # Book spine
    draw.rectangle([book_x - 5, book_y, book_x, book_y + book_height], 
                   fill=(30, 100, 180, 255))
    
    # Add text "LMS" in the center
    try:
        # Try to use a system font
        font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 32)
    except:
        try:
            font = ImageFont.truetype("/System/Library/Fonts/Arial.ttf", 32)
        except:
            font = ImageFont.load_default()
    
    text = "LMS"
    bbox = draw.textbbox((0, 0), text, font=font)
    text_width = bbox[2] - bbox[0]
    text_height = bbox[3] - bbox[1]
    
    text_x = (size - text_width) // 2
    text_y = book_y + book_height + 20
    
    draw.text((text_x, text_y), text, fill=(255, 255, 255, 255), font=font)
    
    # Save the icon
    img.save('icon.png')
    print("✅ Icon created: icon.png")
    
    # Also create a smaller version for the desktop file
    small_img = img.resize((64, 64), Image.Resampling.LANCZOS)
    small_img.save('icon-64.png')
    print("✅ Small icon created: icon-64.png")

if __name__ == "__main__":
    try:
        create_icon()
    except ImportError:
        print("❌ PIL (Pillow) not found. Installing...")
        os.system("pip install Pillow")
        create_icon()
    except Exception as e:
        print(f"❌ Error creating icon: {e}")
        print("Creating a simple placeholder icon...")
        
        # Create a simple colored square as fallback
        img = Image.new('RGBA', (256, 256), (42, 130, 218, 255))
        img.save('icon.png')
        print("✅ Simple icon created: icon.png")
