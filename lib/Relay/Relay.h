#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay {
   public:
    Relay(uint8_t pin);
    void turnOn();
    void turnOff();
    bool isActive();

   protected:
    uint8_t pin;
    bool state;
};

#endif