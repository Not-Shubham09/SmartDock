#pragma once

#include <LovyanGFX.hpp>

#include "App.h"

class DisplayManager
{
private:
    void drawActiveBorder(lgfx::LGFX_Device* display);

public:
    void drawDisplay(lgfx::LGFX_Device* display, App* app, bool active);

    void redrawDisplays(
        lgfx::LGFX_Device* leftDisplay,
        lgfx::LGFX_Device* rightDisplay,
        App* leftApp,
        App* rightApp,
        int activeDisplay
    );
};
