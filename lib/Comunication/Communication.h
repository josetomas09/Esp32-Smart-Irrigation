#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "../../include/config.h"
#include <WiFi.h>
#include <esp_now.h>

class Communication {
public:
    static void begin();
    static bool isWifiConnected();
};

#endif