#pragma once

#include <Arduino.h>
#include <IPAddress.h>

class WiFiManager
{
private:
    const char* ssid = nullptr;
    const char* password = nullptr;

    bool connecting = false;
    unsigned long lastReconnectAttempt = 0;

public:
    void begin(const char* wifiSsid, const char* wifiPassword);

    void update();

    bool isConnected();

    bool isConnecting();

    const char* getStatusText();

    IPAddress getIPAddress();
};
