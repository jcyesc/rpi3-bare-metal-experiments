#include "processor_print.h"
#include "stdint.h"
#include "uart.h"

#define EL0_T_MODE 0x00
#define EL1_T_MODE 0x04
#define EL1_H_MODE 0x05
#define EL2_T_MODE 0x08
#define EL2_H_MODE 0x09
#define EL3_T_MODE 0x0C
#define EL3_H_MODE 0x0D

/*
 * Prints all the information about the ARM Cortex A-53 processor.
 */
void processor_print_all_info() {
    processor_print_pstate();
    processor_print_secure_config_register_el3();
    processor_print_aarch64_processor_feature_register();
    processor_print_sctlr_el3();
}

static inline uint64_t get_current_el() {
    uint64_t exception_level;
    asm volatile ("mrs %0, CurrentEL" : "=r" (exception_level));

    return exception_level;
}

/*
 * Prints the current exception level. The differet values could be:
 *
 *    0x00 : mode_EL0
 *    0x04 : mode_EL1t
 *    0x05 : mode_EL1h
 *    0x08 : mode_EL2t
 *    0x09 : mode_EL2h
 *    0x0C : mode_EL3t
 *    0x0D : mode_EL3h
 */
static inline void processor_print_current_el() {
    uint64_t exception_level = get_current_el();
    uart_puts("processor_print:     CurrentEl: ");

    switch (exception_level) {
    case EL0_T_MODE:
        uart_puts("0x00 : mode_EL0");
        break;
    case EL1_T_MODE:
        uart_puts("0x04 : mode_EL1_thread");
        break;
    case EL1_H_MODE:
        uart_puts("0x05 : mode_EL1_handler");
        break;
    case EL2_T_MODE:
        uart_puts("0x08 : mode_EL2_thread");
        break;
    case EL2_H_MODE:
        uart_puts("0x09 : mode_EL2_handler");
        break;
    case EL3_T_MODE:
        uart_puts("0x0C : mode_EL3_thread");
        break;
    case EL3_H_MODE:
        uart_puts("0x0D : mode_EL3_handler");
        break;
    default:
        uart_hex_uint64(exception_level);
        uart_puts("\n");
        uart_puts("processor_print: undefined exception level: ");

        break;
    }

    uart_puts("\n");
}

/**
 * Prints the Stack select value.
 *
 *     0 = SP_EL0
 *     1 = SP_ELn
 */
static inline void processor_print_sp_sel() {
    // Stack select, SP_EL0 or SP_ELn
    uint64_t stack_selection;
    asm volatile ("mrs %0, SPSel" : "=r" (stack_selection));
    uart_puts("processor_print:     SPSel: ");
    uart_hex_uint64(stack_selection);
    uart_puts("\n");
}

/**
 * Prints the condition flags.
 */
static inline void processor_print_nzcv() {
    uint64_t nzcv_flags;
    asm volatile ("mrs %0, NZCV" : "=r" (nzcv_flags));
    uart_puts("processor_print:     NZCV: ");
    uart_hex_uint64(nzcv_flags);
    uart_puts("\n");
}

/*
 * Prints the current interrupt mask bits: Debug, Asynchronous (SError),
 * Interrupt, FastInterrupt
 *
 *     0x040 : fast interrupt
 *     0x080 : interrupt request
 *     0x100 : asynchronous
 *     0x200 : debug
 */
static inline void processor_print_daif() {
    uint64_t daif;
    asm volatile ("mrs %0, DAIF" : "=r" (daif));
    uart_puts("processor_print:     DAIF: ");
    uart_hex_uint64(daif);
    uart_puts("\n");

    if (daif & 0x040) {
        uart_puts("processor_print:          FIQ Interrupt masked\n");
    }
    if (daif & 0x080) {
        uart_puts("processor_print:          IRQ Interrupt masked\n");
    }
    if (daif & 0x100) {
        uart_puts("processor_print:          Async SError Interrupt masked\n");
    }
    if (daif & 0x200) {
        uart_puts("processor_print:          Debug Interrupt masked\n");
    }
}

/**
 * Prints some of the process state flags. Below it is a
 * description of them:
 *
 * AArch64 does not have the v7-A CPSR register.
 * Settings previously in CPSR can be accessed individually.
 *      NZCV:       Negative, Zero, Carry, Overflow
 *      Q:          Saturation
 *      DAIF:       Exception mask flags (Debug, Asynchronous (SError)
 *                  Interrupt, Fast Interrupt.
 *      SPSel:      Stack select, SP_EL0 or SP_ELn
 *      CurrentEL:  Current Exception Level
 *      E:          Endianness, Aarch32 only.
 *                  Aarche64 is little endian.
 *      IL:         Illegal flag
 *      SS:         Software single step.
 *
 *  Note: By default the Raspberry Pi 3 B and B+ boots at exception level 3.
 */
void processor_print_pstate() {
    uart_puts("processor_print: Processor State (PState)\n");

    processor_print_current_el();
    processor_print_daif();
    processor_print_nzcv();
    processor_print_sp_sel();
}

/*
 * Returns 1 if it is in EL3 level, otherwise it returns 0.
 */
static inline int32_t is_el3_mode() {
    uint64_t exception_level = get_current_el();

    return (exception_level == EL3_T_MODE) || (exception_level == EL3_H_MODE);
}

/*
 * Prints the secure configuration register. This register controls the secure
 * state and trapping of exceptions to EL3.
 */
void processor_print_secure_config_register_el3() {
    if (!is_el3_mode()) {
        uart_puts("processor_print: Unable to print SCR_EL3 "
                  "because current level is not EL3.\n");
        return;
    }

    uint64_t scr_el3;
    asm volatile ("mrs %0, scr_el3" : "=r" (scr_el3));
    uart_puts("processor_print: SCR_EL3: ");
    uart_hex_uint64(scr_el3);
    uart_puts("\n");
}

/**
 * Prints the AArch64 processor feature register 0. The ID_AA64PFR0_EL1
 * register provides information about implemented PE features in AArch64 state.
 */
void processor_print_aarch64_processor_feature_register() {
    uint64_t processor_register;
    asm volatile ("mrs %0, id_aa64pfr0_el1" : "=r" (processor_register));
    uart_puts("processor_print: ID_AA64PFR0_EL1: ");
    uart_hex_uint64(processor_register);
    uart_puts("\n");
}

/*
 * Prints the Control System Register for EL3 (sctlr_el3).
 */
void processor_print_sctlr_el3() {
    if (is_el3_mode()) {
        uint64_t sctlr_el3;
        asm volatile ("isb; mrs %0, sctlr_el3" : "=r" (sctlr_el3));

        uart_puts("processor_print: SCTLR_EL3 = ");
        uart_hex_uint64(sctlr_el3);
        uart_puts("\n");

        return;
    }

    uart_puts("processor_print: Unable to print SCTLR_EL3 because "
              "current level is not EL3.\n");
}
