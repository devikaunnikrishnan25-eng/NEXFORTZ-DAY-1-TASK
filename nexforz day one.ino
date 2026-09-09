// ESP32 Blink Sketch
// Blinks the onboard LED (usually GPIO 2) on and off every second

#define LED_PIN 2  // Change this if your board's onboard LED is on a different pin

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("ESP32 Blink started");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // Turn LED on
  Serial.println("LED ON");
  delay(1000);                   // Wait 1 second

  digitalWrite(LED_PIN, LOW);    // Turn LED off
  Serial.println("LED OFF");
  delay(1000);                   // Wait 1 second
}