# DAY 5 – TASK 3: DHT SENSOR ON ESP32

## 1. Objective

The objective of this task is to interface a DHT sensor with an ESP32 and develop a reliable temperature and humidity monitoring system with error handling and JSON serial output.

The implementation includes:

* Temperature and humidity measurement
* `NaN` detection
* Automatic retry mechanism
* Multiple sensor samples
* Median filtering
* Structured JSON output
* Continuous operation without crashing on sensor errors

The assigned specification mentions a DHT22 sensor. For the actual hardware implementation, a **DHT11 sensor** was used.

---

## 2. Hardware Used

* ESP32 development board
* DHT11 sensor
* 10 kΩ pull-up resistor
* Breadboard
* Jumper wires
* USB cable

---

## 3. Hardware Connections

The DHT11 sensor was connected to **GPIO 4** of the ESP32.

| DHT11 Pin | ESP32  |
| --------- | ------ |
| VCC       | 3.3 V  |
| DATA      | GPIO 4 |
| GND       | GND    |

A 10 kΩ pull-up resistor was connected between the VCC and DATA lines.

---

## 4. Software and Libraries

The implementation was developed using the Arduino IDE.

### Libraries Used

```cpp
#include <DHT.h>
#include <math.h>
```

* `DHT.h` is used for communication with the DHT sensor.
* `math.h` provides the `isnan()` function used to detect invalid sensor readings.

---

## 5. Sensor Configuration

The sensor is configured on GPIO 4.

```cpp
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
```

The DHT sensor is initialized in the `setup()` function using:

```cpp
dht.begin();
```

---

## 6. SensorBundle Structure

A `SensorBundle` structure is used to organize the sensor data and validity information.

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
* Temperature validity status
* Humidity validity status
* Overall sensor status

This allows the program to distinguish between valid and invalid readings.

---

## 7. Sensor Reading and Error Handling

The `readSensor()` function is responsible for obtaining temperature and humidity values.

A reading is considered successful only when both values are valid.

```cpp
if (!isnan(temperature) && !isnan(humidity))
```

If either value is `NaN`, the reading is considered invalid.

The program retries the sensor read up to **three times**.

```text
Sensor Reading
      ↓
Check temperature and humidity
      ↓
   Valid?
   /    \
 Yes     No
  ↓       ↓
Accept   Retry
          ↓
       Attempt 2
          ↓
       Attempt 3
          ↓
   Mark as INVALID
```

A failed attempt is displayed on the Serial Monitor:

```text
DHT11 read failed - attempt 1/3
```

If all three attempts fail:

```text
DHT11 read failed after 3 attempts.
```

The program then continues running instead of stopping.

---

## 8. Sample Collection

The program collects **four samples during each cycle**.

```cpp
const int TOTAL_SAMPLES = 4;
```

Temperature and humidity values are stored separately:

```cpp
float temperatureSamples[TOTAL_SAMPLES];
float humiditySamples[TOTAL_SAMPLES];
```

A **2-second interval** is maintained between sensor readings:

```cpp
const unsigned long SAMPLE_INTERVAL = 2000;
```

If a sensor reading is unsuccessful, `NAN` is stored for that sample.

---

## 9. Median Filtering

A median filter is used to reduce the effect of a single abnormal reading.

The program uses the **last three samples** from each four-sample cycle.

For example:

```text
Sample 2 = 28.2
Sample 3 = 35.0
Sample 4 = 28.4
```

After sorting:

```text
28.2
28.4
35.0
```

The median value is:

```text
28.4
```

Therefore, the abnormal value does not directly become the final filtered result.

The same method is applied to both temperature and humidity.

---

## 10. Median Filter Validation

Before calculating the median, the program checks whether the last three samples are valid.

The filter is applied only when:

* Sample 2 temperature is valid
* Sample 3 temperature is valid
* Sample 4 temperature is valid
* Sample 2 humidity is valid
* Sample 3 humidity is valid
* Sample 4 humidity is valid

If any of these readings are invalid, the median filter is skipped.

The Serial Monitor displays:

```text
Median filter unavailable.
One or more of the last 3 samples is invalid.
```

This prevents invalid values from being used in the filtered result.

---

## 11. JSON Serial Output

The final sensor result is printed in JSON format.

For a valid reading:

```json
{"temp":28.4,"humidity":65.0,"ok":true}
```

The fields are:

| Field      | Description                                  |
| ---------- | -------------------------------------------- |
| `temp`     | Filtered temperature in °C                   |
| `humidity` | Filtered relative humidity in %              |
| `ok`       | Indicates whether the final reading is valid |

When the final reading is invalid:

```json
{"temp":null,"humidity":null,"ok":false}
```

This format makes the sensor data suitable for future IoT telemetry and MQTT communication.

---

## 12. JSON Generation

The `printJSON()` function generates the structured serial output.

For valid sensor data:

```cpp
printJSON(
  filteredTemperature,
  filteredHumidity,
  true
);
```

For invalid data:

```cpp
printJSON(NAN, NAN, false);
```

Therefore, the output clearly indicates whether the sensor data can be used.

---

## 13. Program Flow

The overall operation of the program is:

```text
ESP32 Start
     ↓
Initialize Serial Monitor
     ↓
Initialize DHT11
     ↓
Start sampling cycle
     ↓
Take sensor reading
     ↓
Check for NaN
     ↓
Retry up to 3 times if required
     ↓
Store reading
     ↓
Wait 2 seconds
     ↓
Repeat until 4 samples are collected
     ↓
Check last 3 samples
     ↓
All valid?
   /       \
 Yes        No
  ↓          ↓
Median      Mark
Filter      output invalid
  ↓          ↓
JSON Output
     ↓
Start next cycle
```

---

## 14. Serial Monitor Output

The Serial Monitor operates at:

```text
115200 baud
```

At startup, the program displays the configuration:

```text
======================================
Nexforz Day 5 - DHT11 Sensor
======================================
GPIO: 4
Sensor: DHT11
Retries: 3
Samples per cycle: 4
Median filter: Last 3 samples
```

A normal sampling cycle produces output similar to:

```text
======================================
NEW 4-SAMPLE CYCLE
======================================

Sample 1: temperature_c = XX.X, humidity_percent = XX.X
Sample 2: temperature_c = XX.X, humidity_percent = XX.X
Sample 3: temperature_c = XX.X, humidity_percent = XX.X
Sample 4: temperature_c = XX.X, humidity_percent = XX.X

----- MEDIAN FILTER -----
Median uses Samples 2, 3 and 4.

Filtered temperature_c: XX.X
Filtered humidity_percent: XX.X

SensorBundle JSON:
{"temp":XX.X,"humidity":XX.X,"ok":true}
```

The exact numerical values depend on the readings obtained from the sensor during testing.

---

## 15. Error Handling

The following error-handling mechanisms were implemented:

### Invalid Sensor Readings

`isnan()` is used to detect invalid temperature and humidity values.

### Automatic Retries

A failed sensor reading is retried up to three times.

### Invalid Sample Storage

If all attempts fail, the sample is stored as `NAN`.

### Filter Protection

The median filter is used only when the required samples are valid.

### Continuous Execution

A failed sensor reading does not terminate the ESP32 program. The next cycle continues normally.

---

## 16. Testing and Verification

The implementation was tested using the ESP32 and DHT11 sensor.

The following features were implemented and verified:

| Test                          | Status    |
| ----------------------------- | --------- |
| ESP32 initialization          | Completed |
| DHT sensor initialization     | Completed |
| GPIO 4 configuration          | Completed |
| Temperature reading           | Completed |
| Humidity reading              | Completed |
| `NaN` detection               | Completed |
| Three-attempt retry mechanism | Completed |
| Four-sample collection        | Completed |
| 2-second sampling interval    | Completed |
| Median filtering              | Completed |
| JSON output                   | Completed |
| Invalid JSON output           | Completed |
| Continuous program execution  | Completed |

---

## 17. Five-Minute Serial Log

A continuous Serial Monitor log is required as part of the submission.

The ESP32 was monitored using the Serial Monitor at **115200 baud** to observe the sensor readings, error handling, filtering, and JSON output.

The five-minute log demonstrates:

* Continuous sensor operation
* Repeated temperature readings
* Repeated humidity readings
* Sensor retry behavior
* Sample collection
* Median filtering
* JSON output
* Validity status

The actual Serial Monitor output should be saved or captured directly from the hardware test and submitted along with the sketch.

---

## 18. Optional Deep-Sleep Variant

Deep sleep was listed as an optional bonus feature.

The current implementation focuses on the main sensor-reading requirements and does not include the deep-sleep variant.

The core implementation therefore concentrates on:

* Reliable sensor reading
* Error handling
* Retry mechanism
* Median filtering
* JSON serial output

---

## 19. Learning Outcomes

This task provided practical experience with:

* Interfacing a DHT sensor with ESP32
* Reading temperature and humidity data
* Detecting `NaN` sensor values
* Implementing retry mechanisms
* Handling sensor failures
* Using structures for sensor data
* Collecting multiple samples
* Applying median filtering
* Generating JSON telemetry
* Designing sensor code for continuous operation

---

## 20. Future Scope

The implemented JSON data structure can be extended for future IoT applications.

Possible improvements include:

* Sending the JSON payload to an IoT cloud platform
* Publishing the data using MQTT
* Adding timestamps to sensor readings
* Displaying sensor values on a web dashboard
* Adding additional environmental sensors
* Implementing ESP32 deep sleep for power-efficient operation

---

## 21. Conclusion

The DHT sensor was successfully interfaced with the ESP32 using GPIO 4.

The implementation provides robust handling of invalid sensor readings by checking for `NaN` values and retrying failed reads up to three times. Four samples are collected during each cycle, and the last three valid samples are processed using a median filter to reduce the effect of individual abnormal readings.

The final temperature and humidity values are provided in a structured JSON format containing `temp`, `humidity`, and `ok` fields. When valid data is unavailable, the system outputs `null` values and sets `ok` to `false`.

The program continues operating even when individual sensor readings fail, providing a reliable foundation for future IoT telemetry and MQTT-based communication.

**Day 5 – Task 3 implementation completed successfully.**
