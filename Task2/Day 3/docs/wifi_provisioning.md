# Day 3 – Reliable WiFi Connection Manager

## 1. Introduction

Polyhouse IoT nodes need a reliable WiFi connection to communicate with other systems and transfer sensor data. Since the ESP32 node operates in a field environment, the WiFi connection may occasionally be interrupted because of access-point rebooting, weak signal strength, or temporary network failures.

In this task, a reliable WiFi connection manager was implemented for the ESP32. The ESP32 operates in **WiFi Station (STA) mode** and connects to an existing 2.4 GHz WiFi network.

The connection manager is designed to detect connection failures, avoid continuous connection attempts, retry after increasing delays, and periodically monitor the health of the WiFi connection.

---

## 2. Objective

The main objective of Day 3 is to implement a reliable WiFi connection manager with:

* WiFi Station (STA) mode.
* Connection timeout handling.
* Retry mechanism.
* Progressive retry backoff.
* WiFi disconnection detection.
* Periodic connection health checks.
* Network information logging.
* Secure separation of WiFi credentials using `secrets.h`.

---

## 3. Learning Objectives

After completing this task, the following concepts were implemented and understood:

* Configuring the ESP32 as a WiFi station.
* Connecting to an existing WiFi access point.
* Monitoring WiFi connection status using `WiFi.status()`.
* Detecting successful connections.
* Detecting connection loss.
* Implementing connection timeout handling.
* Implementing retry delays.
* Using RSSI to monitor signal strength.
* Reading the connected access point's BSSID and channel.
* Separating credentials from the main firmware.
* Understanding WiFi provisioning methods for future field deployment.

---

## 4. Hardware and Software Requirements

### Hardware

* ESP32 development board
* USB cable
* 2.4 GHz WiFi access point/router
* Computer for programming and serial monitoring

### Software

* Arduino IDE
* ESP32 Arduino Core
* `WiFi.h` library
* Serial Monitor

---

## 5. WiFi Station Mode

The ESP32 supports different WiFi operating modes. For this task, the ESP32 is configured in **Station mode**.

In Station mode, the ESP32 behaves as a client and connects to an existing WiFi access point.

The following command is used:

```cpp
WiFi.mode(WIFI_STA);
```

The ESP32 does not create its own access point in this mode.

After configuring Station mode, the connection is started using:

```cpp
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
```

The SSID and password are obtained from the separate `secrets.h` file.

---

## 6. Credential Management

WiFi credentials are kept separate from the main source code.

The firmware includes:

```cpp
#include "secrets.h"
```

The credentials are then accessed using:

```cpp
WIFI_SSID
WIFI_PASSWORD
```

A typical `secrets.h` file contains:

```cpp
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

The actual `secrets.h` file should not be uploaded to the public GitHub repository because it contains the WiFi password.

Instead, a template named `secrets.example.h` is provided in the repository.

The actual `secrets.h` file is also added to `.gitignore`.

---

## 7. Connection Timeout

A connection attempt should not continue indefinitely.

A timeout of 10 seconds is configured:

```cpp
const unsigned long CONNECT_TIMEOUT = 10000;
```

When a connection attempt begins, the current time is stored:

```cpp
connectStartTime = millis();
```

The program continuously checks whether the ESP32 has connected.

If the connection is not successful within 10 seconds, the attempt is considered unsuccessful.

The program then stops the current connection attempt and schedules another retry.

This prevents the connection manager from remaining stuck in a connection attempt forever.

---

## 8. Retry and Progressive Backoff

Repeatedly attempting to connect to a failed WiFi network without any delay can unnecessarily consume processing resources.

Therefore, progressive retry delays are implemented.

The configured retry delays are:

```text
2 seconds
4 seconds
8 seconds
16 seconds
30 seconds
```

They are stored in the program as:

```cpp
const unsigned long BACKOFF_DELAYS[] = {
  2000,
  4000,
  8000,
  16000,
  30000
};
```

The retry level is increased after failed attempts.

For example:

```text
First retry  → 2 seconds
Second retry → 4 seconds
Third retry  → 8 seconds
Fourth retry → 16 seconds
Later retries → 30 seconds
```

The retry level is limited so that the delay does not continue increasing indefinitely.

When a connection is successfully established, the backoff level is reset:

```cpp
backoffLevel = 0;
```

This allows the system to return to the shortest retry delay after recovering from a failure.

---

## 9. Connection State Management

The firmware uses state variables to keep track of the WiFi connection process.

Important variables include:

```cpp
bool connecting = false;
bool wasConnected = false;
```

The `connecting` variable indicates whether the ESP32 is currently attempting to establish a connection.

The `wasConnected` variable helps detect when an already-established connection has been lost.

The program therefore distinguishes between:

* Initial connection attempt
* Connection timeout
* Successful connection
* Connection loss
* Reconnection attempt

This makes the connection manager more reliable than simply calling `WiFi.begin()` repeatedly.

---

## 10. Starting a WiFi Connection

The connection process is handled by the `startWiFiConnection()` function.

The function:

1. Prints a connection message.
2. Configures Station mode.
3. Starts the WiFi connection.
4. Records the connection start time.
5. Sets the connection state to active.

The main commands are:

```cpp
WiFi.mode(WIFI_STA);
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

connectStartTime = millis();
connecting = true;
```

---

## 11. Detecting Successful Connection

The program continuously checks:

```cpp
WiFi.status() == WL_CONNECTED
```

When this condition becomes true, the connection attempt is considered successful.

The program then:

* Stops the connection-attempt state.
* Marks the ESP32 as connected.
* Resets the backoff level.
* Prints network information.

The following information is displayed:

### IP Address

```cpp
WiFi.localIP()
```

The IP address assigned to the ESP32 by the WiFi network is displayed.

### RSSI

```cpp
WiFi.RSSI()
```

RSSI represents the received signal strength of the WiFi connection in dBm.

A less negative RSSI value generally indicates a stronger signal.

### WiFi Channel

```cpp
WiFi.channel()
```

This displays the channel currently being used by the connected access point.

### BSSID

```cpp
WiFi.BSSIDstr()
```

The BSSID identifies the specific access point to which the ESP32 is connected.

---

## 12. WiFi Health Monitoring

A periodic health check is implemented using:

```cpp
const unsigned long HEALTH_CHECK_INTERVAL = 5000;
```

Therefore, the WiFi status is checked approximately every 5 seconds.

If the ESP32 is connected, the program prints:

```text
[HEALTH] WiFi OK | RSSI: <value> dBm
```

If the connection is unavailable, it prints:

```text
[HEALTH] WiFi disconnected.
```

This provides continuous information about the network state without blocking the main program.

---

## 13. Detecting Connection Loss

After a successful connection, the program continues to monitor:

```cpp
WiFi.status()
```

If the status changes from connected to disconnected, the program identifies the event as a connection loss.

The following message is printed:

```text
[WIFI] Connection lost!
```

The system then calculates the next retry delay and schedules a new connection attempt.

This allows the ESP32 to recover from temporary WiFi interruptions without requiring a manual restart.

---

## 14. Reconnection Process

When the connection is lost, the ESP32 does not immediately attempt to reconnect continuously.

Instead, it:

1. Detects the connection loss.
2. Selects the current retry delay.
3. Displays the retry delay.
4. Schedules the next attempt.
5. Waits until the retry time.
6. Starts a new connection attempt.

An example message is:

```text
[WIFI] Connection lost!
[WIFI] Retrying in 2 seconds...
```

If the connection continues to fail, the retry delay increases according to the configured backoff levels.

---

## 15. Non-Blocking Connection Handling

The firmware uses `millis()` to manage timing instead of using long blocking delays for the connection-management logic.

For example:

```cpp
millis() - connectStartTime >= CONNECT_TIMEOUT
```

is used to determine whether the connection attempt has exceeded its timeout.

Similarly:

```cpp
millis() >= nextRetryTime
```

is used to determine when the next retry should begin.

This approach allows the main loop to continue running while connection timing is being managed.

This is important for future polyhouse applications because sensor readings and other tasks should not remain blocked indefinitely while the ESP32 is trying to reconnect to WiFi.

---

## 16. Serial Monitoring

The Serial Monitor is configured at:

```cpp
Serial.begin(115200);
```

At startup, the program displays:

```text
================================
   POLYHOUSE WIFI NODE - DAY 3
================================
[WIFI] Starting STA mode...
```

During a successful connection, output similar to the following is produced:

```text
[WIFI] Starting connection attempt...

[WIFI] Connected!
[WIFI] IP Address: <ESP32_IP>
[WIFI] RSSI: <RSSI_VALUE> dBm
[WIFI] Channel: <CHANNEL>
[WIFI] BSSID: <BSSID>
[WIFI] Connection successful.
```

The health-monitoring output appears periodically:

```text
[HEALTH] WiFi OK | RSSI: <RSSI_VALUE> dBm
```

When the WiFi connection is interrupted:

```text
[WIFI] Connection lost!
[WIFI] Retrying in <DELAY> seconds...
```

The actual serial-monitor screenshots obtained during testing are included in the `images` folder.

---

## 17. Testing Procedure

The connection manager was tested using the following procedure.

### Test 1 – ESP32 Startup

1. Connect the ESP32 to the computer.
2. Upload the Day 3 firmware.
3. Open the Serial Monitor.
4. Set the baud rate to 115200.
5. Observe the startup messages.

Expected behavior:

```text
[WIFI] Starting STA mode...
[WIFI] Starting connection attempt...
```

### Test 2 – Successful Connection

The ESP32 was allowed to connect to the configured WiFi network.

After successful association, the Serial Monitor displayed:

* IP address
* RSSI
* Channel
* BSSID

### Test 3 – WiFi Health Check

The connection was monitored continuously.

The ESP32 periodically displayed the WiFi status and RSSI value.

Expected output:

```text
[HEALTH] WiFi OK | RSSI: <value> dBm
```

### Test 4 – Access Point Interruption

The WiFi connection was interrupted to simulate an access-point/network failure.

The ESP32 detected that the connection had been lost and displayed:

```text
[WIFI] Connection lost!
```

The system then scheduled a reconnection attempt.

### Test 5 – Reconnection

After the WiFi network became available again, the ESP32 attempted to reconnect.

After successful reconnection, the system again displayed the IP address, RSSI, channel, and BSSID.

This verified that the connection manager could recover from a WiFi interruption.

---

## 18. RSSI Monitoring

RSSI is used to estimate the received WiFi signal strength.

The firmware obtains the RSSI using:

```cpp
WiFi.RSSI()
```

The value is displayed in dBm.

For example:

```text
[HEALTH] WiFi OK | RSSI: -55 dBm
```

A more negative value generally indicates a weaker received signal.

The Day 3 task notes that signal levels below approximately -80 dBm can indicate an unreliable connection for communication-heavy applications.

RSSI monitoring is therefore useful for diagnosing future HTTP and MQTT communication problems.

---

## 19. WiFi Provisioning Concepts

For a field-deployed polyhouse node, manually editing firmware every time the WiFi network changes would not be convenient.

Several provisioning methods can be considered for future deployment.

### SmartConfig

SmartConfig allows compatible ESP32 devices to receive WiFi credentials through a provisioning process without manually editing the firmware.

### Captive Portal

A captive portal can provide a temporary configuration page where a technician enters the WiFi SSID and password.

This approach can be useful when deploying nodes at different locations.

### WPS

WiFi Protected Setup (WPS) has historically been used for simplified network configuration. However, it is deprecated or discouraged in many modern deployment scenarios and is therefore not used in this implementation.

For this task, credentials remain in `secrets.h`. More advanced persistent configuration methods can be introduced in later tasks.

---

## 20. Important ESP32 WiFi Consideration

The ESP32 used for this task connects to **2.4 GHz WiFi networks**.

Therefore, the access point used for testing must provide a compatible 2.4 GHz network.

A 5 GHz-only WiFi network cannot be used directly for this ESP32 WiFi connection.

---

## 21. Troubleshooting

### Problem: ESP32 does not connect

Possible causes:

* Incorrect SSID.
* Incorrect password.
* WiFi access point is unavailable.
* ESP32 is too far from the access point.
* Network is operating only on an unsupported band.

Solution:

* Verify the credentials in `secrets.h`.
* Ensure the access point is active.
* Use a compatible 2.4 GHz network.
* Check the Serial Monitor for timeout and retry messages.

### Problem: Repeated connection timeouts

The connection manager automatically waits for the configured timeout and then schedules another attempt.

The retry delay increases progressively to avoid continuous connection attempts.

### Problem: Weak WiFi signal

Check the RSSI displayed in the Serial Monitor.

A very low RSSI value may indicate that the ESP32 is too far from the access point or that obstacles are affecting the signal.

### Problem: WiFi credentials exposed in GitHub

The actual `secrets.h` file should not be committed.

The file must be added to `.gitignore`.

Only `secrets.example.h` should be uploaded as a template.

---

## 22. Files Used in the Project

The Day 3 project contains the following important files:

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

The main firmware implements the complete WiFi connection manager.

The `secrets.example.h` file demonstrates the required credential format without exposing the actual password.

The `images` directory contains photographs and Serial Monitor screenshots from testing.

---

## 23. Result

The ESP32 successfully implemented a reliable WiFi connection manager in Station mode.

The system was able to:

* Start in WiFi STA mode.
* Connect to the configured WiFi network.
* Obtain an IP address.
* Display RSSI.
* Display the connected access point's BSSID.
* Display the WiFi channel.
* Perform periodic health checks.
* Detect connection loss.
* Apply progressive retry delays.
* Attempt reconnection after network interruption.
* Maintain WiFi credentials separately from the main source code.

---

## 24. Conclusion

Day 3 successfully established the WiFi connectivity foundation for the ESP32 polyhouse node.

A connection manager was implemented to handle initial WiFi association, connection timeouts, progressive retry delays, connection-loss detection, and periodic health monitoring. Important network information such as IP address, RSSI, BSSID, and channel was also logged through the Serial Monitor.

The use of `secrets.h` keeps WiFi credentials separate from the main firmware source, while `.gitignore` prevents the actual credentials from being uploaded to the repository.

The completed connection manager provides a reliable base for the upcoming IoT communication tasks, where the ESP32 will use the network for services such as HTTP and MQTT communication.

