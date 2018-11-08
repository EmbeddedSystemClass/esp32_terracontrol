#include "I2CMaster.hpp"

I2CMaster::I2CMaster(i2c_port_t i2cChannel, gpio_num_t sdaPin, gpio_num_t sclPin)
        : mI2cChannel(i2cChannel), mSdaPin(sdaPin), mSclPin(sclPin)  {

}

void I2CMaster::init() {
        i2c_config_t conf;

        // Configures this i2c channel as master
        conf.mode = I2C_MODE_MASTER;

        // Configures the SDA line pin
        conf.sda_io_num = this->mSdaPin;

        // Configures that the SDA pin has a pull up
        conf.sda_pullup_en = GPIO_PULLUP_ENABLE;

        // Configures the SCL pin
        conf.scl_io_num = this->mSclPin;

        // Configures that the SCL pin has a pull up
        conf.scl_pullup_en = GPIO_PULLUP_ENABLE;

        // Configures the clock speed in Hz
        conf.master.clk_speed = 100000;

        // Apply configuration to this channel
        i2c_param_config(this->mI2cChannel, &conf);

        // Install the driver to make it usable from now on
        i2c_driver_install(this->mI2cChannel, conf.mode, 0, 0, 0);
}

bool I2CMaster::write(uint8_t address, uint8_t *pData, uint8_t size) {
        // Creates a command handle
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);

        // Write the slave address and master write mode in first byte
        i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_WRITE, true);

        // Write the data
        for (uint8_t i = 0; i < size; i++) {
                i2c_master_write_byte(cmd, pData[i], true);
        }

        // Finish the command
        i2c_master_stop(cmd);

        // Execute the command with a timeout of 1 second
        esp_err_t ret = i2c_master_cmd_begin(this->mI2cChannel, cmd, 1000 / portTICK_RATE_MS);

        // Unlink the command handle (free memory)
        i2c_cmd_link_delete(cmd);

        // Check if send was successful
        if (ret == ESP_OK) {
                return true;
        }

        return false;
}

bool I2CMaster::read(uint8_t address, uint8_t *pData, uint8_t size) {
        // Create a command i2c_cmd_handle_t
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);

        // Write in the first byte the address of the slave and that the master wants to read from it
        i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_READ, true);

        // Read the bytes
        for (uint8_t i = 0; i < size; i++) {
                i2c_master_read_byte(cmd, &pData[i], 0);
        }

        // Finish the command
        i2c_master_stop(cmd);

        // Try to receive it from the slave
        esp_err_t ret = i2c_master_cmd_begin(this->mI2cChannel, cmd, 1000 / portTICK_RATE_MS);

        // Unlink the command handle (free memory)
        i2c_cmd_link_delete(cmd);

        // Check if receive was successful
        if (ret == ESP_OK) {
                return true;
        }

        return false;
}
