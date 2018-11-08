#include <string.h>

#include "Wifi.hpp"
#include "esp_wifi.h"
#include "esp_log.h"

namespace esp32 {

  Wifi Wifi::INSTANCE;
  Wifi& Wifi::getInstance() {
    return Wifi::INSTANCE;
  }

  Wifi::Wifi() : mConnected(false) {

  }

  void Wifi::init() {
    // Initializes the tcp/ip stack
    tcpip_adapter_init();

    // Initialize wifi adapter with default configuration
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK) {
      ESP_LOGE("Wifi", "Failed to Initialize wifi with return code = %d", ret);
      return;
    }

    // Set the wifi storage
    ret = esp_wifi_set_storage(WIFI_STORAGE_RAM);
    if (ret != ESP_OK) {
      ESP_LOGE("Wifi", "Failed to set wifi storage to RAM with return code = %d", ret);
      return;
    }
  }

  esp_err_t Wifi::handleEvent(void *ctx, system_event_t *event) {
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        return ESP_OK;
    case SYSTEM_EVENT_STA_GOT_IP:
        this->mConnected = true;
        return ESP_OK;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        esp_wifi_connect();
        this->mConnected = false;
        return ESP_OK;
    default:
        return ESP_OK;
    }
  }

  void Wifi::connectSta(const std::string& ssid, const std::string& password) {
        // Build the config
        wifi_config_t wifi_config = {};
        strcpy((char*)wifi_config.sta.ssid, ssid.c_str());
        strcpy((char*)wifi_config.sta.password, password.c_str());
        ESP_LOGI("Wifi", "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);

        // Set wifi mode to STA
        esp_err_t ret = esp_wifi_set_mode(WIFI_MODE_STA);
        if (ret != ESP_OK) {
          ESP_LOGE("Wifi", "Failed to set wifi mode to STA with return code = %d", ret);
          return;
        }

        // Apply configuration to wifi
        ret = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
        if (ret != ESP_OK) {
          ESP_LOGE("Wifi", "Failed to apply wifi STA config with return code = %d", ret);
          return;
        }

        // Start WiFi
        ret = esp_wifi_start();
        if (ret != ESP_OK) {
          ESP_LOGE("Wifi", "Failed to start wifi with return code = %d", ret);
          return;
        }
  }

  bool Wifi::isConnected() const {
    return this->mConnected;
  }

}
