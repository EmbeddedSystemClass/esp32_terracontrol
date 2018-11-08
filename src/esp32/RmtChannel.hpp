#ifndef ESP32_RMTCHANNEL_H
#define ESP32_RMTCHANNEL_H

#include <functional>

#include "driver/rmt.h"

namespace esp32 {

class RmtChannel {
public:
/**
 * @brief Constructor
 **/
RmtChannel(rmt_channel_t channel, uint8_t divider);

private:
/**
 * @brief The rmt channel to be used
 **/
rmt_channel_t mChannel;

/**
 * @brief The channel divider of the clock
 **/
uint8_t mDivider;
};

}

#endif
