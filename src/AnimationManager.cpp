#include "AnimationManager.h"
#include "DisplayManager.h"
#include "ThemeManager.h"
#include "LGFX_Config.h"
#include "LGFX_Config2.h"

extern LGFX display1;
extern LGFX2 display2;
extern int selectedDisplay;
extern DisplayManager displayManager;

AnimationManager::AnimationManager()
{
    activeDisplay = nullptr;
    oldApp = nullptr;
    newApp = nullptr;
    type = AnimationType::SLIDE_LEFT;
    startTime = 0;
    duration = 350;
    lastFrameTime = 0;
    animating = false;
}

void AnimationManager::startAnimation(lgfx::LGFX_Device* display, App* old, App* newAp, AnimationType t, unsigned long durationMs)
{
    activeDisplay = display;
    oldApp = old;
    newApp = newAp;
    type = t;
    duration = durationMs;
    startTime = millis();
    lastFrameTime = 0;
    animating = true;
    
    // Draw initial frame immediately to reduce latency
    update();
}

bool AnimationManager::isAnimating() const
{
    return animating;
}

void AnimationManager::update()
{
    if (!animating || activeDisplay == nullptr)
        return;

    unsigned long now = millis();
    
    // Enforce 30 FPS target (at most one frame every 33ms)
    // Always render the first frame (lastFrameTime == 0) and the final frame (p >= 1.0f)
    float p = (float)(now - startTime) / duration;
    
    if (p < 1.0f && lastFrameTime != 0 && now - lastFrameTime < 33)
    {
        return;
    }
    
    lastFrameTime = now;

    if (p >= 1.0f)
    {
        animating = false;
        
        // Render final screen state
        bool isLeft = (activeDisplay == (lgfx::LGFX_Device*)&display1);
        bool isActive = (isLeft && selectedDisplay == 0) || (!isLeft && selectedDisplay == 1);
        
        displayManager.drawDisplay(activeDisplay, newApp, isActive);
        return;
    }

    const Theme& theme = ThemeManager::current();

    if (type == AnimationType::SLIDE_LEFT)
    {
        int splitX = 240 * (1.0f - p);
        
        // 1. Draw old app on left side
        if (splitX > 0)
        {
            activeDisplay->setClipRect(0, 18, splitX, 302);
            oldApp->draw(activeDisplay);
        }
        
        // 2. Draw new app on right side
        if (240 - splitX > 0)
        {
            activeDisplay->setClipRect(splitX, 18, 240 - splitX, 302);
            newApp->draw(activeDisplay);
        }
        
        activeDisplay->clearClipRect();
    }
    else if (type == AnimationType::SLIDE_RIGHT)
    {
        int splitX = 240 * p;
        
        // 1. Draw new app on left side
        if (splitX > 0)
        {
            activeDisplay->setClipRect(0, 18, splitX, 302);
            newApp->draw(activeDisplay);
        }
        
        // 2. Draw old app on right side
        if (240 - splitX > 0)
        {
            activeDisplay->setClipRect(splitX, 18, 240 - splitX, 302);
            oldApp->draw(activeDisplay);
        }
        
        activeDisplay->clearClipRect();
    }
    else if (type == AnimationType::PUSH)
    {
        // Push: animate vertical slide down
        int splitY = 18 + (302 * (1.0f - p));
        
        // 1. Draw old app on top section
        if (splitY > 18)
        {
            activeDisplay->setClipRect(0, 18, 240, splitY - 18);
            oldApp->draw(activeDisplay);
        }
        
        // 2. Draw new app on bottom section
        if (320 - splitY > 0)
        {
            activeDisplay->setClipRect(0, splitY, 240, 320 - splitY);
            newApp->draw(activeDisplay);
        }
        
        activeDisplay->clearClipRect();
    }
    else if (type == AnimationType::FADE)
    {
        // Fade to black and reveal transition
        activeDisplay->setClipRect(0, 18, 240, 302);
        
        if (p < 0.5f)
        {
            // Draw old app + growing black bars
            float f = p * 2.0f;
            oldApp->draw(activeDisplay);
            
            // 10 horizontal bars of height 30
            for (int i = 0; i < 10; i++)
            {
                activeDisplay->fillRect(0, 18 + i * 30, 240, 30 * f, theme.background);
            }
        }
        else
        {
            // Draw new app + shrinking black bars
            float f = (1.0f - p) * 2.0f;
            newApp->draw(activeDisplay);
            
            for (int i = 0; i < 10; i++)
            {
                activeDisplay->fillRect(0, 18 + i * 30, 240, 30 * f, theme.background);
            }
        }
        
        activeDisplay->clearClipRect();
    }
}
