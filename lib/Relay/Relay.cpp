#include "Relay.h"

Relay::Relay(uint8_t pin) : pin(pin), state(false) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
};

void Relay::on() {
    digitalWrite(pin, HIGH);
    state = true;
};

void Relay::off() {
    digitalWrite(pin, LOW);
    state = false;
};

bool Relay::isActive() { return state; };