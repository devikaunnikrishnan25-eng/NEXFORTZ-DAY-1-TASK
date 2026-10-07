#include <WiFi.h>
#include <ArduinoOTA.h>
#include "secrets.h"

// ---------- OTA SETTINGS ----------
const char* OTA_HOSTNAME = "polyhouse-node-01";
const char* OTA_PASSWORD = "devu";

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("===========================");
  Serial.println("   NEXFORZ - DAY 6");
  Serial.println("   OTA UPDATE MODULE");
  Serial.println("===========================");
  Serial.println();

  // Connect to WiFi
  Serial.print("Connecting to WiFi");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");

  // Network information
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  // ---------- OTA CONFIGURATION ----------
  ArduinoOTA.setHostname(OTA_HOSTNAME);

  // Password protection
  ArduinoOTA.setPassword(OTA_PASSWORD);

  // Start OTA
  ArduinoOTA.begin();

  Serial.println("OTA ready!");
  Serial.print("Hostname: ");
  Serial.println(OTA_HOSTNAME);
  Serial.println("OTA password protection: ENABLED");
  Serial.println();
}

// ---------- LOOP ----------
void loop() {

  // Keep OTA service running
  ArduinoOTA.handle();

  Serial.println("ESP32 running - OTA UPDATE SUCCESSFUL");
  delay(5000);
}
