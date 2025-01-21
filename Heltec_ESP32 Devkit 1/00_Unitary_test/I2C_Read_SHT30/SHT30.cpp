#include "SHT30.h"
#include <math.h>

SHT30::SHT30() {
}

boolean SHT30::begin(void) {
    Wire.begin();
    _i2caddr=SHT30_ADDR;
    reset();
    //return (readStatus() == 0x40);
    return true;
}
boolean SHT30::begin(uint8_t sda, uint8_t scl) {
    
    Wire.begin(sda,scl);
    _i2caddr = SHT30_ADDR;
    reset();
    //return (readStatus() == 0x40);
    return true;
}


//boolean S == Scale.  True == Farenheit; False == Celcius
float SHT30::getTemperature(bool S) {
    if (! getTempHum()) {
        return NAN;
    }
    if (S == true) {
        return convertCtoF(temp);
    }
    return temp;
}

float SHT30::convertCtoF(float c) {
    return c * 9 / 5 + 32;
}

float SHT30::getHumidity(void) {
    if (! getTempHum()) {
        return NAN;
    }
    return humidity;
}

void SHT30::reset(void) {
    writeCommand(SHT30_SOFTRESET);
    delay(10);
}

void SHT30::heater(boolean h) {
    if (h) {
        writeCommand(SHT30_HEATEREN);
    } else {
        writeCommand(SHT30_HEATERDIS);
    }
}

uint8_t SHT30::crc8(const uint8_t* data, int len) {
    const uint8_t POLYNOMIAL(0x31);
    uint8_t crc(0xFF);

    for (int j = len; j; --j) {
        crc ^= *data++;

        for (int i = 8; i; --i) {
            crc = (crc & 0x80)
                  ? (crc << 1) ^ POLYNOMIAL
                  : (crc << 1);
        }
    }
    return crc;
}


boolean SHT30::getTempHum(void) {
    uint8_t readbuffer[6];

    writeCommand(SHT30_MEAS_HIGHREP);

    delay(50);
    Wire.requestFrom(_i2caddr, (uint8_t)6);
    if (Wire.available() != 6) {
        return false;
    }
    for (uint8_t i = 0; i < 6; i++) {
        readbuffer[i] = Wire.read();
    }
    uint16_t ST, SRH;
    ST = readbuffer[0];
    ST <<= 8;
    ST |= readbuffer[1];

    if (readbuffer[2] != crc8(readbuffer, 2)) {
        return false;
    }

    SRH = readbuffer[3];
    SRH <<= 8;
    SRH |= readbuffer[4];

    if (readbuffer[5] != crc8(readbuffer + 3, 2)) {
        return false;
    }

    double stemp = ST;
    stemp *= 175;
    stemp /= 0xffff;
    stemp = -45 + stemp;
    temp = stemp;

    double shum = SRH;
    shum *= 100;
    shum /= 0xFFFF;

    humidity = shum;

    return true;
}

void SHT30::writeCommand(uint16_t cmd) {
    Wire.beginTransmission(_i2caddr);
    Wire.write(cmd >> 8);
    Wire.write(cmd & 0xFF);
    Wire.endTransmission();
}
