#ifndef ESP32_EXCEPTION_H
#define ESP32_EXCEPTION_H
#ifdef PETER
#include <string>
#include <exception>

namespace esp32 {

class Exception : std::exception {
public:
  Exception(const std::string& message);

  const char* what() const noexcept override;

private:
  std::string mMessage;
};
}
#endif
#endif
