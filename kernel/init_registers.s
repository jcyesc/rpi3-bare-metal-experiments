/*
 * Initializes the following registers:
 *
 *  - General-purpose registers
 *  - Floating point registers
 *  - Stack pointer registers
 *  - System control registers
 */

.section ".boot"

.global init_general_purpose_regs
init_general_purpose_regs:
    mov x0, xzr
    mov x1, xzr
    mov x2, xzr
    mov x3, xzr
    mov x4, xzr
    mov x5, xzr
    mov x6, xzr
    mov x7, xzr
    mov x8, xzr
    mov x9, xzr
    mov x10, xzr
    mov x11, xzr
    mov x12, xzr
    mov x13, xzr
    mov x14, xzr
    mov x15, xzr
    mov x16, xzr
    mov x17, xzr
    mov x18, xzr
    mov x19, xzr
    mov x20, xzr
    mov x21, xzr
    mov x22, xzr
    mov x23, xzr
    mov x24, xzr
    mov x25, xzr
    mov x26, xzr
    mov x27, xzr
    mov x28, xzr
    mov x29, xzr
    // Do not zero the LR, otherwise we can't return.
    // mov x30, xzr

    ret

.global init_floating_point_regs
init_floating_point_regs:
    fmov d0, xzr
    fmov d1, xzr
    fmov d2, xzr
    fmov d3, xzr
    fmov d4, xzr
    fmov d5, xzr
    fmov d6, xzr
    fmov d7, xzr
    fmov d8, xzr
    fmov d9, xzr
    fmov d10, xzr
    fmov d11, xzr
    fmov d12, xzr
    fmov d13, xzr
    fmov d14, xzr
    fmov d15, xzr
    fmov d16, xzr
    fmov d17, xzr
    fmov d18, xzr
    fmov d19, xzr
    fmov d20, xzr
    fmov d21, xzr
    fmov d22, xzr
    fmov d23, xzr
    fmov d24, xzr
    fmov d25, xzr
    fmov d26, xzr
    fmov d27, xzr
    fmov d28, xzr
    fmov d29, xzr
    fmov d30, xzr
    fmov d31, xzr

    ret


// Sets the stack pointers.
// Note: we can update sp_el1 only if we are in a higher exception level like
// EL2 or EL3. The same applies for sp_el2. In the case of sp_el3, it can only
// be modified if we are in EL3 and use the name sp instead of sp_el3.
.global init_stack_pointer_regs
init_stack_pointer_regs:
    mov     x0, stack_el3_top   // Sets stack pointer for EL3.
    mov     sp, x0              // Assumes that current exception level is EL3.

    mov     x0, stack_el0_top   // Sets stack pointer for EL0.
    msr     sp_el0, x0

    ret

// https://developer.arm.com/docs/ddi0595/b/aarch64-system-registers/scr_el3
.global init_system_registers_el3
init_system_registers_el3:
    msr cptr_el3, xzr                   // Disable coprocessor traps to EL3.
    msr cptr_el2, xzr                   // Disable coprocessor traps to EL2.

    // Configures scrl_el3
    mov     x0, scr_el3_ns_flag               // Sets the non-secure state flag.
    orr     x0, x0, scr_el3_rw_flag           // EL2 Execution state is AArch64.
    orr     x0, x0, scr_el3_routing_daif_flag // SError, IRQ and FIQ interrupts
                                              // are handled in EL3.
    orr     x0, x0, scr_el3_res1_5_to_4       // RES1 [5:4]
    msr     scr_el3, x0

    msr     daifclr, #0xF                     // Enable DAIF interrupts.

    isb

    ret

///////////////////////////////////////////////////////////////////////
// Flags definitions
///////////////////////////////////////////////////////////////////////

// Flags for the Secure Control Register (scr_el3)
// Note: bit 1, bit 2, and bit 3 (IRQ, FIQ and External Abort) are set because
//       interrupts are taken to EL3.
//
// https://developer.arm.com/docs/ddi0595/f/aarch64-system-registers/scr_el3
.equ scr_el3_ns_flag, 1             // Exception levels lower than EL3 are in
                                    // Non-secure state.
.equ scr_el3_rw_flag,  (1 << 10)    // Enables the execution state of EL2 to
                                    // AArch64.
.equ scr_el3_res1_5_to_4,  (3 << 4) // RES1 [5:4] - Reserved bits set to 1.

.equ scr_el3_routing_daif_flag, (7 << 1)  // SError, IRQ and FIQ interrupts are
                                          // handled in EL3.

// Defines the top addresses for EL3 stack. The address
// has to be 8 bytes aligned.
//
// Note: stack grows toward lower addresses.
.equ stack_el3_top, 0x50000
.equ stack_el0_top, 0x40000

