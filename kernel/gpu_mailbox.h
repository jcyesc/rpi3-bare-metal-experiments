/*
 * Defines the interface and constants to be used to
 * communicate with the GPU mailbox.
 */

#ifndef GPU_MAIL_BOX_H
#define GPU_MAIL_BOX_H

#include "stdint.h"

/* Channels */
#define GPU_MAILBOX_CHANNELPOWER            0
#define GPU_MAILBOX_CHANNELFRAME_BUFFER     1
#define GPU_MAILBOX_CHANNEL_VUART           2
#define GPU_MAILBOX_CHANNEL_VCHIQ           3
#define GPU_MAILBOX_CHANNEL_LEDS            4
#define GPU_MAILBOX_CHANNEL_BTNS            5
#define GPU_MAILBOX_CHANNEL_TOUCH           6
#define GPU_MAILBOX_CHANNEL_COUNT           7
#define GPU_MAILBOX_CHANNEL_PROP            8

/* Buffer content constants */
#define GPU_MAILBOX_PROCESS_REQUEST         0

/* Tag format constants */
#define GPU_MAILBOX_TAG_SERIAL_NUMBER       0x10004
#define GPU_MAILBOX_TAG_FIRMWARE_REVISION   0x1
#define GPU_MAILBOX_TAG_BOARD_MODEL         0x10001
#define GPU_MAILBOX_TAG_MAC_ADDRESS         0x10003
#define GPU_MAILBOX_TAG_ARM_MEMORY          0x10005
#define GPU_MAILBOX_TAG_VC_MEMORY           0x10006
#define GPU_MAILBOX_TAG_CURRENT_CLOCK_RATE  0x30002
#define GPU_MAILBOX_TAG_MAX_CLOCK_RATE      0x30004
#define GPU_MAILBOX_TAG_MIN_CLOCK_RATE      0x30007
#define GPU_MAILBOX_TAG_SET_CLOCK_RATE      0x38002
#define GPU_MAILBOX_END_TAG                 0

/* Unique clock IDs */
#define GPU_MAILBOX_CLOCK_ID_UART           0x2
#define GPU_MAILBOX_CLOCK_ID_ARM            0x3
#define GPU_MAILBOX_CLOCK_ID_CORE           0x4
#define GPU_MAILBOX_CLOCK_ID_SDRAM          0x8


/**
 * Sends messages to the mailbox of the GPU.
 *
 * @param channel The channel to be sent to the mail box.
 * @param buffer The buffer to be sent to the mail box.
 * It has to be 16-byte aligned (b0-b3) are always 0. The response from
 * the mailbox will be stored in the buffer.
 * @return Returns 0 if the message was processed successfully, otherwise it
 * returns a negative number.
 */
int32_t gpu_mailbox_process_msg(uint8_t channel, volatile int32_t *buffer);


#endif /* GPU_MAIL_BOX_H */
