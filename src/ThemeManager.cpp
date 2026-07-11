#include "ThemeManager.h"
#include <LovyanGFX.hpp>

// Initialize static member with a default dark theme
Theme ThemeManager::currentTheme = {
    TFT_BLACK,  // background
    TFT_WHITE,  // foreground
    TFT_CYAN,   // accent
    TFT_CYAN,   // border
    TFT_ORANGE, // warning
    TFT_GREEN,  // success
    0x7BEF      // disabled (dark gray)
};

const Theme& ThemeManager::current()
{
    return currentTheme;
}

void ThemeManager::setTheme(const Theme& theme)
{
    currentTheme = theme;
}
