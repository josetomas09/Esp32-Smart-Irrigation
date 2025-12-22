#include "../include/config.h"

#include <Arduino.h>
#include <Blynk.h>
#include <RTC.h>
#include <communication.h>
#include <DHT.h>
#include <Relay.h>
#include <RTClib.h>

relay relay1(13);
relay relay2(14);
DHT dhtSensor(5, DHT11);
RTC_DS3231 rtc;

/* 
    ========================================================
            1. Blynk Config and Functions
    ========================================================
*/

BlynkTimer timer;
uint8_t wateringTime;  // in minutes
bool isWatering = false;

void relay();
bool rtcNTPsync();
void startWatering();
void stopWatering();
void sendTempAndHum();
void wateringControl();


/*
    ========================================================
            2. Setup and Loop
    ========================================================
*/
void setup(){
    Serial.begin(115200);
    Serial.println("Starting...");

    Communication::begin();
    dht.begin();

    // Initialize I2C for RTC DS3231
    Wire.begin(21 /* SDA */, 22 /* SCL */);
    if (!rtc.begin()) {
        Serial.println("⚠️ RTC DS3231 module not found");
        return;
    };

    delay(2000);

    if (Communication::isWifiConnected){
        Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);
        BlynkDelay(1000);
        rtcNTPsync();
    }
    

}

void loop(){
    if(Communication::isWifiConnected){
        Blynk.run();
    }
    timer.run();
}

/* 
    ========================================================
            3. Blynk Functions Implementation
    ========================================================
*/

void startWatering(){
    relay1.on();
    relay2.on();
    isWatering = true;
    Serial.println("Irrigation Started");
}

void stopWatering(){
    relay1.off();
    relay2.off();
    isWatering = false;
    Serial.println("Irrigation Stopped");
}

void relay(){
    relay::Relay(uint8_t pin) : pin(pin), state(false) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
};

void relay::on() {
    digitalWrite(pin, HIGH);
    state = true;
};

void relay::off() {
    digitalWrite(pin, LOW);
    state = false;
};

bool relay::isActive() { return state; };

}

bool rtcNTPsync(){

    configTime(gmtOffset_SEC, 0, ntpServer);
    delay(2000);

    struct tm timeInfo;
    uint8_t retries = 0;
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
    uint16_t ye = timeInfo.tm_year + 1900;
    uint8_t mo = timeInfo.tm_mon + 1;
    uint8_t da = timeInfo.tm_mday;

    uint8_t ho = timeInfo.tm_hour;
    uint8_t mi = timeInfo.tm_min;
    uint8_t se = timeInfo.tm_sec;

    rtc.adjust(DateTime(ye, mo, da, ho, mi, se));

    Serial.println("RTC synchronized with NTP");
    return true;

};

void sendTempAndHum(){
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    BlynkDelay(1000);

    if (isnan(temp) || isnan(humidity)) {
        Serial.println("Error reading DHT11 sensor");
        return;
    }

    BlynkDelay(500);
    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, humidity);
}