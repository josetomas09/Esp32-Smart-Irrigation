#include <Sensor.h>

Sensor tempSensor(5, DHT11_SENSOR);              // Sensor type DHT11 in GPIO 5
Sensor magneticSensor(4, MAGNETIC_SENSOR);       // Sensor type MAGNETIC in GPIO 4

void setup() {
    Serial.begin(115200);
    delay(1000);
}

void loop() {
    DHTData dhtData = tempSensor.readData();
    
    if (dhtData.temperature != -1.0 && dhtData.humidity != -1.0) {
        Serial.println("Temperatura: " + String(dhtData.temperature) + " °C");
        Serial.println("Humedad: " + String(dhtData.humidity) + " %");
    } else {
        Serial.println("Error al leer el sensor DHT11.");
    }

    if(magneticSensor.isTriggered() == HIGH){
        Serial.println("Porton ABIERTO");
    }else{
        Serial.println("Porton CERRADO");
    }

    delay(2000);
}