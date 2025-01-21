/*
ADC Read voltage GPIO 2

*/

#define LIGHTSENSOR_PIN 2
void setup() {
  Serial.begin(115200);
}

void loop() {

  Serial.print(readIlluminance(LIGHTSENSOR_PIN));
  Serial.println(" lux");
  delay(100);
}


float readIlluminance(uint pin) {
  // read analog value and convert to mV
  float mV = (analogRead(pin) * 3300.0) / 1023.0;

  // 5 mV per lux
  float reading = (mV / 5.0);  // Readings are in lux scale

  return reading;  // 1 lux = 1 meter-candle
}

// UV formulas and constants based on:
//   https://www.vishay.com/docs/84339/designingveml6075.pdf