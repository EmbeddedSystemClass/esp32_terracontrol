#ifdef PETER

#include <lwip/err.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/netdb.h>
#include <lwip/dns.h>

#include "HttpServer.hpp"
#include "Exception.hpp"

namespace esp32 {

HttpServer::HttpServer(uint16_t port) : mPort(port) {

}

  void HttpServer::launch() {
    // Configure the server address
    struct sockaddr_in tcpServerAddr;
    tcpServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcpServerAddr.sin_family = AF_INET;
    tcpServerAddr.sin_port = htons(this->mPort);

    // Create socket
    this->mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->mSocket < 0) {
      //ESP_LOGE(TAG, "Failed to allocate socket\n");
      //throw Exception("");
      return;
    }
  }

}
#endif
