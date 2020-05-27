/*
 * Defines the interrupt handlers for EL3 exception levels.
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "stdint.h"

enum interrupt_type { SYNC, IRQ, FIQ, SERROR };

/**
 * Handles EL3 interrupts for synchronous, IRQ, FIQ and SError interrupts for
 * the current EL with SP_EL0.
 */
void intr_el3_handler_thread_mode(enum interrupt_type type,
                                  uint64_t syndrome_register);

/**
 * Handles EL3 interrupts for synchronous, IRQ, FIQ and SError interrupts for
 * the current EL while using SP_ELX.
 */
void intr_el3_handler_handler_mode(enum interrupt_type type,
                                   uint64_t syndrome_register);

/**
 * Handles EL3 interrupts for synchronous, IRQ, FIQ and SError interrupts when
 * there was an exception from lower EL using AArch64.
 */
void intr_el3_handler_changed_el_in_aarch64_state(
        enum interrupt_type type,
        uint64_t syndrome_register);

/**
 * Handles EL3 interrupts for synchronous, IRQ, FIQ and SError interrupts when
 * there was an exception from lower EL using AArch32.
 */
void intr_el3_handler_changed_el_in_aarch32_state(
        enum interrupt_type type,
        uint64_t syndrome_register);

#endif /* INTERRUPTS_H */

