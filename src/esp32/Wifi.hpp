#ifndef ESP32_WIFI_H
#define ESP32_WIFI_H

#include <string>

#include "esp_event_loop.h"

namespace esp32 {

  class Wifi {
    friend class System;
  public:
    /***
     *  Returns the instance of this object
     **/
    static Wifi& getInstance();

    /***
     *  Initializes the wifi
     ***/
    void init();

    /***
     *  Connect
     **/
    void connectSta(const std::string& ssid, const std::string& password);

    /***
     *  Returns whether the wifi is connected or not
     **/
    bool isConnected() const;

  private:
    /***
     *  Constructor
     **/
    Wifi();

    /***
     *  Handles a system event that is wifi specific
     **/
    esp_err_t handleEvent(void *ctx, system_event_t *event);

  private:
    // The static instance for singleton pattern
    static Wifi INSTANCE;
    // Holds the information whether the wifi is connected or not
    bool mConnected;
  };

}

#endif
