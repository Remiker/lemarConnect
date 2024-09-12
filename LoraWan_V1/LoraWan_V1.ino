/*#include <MKRWAN.h>
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
  Serial.begin(115200);
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
  delay(5000);
  Serial.print("Sending packet: ");
  Serial.println(counter);
    double temperature = ENV.readTemperature();
  // send packet
  //LoRa.beginPacket();
  //LoRa.print(temperature);
  //LoRa.print(" celsius");
  //LoRa.endPacket();

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
  delay(1000);
  if (!modem.available()) {
    Serial.println("No downlink message received at this time.");
    return;
  }

  counter++;
  delay(70000);
}*/
/*
  Lora Send And Receive
  This sketch demonstrates how to send and receive data with the MKR WAN 1300/1310 LoRa module.
  This example code is in the public domain.
*/

#include <MKRWAN.h>
#include <Arduino_MKRENV.h>
#include <LoRa.h>
LoRaModem modem;

// Uncomment if using the Murata chip as a module
// LoRaModem modem(Serial1);

#include "arduino_secrets.h"
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
    ;
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }

  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  // NOTE: independent of this setting, the modem will
  // not allow sending more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.
  delay(1000);

  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1)
      ;
  }
}

void loop() {
  Serial.println();
  //Serial.println("Enter a message to send to network");
  //Serial.println("(make sure that end-of-line 'NL' is enabled)");

  //while (!Serial.available());
  //String msg ="";

  Serial.println();
  //  Serial.print("Sending: " + msg + " - ");
  /*for (unsigned int i = 0; i < msg.length(); i++) {
    Serial.print(msg[i] >> 4, HEX);
    Serial.print(msg[i] & 0xF, HEX);
    Serial.print(" ");
  }*/
  Serial.println();
  uint8_t buffers[10];
  uint16_t temperature = ENV.readTemperature() * 100;
  buffers[0] = byte(temperature >> 8);
  buffers[1] = byte(temperature & 0x00FF);
  int err;
  modem.beginPacket();

  modem.write(buffers, 2);
  // modem.print(temperature,HEX);
  // modem.print(temperature);\
  Serial.print("temperature : ");
  Serial.println(temperature);
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
    delay(10000);
  } else {
    Serial.println("Error sending message :(");
    Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
    Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
  }
  delay(1000);
  if (!modem.available()) {
    Serial.println("No downlink message received at this time.");
    return;
  }
  char rcv[64];
  int i = 0;
  while (modem.available()) {
    rcv[i++] = (char)modem.read();
  }
  Serial.print("Received: ");
  for (unsigned int j = 0; j < i; j++) {
    Serial.print(rcv[j] >> 4, HEX);
    Serial.print(rcv[j] & 0xF, HEX);
    Serial.print(" ");
  }
  Serial.println();
}
