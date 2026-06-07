#pragma once

#include "App.h"

class ClockApp : public App
{
public:
    const char* getName() override;

    void draw(lgfx::LGFX_Device* display) override;
};