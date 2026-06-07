#include "QuotesApp.h"

const char* QuotesApp::getName()
{
    return "Quotes";
}

void QuotesApp::draw(lgfx::LGFX_Device* display)
{
    display->fillScreen(TFT_BLACK);

    display->drawRoundRect(10, 10, 220, 300, TFT_GREEN);

    display->setTextColor(TFT_GREEN);
    display->setTextSize(3);

    display->drawCentreString("QUOTE", 120, 30, 2);

    display->setTextColor(TFT_WHITE);
    display->setTextSize(2);

    display->drawCentreString(
        "Keep moving",
        120,
        130,
        2
    );

    display->drawCentreString(
        "forward.",
        120,
        160,
        2
    );
}