#ifndef CONFIG_H
#define CONFIG_H

#define BLYNK_TEMPLATE_ID "TMPL2_1d50D2s"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation Garaje"
#define BLYNK_AUTH_TOKEN "FFpWXZbVi_lb8A2TbVkcSf-ie1Dv1SWi"

// WiFi Credentials
#define WIFI_SSID "TP-Link_CF0E"
#define WIFI_PASSWORD "garaje2023"

// NTP Config
#define ntpServer "ar.pool.ntp.org"
const long gmtOffset_SEC = -3 * 3600;

// ESPNOW Config
const uint8_t MAC_SENDER_1[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t MAC_RECEIVER_1[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#endif