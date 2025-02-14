#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "../../include/config.h"
#include <WiFi.h>


class Communication {
    public:
        static void begin();
        static bool connectWiFi();
        static bool disconnectWifi();
        static bool isWifiConnected();
        static void runBlynk();
        
    private:
        static void connectBlynk();
};

#endif
