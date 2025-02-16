#include "Sensor.h"

Sensor::Sensor(uint8_t pin, SensorType type)
    : pin(pin), type(type), dht(pin, DHT11) {
    pinMode(pin, INPUT);
    if (type == DHT11_SENSOR) {
        dht.begin();
    };
};

DHTData Sensor::readData() {
    // Default values: -1.0 means an error in reading
    DHTData data = {-1.0, -1.0};

    if (type == DHT11_SENSOR) {
        float temp = dht.readTemperature();
        float humidity = dht.readHumidity();

        if (!isnan(temp) && !isnan(humidity)) {
            data.temperature = temp;
            data.humidity = humidity;
        };
    };

    return data;
};

bool Sensor::isTriggered() {
    if (type == MAGNETIC_SENSOR) {
        return digitalRead(pin) == LOW;
    };
    return false;
};