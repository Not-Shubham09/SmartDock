#pragma once

#include <LovyanGFX.hpp>
#include "App.h"

enum class AnimationType
{
    SLIDE_LEFT,
    SLIDE_RIGHT,
    FADE,
    PUSH
};

class AnimationManager
{
private:
    lgfx::LGFX_Device* activeDisplay;
    App* oldApp;
    App* newApp;
    AnimationType type;
    unsigned long startTime;
    unsigned long duration;
    unsigned long lastFrameTime;
    bool animating;

public:
    AnimationManager();
    
    void startAnimation(lgfx::LGFX_Device* display, App* oldApp, App* newApp, AnimationType type, unsigned long durationMs = 350);
    void update();
    bool isAnimating() const;
};
