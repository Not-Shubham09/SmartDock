#include "FontManager.h"

const lgfx::IFont* FontManager::getSmallFont()
{
    return &fonts::Font0;
}

const lgfx::IFont* FontManager::getMediumFont()
{
    return &fonts::Font2;
}

const lgfx::IFont* FontManager::getLargeFont()
{
    return &fonts::Font4;
}
