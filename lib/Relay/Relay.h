#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay {
   public:
    Relay(uint8_t pin);
    void on();
    void off();
    bool isActive();

   protected:
    uint8_t pin;
    bool state;
};

#endif