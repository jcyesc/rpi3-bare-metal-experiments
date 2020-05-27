#include "bcm2837.h"
#include "gpu_mailbox.h"
#include "stdint.h"

#define GPU_MAILBOX    (PERIPHERALS_BASE + 0x0000B880)
#define GPU_MAILBOX_READ       ((volatile uint32_t*)(GPU_MAILBOX + 0x0))
#define GPU_MAILBOX_STATUS     ((volatile uint32_t*)(GPU_MAILBOX + 0x18))
#define GPU_MAILBOX_WRITE      ((volatile uint32_t*)(GPU_MAILBOX + 0x20))
#define GPU_MAILBOX_SUCCESFUL_RESPONSE   0x80000000
#define GPU_MAILBOX_FULL                 0x80000000
#define GPU_MAILBOX_EMPTY                0x40000000


/**
 * Sends messages to the mailbox of the GPU.
 *
 * @param channel The channel to be sent to the mail box.
 * @param buffer The buffer to be sent to the mail box.
 * It has to be 16-byte aligned (b0-b3) are always 0. The response from
 * the mailbox will be stored in the buffer.
 * @return Returns 0 if the message was processed successfully, otherwise it
 * returns a negative number.
 *
 * Notes: The buffer has to have to following format:
 *
 * Buffer contents:
 *
 * u32: buffer size in bytes (including the header values, the end tag and
 *      padding)
 * u32: buffer request/response code
 *     Request codes:
 *         - 0x00000000: process request
 *         - All other values reserved
 *     Response codes:
 *         - 0x80000000: request successful
 *         - 0x80000001: error parsing request buffer (partial response)
 *         - All other values reserved
 * u8...: sequence of concatenated Tag formats (see below)
 * u32: 0x0 (end tag)
 * u8...: padding so it is 16-byte aligned.
 *
 *
 * Tag format:
 *
 * u32: tag identifier
 * u32: value buffer size in bytes
 * u32:
 *     Request codes:
 *         - b31 clear: request
 *         - b30-b0: reserved
 *     Response codes:
 *         - b31 set: response
 *         - b30-b0: value length in bytes
 * u8...: value buffer
 * u8...: padding to align the tag to 32 bits.
 *
 * For more information see: https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
 *
 *
 * Mailbox messages:
 *
 * The mailbox interface has 28 bits (MSB) available for the value and 4 bits
 * (LSB) for the channel.
 *     - Request message: 28 bits (MSB) buffer address
 *     - Response message: 28 bits (MSB) buffer address
 * Channels 8 and 9 are used.
 *     - Channel 8: Request from ARM for response by VC
 *     - Channel 9: Request from VC for response by ARM (none currently defined)
 */
int32_t gpu_mailbox_process_msg(uint8_t channel,
        volatile int32_t *start_msg_address) {
    // Include the channel in the 4 LSB of the buffer address.
    uint64_t msg_address_with_channel =
        (((uint64_t) start_msg_address) & ~0xF) | (channel & 0xF);

    // It is not neccesary to clean GPU_MAILBOX_STATUS, GPU_MAILBOX_READ
    // because they are pointing to Device Memory and Device Memory is not
    // cache.

    // Wait for the mailbox till we can write to the mailbox
    while (*GPU_MAILBOX_STATUS & GPU_MAILBOX_FULL) {
        asm volatile("nop");
    }

    // Write the address of the message to the mailbox
    *GPU_MAILBOX_WRITE = msg_address_with_channel;

    while (1) {
        // Wait for the mailbox till we can read from the buffer.
        while (*GPU_MAILBOX_STATUS & GPU_MAILBOX_EMPTY) {
            asm volatile("nop");
        }

        // Check if it is the reponse to our message.
        if (msg_address_with_channel == *GPU_MAILBOX_READ) {
            break;
        }
    }

    return start_msg_address[1] == GPU_MAILBOX_SUCCESFUL_RESPONSE ? 0 : -1;
}
