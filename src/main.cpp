#include <Sensor.h>
#include <Relay.h>
#include <Communication.h>

Sensor tempSensor(5, DHT11_SENSOR);
Sensor magneticSensor(4, MAGNETIC_SENSOR);

Relay relay1(13);
Relay relay2(14);
Relay relay3(12);

void setup() {
    Serial.begin(115200);
    Communication::begin();
    delay(1000);
}

void loop() {

    Communication::runBlynk();

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

    delay(3000);

    relay1.turnOn();
    relay2.turnOn();
    relay3.turnOn();
    delay(2000);
    relay1.turnOff();
    relay2.turnOff();
    relay3.turnOff();

    delay(3000);

    Serial.print("WiFi connected: ");
    Serial.println(Communication::isWifiConnected() ? "True": "False");

    delay(1000);
}
