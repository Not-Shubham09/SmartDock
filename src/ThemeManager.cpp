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

void ThemeManager::setTheme(int themeIdx)
{
    if (themeIdx == 0)
    {
        // Dark Theme
        currentTheme = {
            TFT_BLACK,
            TFT_WHITE,
            TFT_CYAN,
            TFT_CYAN,
            TFT_ORANGE,
            TFT_GREEN,
            0x7BEF
        };
    }
    else if (themeIdx == 1)
    {
        // Light Theme
        currentTheme = {
            TFT_WHITE,
            TFT_BLACK,
            TFT_BLUE,
            0x7BEF, // border (dark gray)
            TFT_RED,
            TFT_GREEN,
            0xC618 // disabled (light gray)
        };
    }
    else if (themeIdx == 2)
    {
        // Orange Theme
        currentTheme = {
            TFT_BLACK,
            0xFD20, // TFT_ORANGE
            0xFD20,
            0xFD20,
            TFT_RED,
            TFT_GREEN,
            0x7BEF
        };
    }
}
