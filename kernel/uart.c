#include "uart.h"
#include "bcm2837.h"
#include "board_config.h"
#include "gpu_mailbox.h"
#include "stdint.h"

/* PL011 UART registers */
#define UART0_DR      ((volatile uint32_t*)(PERIPHERALS_BASE + 0x00201000))
#define UART0_FR      ((volatile uint32_t*)(PERIPHERALS_BASE + 0x00201018))
#define UART0_IBRD    ((volatile uint32_t*)(PERIPHERALS_BASE + 0x00201024))
#define UART0_FBRD    ((volatile uint32_t*)(PERIPHERALS_BASE + 0x00201028))
#define UART0_LCRH    ((volatile uint32_t*)(PERIPHERALS_BASE + 0x0020102C))
#define UART0_CR      ((volatile uint32_t*)(PERIPHERALS_BASE + 0x00201030))
#define UART0_IMSC    ((volatile uint32_t*)(PERIPHERALS_BASE + 0x00201038))
#define UART0_ICR     ((volatile uint32_t*)(PERIPHERALS_BASE + 0x00201044))

static inline void config_uart_clock_rate() {
    uint32_t clock_rate_in_hz = 4000000;
    uint32_t clear_turbo = 0;
    // TODO: Handle the error code appropriately.
    board_config_set_current_clock_rate(GPU_MAILBOX_CLOCK_ID_UART,
                                        clock_rate_in_hz,
                                        clear_turbo);
}

/**
 * Configures the UART1 port:
 *
 * - Sets the baud rate to 115200
 * - Sets the frame to 8 bits
 * - Sets the GPIOs 14 and 15
 */
void uart_init() {
    register uint32_t reg;

    *UART0_CR = 0;         // turn off UART

    config_uart_clock_rate();

    // Configure UART0
    reg = *GPFSEL1;
    reg &= ~((7 << 12) | (7 << 15)); // gpio14, gpio15
    reg |= (4 << 12) | (4 << 15);    // enable alt function as GPIO
    *GPFSEL1 = reg;
    *GPPUD = 0;

    // waits for the initialization of GPIOs and UART1
    reg = 150;
    while (reg--) {
        asm volatile("nop");
    }
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    // waits for the initialization of GPIOs and UART1
    reg = 150;
    while (reg--) {
        asm volatile("nop");
    }
    *GPPUDCLK0 = 0;           // flush GPIO setup

    *UART0_ICR = 0x7FF;       // clear interrupts
    *UART0_IBRD = 2;          // 115200 baud rate
    *UART0_FBRD = 0xB;
    *UART0_LCRH = 0b11 << 5;  // 8n1
    *UART0_CR = 0x301;        // enable Tx, Rx, FIFO
}

/** Sends a character to the UART1 port. */
void uart_putc(const char c) {
    /* Waits for buffer to be not full, then output */
    while (*UART0_FR & 0x20) {
        asm volatile("nop");
    }

    /* Write the character to the buffer */
    *UART0_DR = c;
}

/** Sends a string to the UART1 port. */
void uart_puts(const char *s) {
    while (*s) {
        /* Convert newline to carrige return + newline */
        if (*s == '\n') {
            uart_putc('\r');
        }
        uart_putc(*s++);
    }
}

/** Receives a character from the UART1 port. */
char uart_getc() {
    char c;

    /* Waits until something is in the buffer */
    while (*UART0_FR & 0x10) {
        asm volatile("nop");
    }

    c = (char) *UART0_DR;

    /* Converts carrige return to newline. */
    return c == '\r' ? '\n' : c;
}

/**  Sends an character in hexadecimal format to the UART1 port. */
void uart_hex_char(char value) {
    for (int bits = 4; bits >= 0; bits -= 4) {
        uint32_t nimble = (value >> bits) & 0xF;
        nimble += nimble > 9 ? 0x37 /* 'A' */: 0x30 /* '0' */;
        uart_putc(nimble);
    }
}

/**
 * Sends a character array in hexadecimal format to the UART1 port.
 * @param array[] Array of characters to be printed in
 *     hexadecimal format.
 * @param len The length of the array.
 * @param separator The separator to be printed between one character
 *    and the next.
 */
void uart_hex_char_array(char array[], int len, char separator) {
    for (int i = 0; i < len; i++) {
        uart_hex_char(array[i]);
        if (i < len - 1) {
            uart_putc(separator);
        }
    }
}

/**  Sends an integer in hexadecimal format to the UART1 port. */
void uart_hex_uint32(uint32_t value) {
    uart_puts("0x");

    for (int bits = 28; bits >= 0; bits -= 4) {
        uint32_t nimble = (value >> bits) & 0xF;
        nimble += nimble > 9 ? 0x37 /* 'A' */: 0x30 /* '0' */;
        uart_putc(nimble);
    }
}

/**  Sends a long in hexadecimal format to the UART1 port. */
void uart_hex_uint64(uint64_t value) {
    uart_puts("0x");

    for (int bits = 60; bits >= 0; bits -= 4) {
        uint32_t nimble = (value >> bits) & 0xF;
        nimble += nimble > 9 ? 0x37 /* 'A' */: 0x30 /* '0' */;
        uart_putc(nimble);
    }
}

