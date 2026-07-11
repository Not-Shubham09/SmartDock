#include "NotificationManager.h"
#include "IconManager.h"
#include "ThemeManager.h"
#include "FontManager.h"
#include "LGFX_Config.h"
#include "LGFX_Config2.h"

extern LGFX display1;
extern LGFX2 display2;
extern void redrawDisplays();

// QueueState implementations
bool NotificationManager::QueueState::push(const Notification& note)
{
    if (count >= MAX_QUEUE) return false;
    queue[tail] = note;
    tail = (tail + 1) % MAX_QUEUE;
    count++;
    return true;
}

Notification NotificationManager::QueueState::pop()
{
    if (count == 0) return { nullptr, 0, 0, 0 };
    Notification note = queue[head];
    head = (head + 1) % MAX_QUEUE;
    count--;
    return note;
}

bool NotificationManager::QueueState::isEmpty() const
{
    return count == 0;
}

// Static members initialization
NotificationManager::QueueState NotificationManager::queue1;
NotificationManager::QueueState NotificationManager::queue2;
NotificationManager::ActiveAnim NotificationManager::anim1;
NotificationManager::ActiveAnim NotificationManager::anim2;
unsigned long NotificationManager::lastFrameTime = 0;

void NotificationManager::postNotification(lgfx::LGFX_Device* display, const char* message, uint16_t color, int iconIdx, unsigned long durationMs)
{
    Notification note = { message, color, iconIdx, durationMs };
    if (display == (lgfx::LGFX_Device*)&display1)
    {
        queue1.push(note);
    }
    else if (display == (lgfx::LGFX_Device*)&display2)
    {
        queue2.push(note);
    }
}

bool NotificationManager::isShowing(lgfx::LGFX_Device* display)
{
    if (display == (lgfx::LGFX_Device*)&display1)
    {
        return anim1.state != AnimState::IDLE;
    }
    else
    {
        return anim2.state != AnimState::IDLE;
    }
}

void NotificationManager::update()
{
    unsigned long now = millis();
    
    // Limit notification updates to 30 FPS to save CPU / SPI bandwidth
    // Render immediately if animating just started (lastFrameTime == 0)
    if (lastFrameTime != 0 && now - lastFrameTime < 33)
    {
        return;
    }
    lastFrameTime = now;

    updateDisplayNotification(&display1, queue1, anim1);
    updateDisplayNotification(&display2, queue2, anim2);
}

void NotificationManager::updateDisplayNotification(lgfx::LGFX_Device* display, QueueState& q, ActiveAnim& anim)
{
    unsigned long now = millis();
    
    switch (anim.state)
    {
        case AnimState::IDLE:
            if (!q.isEmpty())
            {
                anim.currentNote = q.pop();
                anim.state = AnimState::SLIDING_IN;
                anim.stateStartTime = now;
                anim.currentY = -36;
                drawNotification(display, anim);
            }
            break;
            
        case AnimState::SLIDING_IN:
            {
                float p = (float)(now - anim.stateStartTime) / 300.0f; // 300ms slide-in
                if (p >= 1.0f)
                {
                    anim.currentY = 0;
                    anim.state = AnimState::SHOWING;
                    anim.stateStartTime = now;
                }
                else
                {
                    anim.currentY = -36 + (int)(36 * p);
                }
                drawNotification(display, anim);
            }
            break;
            
        case AnimState::SHOWING:
            anim.currentY = 0;
            if (now - anim.stateStartTime >= anim.currentNote.durationMs)
            {
                anim.state = AnimState::SLIDING_OUT;
                anim.stateStartTime = now;
            }
            else
            {
                drawNotification(display, anim);
            }
            break;
            
        case AnimState::SLIDING_OUT:
            {
                float p = (float)(now - anim.stateStartTime) / 300.0f; // 300ms slide-out
                if (p >= 1.0f)
                {
                    anim.currentY = -36;
                    anim.state = AnimState::IDLE;
                    anim.currentNote = { nullptr, 0, 0, 0 };
                    
                    // Clear the notification area by redrawing the screen
                    redrawDisplays();
                }
                else
                {
                    anim.currentY = (int)(-36 * p);
                    drawNotification(display, anim);
                }
            }
            break;
    }
}

void NotificationManager::drawNotification(lgfx::LGFX_Device* display, const ActiveAnim& anim)
{
    if (anim.state == AnimState::IDLE || anim.currentNote.message == nullptr)
        return;

    const Theme& theme = ThemeManager::current();

    // 1. Set clipping to the app layout area just below status bar (Y = 18..54)
    display->setClipRect(10, 18, 220, 32);

    // Redraw what is underneath to prevent double-drawing trails when sliding
    // Draw background
    display->fillRect(15, 18, 210, 32, theme.background);

    // 2. Draw floating notification box (rounded rectangle capsule)
    // fill inside capsule with alert type color (success/warning/accent)
    display->fillRoundRect(15, 18 + anim.currentY, 210, 30, 6, anim.currentNote.color);
    display->drawRoundRect(15, 18 + anim.currentY, 210, 30, 6, theme.border);

    // 3. Draw Icon centered vertically
    int iconX = 25;
    int iconY = 18 + anim.currentY + 11;
    uint16_t iconColor = theme.foreground;
    
    if (anim.currentNote.iconIdx == 0)
        IconManager::drawWiFi(display, iconX, iconY, iconColor, true);
    else if (anim.currentNote.iconIdx == 1)
        IconManager::drawBattery(display, iconX, iconY, iconColor, 100);
    else if (anim.currentNote.iconIdx == 2)
        IconManager::drawClock(display, iconX, iconY, iconColor);
    else if (anim.currentNote.iconIdx == 3)
        IconManager::drawSettings(display, iconX, iconY, iconColor);
    else if (anim.currentNote.iconIdx == 4)
        IconManager::drawQuote(display, iconX, iconY, iconColor);

    // 4. Draw message string
    display->setFont(FontManager::getSmallFont());
    display->setTextColor(theme.foreground);
    display->drawString(anim.currentNote.message, 45, 18 + anim.currentY + 11);

    // 5. Clear clipping rectangle
    display->clearClipRect();
}
