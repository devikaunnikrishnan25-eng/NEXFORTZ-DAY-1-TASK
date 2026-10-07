# Day 6 — OTA Updates Overview (ESP32)

## 1. Objective

The objective of Day 6 was to understand and implement **Over-the-Air (OTA) firmware updates** on the ESP32.

OTA allows firmware to be uploaded to an ESP32 through a Wi-Fi connection without physically connecting the USB cable for every update. This is useful for IoT devices deployed in remote or difficult-to-access locations.

The ESP32 was configured using the **ArduinoOTA** library with a dedicated hostname and password protection.

---

## 2. OTA Concept

In a normal ESP32 development process, firmware is uploaded using a USB cable.

With OTA, the process is:

```text
Arduino IDE
     ↓
Wi-Fi Network
     ↓
ESP32
     ↓
New Firmware
```

After the initial USB upload, the ESP32 can receive subsequent firmware updates through its network connection.

This makes firmware maintenance easier for deployed IoT nodes.

---

## 3. OTA Configuration

The ESP32 was configured with the following OTA hostname:

```text
polyhouse-node-01
```

The `ArduinoOTA` library was used to enable OTA functionality.

Password protection was also configured to prevent unauthorized firmware uploads.

The basic OTA initialization includes:

```cpp
ArduinoOTA.setHostname("polyhouse-node-01");
ArduinoOTA.setPassword("OTA_PASSWORD");

ArduinoOTA.begin();
```

The OTA handler is continuously checked in the main program:

```cpp
ArduinoOTA.handle();
```

This allows the ESP32 to detect and process OTA update requests while the device is running.

---

## 4. Initial USB Upload

The OTA-enabled firmware must first be uploaded to the ESP32 using a USB connection.

The first upload installs the firmware containing the OTA functionality.

After successful boot:

1. ESP32 connects to Wi-Fi.
2. OTA service starts.
3. The ESP32 advertises itself using the hostname `polyhouse-node-01`.
4. The device becomes available as a network port in Arduino IDE.

---

## 5. OTA Network Upload

After the initial USB upload, the USB connection is no longer required for normal firmware updates.

The ESP32 appears as a **Network Port** in Arduino IDE.

The OTA update process is:

```text
Write / Modify Firmware
          ↓
Compile Firmware
          ↓
Select ESP32 Network Port
          ↓
Enter OTA Password
          ↓
Upload Through Wi-Fi
          ↓
ESP32 Reboots
          ↓
New Firmware Runs
```

This demonstrates the main advantage of OTA updates: firmware can be updated wirelessly.

---

## 6. OTA Partition Scheme

OTA requires a suitable ESP32 flash partition layout.

An OTA-compatible partition scheme provides space for multiple application partitions. This allows the ESP32 to store the new firmware separately from the currently running firmware.

Conceptually:

```text
ESP32 Flash
┌─────────────────────────┐
│ Bootloader              │
├─────────────────────────┤
│ Partition Table         │
├─────────────────────────┤
│ Application Slot 0      │
├─────────────────────────┤
│ Application Slot 1      │
├─────────────────────────┤
│ Filesystem              │
└─────────────────────────┘
```

The two application slots are important for OTA because the new firmware can be written to an alternate application partition.

---

## 7. OTA Security

OTA introduces additional security considerations because firmware can be uploaded over a network.

### Security risks

* Unauthorized users on the network may attempt OTA uploads.
* An open OTA service can allow unauthorized firmware installation.
* Unsigned firmware can create firmware authenticity risks.
* The OTA service increases the network attack surface of the IoT device.

### Protection used in this task

The ESP32 uses:

* A dedicated OTA hostname.
* Password-protected OTA access.
* A trusted Wi-Fi network during development.

Open or unauthenticated OTA should not be used for deployment.

---

## 8. Lab vs Production OTA

For a learning-lab environment, password-protected OTA on a trusted local network is sufficient for demonstrating the OTA mechanism.

For production IoT deployments, stronger security should be used.

Production OTA should include:

* Digitally signed firmware images.
* Firmware authenticity verification.
* Secure firmware update mechanisms.
* Encrypted communication where appropriate.
* Protection against unauthorized firmware replacement.
* Rollback and recovery mechanisms.

The goal is to ensure that only trusted firmware is installed on the device.

---

## 9. Rollback Concept

OTA systems can use multiple application partitions to provide safer firmware updates.

If a newly installed firmware image fails to boot correctly, a rollback mechanism can allow the device to return to a previously working firmware version.

Conceptually:

```text
Working Firmware
       ↓
OTA Update
       ↓
New Firmware
       ↓
Boot Check
    ↙     ↘
Success   Failure
   ↓         ↓
Continue   Rollback
```

This reduces the risk of permanently losing access to a deployed IoT device because of a faulty firmware update.

---

## 10. OTA and Deep Sleep

IoT devices often use deep sleep to reduce power consumption.

The planned capstone workflow is:

```text
Wake Up
   ↓
Connect to Wi-Fi
   ↓
Check / Perform OTA
   ↓
Read Sensors
   ↓
Upload Data
   ↓
Enter Deep Sleep
   ↓
Wake After 55 Seconds
```

The ESP32 must remain awake while an OTA update is being performed.

Therefore, the device should complete OTA handling before entering deep sleep.

A basic wake timer can be configured using:

```cpp
#define uS_TO_S_FACTOR 1000000ULL
#define SLEEP_TIME 55

esp_sleep_enable_timer_wakeup(
    SLEEP_TIME * uS_TO_S_FACTOR
);
```

Full deep-sleep integration is planned for the later capstone. Day 6 focuses mainly on understanding OTA functionality.

---

## 11. OTA Workflow

The complete Day 6 workflow is:

```text
ESP32
  ↓
Configure OTA-Compatible Partition Scheme
  ↓
Upload OTA Firmware Through USB
  ↓
Connect ESP32 to Wi-Fi
  ↓
Start ArduinoOTA
  ↓
ESP32 Appears as Network Port
  ↓
Upload Updated Firmware Through Wi-Fi
  ↓
Verify Successful Update
  ↓
Document OTA Security
```

---

## 12. Checklist

The following items are required for completion of Day 6:

* [ ] OTA-compatible partition scheme selected.
* [ ] Two application slots available.
* [ ] First firmware upload completed through USB.
* [ ] ESP32 available through OTA Network Port.
* [ ] Subsequent firmware upload completed through OTA.
* [ ] OTA password protection enabled.
* [ ] Open/unauthenticated OTA rejected.
* [ ] OTA security policy documented.
* [ ] Deep-sleep wake timer snippet saved.
* [ ] Successful OTA upload screenshot added.

---

## 13. Expected Outcome

At the end of Day 6, the ESP32 should be capable of receiving firmware updates wirelessly through Wi-Fi using ArduinoOTA.

The task also provides an understanding of:

* OTA firmware updates.
* OTA-compatible ESP32 partitions.
* Password protection.
* Firmware security.
* Rollback concepts.
* Deep-sleep interaction with OTA.

These concepts will be used later when developing the complete IoT node and capstone system.

---

## 14. Conclusion

Day 6 introduced wireless firmware updating for ESP32 using **ArduinoOTA**.

The initial firmware is installed through USB, after which the ESP32 can be updated through its Wi-Fi Network Port. Password protection is used to improve security, while OTA-compatible partitions provide the foundation for safer firmware updates.

The task also introduced the relationship between OTA and deep sleep, which will become important for the low-power IoT node developed in the later stages of the Nexforz IoT Learning Lab.

