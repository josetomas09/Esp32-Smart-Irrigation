#include <esp_now.h>
#include <WiFi.h>

#include "const.h"

/*
  ################################################
    ESP32 Mini - Sender
  ################################################
*/

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void SendMessage();
void static RegisterPeeks();
void static InitEspNow();


void setup() {
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);

  // #### Debug MAC Address ####
  Serial.println(WiFi.macAddress());
  WiFi.macAddress(MAC_RECEIVER);
  Serial.print("Receiver MAC: ");
  for(int i = 0; i < 6; i++) {
    Serial.printf("%02X", MAC_RECEIVER[i]);
    if(i < 5) Serial.print(":");
  }
  // ###########################

  delay(1000);
  InitEspNow();
}


void loop() {
	SendMessage();

	delayMicroseconds(200);
}



void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
	Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void SendMessage() {
	// TODO: Change the data to send
	
	/*Example:
	float myFloat = 123.456;
	uint8_t payload[sizeof(float)];
	memcpy(payload, &myFloat, sizeof(float));

	*/
	
	uint8_t payload;

	esp_err_t result = esp_now_send(MAC_RECEIVER, (uint8_t*)&payload, sizeof(float));

	if(result == ESP_OK) {
		Serial.println("Sent with success");
	}else {
		Serial.println("Error sending the data");
	}
}

void static RegisterPeeks() {
	esp_now_peer_info_t peerInfo;
	memcpy(peerInfo.peer_addr, MAC_RECEIVER, 6);
	peerInfo.channel = 0;
	peerInfo.encrypt = false;

	if(esp_now_add_peer(&peerInfo) != ESP_OK) {
		Serial.println("Failed to add peer");
	} else {
		Serial.print("Registered peer ");
	}
}

void static InitEspNow() {
	if(esp_now_init() != ESP_OK) {
		Serial.println("Error initializing ESP-NOW");
	} else {
		esp_now_register_send_cb(OnDataSent);

		RegisterPeeks();
	}
}