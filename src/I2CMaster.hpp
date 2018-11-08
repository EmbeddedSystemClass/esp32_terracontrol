#ifndef I2CMASTER_H
#define I2CMASTER_H

#include "driver/i2c.h"

class I2CMaster {
public:
  /***
   *  Constructor
   **/
  I2CMaster(i2c_port_t i2cChannel, gpio_num_t sdaPin, gpio_num_t sclPin);

  /***
   *  Initializes this class
   **/
  void init();

  /***
   *  Sends data to the slave with given address
   **/
  bool write(uint8_t address, uint8_t* pData, uint8_t size);

  /***
   *  Receives data from a slave with given address
   **/
  bool read(uint8_t address, uint8_t* pData, uint8_t size);

private:
  i2c_port_t mI2cChannel;
  gpio_num_t mSdaPin;
  gpio_num_t mSclPin;
};

#endif
