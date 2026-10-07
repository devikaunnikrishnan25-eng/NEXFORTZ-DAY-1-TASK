# Day 6 — OTA Updates Overview (ESP32)

## Overview

Day 6 focused on implementing **Over-the-Air (OTA) firmware updates** on the ESP32 using `ArduinoOTA`.

The ESP32 was configured with an OTA-compatible partition scheme, the hostname `polyhouse-node-01`, and password protection. The initial firmware was uploaded through USB, followed by firmware updates through the OTA Network Port.

The task also covered basic OTA security, authentication, rollback concepts, and the interaction between OTA updates and deep sleep.

## Objectives

- Configure OTA support on the ESP32.
- Enable `ArduinoOTA` with hostname `polyhouse-node-01`.
- Enable password-protected OTA updates.
- Perform firmware updates through the OTA Network Port.
- Verify OTA authentication.
- Understand OTA partitioning and rollback.
- Understand how OTA interacts with deep sleep.

## Deliverables

- OTA-enabled ESP32 firmware
- OTA upload verification
- OTA authentication test
- OTA security policy documentation
- Deep-sleep wake timer snippet

## Evidence

Screenshots of the OTA configuration, partition scheme, successful OTA upload, and authentication failure are available in the [`images`](images/) folder.
