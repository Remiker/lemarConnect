#include <MKRWAN.h>
#include "arduino_secrets.h"
#include <Arduino_MKRENV.h>
#include <SPI.h>
#include <LoRa.h>
// JE FAIT UN TEST GITHUB 
LoRaModem modem;
int counter = 0;
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");

 if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };

delay(1000);

  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }
   int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }
  modem.minPollInterval(60);
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
    double temperature = ENV.readTemperature();
  // send packet
  LoRa.beginPacket();
  LoRa.print(temperature);
  LoRa.print(" celsius");
  LoRa.endPacket();

int err;
  modem.beginPacket();
  modem.print(temperature);
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
    Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
    Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
  }
  //delay(1000);
  if (!modem.available()) {
    Serial.println("No downlink message received at this time.");
    return;
  }

  counter++;
  delay(60*1000);
}