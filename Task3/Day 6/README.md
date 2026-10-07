
# Day 6 — OTA Updates Overview (ESP32)

**Nexforz IoT Learning Lab — Phase 1: ESP Setup**

## Overview

Day 6 focused on **Over-the-Air (OTA) firmware updates** for the ESP32.

OTA allows new firmware to be uploaded to an ESP32 over Wi-Fi without physically connecting the USB cable every time. This is especially useful for IoT nodes deployed in locations where accessing the hardware regularly may not be practical.

For this task, the ESP32 was configured for OTA updates using **ArduinoOTA** with the hostname:

```text
polyhouse-node-01
```

Password protection was also included to prevent unauthorized OTA uploads.

---

## Learning Objectives

* Understand how OTA firmware updates work on ESP32.
* Configure `ArduinoOTA` for wireless firmware updates.
* Set the ESP32 hostname to `polyhouse-node-01`.
* Protect OTA updates using a password.
* Understand the requirement for an OTA-compatible partition scheme.
* Understand the basic concept of rollback and recovery.
* Understand the relationship between OTA and deep sleep.
* Identify basic OTA security risks.

---

## Hardware / Software Used

### Hardware

* ESP32 WROOM-32 development board
* USB cable
* Wi-Fi network

### Software

* Arduino IDE
* ESP32 Arduino Core
* `ArduinoOTA` library

---

## OTA Configuration

The ESP32 was configured with the following OTA hostname:

```text
polyhouse-node-01
```

OTA password protection was enabled so that firmware cannot be uploaded through an open, unauthenticated OTA connection.

The initial firmware is uploaded to the ESP32 through USB. After the device connects to Wi-Fi and starts the OTA service, the ESP32 can be selected as a **Network Port** in Arduino IDE for subsequent firmware uploads.

### Basic OTA Flow

```text
Arduino IDE
     │
     │ First upload
     ▼
   USB
     │
     ▼
   ESP32
     │
     │ Wi-Fi
     ▼
ArduinoOTA Service
     │
     │ Subsequent upload
     ▼
Network Port
```

---

## OTA Partition Scheme

OTA updates require an ESP32 partition layout that provides space for multiple application partitions.

The OTA-compatible layout provides two application slots so that new firmware can be written without immediately overwriting the currently running firmware.

Conceptually:

```text
ESP32 Flash
┌─────────────────────────────┐
│ Bootloader / Partition Data │
├─────────────────────────────┤
│ Application Slot 0          │
├─────────────────────────────┤
│ Application Slot 1          │
├─────────────────────────────┤
│ Filesystem                  │
└─────────────────────────────┘
```

This arrangement supports safer firmware updates and provides the basis for firmware recovery if a new image fails to boot correctly.

---

## USB Upload → OTA Upload

The intended workflow for Day 6 is:

### Step 1 — Initial USB Upload

The OTA-enabled firmware is first uploaded using the USB connection.

### Step 2 — ESP32 Connects to Wi-Fi

After booting, the ESP32 connects to the configured Wi-Fi network.

### Step 3 — OTA Service Starts

`ArduinoOTA` starts and advertises the device using the hostname:

```text
polyhouse-node-01
```

### Step 4 — Network Port Appears

The ESP32 becomes available as a network upload target in Arduino IDE.

### Step 5 — OTA Firmware Upload

The next firmware version can be uploaded through the ESP32's OTA Network Port without reconnecting the USB cable.

---

## OTA Security

OTA provides convenience, but it also introduces security risks.

### Main Risks

* Unauthorized devices on the same network may attempt to access the OTA service.
* An unprotected OTA endpoint can allow unauthorized firmware uploads.
* Unsigned firmware images can create a firmware authenticity risk.
* OTA services increase the network attack surface of an IoT device.

### Protection Used

For this lab:

* OTA password protection is enabled.
* The ESP32 uses a known hostname.
* OTA should only be used on a trusted network.
* USB recovery should be kept available during development.

For production deployments, stronger firmware authenticity and integrity mechanisms should be used.

---

## Deep Sleep and OTA

IoT nodes may use deep sleep to reduce power consumption.

A typical future capstone workflow is:

```text
Wake
  ↓
Initialize
  ↓
Connect to Wi-Fi
  ↓
Check / perform OTA if required
  ↓
Read sensors
  ↓
Upload telemetry
  ↓
Enter Deep Sleep
  ↓
Wake after configured interval
```

OTA must be completed **before the ESP32 enters deep sleep**.

If the device enters deep sleep while an OTA update is in progress, the update may fail.

For the Day 6 capstone concept, the node should therefore remain awake during the OTA process and only enter deep sleep after OTA handling is complete.

---

## Deep Sleep Wake Timer Concept

The following snippet demonstrates the basic deep-sleep timer concept that can be integrated later:

```cpp
#define uS_TO_S_FACTOR 1000000ULL
#define SLEEP_TIME 55

void setup() {
  Serial.begin(115200);

  // Configure wake-up after 55 seconds
  esp_sleep_enable_timer_wakeup(
    SLEEP_TIME * uS_TO_S_FACTOR
  );

  // OTA and other tasks should be completed
  // before entering deep sleep.

  Serial.println("Entering deep sleep...");
  esp_deep_sleep_start();
}

void loop() {
}
```

This snippet is saved for future capstone integration. Full OTA + deep-sleep integration is not the main focus of Day 6.

---

## Rollback Concept

OTA-capable ESP32 systems can use multiple application partitions to support safer firmware updates.

Conceptually:

```text
Current Firmware
       ↓
New Firmware Written
       ↓
New Firmware Boot
       ↓
Boot Verification
       │
   ┌───┴───┐
   ↓       ↓
Success   Failure
   ↓       ↓
Keep      Recovery /
New       Previous
Firmware  Firmware
```

The important idea is that a failed firmware update should not permanently leave the device unusable. Production systems should implement appropriate image validation and rollback mechanisms.

---

## Day 6 Checklist

* [ ] Partition scheme supports OTA with two application slots.
* [ ] Initial firmware upload completed through USB.
* [ ] Subsequent firmware upload completed through OTA Network Port.
* [ ] OTA hostname configured as `polyhouse-node-01`.
* [ ] OTA password protection enabled.
* [ ] Open/unauthenticated OTA access rejected.
* [ ] OTA security policy documented in `docs/ota_policy.md`.
* [ ] Deep-sleep wake timer snippet saved for capstone integration.
* [ ] Successful OTA upload screenshot added to the repository.

---

## Deliverables

The Day 6 submission contains:

```text
Day6/
├── README.md
├── firmware/
│   └── ota_update/
│       └── ota_update.ino
├── docs/
│   ├── ota_policy.md
│   └── deep_sleep_ota.md
└── images/
    ├── ota_network_port.png
    ├── ota_upload_success.png
    └── partition_scheme.png
```

Screenshots should be added to the `images` folder as evidence of the completed OTA setup.

---

## Conclusion

Day 6 introduced OTA firmware updating on the ESP32 using `ArduinoOTA`. The device can be prepared for wireless firmware updates after the initial USB upload.

The session also covered OTA security, partition requirements, rollback concepts, and the interaction between OTA and deep sleep. These concepts will be useful for the later IoT capstone, where a deployed node may need to update firmware without requiring physical access.
