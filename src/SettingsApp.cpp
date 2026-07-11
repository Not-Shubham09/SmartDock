#include "SettingsApp.h"
#include "ThemeManager.h"

const char* SettingsApp::getName()
{
    return "Settings";
}

void SettingsApp::draw(lgfx::LGFX_Device* display)
{
    const Theme& theme = ThemeManager::current();
    display->fillScreen(theme.background);

    display->drawRoundRect(
        10,
        24,
        220,
        286,
        theme.warning
    );

    display->setTextColor(theme.warning);
    display->setTextSize(3);

    display->drawCentreString(
        "SETTINGS",
        120,
        45,
        2
    );
}