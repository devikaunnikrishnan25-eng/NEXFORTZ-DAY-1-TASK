# Day 2 – Arduino Core for ESP32 & USB Drivers

## Objective

Configure a repeatable ESP32 development environment using the Arduino IDE, ESP32 Arduino Core 3.x, required libraries, and an OTA-capable partition scheme.

## Environment

- IDE: Arduino IDE
- Board: ESP32 Dev Module
- ESP32 Arduino Core: 3.3.11
- Flash Size: 4MB (32Mb)
- Partition Scheme: Minimal SPIFFS (1.9MB APP with OTA/128KB SPIFFS)
- Flash Frequency: 80MHz
- Flash Mode: QIO
- PSRAM: Disabled

## Libraries

- DHT sensor library – 1.4.7
- PubSubClient – 2.8
- ArduinoJson – 7.4.3
- ArduinoOTA – provided by the ESP32 Arduino Core

The pinned library information is available in `libraries.txt`.

## Environment Check

The `day02_env_check.ino` sketch was created and successfully compiled using the ESP32 Arduino Core.

The sketch checks:

- Arduino Core version
- Free heap
- ESP32 chip model
- Chip revision
- CPU frequency

Serial output and USB-UART/COM-port verification will be completed when the physical ESP32 board is available.

## Documentation

Detailed build environment settings are available in:

`docs/build_environment.md`

## Firmware

Environment verification firmware:

`firmware/day02_env_check.ino`

## Evidence

Screenshots showing the ESP32 board package, partition scheme, and installed libraries are available in the `images` folder.
