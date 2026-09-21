#include <WiFi.h>
#include <time.h>
#include <ArduinoJson.h>

// ==================================================
// WiFi Credentials
// ==================================================
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ==================================================
// NTP Configuration
// ==================================================
const char* ntpServer1 = "time.nist.gov";
const char* ntpServer2 = "pool.ntp.org";

// IST = UTC + 5 hours 30 minutes
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

// ==================================================
// WiFi Connection
// ==================================================
bool connectWiFi() {

  Serial.println();
  Serial.println("Connecting to WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 20) {

    delay(500);
    Serial.print(".");
    attempts++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("WiFi connected!");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    return true;

  } else {

    Serial.println("WiFi connection FAILED!");

    return false;
  }
}

// ==================================================
// NTP Time Synchronization
// ==================================================
bool syncTime() {

  Serial.println();
  Serial.println("Starting NTP synchronization...");

  configTime(
    gmtOffset_sec,
    daylightOffset_sec,
    ntpServer1,
    ntpServer2
  );

  struct tm timeinfo;

  // Wait maximum 10 seconds for valid time
  for (int i = 0; i < 20; i++) {

    if (getLocalTime(&timeinfo)) {

      Serial.println();
      Serial.println("NTP synchronization successful!");

      Serial.printf(
        "Current time: %02d-%02d-%04d %02d:%02d:%02d\n",
        timeinfo.tm_mday,
        timeinfo.tm_mon + 1,
        timeinfo.tm_year + 1900,
        timeinfo.tm_hour,
        timeinfo.tm_min,
        timeinfo.tm_sec
      );

      return true;
    }

    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("NTP synchronization FAILED!");

  return false;
}

// ==================================================
// ISO-8601 Timestamp
// ==================================================
String isoTimestamp() {

  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {

    return "TIME_NOT_SYNCED";
  }

  char timestamp[30];

  strftime(
    timestamp,
    sizeof(timestamp),
    "%Y-%m-%dT%H:%M:%S%z",
    &timeinfo
  );

  String ts = String(timestamp);

  // Convert +0530 to +05:30
  if (ts.length() >= 5) {

    ts = ts.substring(0, ts.length() - 2)
       + ":"
       + ts.substring(ts.length() - 2);
  }

  return ts;
}

// ==================================================
// Create JSON Payload
// ==================================================
void printJsonPayload() {

  String timestamp = isoTimestamp();

  // Create JSON document
  JsonDocument doc;

  doc["device"] = "ESP32";
  doc["timestamp"] = timestamp;

  // Print JSON to Serial Monitor
  Serial.println();
  Serial.println("JSON Payload:");

  serializeJsonPretty(doc, Serial);

  Serial.println();
}

// ==================================================
// Setup
// ==================================================
void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("Nexforz Day 4 - Time Service");
  Serial.println("==============================");

  // -----------------------------------------------
  // Step 1: Connect to WiFi
  // -----------------------------------------------

  if (connectWiFi()) {

    // ---------------------------------------------
    // Step 2: Synchronize time using NTP
    // ---------------------------------------------

    if (syncTime()) {

      // -------------------------------------------
      // Step 3: Generate JSON payload
      // -------------------------------------------

      printJsonPayload();

    } else {

      Serial.println();
      Serial.println("Initial NTP sync failed.");
      Serial.println("Retrying NTP synchronization...");

      delay(5000);

      if (syncTime()) {

        printJsonPayload();

      } else {

        Serial.println("NTP retry failed.");
      }
    }
  }
}

// ==================================================
// Loop
// ==================================================
void loop() {

  // -----------------------------------------------
  // Check WiFi connection
  // -----------------------------------------------

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println();
    Serial.println("WiFi disconnected!");
    Serial.println("Attempting WiFi reconnection...");

    if (connectWiFi()) {

      Serial.println("WiFi reconnected!");

      // Re-sync time after WiFi reconnects
      if (syncTime()) {

        printJsonPayload();

      } else {

        Serial.println("NTP synchronization failed after reconnect.");
      }
    }

  } else {

    // ---------------------------------------------
    // WiFi is connected
    // Print current JSON payload
    // every 5 seconds
    // ---------------------------------------------

    printJsonPayload();
  }

  delay(5000);
}
