#include <SHT30.h>
#define I2C_SDA 18
#define I2C_SCL 19

// Library

SHT30 sht30 = SHT30();


void setup() {
  Serial.begin(115200);
  Serial.println("begin...");
  sht30.begin(I2C_SDA,I2C_SCL);
}

void loop() {
  float temp = sht30.getTemperature();
  float hum = sht30.getHumidity();
  Serial.print("Temp = ");
  Serial.print(temp);
  Serial.println(" C");  //The unit for  Celsius because original arduino don't support speical symbols
  Serial.print("Hum = ");
  Serial.print(hum);
  Serial.println("%");
  Serial.println();
  delay(1000);
}
