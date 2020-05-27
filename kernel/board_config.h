#ifndef BOARD_INFO_H
#define BOARD_INFO_H

#include "stdint.h"

/**
 * Sets the current clock rate for the given clock_id.
 *
 * Return 0 on success, otherwise a negative number.
 *
 * https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface#set-clock-rate
 */
int board_config_set_current_clock_rate(const uint32_t clock_id,
        uint32_t new_clock_rate_in_hz, uint32_t skip_setting_turbo);

#endif /* BOARD_INFO_H */
