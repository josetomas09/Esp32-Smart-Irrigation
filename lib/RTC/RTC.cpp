#include "RTC.h"
#include <Communication.h>
#include "../../include/config.h"  // Include config.h for acces to WIFI_SSID and WIFI_PASSWORD

RTCManager::RTCManager() {}

void RTCManager::begin() {
    if (!rtc.begin()) {
        Serial.println("⚠️ RTC DS3231 module not found");
        return;
    };

    if (rtc.lostPower()) {
        Serial.println("⚠️ The RTC lost power, synchronizing...");
        syncWiFi();
    };
};

bool RTCManager::syncWiFi() {
    if (!Communication::isWifiConnected()) {
        Serial.println("No WiFi connected. Trying to connect...");
        Communication::begin();
        delay(2000);

        if (!Communication::isWifiConnected()) {
            Serial.println("❌ Could not connect to WiFi");
            return false;
        }
    }

    configTime(gmtOffset_SEC, 0, ntpServer);
    delay(2000);

    struct tm timeInfo;
    int retries = 0;
    while (!getLocalTime(&timeInfo) && retries < 10) {
        Serial.println("Syncing NTP...");
        delay(500);
        retries++;
    };

    if (retries >= 10) {
        Serial.println("Error: Could not sync time after multiple attempts.");
        return false;
    };

    // Save the time in the RTC DS3231.
    int ye = timeInfo.tm_year + 1900;
    int mo = timeInfo.tm_mon + 1;
    int da = timeInfo.tm_mday;

    int ho = timeInfo.tm_hour;
    int mi = timeInfo.tm_min;
    int se = timeInfo.tm_sec;

    rtc.adjust(DateTime(ye, mo, da, ho, mi, se));

    Serial.println("RTC synchronized with NTP");
    return true;
};

void RTCManager::printDateTime() {
    DateTime now = rtc.now();
    Serial.printf("RTC Date: %02d-%02d-%04d  Time: %02d:%02d:%02d\n", now.day(),
                  now.month(), now.year(), now.hour(), now.minute(),
                  now.second());
};

DateTime RTCManager::now() {  
    return rtc.now();  
}

bool RTCManager::lostPower() { return rtc.lostPower(); };
