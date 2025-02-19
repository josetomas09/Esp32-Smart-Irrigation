#ifndef RTC_H
#define RTC_H

#include <Communication.h>
#include <RTClib.h>
#include <Wire.h>
#include <time.h>

#include "../../include/config.h"

class RTCManager {
   public:
    RTCManager();
    void begin();
    bool syncWiFi();
    void printDateTime();
    bool lostPower();
    DateTime now();

   private:
    RTC_DS3231 rtc;
};

#endif