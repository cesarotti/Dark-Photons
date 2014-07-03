// the setup routine runs once when you press reset:
void setup() {
  /*
  initialize serial communication at
  300, 600, 1200, 2400, 4800, 9600, 14400, 19200,
  28800, 38400, 57600, or 115200 bits per second:
  */
  SerialUSB.begin(115200);
  while (!Serial);
}

// the loop routine runs over and over again forever:
void loop() {
  analogReadResolution(12);
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
}
