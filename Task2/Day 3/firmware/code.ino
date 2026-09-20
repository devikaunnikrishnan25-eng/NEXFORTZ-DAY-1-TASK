#include <WiFi.h>
#include <time.h>
#include "secrets.h"

// ---------- WiFi Settings ----------
const unsigned long CONNECT_TIMEOUT = 10000;       // 10 seconds
const unsigned long HEALTH_CHECK_INTERVAL = 5000; // 5 seconds

const unsigned long BACKOFF_DELAYS[] = {
  2000,   // 2 seconds
  4000,   // 4 seconds
  8000,   // 8 seconds
  16000,  // 16 seconds
  30000   // 30 seconds
};

const int MAX_BACKOFF_LEVEL = 4;

// ---------- NTP Settings ----------
const char* NTP_SERVER = "pool.ntp.org";
const long GMT_OFFSET_SEC = 19800;   // IST = UTC + 5:30
const int DAYLIGHT_OFFSET_SEC = 0;

// ---------- State Variables ----------
unsigned long connectStartTime = 0;
unsigned long lastHealthCheck = 0;
unsigned long nextRetryTime = 0;

int backoffLevel = 0;
bool connecting = false;
bool wasConnected = false;

// ---------- NTP Time Sync ----------
void syncTime() {
  Serial.println("[NTP] Synchronizing time...");

  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);

  struct tm timeinfo;

  if (getLocalTime(&timeinfo, 10000)) {
    Serial.println("[NTP] Time synchronized.");

    char timeString[30];
    strftime(timeString, sizeof(timeString),
             "%Y-%m-%dT%H:%M:%S%z", &timeinfo);

    Serial.print("[NTP] Current time: ");
    Serial.println(timeString);
  } 
  else {
    Serial.println("[NTP] Time synchronization failed.");
  }
}

// ---------- Start WiFi Connection ----------
void startWiFiConnection() {
  Serial.println();
  Serial.println("[WIFI] Starting connection attempt...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  connectStartTime = millis();
  connecting = true;
}

// ---------- Handle WiFi Connection ----------
void handleWiFiConnection() {

  if (connecting) {

    if (WiFi.status() == WL_CONNECTED) {

      connecting = false;
      wasConnected = true;
      backoffLevel = 0;

      Serial.println();
      Serial.println("[WIFI] Connected!");

      Serial.print("[WIFI] IP Address: ");
      Serial.println(WiFi.localIP());

      Serial.print("[WIFI] RSSI: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");

      Serial.print("[WIFI] Channel: ");
      Serial.println(WiFi.channel());

      Serial.print("[WIFI] BSSID: ");
      Serial.println(WiFi.BSSIDstr());

      Serial.println("[WIFI] Connection successful.");
      Serial.println();

      // Synchronize time after WiFi connection
      syncTime();
    }

    else if (millis() - connectStartTime >= CONNECT_TIMEOUT) {

      connecting = false;

      Serial.println();
      Serial.println("[WIFI] Connection timeout.");

      unsigned long retryDelay = BACKOFF_DELAYS[backoffLevel];

      Serial.print("[WIFI] Retrying in ");
      Serial.print(retryDelay / 1000);
      Serial.println(" seconds...");

      nextRetryTime = millis() + retryDelay;

      if (backoffLevel < MAX_BACKOFF_LEVEL) {
        backoffLevel++;
      }
    }
  }

  // ---------- Connection Lost ----------
  if (!connecting && WiFi.status() != WL_CONNECTED) {

    if (wasConnected) {

      Serial.println("[WIFI] Connection lost!");

      wasConnected = false;

      unsigned long retryDelay = BACKOFF_DELAYS[backoffLevel];

      Serial.print("[WIFI] Retrying in ");
      Serial.print(retryDelay / 1000);
      Serial.println(" seconds...");

      nextRetryTime = millis() + retryDelay;

      if (backoffLevel < MAX_BACKOFF_LEVEL) {
        backoffLevel++;
      }
    }

    // Start another connection attempt
    if (millis() >= nextRetryTime) {
      startWiFiConnection();
    }
  }
}

// ---------- Setup ----------
void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("   POLYHOUSE WIFI NODE - DAY 3");
  Serial.println("================================");

  Serial.println("[WIFI] Starting STA mode...");

  startWiFiConnection();
}

// ---------- Main Loop ----------
void loop() {

  handleWiFiConnection();

  // ---------- WiFi Health Check ----------
  if (millis() - lastHealthCheck >= HEALTH_CHECK_INTERVAL) {

    lastHealthCheck = millis();

    if (WiFi.status() == WL_CONNECTED) {

      Serial.print("[HEALTH] WiFi OK | RSSI: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");

    } else {

      Serial.println("[HEALTH] WiFi disconnected.");
    }
  }
}
