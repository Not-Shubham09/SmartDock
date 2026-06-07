#pragma once

#include <LovyanGFX.hpp>

class App
{
public:
    virtual const char* getName() = 0;

    virtual void draw(lgfx::LGFX_Device* display) = 0;

    virtual void update(lgfx::LGFX_Device* display) {}

    virtual void onEncoder(int direction) {}

    virtual void onClick() {}

    virtual ~App() {}
};