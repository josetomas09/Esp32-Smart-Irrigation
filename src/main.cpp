#include "../../include/config.h"

#include <Arduino.h>
#include <BlynkSimpleEsp32.h>
#include <RTC.h>
#include <Relay.h>
#include <Sensor.h>
#include <Communication.h>


RTCManager rtc;
Sensor tempSensor(5, DHT11_SENSOR);
Sensor magneticSensor(4, MAGNETIC_SENSOR);
Relay relay1(13);
Relay relay2(14);

// ---------- Blynk config ----------
BlynkTimer timer;
int wateringTime;
bool isWatering = false;
// ---------- Blynk Functions ----------

void startWatering(){
    relay1.on();
    relay2.on();
    isWatering = true;
}

void stopWatering(){
    relay1.off();
    relay2.off();
    isWatering = false;
}

// Send Temperature(V2) and Humidity(V3)
void sendTempAndHum() {
    DHTData dhtData = tempSensor.readData();
    if (isnan(dhtData.temperature) || isnan(dhtData.humidity)) {
        Serial.println("Error reading DHT11 sensor");
        return;
    }

    Blynk.virtualWrite(V2, dhtData.temperature);
    Blynk.virtualWrite(V3, dhtData.humidity);

    Serial.print("Temperature: ");
    Serial.print(dhtData.temperature);
    Serial.print(" °C\t");
    Serial.print("Humidity: ");
    Serial.print(dhtData.humidity);
    Serial.println(" %");
}

// Manual waretring
BLYNK_WRITE(V1) {
    int estado = param.asInt();
    if (estado == 0) {
        startWatering();
        Serial.println("Manually Irrigation ✅");
    } else {
        stopWatering();
        Serial.println("Manually Irrigation ❌");
    }
}

// ---------- Logic for the Smart Irrigation ----------

BLYNK_WRITE(V0){
    wateringTime = param.asInt();
    Serial.print("Updated watering time: ");
    Serial.println(wateringTime);

}

void wateringControl(){
    DateTime now = rtc.now();
    int actualHour = now.hour();
    
    Serial.printf("Actual Hour: %02d:00 - Watering Hour: %02d:00\n", actualHour, wateringTime);

    if(actualHour == wateringTime){
        startWatering();
    }

}



void setup() {
    Serial.begin(115200);
    Serial.println("🚀 Starting System...");

    Communication::begin();

    delay(2000);


    if (Communication::isWifiConnected()) {
        Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);
        rtc.begin();
        rtc.syncWiFi();
    }

    timer.setInterval(10000L, sendTempAndHum);
}

void loop() {

    if (Communication::isWifiConnected()) {
        Blynk.run();
    }
    timer.run();

}