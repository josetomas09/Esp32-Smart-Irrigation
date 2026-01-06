#include "../include/config.h"

#include <Arduino.h>

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h> 

#include <Wire.h>
#include <RTC.h>
#include <communication.h>
#include <DHT.h>
#include <RTClib.h>
#include <Relay.h>
#include <IrrigationController.h>

/* 
    ========================================================
                2. Global Variables and Objects
    ========================================================
*/

uint8_t DHTPIN = 5;
uint8_t RELAY1_PIN = 13;
uint8_t RELAY2_PIN = 15;
uint8_t RTC_SDA_PIN = 21;
uint8_t RTC_SCL_PIN = 22;


DHT dht(5, DHT11); 
RTC_DS3231 rtc;
BlynkTimer timer;

// Relays de riego
Relay electrovalvula_1(RELAY1_PIN, "Valvula Zona 1");
Relay electrovalvula_2(RELAY2_PIN, "Válvula Zona 2");

// Controlador de riego
IrrigationController riego(&rtc);

// Declaraciones de funciones
bool rtcNTPsync();
void sendTempAndHum();

/* 
    ========================================================
                3. Setup and Loop
    ========================================================
*/

void setup(){
    Serial.begin(115200);
    Serial.println("Iniciando...");

    Communication::begin();
    dht.begin();

    Wire.begin(RTC_SDA_PIN, RTC_SCL_PIN);
    
    
    riego.addIrrigationRelay(&electrovalvula_1);
    riego.addIrrigationRelay(&electrovalvula_2);
    riego.begin();

    if(Communication::isWifiConnected()){
        Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);
        delay(1000);
        rtcNTPsync();
    }
    
    timer.setInterval(2000L, sendTempAndHum);
    timer.setInterval(60000L, []() { riego.update(); });
}

void loop(){
    if(Communication::isWifiConnected()){
         Blynk.run();
    }
    timer.run();
}

/* 
    ========================================================
                3. Helper Functions
    ========================================================
*/

bool rtcNTPsync(){
    
    configTime(gmtOffset_SEC, 0, ntpServer);
    delay(2000);

    struct tm timeInfo;
    uint8_t retries = 0;
    while(!getLocalTime(&timeInfo) && retries < 20) {
        Serial.println("Sincronizando NTP...");
        delay(1000);
        retries++;
    };

    if(retries >= 20) {
        Serial.println("Error: No se pudo sincronizar fecha y hora luego de varios intentos.");
        return false;
    }

    // Guardar la fecha y hora en RTC DS3231.
    uint16_t ye = timeInfo.tm_year + 1900;
    uint8_t mo = timeInfo.tm_mon + 1;
    uint8_t da = timeInfo.tm_mday;

    uint8_t ho = timeInfo.tm_hour;
    uint8_t mi = timeInfo.tm_min;
    uint8_t se = timeInfo.tm_sec;

    rtc.adjust(DateTime(ye, mo, da, ho, mi, se));

    Serial.println("RTC sincronizado con NTP");
    return true;
}

/* 
    ========================================================
                4. Blynk Functions
    ========================================================
*/

void sendTempAndHum(){
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    if(isnan(temp) || isnan(humidity)) {
        Serial.println("Error leyendo sensor DHT11");
        return;
    }



    Serial.print("Temp: "); Serial.print(temp);
    Serial.print(" Hum: "); Serial.println(humidity);

    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, humidity);
}

// Riego manual
BLYNK_WRITE(V2) {
    int v = param.asInt();
    if(v == 1) {
        riego.enableManualMode();
    } else {
        riego.disableManualMode();
    }
}


BLYNK_WRITE(V3) {
    TimeInputParam t(param);
    
    riego.clearSchedule();

    // 1. Configurar horario de inicio
    if(t.hasStartTime()) {
        long startSecs = t.getStartHour() * 3600 + t.getStartMinute() * 60 + t.getStartSecond();
        long stopSecs = -1;
        
        // 2. Configurar horario de fin
        if(t.hasStopTime()) {
            stopSecs = t.getStopHour() * 3600 + t.getStopMinute() * 60 + t.getStopSecond();
        }
        
        if(stopSecs != -1) {
            riego.setSchedule(startSecs, stopSecs);
        }
    } else if(t.isStartSunrise()) {
        riego.setStartAtSunrise(true);
    } else if(t.isStartSunset()) {
        riego.setStartAtSunset(true);
    }

    // Configurar fin en sunrise/sunset si aplica
    if(t.isStopSunrise()) {
        riego.setStopAtSunrise(true);
    } else if(t.isStopSunset()) {
        riego.setStopAtSunset(true);
    }

    // 3. Configurar días activos
    for(int i = 1; i <= 7; i++) {
        riego.setDayActive(i, t.isWeekdaySelected(i));
    }
    
    Serial.println("Nueva programación recibida");
}

BLYNK_READ(V4) {
    Blynk.virtualWrite(V4, Communication::recivedChar);
}