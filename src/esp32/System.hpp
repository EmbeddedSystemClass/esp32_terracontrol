#ifndef ESP32_SYSTEM_H
#define ESP32_SYSTEM_H

#include "esp_event_loop.h"

namespace esp32 {

  class System {
  public:
    /***
     *  Returns the instance of this object
     **/
    static System& getInstance();

    /***
     *  Initializes the system
     **/
    void init();

    /***
     *  Handles system events
     **/
    esp_err_t handleEvent(void *ctx, system_event_t *event);

  private:
    /***
     *  Constructor
     **/
    System();

  private:
    static System INSTANCE;
  };

}

#endif
