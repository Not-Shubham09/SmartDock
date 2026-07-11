#include "DisplayManager.h"
#include "StatusBar.h"
#include "ThemeManager.h"

void DisplayManager::drawActiveBorder(lgfx::LGFX_Device* display)
{
    for (int inset = 0; inset < 3; inset++)
    {
        display->drawRect(
            inset,
            inset,
            display->width() - (inset * 2),
            display->height() - (inset * 2),
            ThemeManager::current().accent
        );
    }
}

void DisplayManager::drawDisplay(
    lgfx::LGFX_Device* display,
    App* app,
    bool active
)
{
    app->draw(display);

    StatusBar::draw(display, app->getName(), true);

    if (active)
        drawActiveBorder(display);
}

void DisplayManager::redrawDisplays(
    lgfx::LGFX_Device* leftDisplay,
    lgfx::LGFX_Device* rightDisplay,
    App* leftApp,
    App* rightApp,
    int activeDisplay
)
{
    drawDisplay(leftDisplay, leftApp, activeDisplay == 0);
    drawDisplay(rightDisplay, rightApp, activeDisplay == 1);
}
