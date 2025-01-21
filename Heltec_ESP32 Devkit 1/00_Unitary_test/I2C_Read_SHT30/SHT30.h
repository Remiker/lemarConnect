#ifndef _SHT30_H_
#define _SHT30_H_

#include "Arduino.h"
#include "Wire.h"

#define SHT30_ADDR    0x44
#define SHT30_MEAS_HIGHREP_STRETCH 0x2C06
#define SHT30_MEAS_MEDREP_STRETCH  0x2C0D
#define SHT30_MEAS_LOWREP_STRETCH  0x2C10
#define SHT30_MEAS_HIGHREP         0x2400
#define SHT30_MEAS_MEDREP          0x240B
#define SHT30_MEAS_LOWREP          0x2416
#define SHT30_READSTATUS           0xF32D
#define SHT30_CLEARSTATUS          0x3041
#define SHT30_SOFTRESET            0x30A2
#define SHT30_HEATEREN             0x306D
#define SHT30_HEATERDIS            0x3066

class SHT30 {
  public:
    SHT30();
    boolean begin(uint8_t sda, uint8_t scl);
    boolean begin(void);
    float getTemperature(bool S = false);
    float convertCtoF(float);
    float getHumidity(void);
    void reset(void);
    void heater(boolean);
    uint8_t crc8(const uint8_t* data, int len);

  private:
    boolean getTempHum(void);
    void writeCommand(uint16_t cmd);

    uint8_t _i2caddr;
    float humidity, temp;

};

#endif
