#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>

enum SensorType{
    DHT11_SENSOR,
    MAGNETIC_SENSOR
};

struct DHTData{
    float temperature;
    float humidity;
};


class Sensor{
    public:
        Sensor(uint8_t pin, SensorType type);
    
        DHTData readData();
        bool isTriggered();
        uint8_t getPin() const;
    
    protected:
        uint8_t pin;
        SensorType type;
        DHT dht;
};

#endif