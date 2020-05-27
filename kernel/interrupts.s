/*
 * ARM Cortex A-53 vector table initialization
 *
 * Initializes the vector tables for the EL3 exception level.
 */
.section ".boot"

// Set the vector tables for EL3 exception level.
.global set_vector_tables
set_vector_tables:
    ldr x0, =vector_table_el3
    msr vbar_el3, x0
    isb

    ret

///////////////////////////////////////////////////////////////////////
// Vector tables definition and helper functions.
//
// Vector table contains 16 entries.
// Each entry is 128 bytes and contains at most 32 instructions.
// Vector table is align a 2Kb-align address (0x800)
///////////////////////////////////////////////////////////////////////

// Vector table for EL3.
.balign 0x800
vector_table_el3:
// Handlers for current EL with SP_EL0
.balign 0x80
el3_curr_el_sp0_sync:
    mov   x0, sync_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_thread_mode

.balign 0x80
el3_curr_el_sp0_irq:
    mov   x0, irq_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_thread_mode

.balign 0x80
el3_curr_el_sp0_fiq:
    mov   x0, fiq_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_thread_mode

.balign 0x80
el3_curr_el_sp0_serror:
    mov   x0, serror_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_thread_mode

// Handlers for current EL with SP_ELx
.balign 0x80
el3_curr_el_spx_sync:
    mov   x0, sync_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_handler_mode

.balign 0x80
el3_curr_el_spx_irq:
    mov   x0, irq_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_handler_mode

.balign 0x80
el3_curr_el_spx_fiq:
    mov   x0, fiq_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_handler_mode

.balign 0x80
el3_curr_el_spx_serror:
    mov   x0, serror_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_handler_mode

// Handlers for lower EL using AArch64
.balign 0x80
el3_lower_el_aarch64_sync:
    mov   x0, sync_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_changed_el_in_aarch64_state

.balign 0x80
el3_lower_el_aarch64_irq:
    mov   x0, irq_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_changed_el_in_aarch64_state

.balign 0x80
el3_lower_el_aarch64_fiq:
    mov   x0, fiq_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_changed_el_in_aarch64_state

.balign 0x80
el3_lower_el_aarch64_serror:
    mov   x0, serror_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_changed_el_in_aarch64_state

// Handlers for lower EL using AArch32
.balign 0x80
el3_lower_el_aarch32_sync:
    mov   x0, sync_type
    mrs   x1, esr_el3
    bl   intr_el3_handler_changed_el_in_aarch32_state

.balign 0x80
el3_lower_el_aarch32_irq:
    mov   x0, irq_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_changed_el_in_aarch32_state

.balign 0x80
el3_lower_el_aarch32_fiq:
    mov   x0, fiq_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_changed_el_in_aarch32_state

.balign 0x80
el3_lower_el_aarch32_serror:
    mov   x0, serror_type
    mrs   x1, esr_el3
    bl    intr_el3_handler_changed_el_in_aarch32_state


// FLAG defintions for the type of interrupts
.equ sync_type,     0
.equ irq_type,      1
.equ fiq_type,      2
.equ serror_type,   3

