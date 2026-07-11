#pragma once

#include <LovyanGFX.hpp>

struct Notification
{
    char message[32];
    uint16_t color;
    int iconIdx;
    unsigned long durationMs;
};

class NotificationManager
{
private:
    enum class AnimState
    {
        IDLE,
        SLIDING_IN,
        SHOWING,
        SLIDING_OUT
    };

    struct QueueState
    {
        static const int MAX_QUEUE = 5;
        Notification queue[MAX_QUEUE];
        int head = 0;
        int tail = 0;
        int count = 0;

        bool push(const Notification& note);
        Notification pop();
        bool isEmpty() const;
    };

    struct ActiveAnim
    {
        AnimState state = AnimState::IDLE;
        Notification currentNote = { "", 0, 0, 0 };
        unsigned long stateStartTime = 0;
        int currentY = -36;
    };

    static QueueState queue1;
    static QueueState queue2;
    static ActiveAnim anim1;
    static ActiveAnim anim2;
    static unsigned long lastFrameTime;

    static void updateDisplayNotification(lgfx::LGFX_Device* display, QueueState& q, ActiveAnim& anim);
    static void drawNotification(lgfx::LGFX_Device* display, const ActiveAnim& anim);

public:
    static void postNotification(lgfx::LGFX_Device* display, const char* message, uint16_t color, int iconIdx, unsigned long durationMs = 2500);
    static void update();
    static bool isShowing(lgfx::LGFX_Device* display);
};
