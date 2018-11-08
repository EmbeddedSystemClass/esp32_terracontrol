#include "System.hpp"
#include "Wifi.hpp"
#include "nvs_flash.h"

namespace esp32 {

  extern "C" {
    static esp_err_t event_handler(void *ctx, system_event_t *event) {
        return System::getInstance().handleEvent(ctx, event);
    }
  }

  System System::INSTANCE;
  System& System::getInstance() {
    return System::INSTANCE;
  }

  System::System() {

  }

  esp_err_t System::handleEvent(void *ctx, system_event_t *event) {
      switch(event->event_id) {
      case SYSTEM_EVENT_STA_START:
      case SYSTEM_EVENT_STA_GOT_IP:
      case SYSTEM_EVENT_STA_DISCONNECTED:
        return Wifi::getInstance().handleEvent(ctx, event);
      default:
          return ESP_OK;
      }
  }

  void System::init() {
    nvs_flash_init();
    esp_event_loop_init(event_handler, nullptr);
  }

}
