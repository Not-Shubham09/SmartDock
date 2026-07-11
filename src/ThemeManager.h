#pragma once

#include "Theme.h"

class ThemeManager
{
private:
    static Theme currentTheme;

public:
    static const Theme& current();
    static void setTheme(const Theme& theme);
    static void setTheme(int themeIdx);
};
