#pragma once

#include <LovyanGFX.hpp>

class StatusBar
{
private:
    struct DisplayState
    {
        bool wifiConnected;
        int lastMinute;
        const char* lastAppName;
    };
    static DisplayState state1;
    static DisplayState state2;

public:
    static void draw(lgfx::LGFX_Device* display, const char* appName, bool forceRedraw = false);
};
