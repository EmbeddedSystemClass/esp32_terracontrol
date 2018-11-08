#ifndef HTU21D_H
#define HTU21D_H

#include "I2CMaster.hpp"

class Htu21d {
public:
  /***
   *  Constructor
   **/
  Htu21d(I2CMaster& i2cmaster);

  /***
   *  Reads the temperature
   **/
  float readTemperature();

  /***
   *  Reads the humidity
   **/
  float readHumidity();

private:
  /***
   *  Tries to read a value from the sensor
   **/
  uint16_t readValue(uint8_t cmd);

  /***
   *  Compares the crc
   **/
  uint8_t checkCRC(uint16_t message_from_sensor, uint8_t check_value_from_sensor);

private:
  I2CMaster& mI2c;
};

#endif
