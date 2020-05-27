#include "board_config.h"
#include "gpu_mailbox.h"
#include "stdint.h"

#define GPU_MAILBOX_DEFAULT_LENGTH_MSG 8

/*
 * Defines the fields required for the mailbox message.
 * Mailbox message buffer has to be 16-byte aligned.
 */
typedef volatile struct __attribute__((aligned(16))) mailbox_msg {
    int32_t length_in_bytes;         // Length of the message.
    int32_t buffer_request_code;     // Request code of the message.
    int32_t tag_identifier;          // Tag of the requested data.
    int32_t response_length_bytes;   // Length of the response
    int32_t request_codes;           // Request codes b31 clear:
                                     // request, b30-b0 reserverd
    int32_t tag_end_or_value1;       // Value for the response or tag end.
    int32_t tag_end_or_value2;       // Value for the response or tag end.
    int32_t tag_end_or_value3;       // Value for the response or tag end.
    int32_t tag_end;                 // End of the tag or padding.
} MailboxMsg;

/* Default mailbox message. */
const MailboxMsg MAILBOX_MSG_TEMPLATE = {
    .length_in_bytes = GPU_MAILBOX_DEFAULT_LENGTH_MSG * sizeof(int32_t),
    .buffer_request_code = GPU_MAILBOX_PROCESS_REQUEST,
    .request_codes = 0,
    .tag_end_or_value1 = GPU_MAILBOX_END_TAG,
    .tag_end_or_value2 = GPU_MAILBOX_END_TAG,
    .tag_end_or_value3 = GPU_MAILBOX_END_TAG,
    .tag_end = GPU_MAILBOX_END_TAG
};

/**
 * Sets the current clock rate for the given clock_id.
 *
 * Return 0 on success, otherwise a negative number.
 *
 * https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface#set-clock-rate
 */
int board_config_set_current_clock_rate(const uint32_t clock_id,
        uint32_t new_clock_rate_in_hz, uint32_t skip_setting_turbo) {
    MailboxMsg msg = MAILBOX_MSG_TEMPLATE;
    msg.length_in_bytes = 9 * sizeof(int32_t);
    msg.tag_identifier = GPU_MAILBOX_TAG_SET_CLOCK_RATE;
    msg.response_length_bytes = 12;
    msg.tag_end_or_value1 = clock_id;
    msg.tag_end_or_value2 = new_clock_rate_in_hz;
    msg.tag_end_or_value3 = skip_setting_turbo;

    // Sends the message to GPU and sets the response in msgr if success.
    if (gpu_mailbox_process_msg(GPU_MAILBOX_CHANNEL_PROP, (int32_t *) &msg)) {
        return -1;
    }

    // The mailbox writes the response in msg.tag_end_or_value1 and
    // msg.tag_end_or_value1.
    int32_t clock_id_resp = msg.tag_end_or_value1;
    int32_t new_clock_rate_in_hz_resp = msg.tag_end_or_value2;

    if (clock_id != clock_id_resp) {
        return -2;
    }

    if (new_clock_rate_in_hz != new_clock_rate_in_hz_resp) {
        return -3;
    }

    return 0;
}
