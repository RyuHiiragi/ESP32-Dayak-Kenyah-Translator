# Dayak Kenyah Language Translator (ESP32)

![Project Banner](https://i.pinimg.com/736x/cc/82/6d/cc826d6dccc9dcf66760320f295a688f.jpg)

## Project Overview
This project implements a bidirectional translator between Indonesian and Dayak Kenyah language using an ESP32 microcontroller. It features a web-based interface with:
- 200+ vocabulary entries
- Phrase translation capability
- Responsive design with modern CSS
- Font Awesome icons integration
- Unknown word handling
- Case-insensitive translation

## Key Features
✅ **Bidirectional Translation**  
   Supports both Indonesian → Dayak Kenyah and Dayak Kenyah → Indonesian

✅ **Phrase Translation**  
   Automatically splits and processes multi-word inputs (e.g., "rumah makan" → "amin uman")

✅ **Smart Fallback**  
   Retains original words when no translation is found

✅ **Modern Web Interface**  
   - Responsive design for mobile devices
   - Google Fonts integration
   - CSS variables for theming
   - Interactive language swap button

✅ **Performance Optimization**  
   Uses PROGMEM to store web assets and dictionary data

## Requirements
**Hardware:**
- ESP32 development board
- Stable WiFi connection

**Software:**
- Arduino IDE 1.8.19+
- Libraries:
  - WiFi.h (ESP32 built-in)
  - WebServer.h (ESP32 built-in)
  - ArduinoJson v6.21.4+

**Credits**
Developed by:
Muhammad Rizky Saputra (XI TJKT 2)
GitHub: @RyuHiiragi
Instagram: @kysukamieayam

**Limitations**
Exact word/phrase matches only
No grammar conjugation support
Case-insensitive only for Dayak→Indonesian
