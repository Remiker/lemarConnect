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
  // Setting up serial
  Serial.begin(115200);
  while (!Serial)
    ;

  // Setting up RF Band
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };

  // Info on Module
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());


  // OTAA connection
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
  uint8_t buffers[8];
  //// Get data from sensors
  uint16_t temperature = ENV.readTemperature() * 100;
  uint16_t humidity = ENV.readHumidity() * 100;
  uint16_t pressure = ENV.readPressure() * 100;
  uint16_t illuminance = ENV.readIlluminance() * 100;
  ////////////////////////
  ///// Store Data in Buffer
  buffers[0] = byte(temperature >> 8);
  buffers[1] = byte(temperature & 0x00FF);  //  fix error on mask
  buffers[2] = byte(humidity >> 8);
  buffers[3] = byte(humidity & 0x00FF);
  buffers[4] = byte(pressure >> 8);
  buffers[5] = byte(pressure & 0x00FF);
  buffers[6] = byte(illuminance >> 8);
  buffers[7] = byte(illuminance & 0x00FF);
  ////////////////////////////////////


  ///// send data to TTN
  int err;


  modem.beginPacket();

  modem.write(buffers, 8);  // Sending of the two octets of temp via LoRa

  Serial.print("Temperature : ");
  Serial.println(ENV.readTemperature());
  
Serial.print("Humidity : ");
  Serial.println(ENV.readHumidity());
  
Serial.print("Pressure : ");
  Serial.println(ENV.readPressure());
  
Serial.print("Illuminance : ");
  Serial.println(temperaENV.readIlluminance());
  


  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
    //delay(10000);  // Try to get a longer delay for
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
  Serial.println();
}


void sendMessage() {
  Serial.println("Enter a message to send to network");
  Serial.println("(make sure that end-of-line 'NL' is enabled)");

  while (!Serial.available())
    ;
  String msg = "";

  Serial.print("Sending: " + msg + " - ");
  for (unsigned int i = 0; i < msg.length(); i++) {
    Serial.print(msg[i] >> 4, HEX);
    Serial.print(msg[i] & 0xF, HEX);
    Serial.print(" ");
  }
}

void recieveMessage() {
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
}