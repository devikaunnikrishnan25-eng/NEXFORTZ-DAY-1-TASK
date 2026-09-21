# DAY 5 – DHT SENSOR WITH ERROR HANDLING

## 1. Introduction

The objective of Day 5 was to interface a temperature and humidity sensor with the ESP32 and implement reliable sensor reading with proper error handling.

The Nexforz IoT Learning Lab task specifies the use of a DHT22 sensor. For the hardware implementation, a **DHT11 sensor** was used instead. The DHT11 was connected to **GPIO 4** of the ESP32.

The program was designed to handle failed sensor readings without stopping the ESP32 program. It also collects multiple samples and applies a median filter to reduce the effect of occasional abnormal readings. The final sensor data is displayed in a structured JSON format for future IoT and MQTT integration.

---

## 2. Objectives

The main objectives of this task were:

* Interface a DHT sensor with the ESP32.
* Read temperature and humidity values.
* Use GPIO 4 for sensor data.
* Implement error handling for invalid sensor readings.
* Retry failed readings up to three times.
* Collect multiple sensor samples.
* Apply a median filter to the latest three samples.
* Prevent sensor failures from crashing the main program.
* Generate structured JSON output.

---

## 3. Hardware Requirements

The following hardware was used:

* ESP32 development board
* DHT11 temperature and humidity sensor
* 10 kΩ pull-up resistor
* Breadboard
* Jumper wires
* USB cable

---

## 4. Software Requirements

The following software and libraries were used:

* Arduino IDE
* ESP32 Arduino core
* DHT sensor library
* `math.h` library for `isnan()` functionality

---

## 5. Hardware Connections

The DHT11 sensor was connected to the ESP32 as follows:

| DHT11 Pin | ESP32 Connection |
| --------- | ---------------- |
| VCC       | 3.3 V            |
| DATA      | GPIO 4           |
| GND       | GND              |

A 10 kΩ pull-up resistor was used between the VCC and DATA lines.

### Connection Diagram

```text
        DHT11
      ┌─────────┐
 VCC ─┤         ├── 3.3V
 DATA ┤         ├── GPIO 4
 GND ─┤         ├── GND
      └─────────┘
         │
      10 kΩ
     Pull-up
```

---

## 6. Program Structure

The program is divided into several main sections:

1. DHT11 configuration
2. `SensorBundle` structure
3. Sample storage
4. Median filtering
5. Sensor reading with retries
6. JSON output
7. Setup
8. Main loop

---

## 7. SensorBundle Structure

A `SensorBundle` structure was created to store the sensor readings and their validity status.

```cpp
struct SensorBundle {
  float temperature;
  float humidity;
  bool temperatureValid;
  bool humidityValid;
  bool ok;
};
```

The structure stores:

* Temperature value
* Humidity value
* Temperature validity
* Humidity validity
* Overall reading status

This makes it easier to determine whether a sensor reading is valid before using it.

---

## 8. Sensor Reading and Error Handling

The `readSensor()` function reads temperature and humidity from the DHT11.

The program attempts to read the sensor a maximum of **three times** if the first attempt fails.

The sensor values are checked using `isnan()`.

If both temperature and humidity are valid:

```text
Reading successful
       ↓
Store temperature
       ↓
Store humidity
       ↓
Mark reading as valid
```

If the reading fails:

```text
Reading failed
      ↓
Retry
      ↓
Maximum 3 attempts
      ↓
If all fail → Mark reading INVALID
```

The program prints the failed attempt number to the Serial Monitor.

Example:

```text
DHT11 read failed - attempt 1/3
```

If all three attempts fail:

```text
DHT11 read failed after 3 attempts.
```

The program does not terminate or crash when this happens.

---

## 9. Sampling Method

The program collects **four sensor readings per cycle**.

```cpp
const int TOTAL_SAMPLES = 4;
```

There is a **2-second interval** between consecutive readings:

```cpp
const unsigned long SAMPLE_INTERVAL = 2000;
```

Each sample is stored in arrays:

```cpp
float temperatureSamples[TOTAL_SAMPLES];
float humiditySamples[TOTAL_SAMPLES];
```

Valid readings are stored normally, while failed readings are stored as `NAN`.

---

## 10. Median Filtering

A median filter is used to reduce the effect of sudden abnormal sensor values.

The program uses the **last three samples**, which are Samples 2, 3 and 4.

For example, if three temperature readings are:

```text
28.2
35.0
28.4
```

The sorted values are:

```text
28.2
28.4
35.0
```

The median value is:

```text
28.4 °C
```

Therefore, the abnormal value of 35.0 °C does not directly affect the final filtered result.

The same median filtering process is applied to humidity.

---

## 11. Median Filter Validation

Before applying the median filter, the program checks whether the last three samples are valid.

The filter is applied only when:

* Sample 2 temperature is valid
* Sample 3 temperature is valid
* Sample 4 temperature is valid
* Sample 2 humidity is valid
* Sample 3 humidity is valid
* Sample 4 humidity is valid

If any of these readings are invalid, the median filter is not used.

The Serial Monitor then displays:

```text
Median filter unavailable.
One or more of the last 3 samples is invalid.
```

---

## 12. JSON Output

After successful filtering, the final sensor values are printed in JSON format.

Example:

```json
{"temp":28.4,"humidity":65.2,"ok":true}
```

The JSON fields are:

| Field      | Description                                  |
| ---------- | -------------------------------------------- |
| `temp`     | Filtered temperature in °C                   |
| `humidity` | Filtered relative humidity in %              |
| `ok`       | Indicates whether the final reading is valid |

If the required samples are not valid, the program outputs:

```json
{"temp":null,"humidity":null,"ok":false}
```

This format can be used later for IoT telemetry, MQTT communication, and cloud integration.

---

## 13. Serial Monitor Output

The program prints information about every sampling cycle.

A typical output structure is:

```text
======================================
NEW 4-SAMPLE CYCLE
======================================

Sample 1: temperature_c = 28.3, humidity_percent = 64.0
Sample 2: temperature_c = 28.4, humidity_percent = 64.0
Sample 3: temperature_c = 28.4, humidity_percent = 65.0
Sample 4: temperature_c = 28.3, humidity_percent = 65.0

----- MEDIAN FILTER -----
Median uses Samples 2, 3 and 4.

Filtered temperature_c: 28.4
Filtered humidity_percent: 65.0

SensorBundle JSON:
{"temp":28.4,"humidity":65.0,"ok":true}
```

---

## 14. Error Handling Implemented

The following error-handling mechanisms were implemented:

### 14.1 NaN Detection

The program checks for invalid sensor values using:

```cpp
isnan()
```

### 14.2 Retry Mechanism

Each failed sensor reading is retried up to three times.

### 14.3 Invalid Sample Handling

If all three attempts fail, the sample is marked as invalid using `NAN`.

### 14.4 Median Filter Validation

The median filter is used only when all three required samples are valid.

### 14.5 Continuous Program Execution

Sensor failures do not stop the ESP32 `loop()` function. The program continues with the next sampling cycle.

---

## 15. Testing

The implementation was tested by connecting the DHT11 to GPIO 4 and monitoring the output through the Arduino IDE Serial Monitor.

The following functionality was verified:

* ESP32 successfully initializes the DHT11.
* Temperature readings are obtained.
* Humidity readings are obtained.
* Four samples are collected per cycle.
* A 2-second interval is maintained between readings.
* Failed readings are detected.
* Failed readings are retried.
* Invalid samples are handled using `NAN`.
* Median filtering is applied to the last three samples when valid.
* JSON output is generated successfully.
* The program continues running even when a reading fails.

---

## 16. Learning Outcomes

Through this task, the following concepts were learned:

* Interfacing a DHT sensor with ESP32.
* Reading temperature and humidity data.
* Handling unreliable sensor readings.
* Using retry mechanisms for sensor communication.
* Working with `NAN` values and `isnan()`.
* Using structures to organize sensor data.
* Applying a median filter to sensor readings.
* Generating machine-readable JSON output.
* Designing sensor code that can continue operating despite individual read failures.

---

## 17. Future Scope

The sensor data generated in this task can be used in future IoT applications.

Possible extensions include:

* Sending sensor data to the Nexforz IoT Lab.
* Publishing sensor readings using MQTT.
* Storing telemetry data in a cloud platform.
* Adding timestamps to sensor readings.
* Monitoring temperature and humidity remotely.
* Adding additional sensors for environmental monitoring.

---

## 18. Conclusion

The Day 5 DHT sensor task was successfully implemented using an ESP32 and DHT11 sensor.

The system reads temperature and humidity through GPIO 4, performs up to three retries for failed readings, stores multiple samples, and applies a median filter to the last three valid samples. Invalid readings are handled safely without stopping the program.

The final filtered sensor values are converted into a simple JSON structure containing temperature, humidity, and validity status. This provides a suitable foundation for the upcoming IoT telemetry and MQTT tasks.

**Day 5 implementation completed successfully.**

