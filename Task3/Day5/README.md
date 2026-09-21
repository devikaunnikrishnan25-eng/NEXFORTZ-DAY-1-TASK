# Day 5 – DHT Sensor with Error Handling

## Overview

This task implements a **DHT11 temperature and humidity sensor** with robust error handling on an ESP32. The sensor is connected to **GPIO 4** and the readings are processed using a `SensorBundle` structure with validity flags.

The system performs multiple readings, retries failed sensor reads, applies a **median filter over the last 3 valid samples**, and outputs the final result in a simple JSON format.

## Features

* DHT11 temperature and humidity measurement
* GPIO 4 used for sensor data
* Up to **3 read attempts** for failed readings
* `isnan()` based error detection
* `SensorBundle` structure with validity flags
* 4 samples collected per cycle
* Median filtering using the last 3 samples
* JSON-formatted serial output
* Invalid readings do not crash the program

## Hardware

* ESP32 development board
* DHT11 sensor
* 10 kΩ pull-up resistor
* Jumper wires
* Breadboard

### Wiring

| DHT11 | ESP32  |
| ----- | ------ |
| VCC   | 3.3 V  |
| DATA  | GPIO 4 |
| GND   | GND    |

A **10 kΩ pull-up resistor** is connected between VCC and the DATA line.

## Software

* Arduino IDE
* ESP32 board package
* DHT sensor library
* `math.h` for `isnan()`

## Working

The ESP32 reads temperature and humidity from the DHT11 every 2 seconds.

If a reading fails, the program retries up to 3 times. Failed readings are marked as invalid instead of stopping the main loop.

Four samples are collected in each cycle. The last three samples are then passed through a median filter to reduce the effect of occasional noisy readings.

The filtered result is printed as JSON:

```json
{"temp":28.4,"humidity":65.2,"ok":true}
```

If the required samples are invalid, the system reports:

```json
{"temp":null,"humidity":null,"ok":false}
```

## Error Handling

The program handles:

* Invalid/NaN temperature readings
* Invalid/NaN humidity readings
* Sensor read failures
* Multiple retry attempts
* Invalid samples during median filtering

The ESP32 continues running even when sensor readings fail.

## Conclusion

The DHT11 sensor was successfully interfaced with the ESP32 using GPIO 4. Error handling, retry logic, median filtering, and structured JSON output were implemented to provide reliable sensor data for future IoT telemetry and MQTT tasks.

