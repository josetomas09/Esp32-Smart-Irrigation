#include "RTC.h"

RTCManager::RTCManager() {}

void RTCManager::begin() {
    if (!rtc.begin()) {
        Serial.println("⚠️ No se encontró el módulo RTC DS3231");
        return;
    };

    if (rtc.lostPower()) {
        Serial.println("⚠️ El RTC perdió energía, sincronizando...");
        syncWiFi();
    };
};

bool RTCManager::syncWiFi() {
    Communication::connectWiFi();
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
        Communication::disconnectWifi();
        return false;
    };

    // Save the Date and Time on RTC DS3231.
    int ye = timeInfo.tm_year + 1900;
    int mo = timeInfo.tm_mon + 1;
    int da = timeInfo.tm_mday;

    int ho = timeInfo.tm_hour;
    int mi = timeInfo.tm_min;
    int se = timeInfo.tm_sec;

    delay(100);

    rtc.adjust(DateTime(ye, mo, da, ho, mi, se));

    Serial.println("RTC synchronized with NTP");
    return true;
};

void RTCManager::printDateTime() {
    DateTime now = rtc.now();

    Serial.printf("RTC Date: %02d-%02d-%04d  Time: %02d:%02d:%02d\n", now.day(),
                  now.month(), now.year(), now.hour(), now.minute(),
                  now.second());

    struct tm timeInfo;
    if (getLocalTime(&timeInfo)) {
        Serial.printf("NTP Date: %02d-%02d-%04d  Time: %02d:%02d:%02d\n", ye,
                      mo, da, ho, mi, se);
    } else {
        Serial.println("Failed to get NTP time");
    }
};

bool RTCManager::lostPower() { return rtc.lostPower(); };