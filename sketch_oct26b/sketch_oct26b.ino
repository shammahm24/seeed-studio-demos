void setup() {
  Serial.begin(115200);  // Start serial communication
}

void loop() {
  Serial.println("ESP32-C3 is alive!");
  delay(1000);
}
