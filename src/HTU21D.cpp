#include "HTU21D.hpp"

#define HTU21D_ADDRESS 0x40  //Unshifted 7-bit I2C address for the sensor

#define ERROR_READING_VALUE 0xFFFF

#define TRIGGER_TEMP_MEASURE_HOLD  0xE3
#define TRIGGER_HUMD_MEASURE_HOLD  0xE5
#define TRIGGER_TEMP_MEASURE_NOHOLD  0xF3
#define TRIGGER_HUMD_MEASURE_NOHOLD  0xF5
#define WRITE_USER_REG  0xE6
#define READ_USER_REG  0xE7
#define SOFT_RESET  0xFE

#define USER_REGISTER_RESOLUTION_MASK 0x81
#define USER_REGISTER_RESOLUTION_RH12_TEMP14 0x00
#define USER_REGISTER_RESOLUTION_RH8_TEMP12 0x01
#define USER_REGISTER_RESOLUTION_RH10_TEMP13 0x80
#define USER_REGISTER_RESOLUTION_RH11_TEMP11 0x81

#define USER_REGISTER_END_OF_BATTERY 0x40
#define USER_REGISTER_HEATER_ENABLED 0x04
#define USER_REGISTER_DISABLE_OTP_RELOAD 0x02

Htu21d::Htu21d(I2CMaster& i2cmaster) : mI2c(i2cmaster) {

}

float Htu21d::readTemperature() {
  // Reads the temperature from the sensor
  uint16_t rawTemperature = readValue(TRIGGER_TEMP_MEASURE_NOHOLD);

  // Checks weather an error occured
  // TODO

  //Given the raw temperature data, calculate the actual temperature
  float tempTemperature = rawTemperature * (175.72 / 65536.0); //2^16 = 65536
  float realTemperature = tempTemperature - 46.85; //From page 14

  return (realTemperature);
}

float Htu21d::readHumidity() {
  // Reads the humidity from the sensor
  uint16_t rawHumidity = readValue(TRIGGER_HUMD_MEASURE_NOHOLD);

  // Checks weather an error occured
  // TODO

  //Given the raw humidity data, calculate the actual relative humidity
  float tempRH = rawHumidity * (125.0 / 65536.0); //2^16 = 65536
  float rh = tempRH - 6.0; //From page 14

  return (rh);
}

uint16_t Htu21d::readValue(uint8_t cmd) {
  // Request a value reading
  this->mI2c.write(HTU21D_ADDRESS, &cmd, 1);

  // Begin reading
  uint8_t counter = 0;
  uint8_t recBuffer[3];
  bool validResult = false;
  do {
    // Wait for 50ms to give the sensor the chance to read the values
    vTaskDelay(50 / portTICK_RATE_MS);

    // Try to receive
    if (!this->mI2c.read(HTU21D_ADDRESS, recBuffer, 3)) {
        // We did not received a valid result
    } else {
      // we received a valid result end the loop here
      validResult = true;
      break;
    }
  } while (counter++ < 3);

  // End here when no valid result is provided
  if (!validResult) {
    printf("%s: No valid result\n", __PRETTY_FUNCTION__);
    return ERROR_READING_VALUE;
  }

  // Get the raw value from the receive buffer
  uint16_t rawValue = ((uint16_t) recBuffer[0] << 8) | (uint16_t) recBuffer[1];

  // Check the crc code
  if (checkCRC(rawValue, recBuffer[2]) != 0) {
    printf("%s: Invalid crc\n", __PRETTY_FUNCTION__);
    return ERROR_READING_VALUE;
  }

  return rawValue & 0xFFFC; // Zero out the status bits
}

//Give this function the 2 byte message (measurement) and the check_value byte from the HTU21D
//If it returns 0, then the transmission was good
//If it returns something other than 0, then the communication was corrupted
//From: http://www.nongnu.org/avr-libc/user-manual/group__util__crc.html
//POLYNOMIAL = 0x0131 = x^8 + x^5 + x^4 + 1 : http://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks
#define SHIFTED_DIVISOR 0x988000 //This is the 0x0131 polynomial shifted to farthest left of three bytes

uint8_t Htu21d::checkCRC(uint16_t message_from_sensor, uint8_t check_value_from_sensor) {
  //Test cases from datasheet:
  //message = 0xDC, checkvalue is 0x79
  //message = 0x683A, checkvalue is 0x7C
  //message = 0x4E85, checkvalue is 0x6B

  uint32_t remainder = (uint32_t)message_from_sensor << 8; //Pad with 8 bits because we have to add in the check value
  remainder |= check_value_from_sensor; //Add on the check value

  uint32_t divsor = (uint32_t)SHIFTED_DIVISOR;

  for (int i = 0 ; i < 16 ; i++) //Operate on only 16 positions of max 24. The remaining 8 are our remainder and should be zero when we're done.
  {
    //Serial.print("remainder: ");
    //Serial.println(remainder, BIN);
    //Serial.print("divsor:    ");
    //Serial.println(divsor, BIN);
    //Serial.println();

    if ( remainder & (uint32_t)1 << (23 - i) ) //Check if there is a one in the left position
      remainder ^= divsor;

    divsor >>= 1; //Rotate the divsor max 16 times so that we have 8 bits left of a remainder
  }

  return (uint8_t)remainder;
}
