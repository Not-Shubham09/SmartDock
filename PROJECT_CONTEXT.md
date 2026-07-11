# SmartDock

Path:
/Users/shubhamhooda/Documents/PlatformIO/Projects/SmartDock

## Hardware

ESP32 Dev Module

### TFT #1

SCK -> GPIO18
MOSI -> GPIO23
CS -> GPIO15
DC -> GPIO2
RST -> GPIO4

### TFT #2

SCK -> GPIO18
MOSI -> GPIO23
CS -> GPIO5
DC -> GPIO27
RST -> GPIO26

### Rotary Encoder

CLK -> GPIO32
DT -> GPIO33
SW -> GPIO25

Platform
ESP32 Dev Module
PlatformIO
LovyanGFX
---

## Libraries

LovyanGFX
PlatformIO

---

## Current Goal

Build SmartDock OS.

Features:
- Dual independent displays
- Rotary navigation
- Clock
- Weather
- Notes
- Todo
- Pomodoro
- Settings
- Music
- Anime Wallpapers

Rules
- Never modify LGFX_Config.h
- Never modify LGFX_Config2.h
- Never change GPIO assignments
- Modify existing files in-place
- Do not create duplicate project structures
- Apps inherit from App
- Keep app logic out of main.cpp
- Use ThemeManager colors instead of hardcoding colors in apps
- Use FontManager fonts instead of directly selecting fonts in apps
- Apps must render below the status bar (Y >= 18)
- Use IconManager for visual icons/placeholders instead of text or assets

Current Status
- Dual displays working
- Navigation working
- Active display border working
- WiFiManager exists
- ThemeManager exists
- FontManager exists
- StatusBar exists
- IconManager exists