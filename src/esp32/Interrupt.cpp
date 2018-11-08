#include <esp_intr_alloc.h>

#include "Interrupt.hpp"

namespace esp32 {

void interrupt_callback(void *arg) {

}

Interrupt& Interrupt::getInstance() {
        return INSTANCE;
}

void Interrupt::registerHandler(int interruptSource, const std::function<void()>&callback) {
        // Generate handler index as pointer
        uint16_t* pHandlerIndex = new uint16_t(this->mHandlerIndex++);

        // Store the callback
        this->mHandlerMap[pHandlerIndex] = callback;

        // Register interrupt with our general callback
        esp_intr_alloc(interruptSource, 0, interrupt_callback, pHandlerIndex, nullptr);
}

}
