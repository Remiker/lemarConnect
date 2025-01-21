/*
ADC Read voltage GPIO 2
*/
void setup() {
  Serial.begin(115200);
  /

}

void loop() {

   Serial.println(analogRead(2));
  delay(100);
  
}

//See more APIs about ADC here: https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/blob/master/esp32/cores/esp32/esp32-hal-adc.h

