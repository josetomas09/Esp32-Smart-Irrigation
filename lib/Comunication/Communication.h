#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "../../include/config.h"  // 🔥 Ruta relativa a communication.h
#include <Arduino.h>
#include <WiFi.h>


class Communication {
    public:
        static void begin();
        static bool connectWiFi();
        static bool disconnectWifi();
        static bool isWifiConnected();
        static void runBlynk();
        
    private:
        static void connectBlynk();  // 🔥 Lo dejamos privado, ya que solo se usa dentro de `begin()`
};

#endif
