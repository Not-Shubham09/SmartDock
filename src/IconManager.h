#pragma once

#include <LovyanGFX.hpp>

class IconManager
{
public:
    static void drawWiFi(lgfx::LGFX_Device* display, int32_t x, int32_t y, uint16_t color, bool connected);
    static void drawBattery(lgfx::LGFX_Device* display, int32_t x, int32_t y, uint16_t color, int percentage);
    static void drawClock(lgfx::LGFX_Device* display, int32_t x, int32_t y, uint16_t color);
    static void drawSettings(lgfx::LGFX_Device* display, int32_t x, int32_t y, uint16_t color);
    static void drawQuote(lgfx::LGFX_Device* display, int32_t x, int32_t y, uint16_t color);
};
