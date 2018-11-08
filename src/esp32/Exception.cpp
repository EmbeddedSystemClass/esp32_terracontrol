#include "Exception.hpp"
#ifdef PETER
namespace esp32 {

Exception::Exception(const std::string& msg) : mMessage(msg) {

}

  const char* Exception::what() const noexcept {
    return this->mMessage.c_str();
  }

}
#endif
