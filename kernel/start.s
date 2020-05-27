/*
 * Cortex A-53 boot code.
 *
 * This is the first code to be executed by the Cortex A-53 processor
 * after the RPi GPU initialized the hardware.
 *
 * This boot code is in charge of:
 *     - putting the cores 1, 2 and 3 to sleep.
 *     - setting up the stack pointer.
 *     - configuring EL3 to handle the exceptions and interruptions
 *       (IRQ, FIQ and SError).
 *     - setting the vector tables for  EL3.
 *     - jumping to the main function in EL3.
 */

.section ".boot"

.global _start
_start:

// Puts cores 1, 2 and 3 to sleep using WFE (Wait for Exception).
sleep_cores_1_2_3:
    mrs     x0, mpidr_el1   // Gets core number using the
                            // multiprocessor affinity register.
    and     x0, x0, #3      // Checks if the core id is 1, 2 or 3
    cbz     x0, cpu_0_code  // If core 0, it jumps to cpu_0_code.

infinite_loop:
    wfe                     // Puts the core to sleep (Wait for event)
    b       infinite_loop

// Starts initialization
cpu_0_code:
    bl init_general_purpose_regs
    bl init_floating_point_regs
    bl init_stack_pointer_regs
    bl init_system_registers_el3

    bl set_vector_tables

boot_operating_system:
    bl      main    // This function will not return.

