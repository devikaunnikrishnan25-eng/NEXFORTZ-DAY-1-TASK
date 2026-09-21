# Day 4 – NTP Time Sync & Timestamps

## Objective

The objective of Day 4 is to synchronize the ESP32 system clock using **Network Time Protocol (NTP)** after establishing a WiFi connection.

Accurate timestamps are important for IoT telemetry because sensor readings need to be associated with the correct time. The ESP32 does not have a battery-backed real-time clock by default, so the system clock must be synchronized through the network.

The implementation configures NTP using `configTime()`, verifies synchronization using `getLocalTime()`, and generates timestamps in **ISO-8601 format** for use in telemetry and JSON payloads.

---

## Learning Objectives

* Configure NTP/SNTP using `configTime()`.
* Synchronize the ESP32 clock after WiFi connection.
* Configure Indian Standard Time (IST) using UTC+05:30.
* Verify successful time synchronization.
* Detect NTP synchronization failure.
* Implement a timeout while waiting for valid time.
* Generate ISO-8601 formatted timestamps.
* Integrate time synchronization with the WiFi connection flow.
* Prepare timestamps for future JSON and MQTT telemetry.

---

## Requirements

* ESP32 development board
* WiFi connection
* Arduino IDE
* ESP32 Arduino Core
* NTP server
* Serial Monitor

---

## NTP Configuration

The ESP32 uses Network Time Protocol to obtain the current time from an NTP server.

The system is configured for **Indian Standard Time (IST), UTC+05:30**.

The implementation uses `configTime()` to configure the time synchronization service.

NTP servers can be configured so that the ESP32 can obtain accurate time after connecting to WiFi.

---

## Time Synchronization

After WiFi connectivity is established, the ESP32 requests the current time from the configured NTP server.

The synchronization process is verified using:

```cpp
getLocalTime()
```

The firmware waits for valid time information while also using a timeout so that the device does not remain blocked indefinitely if the NTP server is unavailable.

---

## ISO-8601 Timestamp

After successful synchronization, the current time is formatted as an ISO-8601 timestamp.

Example:

```text
2026-09-17T17:30:45+05:30
```

The format contains:

```text
YYYY-MM-DDTHH:MM:SS+05:30
```

This format is useful for IoT systems because timestamps can be stored consistently and included directly in JSON telemetry.

---

## JSON Telemetry

The generated timestamp can be attached to telemetry data.

Example:

```json
{
  "device": "ESP32",
  "timestamp": "2026-09-17T17:30:45+05:30"
}
```

This prepares the system for future HTTP and MQTT communication tasks.

---

## Testing

The ESP32 was connected to WiFi and the NTP synchronization process was tested using the Serial Monitor.

The Serial Monitor was configured at:

```text
115200 baud
```

The successful synchronization produced the current date and time in IST.

The ISO-8601 timestamp was also generated and displayed as part of the JSON payload.

---

## Expected Output

A successful synchronization produces output similar to:

```text
[NTP] Synchronizing time...
[NTP] Time synchronized successfully.
[NTP] Current time: 2026-09-17 17:30:45
```

The generated JSON payload is displayed in a format similar to:

```json
{
  "device": "ESP32",
  "timestamp": "2026-09-17T17:30:45+05:30"
}
```

The exact timestamp depends on the time at which the ESP32 was tested.

---

## Error Handling

If the ESP32 cannot obtain valid time information within the configured timeout period, the synchronization attempt is treated as unsuccessful.

The system can report the failure through the Serial Monitor and retry synchronization.

This prevents the application from waiting indefinitely for an unavailable NTP server.

---

## WiFi and NTP Integration

NTP synchronization is performed after WiFi connectivity is established.

The overall sequence is:

```text
ESP32 Startup
      ↓
Connect to WiFi
      ↓
WiFi Connected
      ↓
Start NTP Synchronization
      ↓
Verify Time
      ↓
Generate ISO-8601 Timestamp
      ↓
Attach Timestamp to Telemetry
```

If WiFi is disconnected and later restored, the time synchronization process can be performed again to ensure that the system clock remains accurate.

---

## Result

The ESP32 successfully synchronized its system clock using NTP after connecting to WiFi.

The implementation verified the synchronized time using `getLocalTime()` and generated ISO-8601 timestamps suitable for JSON telemetry.

The system therefore provides a reliable time source for future sensor readings, HTTP requests, MQTT messages, and cloud telemetry.

---

## Conclusion

Day 4 successfully implemented network-based time synchronization for the ESP32.

The device can obtain accurate time after establishing WiFi connectivity, verify the synchronization status, use the IST timezone, and generate standardized ISO-8601 timestamps.

This provides the timekeeping foundation required for the upcoming IoT telemetry tasks, where sensor data will need accurate timestamps for storage, monitoring, and analysis.

