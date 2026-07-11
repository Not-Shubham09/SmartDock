#include "WiFiManager.h"

#include <WiFi.h>

void WiFiManager::begin(const char* wifiSsid, const char* wifiPassword)
{
    ssid = wifiSsid;
    password = wifiPassword;

    if (ssid == nullptr || ssid[0] == '\0')
    {
        connecting = false;
        return;
    }

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);

    connecting = true;
    lastReconnectAttempt = millis();
}

void WiFiManager::update()
{
    const unsigned long reconnectIntervalMs = 30000;

    if (ssid == nullptr || ssid[0] == '\0')
        return;

    if (WiFi.status() == WL_CONNECTED)
    {
        connecting = false;
        return;
    }

    connecting = true;

    unsigned long now = millis();

    if (now - lastReconnectAttempt >= reconnectIntervalMs)
    {
        WiFi.disconnect(false);
        WiFi.begin(ssid, password);
        lastReconnectAttempt = now;
    }
}

bool WiFiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiManager::isConnecting()
{
    return connecting && !isConnected();
}

const char* WiFiManager::getStatusText()
{
    if (ssid == nullptr || ssid[0] == '\0')
        return "Not configured";

    if (isConnected())
        return "Connected";

    if (isConnecting())
        return "Connecting";

    return "Disconnected";
}

IPAddress WiFiManager::getIPAddress()
{
    return WiFi.localIP();
}
