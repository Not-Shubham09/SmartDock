#include "SettingsApp.h"

const char* SettingsApp::getName()
{
    return "Settings";
}

void SettingsApp::draw(lgfx::LGFX_Device* display)
{
    display->fillScreen(TFT_BLACK);

    display->drawRoundRect(
        10,
        10,
        220,
        300,
        TFT_ORANGE
    );

    display->setTextColor(TFT_ORANGE);
    display->setTextSize(3);

    display->drawCentreString(
        "SETTINGS",
        120,
        40,
        2
    );
}