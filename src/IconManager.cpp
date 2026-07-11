#include "IconManager.h"
#include "FontManager.h"

void IconManager::drawWiFi(lgfx::LGFX_Device* display, int32_t x, int32_t y, uint16_t color, bool connected)
{
    // Vector drawing for high-fidelity rendering
    if (connected)
    {
        // 4 bars of signal strength
        display->fillRect(x,     y + 6, 2, 2, color);
        display->fillRect(x + 3, y + 4, 2, 4, color);
        display->fillRect(x + 6, y + 2, 2, 6, color);
        display->fillRect(x + 9, y,     2, 8, color);
    }
    else
    {
        uint16_t disabledColor = 0x7BEF; // dark gray
        display->fillRect(x,     y + 6, 2, 2, disabledColor);
        display->fillRect(x + 3, y + 4, 2, 4, disabledColor);
        display->fillRect(x + 6, y + 2, 2, 6, disabledColor);
        display->fillRect(x + 9, y,     2, 8, disabledColor);
        
        // Draw red diagonal line to cross out the signal bars
        display->drawLine(x - 1, y - 1, x + 11, y + 9, TFT_RED);
    }
}

void IconManager::drawBattery(lgfx::LGFX_Device* display, int32_t x, int32_t y, uint16_t color, int percentage)
{
    // Vector drawing of battery
    // Body: 14x8 rectangle
    display->drawRect(x, y, 14, 8, color);
    // Tip/Cap: 2x4 rectangle at the right
    display->fillRect(x + 14, y + 2, 2, 4, color);
    
    // Fill level: 10x4 inside body
    int fillWidth = (percentage * 10) / 100;
    if (fillWidth > 0)
    {
        uint16_t fillCol = (percentage < 20) ? TFT_RED : color;
        display->fillRect(x + 2, y + 2, fillWidth, 4, fillCol);
    }
}

void IconManager::drawClock(lgfx::LGFX_Device* display, int32_t x, int32_t y, uint16_t color)
{
    // Circle of radius 5, center at (x+5, y+5)
    display->drawCircle(x + 5, y + 5, 5, color);
    // Hands
    display->drawLine(x + 5, y + 5, x + 5, y + 2, color); // hour
    display->drawLine(x + 5, y + 5, x + 8, y + 5, color); // minute
}

void IconManager::drawSettings(lgfx::LGFX_Device* display, int32_t x, int32_t y, uint16_t color)
{
    // Central hole
    display->drawCircle(x + 5, y + 5, 2, color);
    // Outer gear circle
    display->drawCircle(x + 5, y + 5, 4, color);
    // Teeth (simple dots)
    display->drawPixel(x + 5, y, color);
    display->drawPixel(x + 5, y + 10, color);
    display->drawPixel(x, y + 5, color);
    display->drawPixel(x + 10, y + 5, color);
    display->drawPixel(x + 2, y + 2, color);
    display->drawPixel(x + 8, y + 8, color);
    display->drawPixel(x + 2, y + 8, color);
    display->drawPixel(x + 8, y + 2, color);
}

void IconManager::drawQuote(lgfx::LGFX_Device* display, int32_t x, int32_t y, uint16_t color)
{
    // Speech bubble: round rect 11x7
    display->drawRoundRect(x, y, 11, 7, 2, color);
    // tail: triangle/lines
    display->drawLine(x + 2, y + 6, x + 1, y + 9, color);
    display->drawLine(x + 1, y + 9, x + 4, y + 6, color);
}
