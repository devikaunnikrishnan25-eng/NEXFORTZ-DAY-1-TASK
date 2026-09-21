# Day 2 – Arduino Core for ESP32 & USB Drivers

## Objective

Configure a repeatable ESP32 development environment using Arduino IDE, ESP32 Arduino Core 3.x, required libraries, and an OTA-capable partition scheme.

## Development Environment

- IDE: Arduino IDE
- Board: ESP32 Dev Module
- ESP32 Arduino Core: 3.3.11
- Target Platform: ESP32
- Operating System: Windows

## ESP32 Board Configuration

The following board settings were configured in Arduino IDE:

- Board: ESP32 Dev Module
- Flash Size: 4MB (32Mb)
- Partition Scheme: Minimal SPIFFS (1.9MB APP with OTA/128KB SPIFFS)
- Flash Frequency: 80MHz
- Flash Mode: QIO
- PSRAM: Disabled

The Minimal SPIFFS partition scheme was selected to provide space for future OTA updates.

## ESP32 Arduino Core

The ESP32 board package was installed through the Arduino IDE Boards Manager.

The installed version was:

```text
ESP32 Arduino Core: 3.3.11
