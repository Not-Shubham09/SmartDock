# SmartDock

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

Long press should switch active display.

Selected display should be highlighted.

Do not modify hardware pin assignments.