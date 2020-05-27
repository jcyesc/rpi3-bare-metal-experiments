#ifndef BCM2837_H
#define BCM2837_H

#include "stdint.h"

/* Devices in the Raspberry Pi 3 Model B and B+ are configured in this address.
 * The Raspberry Pi 3 Model B and B+ have 1 GB of memory, which means
 * 0x3F000000 is the last block of 16KB (0x40000000 is memory size).
 */
#define PERIPHERALS_BASE       0x3F000000

#define GPFSEL1         ((volatile uint32_t*)(PERIPHERALS_BASE + 0x00200004))
#define GPPUD           ((volatile uint32_t*)(PERIPHERALS_BASE + 0x00200094))
#define GPPUDCLK0       ((volatile uint32_t*)(PERIPHERALS_BASE + 0x00200098))


#endif /* BCM2837_H */
