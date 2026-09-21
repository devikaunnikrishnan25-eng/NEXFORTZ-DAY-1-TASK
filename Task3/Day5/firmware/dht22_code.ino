#include <DHT.h>
#include <math.h>

// -------------------- DHT11 Configuration --------------------
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// -------------------- SensorBundle --------------------
struct SensorBundle {
  float temperature;
  float humidity;
  bool temperatureValid;
  bool humidityValid;
  bool ok;
};

// -------------------- Configuration --------------------
const int TOTAL_SAMPLES = 4;
const int MEDIAN_SIZE = 3;

const unsigned long SAMPLE_INTERVAL = 2000;

// Arrays to store the latest 4 readings
float temperatureSamples[TOTAL_SAMPLES];
float humiditySamples[TOTAL_SAMPLES];


// ============================================================
// Median of 3 values
// ============================================================
float median3(float a, float b, float c) {

  if (a > b) {
    float temp = a;
    a = b;
    b = temp;
  }

  if (b > c) {
    float temp = b;
    b = c;
    c = temp;
  }

  if (a > b) {
    float temp = a;
    a = b;
    b = temp;
  }

  return b;
}


// ============================================================
// Read DHT11 with up to 3 retries
// ============================================================
SensorBundle readSensor() {

  SensorBundle reading;

  reading.temperature = NAN;
  reading.humidity = NAN;
  reading.temperatureValid = false;
  reading.humidityValid = false;
  reading.ok = false;

  for (int attempt = 1; attempt <= 3; attempt++) {

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (!isnan(temperature) && !isnan(humidity)) {

      reading.temperature = temperature;
      reading.humidity = humidity;

      reading.temperatureValid = true;
      reading.humidityValid = true;
      reading.ok = true;

      return reading;
    }

    Serial.print("DHT11 read failed - attempt ");
    Serial.print(attempt);
    Serial.println("/3");

    if (attempt < 3) {
      delay(1000);
    }
  }

  Serial.println("DHT11 read failed after 3 attempts.");

  return reading;
}


// ============================================================
// Print JSON
// ============================================================
void printJSON(float temperature, float humidity, bool ok) {

  Serial.print("{");

  Serial.print("\"temp\":");

  if (ok) {
    Serial.print(temperature, 1);
  } else {
    Serial.print("null");
  }

  Serial.print(",");

  Serial.print("\"humidity\":");

  if (ok) {
    Serial.print(humidity, 1);
  } else {
    Serial.print("null");
  }

  Serial.print(",");

  Serial.print("\"ok\":");

  if (ok) {
    Serial.print("true");
  } else {
    Serial.print("false");
  }

  Serial.println("}");
}


// ============================================================
// SETUP
// ============================================================
void setup() {

  Serial.begin(115200);

  delay(1000);

  dht.begin();

  Serial.println();
  Serial.println("======================================");
  Serial.println("Nexforz Day 5 - DHT11 Sensor");
  Serial.println("======================================");

  Serial.println("GPIO: 4");
  Serial.println("Sensor: DHT11");
  Serial.println("Retries: 3");
  Serial.println("Samples per cycle: 4");
  Serial.println("Median filter: Last 3 samples");
  Serial.println();
}


// ============================================================
// LOOP
// ============================================================
void loop() {

  Serial.println("======================================");
  Serial.println("NEW 4-SAMPLE CYCLE");
  Serial.println("======================================");

  // ----------------------------------------------------------
  // Take 4 readings
  // ----------------------------------------------------------

  for (int i = 0; i < TOTAL_SAMPLES; i++) {

    SensorBundle reading = readSensor();

    if (reading.ok) {

      temperatureSamples[i] = reading.temperature;
      humiditySamples[i] = reading.humidity;

      Serial.print("Sample ");
      Serial.print(i + 1);
      Serial.print(": temperature_c = ");
      Serial.print(reading.temperature, 1);

      Serial.print(", humidity_percent = ");
      Serial.println(reading.humidity, 1);

    } else {

      // Store NAN for failed reading
      temperatureSamples[i] = NAN;
      humiditySamples[i] = NAN;

      Serial.print("Sample ");
      Serial.print(i + 1);
      Serial.println(": INVALID");
    }

    // Wait 2 seconds before next DHT11 reading
    if (i < TOTAL_SAMPLES - 1) {
      delay(SAMPLE_INTERVAL);
    }
  }


  // ----------------------------------------------------------
  // Median filtering
  // Use the LAST THREE samples
  // ----------------------------------------------------------

  Serial.println();
  Serial.println("----- MEDIAN FILTER -----");

  // Check whether last 3 samples are valid
  if (!isnan(temperatureSamples[1]) &&
      !isnan(temperatureSamples[2]) &&
      !isnan(temperatureSamples[3]) &&
      !isnan(humiditySamples[1]) &&
      !isnan(humiditySamples[2]) &&
      !isnan(humiditySamples[3])) {

    float filteredTemperature = median3(
      temperatureSamples[1],
      temperatureSamples[2],
      temperatureSamples[3]
    );

    float filteredHumidity = median3(
      humiditySamples[1],
      humiditySamples[2],
      humiditySamples[3]
    );

    Serial.println("Median uses Samples 2, 3 and 4.");

    Serial.print("Filtered temperature_c: ");
    Serial.println(filteredTemperature, 1);

    Serial.print("Filtered humidity_percent: ");
    Serial.println(filteredHumidity, 1);

    // --------------------------------------------------------
    // JSON output
    // --------------------------------------------------------

    Serial.println();
    Serial.println("SensorBundle JSON:");

    printJSON(
      filteredTemperature,
      filteredHumidity,
      true
    );

  } else {

    Serial.println("Median filter unavailable.");
    Serial.println("One or more of the last 3 samples is invalid.");

    Serial.println();
    Serial.println("SensorBundle JSON:");

    printJSON(NAN, NAN, false);
  }


  Serial.println();

  // Small pause before starting next cycle
  delay(2000);
}
