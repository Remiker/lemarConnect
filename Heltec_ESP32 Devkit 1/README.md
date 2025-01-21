# Devkit Heltec HT-CT62 (Wireless Mini Shell) : ESP32 C3 + SX1262
The HT-CT62 is an ESP32-based module featuring LoRa capabilities powered by the SX1262 chip.

This devkit was designed to simplify LoRa prototyping. The main PCB and sensors are optimized for seamless integration with breadboards, ensuring an easy and efficient prototyping experience. Additionally, we prioritized affordability, keeping the production cost per card under 10€, making it an accessible and cost-effective solution for developers.
[
![Picture](https://github.com/Remiker/lemarConnect/blob/tl_branch/Heltec_ESP32%20Devkit%201/_assets/HTCT62%20and%20Sensors.jpg?raw=true)
## How to run the project
# Hardware :

- A CP2102 programer (or any other type of TTL to UART converter)-
  - Without RTS and DTR : [HiLego - CP2102 ](https://www.amazon.fr/HiLetgo-CP2102-Module-convertisseur-s%C3%A9rie/dp/B00LODGRV8)
  - With RTS and DTR : [USB TTL Module](https://www.amazon.fr/dp/B092ZQ4T9D?ref=ppx_yo2ov_dt_b_fed_asin_title)
- 2 Breadboard clipped on the sides (It doesnt fit on one)
- Any alimentation that can deliver 3.3V and up to 100mA (mabe in the worst condition 150mA, we never came above 100mA)
### Software:
- Arduino IDE
- Install the Heltec Boards and Library see this link : [here](https://docs.heltec.org/en/node/esp32/esp32_general_docs/quick_start.html)
- Install SHT30 Library I modified from Groove Sensor SHT31 Library  :  [Link Here](https://github.com/Seeed-Studio/Grove_SHT31_Temp_Humi_Sensor)  // Added the option of choosing the SDA/SCL pins // Note also that this library works with other board

## Documentation and schematics :
All the schematics and PCB CAD are made on Altium Designer, be carefull some parte of the documentation might be in french

