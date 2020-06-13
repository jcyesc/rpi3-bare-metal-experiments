#include "processor_print.h"
#include "stdint.h"
#include "uart.h"

#define BARE_METAL_KERNEL_VERSION "0.1.9"

void generate_synchronous_interrupt();

/*
 * First C function executed.
 */
void main() {
    uart_init();
    uart_puts("main: Bare Metal EL3 interrupts " BARE_METAL_KERNEL_VERSION "\n");

    processor_print_all_info();

    // One interrupt will be generated here.
    asm volatile ("smc #15");

    // Another interrupt will be generated here.
    generate_synchronous_interrupt();

    // Uncomment this line and see what happens and see if this could be
    // corrected.
    // uart_puts("Returning from the synchronous interrupt!!\n");

    while (1) {
        uart_puts("\nmain: Write a char: ");
        uart_putc(uart_getc());
    }
}

void generate_synchronous_interrupt() {
    uart_puts("\nmain: About to generate a synchronous interrupt!");
    // generate a Data Abort with a bad address access
    uint32_t invalid_address  __attribute__ ((unused)) =
        *((volatile unsigned int*) 0xFFFFFFFFFFFFFFFF);
}


