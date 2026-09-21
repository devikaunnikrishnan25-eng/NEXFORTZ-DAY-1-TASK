
# Day 3 – Reliable WiFi Connection Manager

## Objective

The objective of Day 3 is to implement a reliable WiFi connection manager for an ESP32-based polyhouse node operating in **WiFi Station (STA) mode**.

The connection manager connects to an existing 2.4 GHz WiFi network using credentials stored separately in `secrets.h`. It provides connection timeout handling, progressive retry backoff, connection-loss detection, periodic health checks, and useful network diagnostics.

## Learning Objectives

* Implement WiFi STA mode on ESP32.
* Connect using credentials stored in `secrets.h`.
* Implement connection timeout and retry handling.
* Use progressive retry backoff after connection failures.
* Detect WiFi disconnections.
* Perform periodic WiFi health checks.
* Display IP address, RSSI, BSSID, and channel information.
* Understand WiFi provisioning options for future field deployment.

## Requirements

* ESP32 development board
* 2.4 GHz WiFi network
* Arduino IDE
* ESP32 Arduino Core
* `WiFi.h`
* `secrets.h` for local WiFi credentials
* USB cable

## Folder Structure

```text
Day 3
├── README.md
├── docs
│   └── Day3_WiFi_Connection_Manager.md
├── firmware
│   ├── Day3_WiFi_Connection_Manager.ino
│   └── secrets.example.h
└── images
```

## Implementation

The ESP32 is configured in Station mode using:

```cpp
WiFi.mode(WIFI_STA);
```

The WiFi credentials are obtained from:

```cpp
#include "secrets.h"
```

The connection manager uses a 10-second connection timeout. If the connection attempt fails, the ESP32 waits before attempting another connection.

The retry delays are:

```text
2 seconds
4 seconds
8 seconds
16 seconds
30 seconds
```

This prevents the ESP32 from continuously attempting to reconnect without a delay.

After successful connection, the program displays:

* IP address
* RSSI
* WiFi channel
* BSSID

The program also performs a WiFi health check every 5 seconds.

## Testing

The ESP32 was tested by connecting it to the available WiFi network and monitoring the Serial Monitor at:

```text
115200 baud
```

The successful connection displayed the network information including IP address, RSSI, channel, and BSSID.

The WiFi connection was also interrupted to verify that the ESP32 detected the connection loss and initiated the retry process.

## Expected Serial Output

```text
================================
   POLYHOUSE WIFI NODE - DAY 3
================================
[WIFI] Starting STA mode...

[WIFI] Starting connection attempt...

[WIFI] Connected!
[WIFI] IP Address: <ESP32_IP>
[WIFI] RSSI: <RSSI_VALUE> dBm
[WIFI] Channel: <CHANNEL>
[WIFI] BSSID: <BSSID>
[WIFI] Connection successful.

[HEALTH] WiFi OK | RSSI: <RSSI_VALUE> dBm
```

When the connection is interrupted:

```text
[WIFI] Connection lost!
[WIFI] Retrying in <DELAY> seconds...
```

The actual Serial Monitor screenshots are included in the `images` folder.

## Security

The actual WiFi credentials are stored in:

```text
secrets.h
```

This file must not be committed to GitHub.

A template file named:

```text
secrets.example.h
```

is provided so that the required format can be understood without exposing the actual WiFi password.

## Result

A working WiFi connection manager was implemented for the ESP32. The system successfully connects to an existing WiFi network, reports network information, monitors connection health, detects connection loss, and attempts reconnection using progressive retry delays.

## Conclusion

Day 3 successfully established the WiFi connectivity foundation required for the later polyhouse IoT tasks. The ESP32 can operate as a WiFi station, maintain awareness of its connection state, and recover from network interruptions without requiring a manual reset.

The connection manager and network diagnostics developed in this task provide the foundation for future communication tasks such as HTTP and MQTT-based data transmission.
