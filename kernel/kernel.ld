/******************************************************************************
*	Linker script to generate the RPi kernel images.
*
* See linker information in:
* https://sourceware.org/binutils/docs-2.34/ld/index.html
******************************************************************************/

SECTIONS {

	/*
	 * The starting address in the RPi 3 Model B and B+
	 * 524288 bytes = 0x80000
	 */
    . = 0x80000;
    
	/* Contains the binary image. */
	.text : {
		/* Contains the boot code */
		*(.boot)

		/* Contains the rest of the code */
		*(.text .text.*)
	}

	/* Contains constant data (read only data */
	.rodata : {
		*(.rodata .rodata.*)
	}

    . = ALIGN(4096);
    /* https://sourceware.org/binutils/docs-2.24/ld/PROVIDE.html#PROVIDE */
    PROVIDE(linker_start_data_section = .);

	/* Contains the variables. */
	.data : {
		*(.data)
	}

	/* The BSS segment contains unitialized static variables filled with zeros. */
	.bss : {
		*(.bss)
		*(COMMON)
	}

    . = ALIGN(4096);
    /* https://sourceware.org/binutils/docs-2.24/ld/PROVIDE.html#PROVIDE */
    PROVIDE(linker_end_data_section = .);

	/* Everything else that will be discarded. */
	/DISCARD/ : {
		*(*)
	}
}


