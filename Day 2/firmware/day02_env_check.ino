#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("=== ESP32 Day 2 Environment Check ===");

  // ESP32 Arduino Core version
  Serial.print("Arduino Core Version: ");
  Serial.println(ESP_ARDUINO_VERSION_STR);

  // Free heap memory
  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");

  // Chip information
  Serial.print("Chip Model: ");
  Serial.println(ESP.getChipModel());

  Serial.print("Chip Revision: ");
  Serial.println(ESP.getChipRevision());

  // CPU frequency
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
