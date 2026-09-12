# Day 1 – ESP32 Development Environment Setup

## Objective

Set up the ESP32 development environment using Arduino IDE and verify the basic functionality of the ESP32 development board through a Blink test and Serial Monitor output.

## Hardware Used

- ESP32 Development Board
- USB Data Cable

## Software Used

- Arduino IDE
- ESP32 Arduino Core by Espressif Systems

## Board Configuration

The following board configuration was used:

- Board: ESP32 Dev Module
- Flash Size: 4MB (32Mb)
- Partition Scheme: Default 4MB with spiffs
- Flash Frequency: 80MHz
- Flash Mode: QIO
- PSRAM: Disabled

## ESP32 Arduino Core

The ESP32 board package was installed through the Arduino IDE Boards Manager.

The ESP32 Arduino Core provides the required support for programming and developing applications for ESP32-based boards.

## Blink Test

A basic LED Blink program was used to verify that the ESP32 development environment was working correctly.

The onboard LED connected to GPIO 2 was controlled using the Arduino `digitalWrite()` function.

The program repeatedly:

1. Turns the LED ON.
2. Waits for a short delay.
3. Turns the LED OFF.
4. Waits again.

## Serial Monitor Test

The Serial Monitor was configured at:

```text
115200 baud
