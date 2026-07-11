#include "QuotesApp.h"
#include "ThemeManager.h"

const char* QuotesApp::getName()
{
    return "Quotes";
}

void QuotesApp::draw(lgfx::LGFX_Device* display)
{
    const Theme& theme = ThemeManager::current();
    display->fillScreen(theme.background);

    display->drawRoundRect(10, 24, 220, 286, theme.success);

    display->setTextColor(theme.success);
    display->setTextSize(3);

    display->drawCentreString("QUOTE", 120, 35, 2);

    display->setTextColor(theme.foreground);
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