#ifndef ESP32_HTTPSERVER_H
#define ESP32_HTTPSERVER_H
#ifdef PETER
#include <stdint.h>

namespace esp32 {

class HttpServer {
public:
  /**
   * @brief Constructor.
   **/
  HttpServer(uint16_t port);

  /**
   *
   **/
  void launch();

private:
  /**
   * @brief The server port
   **/
  uint16_t mPort;
  int mSocket = -1;
};

}

#endif
#endif
