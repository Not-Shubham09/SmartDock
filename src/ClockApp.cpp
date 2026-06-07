#include "ClockApp.h"

const char* ClockApp::getName()
{
    return "Clock";
}

void ClockApp::draw(lgfx::LGFX_Device* display)
{
    display->fillScreen(TFT_BLACK);

    display->drawRoundRect(10, 10, 220, 300, TFT_CYAN);

    display->setTextColor(TFT_CYAN);
    display->setTextSize(3);

    display->drawCentreString("CLOCK", 120, 30, 2);

    display->setTextColor(TFT_WHITE);
    display->setTextSize(5);

    display->drawCentreString("22:45", 120, 130, 2);

    display->setTextSize(2);

    display->drawCentreString("SmartDock OS", 120, 250, 2);
}