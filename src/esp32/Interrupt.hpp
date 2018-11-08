#ifndef ESP32_INTERRUPT_H
#define ESP32_INTERRUPT_H

#include <functional>
#include <map>

namespace esp32 {

class Interrupt {
public:
/**
 * @brief Returns the instance of this type
 **/
static Interrupt& getInstance();

/**
 * @brief
 **/
void registerHandler(int interruptSource, const std::function<void()>&callback);

private:
/**
 * @brief The static instance of the interrupt object
 **/
static Interrupt INSTANCE;

/**
 * @brief Holding a counter that increases for every added callback to give everyone an unique id.
 **/
uint16_t mHandlerIndex = 0;

/**
 * @brief The map holding the interrupt handlers
 **/
std::map<uint16_t*, std::function<void()> > mHandlerMap;
};

}

#endif
