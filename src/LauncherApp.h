#pragma once

#include "App.h"
#include "ThemeManager.h"
#include "FontManager.h"
#include "IconManager.h"
#include "AppManager.h"

extern AppManager appManager;

class LauncherApp : public App
{
private:
    int selectedIdx = 0;

public:
    const char* getName() override
    {
        return "Launcher";
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

        // Draw Launcher border
        display->drawRoundRect(10, 24, 220, 286, theme.border);

        // Draw SELECT APP header
        display->setFont(FontManager::getMediumFont());
        display->setTextColor(theme.foreground);
        display->drawCentreString("SELECT APP", 120, 35, 2);

        int count = appManager.getCount();
        if (count == 0) return;

        int prevIdx = (selectedIdx - 1 + count) % count;
        int nextIdx = (selectedIdx + 1) % count;

        App* prevApp = appManager.getApp(prevIdx);
        App* curApp = appManager.getApp(selectedIdx);
        App* nextApp = appManager.getApp(nextIdx);

        display->setFont(FontManager::getMediumFont());

        // 1. Draw previous app (dimmed)
        display->setTextColor(theme.disabled);
        display->setTextSize(2);
        display->drawCentreString(prevApp->getName(), 120, 80, 2);

        // 2. Draw current app (highlighted with outline box & icon)
        display->setTextColor(theme.accent);
        display->drawRoundRect(25, 125, 190, 65, 8, theme.accent);
        
        display->setTextSize(2);
        display->drawCentreString(curApp->getName(), 120, 155, 2);

        // Draw icon
        int iconX = 110;
        int iconY = 135;
        if (selectedIdx == 0)
            IconManager::drawClock(display, iconX, iconY, theme.accent);
        else if (selectedIdx == 1)
            IconManager::drawQuote(display, iconX, iconY, theme.accent);
        else if (selectedIdx == 2)
            IconManager::drawSettings(display, iconX, iconY, theme.accent);

        // 3. Draw next app (dimmed)
        display->setTextColor(theme.disabled);
        display->setTextSize(2);
        display->drawCentreString(nextApp->getName(), 120, 215, 2);
    }
};
