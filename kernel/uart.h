#ifndef UART_H
#define UART_H

#include "stdint.h"

// TODO: Define a stdio.h library to handle multiple parameters.
#define UART_PRINT_LONG(msg, num) \
    uart_puts(msg); \
    uart_hex_uint64(num); \
    uart_puts("\n"); \

/* Inits the UART device (gpios 14 and 15). */
void uart_init();

/* Puts a char in the UART port. */
void uart_putc(char c);

/* Puts a string in the UART port. */
void uart_puts(const char *s);

/* Gets a character from the UART port. */
char uart_getc();

/**  Sends an character in hexadecimal format to the UART1 port. */
void uart_hex_char(char value);

/**
 * Sends a character array in hexadecimal format to the UART1 port.
 * @param array[] Array of characters to be printed in
 *     hexadecimal format.
 * @param len The length of the array.
 * @param separator The separator to be printed between one character
 *    and the next.
 */
void uart_hex_char_array(char array[], int len, char separator);

/**  Sends an integer in hexadecimal format to the UART1 port. */
void uart_hex_uint32(uint32_t value);

/**  Sends a long in hexadecimal format to the UART1 port. */
void uart_hex_uint64(uint64_t  value);

#endif /* UART_H */
