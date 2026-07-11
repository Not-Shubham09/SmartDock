#pragma once

#include "App.h"
#include "ThemeManager.h"
#include "FontManager.h"
#include "IconManager.h"
#include "AppManager.h"
#include "WiFiManager.h"
#include <time.h>

extern AppManager appManager;
extern WiFiManager wifiManager;

class LauncherApp : public App
{
private:
    int selectedIdx = 0;

public:
    const char* getName() override
    {
        return "Home";
    }

    void setSelectedIdx(int idx)
    {
        selectedIdx = idx;
    }

    int getSelectedIdx() const
    {
        return selectedIdx;
    }

    void draw(lgfx::LGFX_Device* display) override
    {
        const Theme& theme = ThemeManager::current();
        display->fillScreen(theme.background);

        // 1. Draw outer Dashboard border
        display->drawRoundRect(10, 24, 220, 286, 8, theme.border);

        // 2. Draw Header
        display->setFont(FontManager::getMediumFont());
        display->setTextColor(theme.foreground);
        display->drawString("Home", 20, 32);

        // Draw WiFi and Battery indicators in the header on the right
        int wifiX = 175;
        int headerY = 32;
        IconManager::drawWiFi(display, wifiX, headerY, theme.foreground, wifiManager.isConnected());
        IconManager::drawBattery(display, 200, headerY, theme.foreground, 100);

        // 3. Read & Format Date/Time (with NTP check or 09:41 / Saturday fallback)
        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        
        char timeStr[6] = "09:41";
        char dayStr[16] = "Saturday";
        
        if (rawtime > 100000)
        {
            strftime(timeStr, sizeof(timeStr), "%H:%M", timeinfo);
            strftime(dayStr, sizeof(dayStr), "%A", timeinfo);
        }

        // Draw Time (Large size font)
        display->setFont(FontManager::getLargeFont());
        display->setTextColor(theme.foreground);
        display->drawString(timeStr, 20, 52);

        // Draw Day
        display->setFont(FontManager::getMediumFont());
        display->drawString(dayStr, 20, 95);

        // 4. Draw Split Column: Today's Tasks
        display->setTextColor(theme.accent);
        display->drawString("Today's Tasks", 130, 55);

        display->setFont(FontManager::getSmallFont());
        display->setTextColor(theme.foreground);
        display->drawString("[x] SmartDock", 130, 80);
        display->drawString("[ ] Hydrate", 130, 98);
        display->drawString("[ ] Read Quote", 130, 116);

        // 5. Draw Weather Widget (Left/Center)
        // Sunny Sun icon
        int sunX = 20;
        int weatherY = 140;
        display->fillCircle(sunX + 6, weatherY + 6, 6, 0xFD20); // Gold/Orange sun
        for (int angle = 0; angle < 360; angle += 45)
        {
            float rad = angle * 3.14159f / 180.0f;
            int rx = sunX + 6 + (int)(10 * cos(rad));
            int ry = weatherY + 6 + (int)(10 * sin(rad));
            display->drawPixel(rx, ry, 0xFD20);
        }
        display->setFont(FontManager::getSmallFont());
        display->setTextColor(theme.foreground);
        display->drawString("24 C Sunny", sunX + 22, weatherY + 3);

        // 6. Draw Selected App Card at the Bottom
        int count = appManager.getCount();
        if (count > 0)
        {
            App* curApp = appManager.getApp(selectedIdx);

            // Card container
            display->drawRoundRect(18, 200, 204, 75, 8, theme.accent);
            
            // Draw card dark filling
            display->fillRoundRect(19, 201, 202, 73, 7, 0x0821); // transparent-like dark teal

            // Draw app icon centered on the left
            int iconX = 35;
            int iconY = 222;
            if (selectedIdx == 0)
                IconManager::drawClock(display, iconX, iconY, theme.accent);
            else if (selectedIdx == 1)
                IconManager::drawQuote(display, iconX, iconY, theme.accent);
            else if (selectedIdx == 2)
                IconManager::drawSettings(display, iconX, iconY, theme.accent);

            // Draw App Name
            display->setFont(FontManager::getMediumFont());
            display->setTextColor(theme.foreground);
            display->drawString(curApp->getName(), 75, 218);

            // Draw subtext
            display->setFont(FontManager::getSmallFont());
            display->setTextColor(theme.disabled);
            display->drawString("Rotate to choose app", 75, 245);
        }
    }
};
