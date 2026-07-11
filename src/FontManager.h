#pragma once

#include <LovyanGFX.hpp>

class FontManager
{
public:
    static const lgfx::IFont* getSmallFont();
    static const lgfx::IFont* getMediumFont();
    static const lgfx::IFont* getLargeFont();
};
