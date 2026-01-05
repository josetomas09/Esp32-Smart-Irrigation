#include "Relay.h"

Relay::Relay(uint8_t p, String relayName) 
    : pin(p), state(false), name(relayName) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Relay::on() {
    digitalWrite(pin, HIGH);
    state = true;
    if (name != "") {
        Serial.print("Relay '");
        Serial.print(name);
        Serial.println("' encendido");
    }
}

void Relay::off() {
    digitalWrite(pin, LOW);
    state = false;
    if (name != "") {
        Serial.print("Relay '");
        Serial.print(name);
        Serial.println("' apagado");
    }
}

void Relay::toggle() {
    if (state) {
        off();
    } else {
        on();
    }
}

bool Relay::isActive() const {
    return state;
}