#include "Communication.h"
#include <Arduino.h>

void Communication::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n✅ WiFi connected successfully");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\n❌ Could not connect to WiFi");
        Serial.print("WiFi status: ");
        Serial.println(WiFi.status());
    }

}

bool Communication::isWifiConnected() {
    return WiFi.status() == WL_CONNECTED;
}