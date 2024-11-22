#include <MKRWAN.h>
#include <Arduino_MKRENV.h>
#include <LoRa.h>

// Uncomment if using the Murata chip as a module
// LoRaModem modem(Serial1);

#include "arduino_secrets.h"
LoRaModem modem;
#define LORA_DELAY 60000
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;
int waitToSendMessage = 0;
int enableDebug = 0;

void setup()
{
  // Setting up serial
  Serial.begin(115200);

  // Setting up RF Band
  if (!modem.begin(EU868))
  {
    Serial.println("Failed to start module");
    while (1)
    {
    }
  };

  // Info on Module
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  // OTAA connection
  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected)
  {
    Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());
    while (!connected)
    {
      Serial.println("Something went wrong; are you indoor? Move near a window and retry");
      Serial.println("Retrying...");
      connected = modem.joinOTAA(appEui, appKey);
      delay(1000);
    }
  }

  // Set poll interval to 60 secs.
  modem.minPollInterval(60);
  // NOTE: independent of this setting, the modem will
  // not allow sending more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.

  if (!ENV.begin())
  {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1)
      ;
  }
  waitToSendMessage = millis();
}
uint16_t temperature;
  uint16_t humidity ;
  uint16_t pressure;
  uint16_t illuminance ;
   uint8_t buffer[8];
void loop()
{
 
  //// Get data from sensors
   temperature = ENV.readTemperature() * 100;
   humidity = ENV.readHumidity() * 100;
   pressure = ENV.readPressure() * 100;
   illuminance = ENV.readIlluminance() * 100;
  ////////////////////////
  ///// Store Data in Buffer
  buffer[0] = byte(temperature >> 8);
  buffer[1] = byte(temperature & 0x00FF);
  buffer[2] = byte(humidity >> 8);
  buffer[3] = byte(humidity & 0x00FF);
  buffer[4] = byte(pressure >> 8);
  buffer[5] = byte(pressure & 0x00FF);
  buffer[6] = byte(illuminance >> 8);
  buffer[7] = byte(illuminance & 0x00FF);
  ////////////////////////////////////

  /////////////// Data Debug
  if (enableDebug)
  {
    //Serial.print("Temperature : ");
    //Serial.println(ENV.readTemperature());

    //Serial.print("Humidity : ");
    //Serial.println(ENV.readHumidity());

    //Serial.print("Pressure : ");
    //Serial.println(ENV.readPressure());
for(int x;x<8;x++)
{
  illuminance += ENV.readIlluminance() * 100;
}
illuminance = illuminance/9;
    Serial.print("Illuminance : ");
    Serial.println(illuminance/100);
  }

  ///////////// send data to TTN
  if (millis() - waitToSendMessage > LORA_DELAY)
  {
    waitToSendMessage = millis();
    modem.beginPacket();
    modem.write(buffer, 8);
    int err;
    err = modem.endPacket(true);
    if (enableDebug)
    {
      if (err > 0)
      {
        Serial.println("Message sent correctly!");
      }
      else
      {
        Serial.println("Error sending message :(");
        Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
        Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
      }
    }
    if (!modem.available())
    {
      Serial.println("No downlink message received at this time.");
      return;
    }
  }
  /////////////////////////////////
}

void sendMessage()
{
  Serial.println("Enter a message to send to network");
  Serial.println("(make sure that end-of-line 'NL' is enabled)");

  while (!Serial.available())
    ;
  String msg = "";

  Serial.print("Sending: " + msg + " - ");
  for (unsigned int i = 0; i < msg.length(); i++)
  {
    Serial.print(msg[i] >> 4, HEX);
    Serial.print(msg[i] & 0xF, HEX);
    Serial.print(" ");
  }
}

void recieveMessage()
{
  char rcv[64];
  int i = 0;
  while (modem.available())
  {
    rcv[i++] = (char)modem.read();
  }
  Serial.print("Received: ");
  for (unsigned int j = 0; j < i; j++)
  {
    Serial.print(rcv[j] >> 4, HEX);
    Serial.print(rcv[j] & 0xF, HEX);
    Serial.print(" ");
  }
}