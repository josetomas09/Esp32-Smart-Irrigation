#include "Communication.h"
#include <BlynkSimpleEsp32.h>

void Communication::begin() {
    connectWiFi();
    connectBlynk();
}

bool Communication::connectWiFi(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi...");

    while (WiFi.status() != WL_CONNECTED){
        Serial.println("❌ Error.");
        return false;
    }

    Serial.println("✅ Connected.");
    return true;
}

bool Communication::disconnectWifi() {
    WiFi.disconnect();
    return true;
}

bool Communication::isWifiConnected(){
    return WiFi.status() == WL_CONNECTED;
}

void Communication::connectBlynk() {
    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);

}

void Communication::runBlynk() {
    Blynk.run();
}