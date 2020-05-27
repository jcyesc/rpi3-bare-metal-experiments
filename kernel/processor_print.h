#ifndef PROCESSOR_PRINT_H
#define PROCESSOR_PRINT_H

/*
 * Prints all the information about the ARM Cortex A-53 processor and its
 * configuration registers.
 */
void processor_print_all_info();

/*
 * Prints Prcoessor State register: CurrentEL, DAIF, NZCV, SPSel.
 */
void processor_print_pstate();

/*
 * Prints the secure configuration register. This register controls the secure
 * state and trapping of exceptions to EL3.
 */
void processor_print_secure_config_register_el3();

/**
 * Prints the AArch64 processor feature register 0. The ID_AA64PFR0_EL1
 * register provides information about implemented PE features in AArch64 state.
 */
void processor_print_aarch64_processor_feature_register();

/*
 * Prints the Control System Register for EL3 (sctlr_el3).
 */
void processor_print_sctlr_el3();

#endif /* PROCESSOR_PRINT_H */
