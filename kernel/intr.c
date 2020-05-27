/*
 * Implements the interrupt handlers for EL3 exception level.
 *
 * Note: Assumes that the interrupts are enable after initializing the UART.
 */

#include "intr.h"
#include "stdint.h"
#include "uart.h"


static inline void intr_print_handler_info(const char *msg,
                                           enum interrupt_type type,
                                           uint64_t syndrome_register) {
    uart_puts("\n------------ Start -----------\n");
    uart_puts(msg);
    uart_puts("\nType ");
    uart_hex_uint32(type);
    uart_puts("\nesr_elx = ");
    uart_hex_uint32(syndrome_register);
    uart_puts("\n------------ End -----------\n");
}

/**
 * Handles EL3 interrupts for synchronous, IRQ, FIQ and SError interrupts for
 * the current EL with SP_EL0.
 */
void intr_el3_handler_thread_mode(enum interrupt_type type,
                                  uint64_t syndrome_register) {
    intr_print_handler_info("intr_el3_handler_thread_mode()",
                            type, syndrome_register);
}

/**
 * Handles EL3 interrupts for synchronous, IRQ, FIQ and SError interrupts for
 * the current EL while using SP_ELX.
 */
void intr_el3_handler_handler_mode(enum interrupt_type type,
                                   uint64_t syndrome_register) {
    intr_print_handler_info("intr_el3_handler_handler_mode()",
                            type, syndrome_register);
}

/**
 * Handles EL3 interrupts for synchronous, IRQ, FIQ and SError interrupts when
 * there was an exception from lower EL using AArch64.
 */
void intr_el3_handler_changed_el_in_aarch64_state(
        enum interrupt_type type,
        uint64_t syndrome_register) {
    intr_print_handler_info("intr_el3_handler_changed_el_in_aarch64_state()",
                            type, syndrome_register);
}

/**
 * Handles EL3 interrupts for synchronous, IRQ, FIQ and SError interrupts when
 * there was an exception from lower EL using AArch32.
 */
void intr_el3_handler_changed_el_in_aarch32_state(
        enum interrupt_type type,
        uint64_t syndrome_register) {
    intr_print_handler_info("intr_el3_handler_changed_el_in_aarch32_state()",
                            type, syndrome_register);
}

