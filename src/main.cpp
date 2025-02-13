#include <Sensor.h>
#include <Relay.h>

Sensor tempSensor(5, DHT11_SENSOR);              // Sensor type DHT11 in GPIO 5
Sensor magneticSensor(4, MAGNETIC_SENSOR);       // Sensor type MAGNETIC in GPIO 4

Relay relay1(13);
Relay relay2(14);
Relay relay3(12);                               // This GPIO (12) has a lot of problems when I upload the program, I read somewhere on the internet that it is quite common.

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
    
    delay(3000);

    relay1.turnOn();
    relay2.turnOn();
    relay3.turnOn();
    delay(2000);
    relay1.turnOff();
    relay2.turnOff();
    relay3.turnOff();
    
    delay(1000);
}