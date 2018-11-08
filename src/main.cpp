#include <stdio.h>
#include <string.h>
#include <map>
#include <cmath>
#include "esp_system.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include <lwip/err.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/netdb.h>
#include <lwip/dns.h>

#include "esp32/System.hpp"
#include "esp32/Wifi.hpp"

#include "WS2812.hpp"
#include "HTU21D.hpp"

#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                            0x0              /*!< I2C ack value */
#define NACK_VAL                           0x1              /*!< I2C nack value */

extern "C" {
void app_main();
}

I2CMaster i2cMaster(I2C_NUM_1, GPIO_NUM_18, GPIO_NUM_19);

void led_test_task(void* pvParameter) {
        const uint16_t numLeds = 31;
        WS2812 ws(GPIO_NUM_23, numLeds);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("Starting LED Test...\n");
        while (1) {
                for (uint16_t i = 0; i < numLeds; i++) {

                        for (uint16_t j = 0; j < numLeds; j++) {
                                if (j <= i) {
                                        ws.setPixel(j, 0, 16, 0);
                                } else {
                                        ws.setPixel(j, 0, 0, 16);
                                }
                        }
                        printf("Going to show\n");
                        ws.show();
                        printf("Showing %d of %d\n", i + 1, numLeds);
                        vTaskDelay(500 / portTICK_PERIOD_MS);
                }


        }
}

void relais_toggle_task(void* pvParameter) {
        gpio_pad_select_gpio(GPIO_NUM_14);
        gpio_set_direction(GPIO_NUM_14, GPIO_MODE_OUTPUT);

        while (true) {
                gpio_set_level(GPIO_NUM_14, 0);
                printf("Is off\n");
                vTaskDelay(3000 / portTICK_PERIOD_MS);
                gpio_set_level(GPIO_NUM_14, 1);
                printf("Is ON\n");
                vTaskDelay(3000 / portTICK_PERIOD_MS);
        }
}

void relais_alwayson_task(void* pvParameter) {
        gpio_pad_select_gpio(GPIO_NUM_14);
        gpio_set_direction(GPIO_NUM_14, GPIO_MODE_OUTPUT);
        gpio_set_level(GPIO_NUM_14, 1);

        while (true)
                vTaskDelay(10000 / portTICK_PERIOD_MS);
}

void temp_sensor_task(void* pvParameter) {
        Htu21d sensor(i2cMaster);

        while (true) {
                float temp = sensor.readTemperature();
                float humidity = sensor.readHumidity();
                printf("Temperature: %.2f | Humidity: %.2f\n", temp, humidity);

                vTaskDelay(2000 / portTICK_RATE_MS);
        }
}

void app_main() {
        printf("### Starting...\n");
        esp32::System::getInstance().init();
        //esp32::Wifi::getInstance().init();
        //esp32::Wifi::getInstance().connectSta("", "");
        i2cMaster.init();
        printf("### Started!!!\n");

        xTaskCreate(&led_test_task, "led_test_task", 2048, NULL, 5, NULL);
        //xTaskCreate(&relais_toggle_task, "relais_toggle_task", 2048, NULL, 5, NULL);
        xTaskCreate(&relais_alwayson_task, "relais_alwayson_task", 2048, NULL, 5, NULL);
        xTaskCreate(&temp_sensor_task, "temp_sensor_task", 2048, NULL, 5, NULL);
}
