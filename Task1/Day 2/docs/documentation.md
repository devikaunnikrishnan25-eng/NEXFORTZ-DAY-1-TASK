Day 2 – Build Environment

1. Introduction

Day 2 focused on setting up and verifying a repeatable development environment for ESP32-based IoT development.

The development environment was configured using Arduino IDE and the ESP32 Arduino Core. The required libraries were installed and the ESP32 board configuration was set up with an OTA-capable partition scheme.

The environment was then tested using a dedicated environment-check firmware. The firmware was compiled, uploaded to the ESP32 development board, and verified through the Serial Monitor.

USB-UART communication and the COM port were also verified to ensure reliable communication between the ESP32 development board and the computer.

2. Objectives

The main objectives of this session were:

Install and verify the ESP32 Arduino Core.

Configure the ESP32 Dev Module in Arduino IDE.

Select an OTA-capable partition scheme.

Install the required libraries for upcoming IoT tasks.

Create a pinned library list for reproducible development.

Create an environment-check firmware.

Compile and upload the firmware to the ESP32.

Verify Serial Monitor communication.

Verify USB-UART and COM-port connectivity.

Document the complete build environment for future development.

3. Development Environment

Component

Configuration

IDE

Arduino IDE

Target Board

ESP32 Dev Module

ESP32 Arduino Core

3.3.11

Target Platform

ESP32

Operating System

Windows

Serial Baud Rate

115200

The ESP32 Arduino Core version 3.3.11 was installed through the Arduino IDE Boards Manager.

The ESP32 Dev Module was selected as the target board for compilation and firmware upload.

4. ESP32 Board Package Installation

The ESP32 board support package was installed using the Arduino IDE Boards Manager.

The installed package was:

esp32 by Espressif Systems
Version: 3.3.11

This satisfies the requirement for an ESP32 Arduino Core version of 3.0 or higher.

After installation, the following board was selected:

Tools → Board → ESP32 Arduino → ESP32 Dev Module

The board selection was verified before compiling the environment-check firmware.

5. ESP32 Board Configuration

The following board configuration was used during the session:

Board: ESP32 Dev Module

Flash Size: 4MB (32Mb)

Partition Scheme: Minimal SPIFFS (1.9MB APP with OTA/128KB SPIFFS)

Flash Frequency: 80MHz

Flash Mode: QIO

PSRAM: Disabled

The selected partition scheme provides an OTA-capable application layout. This is useful for future tasks involving Over-The-Air firmware updates.

The partition configuration also provides SPIFFS storage for configuration files and other data that may be required in later IoT tasks.

6. OTA-Capable Partition Scheme

The partition scheme was changed from the default configuration to:

Minimal SPIFFS (1.9MB APP with OTA/128KB SPIFFS)

This configuration was selected because future tasks may require OTA firmware updates.

The partition layout provides:

Application memory for the firmware.

OTA support.

SPIFFS storage for configuration files and other data.

The partition scheme was successfully selected and verified in Arduino IDE.

7. Required Libraries

The required libraries for the upcoming IoT development tasks were installed through the Arduino IDE Library Manager.

The following libraries were installed:

Library

Version

DHT sensor library

1.4.7

PubSubClient

2.8

ArduinoJson

7.4.3

DHT Sensor Library

The DHT sensor library is required for interfacing with DHT-series temperature and humidity sensors in upcoming tasks.

Installed version:

DHT sensor library 1.4.7

PubSubClient

PubSubClient provides MQTT client functionality for ESP32-based IoT applications.

Installed version:

PubSubClient 2.8

ArduinoJson

ArduinoJson is used for creating, parsing, and handling JSON data structures.

Installed version:

ArduinoJson 7.4.3

ArduinoOTA

ArduinoOTA is provided as part of the ESP32 Arduino Core and does not require a separate library installation for this environment.

The installed ESP32 Arduino Core provides the required ArduinoOTA support.

8. Library Version Tracking

A libraries.txt file was created to document the installed library versions.

The file contains the required library dependencies and their versions so that the development environment can be reproduced consistently.

The pinned libraries are:

DHT sensor library@1.4.7
PubSubClient@2.8
ArduinoJson@7.4.3
# ArduinoOTA is provided by ESP32 Arduino Core 3.3.11

This file is stored in the Day 2 directory.

9. Environment Check Firmware

A dedicated firmware sketch named:

day02_env_check.ino

was created to verify the ESP32 development environment.

The firmware was designed to display important ESP32 system information through the Serial Monitor.

The firmware checks:

ESP32 Arduino Core version.

Free heap memory.

ESP32 chip model.

Chip revision.

CPU frequency.

10. Environment Check Code

The following firmware was used:

#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("=== ESP32 Day 2 Environment Check ===");

  Serial.print("Arduino Core Version: ");
  Serial.println(ESP_ARDUINO_VERSION_STR);

  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");

  Serial.print("Chip Model: ");
  Serial.println(ESP.getChipModel());

  Serial.print("Chip Revision: ");
  Serial.println(ESP.getChipRevision());

  Serial.print("CPU Frequency: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");

  Serial.println("Environment check completed successfully.");
}

void loop() {
  delay(2000);

  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
}

11. Firmware Compilation

The day02_env_check.ino firmware was compiled using the configured ESP32 development environment.

The compilation was completed successfully without errors.

This verified that:

The ESP32 Arduino Core was correctly installed.

The ESP32 Dev Module was correctly selected.

The Arduino IDE could compile ESP32-specific code.

The required ESP32 system functions were available.

12. USB-UART and COM Port Verification

The ESP32 development board was connected to the computer using a USB data cable.

The USB connection was checked through Windows Device Manager and Arduino IDE.

The ESP32 USB-UART interface was detected by the computer and a COM port was made available for communication.

The COM port was then selected in Arduino IDE for firmware uploading.

This verified communication between:

Computer
    ↓
USB Cable
    ↓
ESP32 USB-UART Interface
    ↓
ESP32 Development Board

The USB connection was successfully used for firmware upload and Serial Monitor communication.

13. Firmware Upload

After selecting the correct ESP32 board and COM port, the day02_env_check.ino firmware was uploaded to the ESP32 development board.

The upload was completed successfully.

This confirmed that the configured Arduino IDE environment was capable of communicating with the ESP32 and transferring firmware to the board.

14. Serial Monitor Verification

After uploading the firmware, the Arduino IDE Serial Monitor was opened.

The baud rate was set to:

115200

The environment-check firmware successfully produced system information through the Serial Monitor.

The output includes information such as:

=== ESP32 Day 2 Environment Check ===
Arduino Core Version: 3.3.11
Free Heap: XXXXX bytes
Chip Model: ESP32...
Chip Revision: ...
CPU Frequency: ... MHz
Environment check completed successfully.

The free heap value is continuously displayed in the main loop to confirm that the ESP32 is running the firmware correctly.

The exact free heap and chip information may vary depending on the ESP32 board and the current system state.

15. Verification Results

Verification Item

Status

ESP32 Arduino Core installed

Completed

ESP32 Core version ≥ 3.0

Completed

ESP32 Dev Module selected

Completed

Flash configuration verified

Completed

OTA-capable partition scheme selected

Completed

DHT sensor library installed

Completed

PubSubClient installed

Completed

ArduinoJson installed

Completed

Environment-check firmware created

Completed

Firmware compilation

Completed

USB-UART connection

Completed

COM-port detection

Completed

Firmware upload

Completed

Serial Monitor communication

Completed

Environment information verified

Completed

16. Troubleshooting

During the hardware verification process, the COM port was checked to ensure that the ESP32 was properly recognized by the computer.

USB-UART connectivity was verified using Windows Device Manager and Arduino IDE.

The USB cable and board connection were checked before performing the firmware upload.

Once the ESP32 was detected correctly, the firmware was uploaded and the Serial Monitor was used to verify the environment.

17. Project Directory Structure

The Day 2 files are organized as follows:

Day 2/
├── README.md
├── docs/
│   └── build_environment.md
├── firmware/
│   └── day02_env_check.ino
├── images/
│   ├── DHT sensor.jpeg
│   ├── arduinojson.jpeg
│   ├── esp32_board_package.jpeg
│   ├── partition scheme.jpeg
│   └── pubsubclient.jpeg
└── libraries.txt

The README.md provides a quick overview of the task.

The docs folder contains the detailed build environment documentation.

The firmware folder contains the ESP32 environment-check firmware.

The images folder contains screenshots and evidence collected during the setup.

The libraries.txt file contains the required library versions.

18. Evidence

Screenshots and supporting evidence were collected during the Day 2 setup.

The evidence includes:

ESP32 Arduino Core installation.

ESP32 board configuration.

OTA-capable partition scheme.

DHT sensor library installation.

PubSubClient installation.

ArduinoJson installation.

ESP32 environment verification.

These files are stored in the images directory of the Day 2 folder.

19. Learning Outcomes

After completing this session, the following concepts and practical skills were covered:

Installing the ESP32 Arduino Core.

Selecting and configuring an ESP32 development board.

Understanding ESP32 flash and partition configuration.

Selecting an OTA-capable partition scheme.

Installing and tracking library dependencies.

Compiling ESP32 firmware.

Uploading firmware through a USB connection.

Identifying and selecting the correct COM port.

Using the Serial Monitor for ESP32 debugging.

Reading basic ESP32 system information.

Maintaining documentation for a reproducible development environment.

20. Conclusion

The Day 2 ESP32 development environment was successfully configured and tested.

The ESP32 Arduino Core version 3.3.11 was installed and the ESP32 Dev Module was selected as the target board. An OTA-capable Minimal SPIFFS partition scheme was configured to support future OTA-related development.

The required DHT sensor library, PubSubClient, and ArduinoJson libraries were installed and their versions were documented in the libraries.txt file.

A dedicated day02_env_check.ino firmware was developed to verify the ESP32 Arduino Core version, free heap memory, chip model, chip revision, and CPU frequency.

The firmware was successfully compiled and uploaded to the ESP32 development board. USB-UART communication, COM-port detection, and Serial Monitor communication were also verified successfully.

Therefore, the complete Day 2 build environment setup and verification was successfully completed.
