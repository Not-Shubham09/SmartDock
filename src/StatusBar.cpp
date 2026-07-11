#include "StatusBar.h"
#include "ThemeManager.h"
#include "FontManager.h"
#include "WiFiManager.h"
#include "LGFX_Config.h"
#include <time.h>

extern LGFX display1;

StatusBar::DisplayState StatusBar::state1 = { false, -1, nullptr };
StatusBar::DisplayState StatusBar::state2 = { false, -1, nullptr };

void StatusBar::draw(lgfx::LGFX_Device* display, const char* appName, bool forceRedraw)
{
    extern WiFiManager wifiManager;

    bool wifiConnected = wifiManager.isConnected();

    time_t now = time(nullptr);
    struct tm timeInfo;
    localtime_r(&now, &timeInfo);
    int currentMinute = (timeInfo.tm_year >= 120) ? timeInfo.tm_min : -1;

    DisplayState& state = (display == (lgfx::LGFX_Device*)&display1) ? state1 : state2;

    if (!forceRedraw &&
        state.wifiConnected == wifiConnected &&
        state.lastMinute == currentMinute &&
        state.lastAppName == appName)
    {
        return; // No change, skip drawing to save SPI bandwidth
    }

    state.wifiConnected = wifiConnected;
    state.lastMinute = currentMinute;
    state.lastAppName = appName;

    const Theme& theme = ThemeManager::current();

    // Clear status bar background
    display->fillRect(0, 0, 240, 17, theme.background);

    // Draw status bar separator line
    display->drawFastHLine(0, 17, 240, theme.border);

    // Apply font settings
    display->setFont(FontManager::getSmallFont());
    display->setTextColor(theme.foreground);

    // 1. Draw WiFi status (left-aligned)
    const char* wifiText = wifiConnected ? "WiFi" : "No WiFi";
    display->drawString(wifiText, 8, 5);

    // 2. Draw Battery placeholder (next to WiFi)
    display->drawString("100%", 60, 5);

    // 3. Draw App Name (center-aligned)
    if (appName != nullptr)
    {
        display->drawCentreString(appName, 120, 5);
    }

    // 4. Draw Current Time (right-aligned)
    char timeStr[6] = "--:--";
    if (currentMinute != -1)
    {
        strftime(timeStr, sizeof(timeStr), "%H:%M", &timeInfo);
    }
    display->drawString(timeStr, 240 - 8 - display->textWidth(timeStr), 5);
}
