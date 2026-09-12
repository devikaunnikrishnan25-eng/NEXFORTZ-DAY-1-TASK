# Day 2 – Build Environment

## 1. Development Environment

- IDE: Arduino IDE
- Board: ESP32 Dev Module
- ESP32 Arduino Core: 3.3.11
- Target platform: ESP32
- Operating System: Windows

## 2. ESP32 Board Configuration

The following board settings were configured in Arduino IDE:

- Board: ESP32 Dev Module
- Flash Size: 4MB (32Mb)
- Partition Scheme: Minimal SPIFFS (1.9MB APP with OTA/128KB SPIFFS)
- Flash Frequency: 80MHz
- Flash Mode: QIO
- PSRAM: Disabled

The Minimal SPIFFS partition scheme was selected to provide space for future OTA updates.

## 3. Required Libraries

The following libraries were installed:

| Library | Version |
|---|---|
| DHT sensor library | 1.4.7 |
| PubSubClient | 2.8 |
| ArduinoJson | 7.4.3 |

ArduinoOTA is provided as part of the ESP32 Arduino Core and does not require a separate library installation.

## 4. Environment Check

The `day02_env_check.ino` sketch was created to verify the ESP32 build environment.

The sketch checks:

- ESP32 Arduino Core version
- Free heap memory
- ESP32 chip model
- Chip revision
- CPU frequency

The sketch compiled successfully using the configured ESP32 environment.

Serial output verification will be completed when the physical ESP32 development board is available.

## 5. USB-UART Driver Verification

USB-UART/COM-port verification is pending because the physical ESP32 development board is currently unavailable.

Once the board is available, the COM port will be checked in Windows Device Manager and the appropriate CP2102/CH340 USB-UART driver will be verified.

## 6. Build Verification Status

- ESP32 board package installed: Completed
- ESP32 Dev Module selected: Completed
- OTA-capable partition scheme configured: Completed
- Required libraries installed: Completed
- Environment check sketch compiled successfully: Completed
- Serial environment-check output: Pending hardware
- USB-UART/COM port verification: Pending hardware
