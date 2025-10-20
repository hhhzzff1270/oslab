OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS {
    . = 0x80000000;
    .text : {
        *(.text .text.*)
    }
    .rodata : {
        *(.rodata .rodata.*)
        . = ALIGN(8);
    }
    .data : {
        *(.data .data.*)
        . = ALIGN(8);
    }
    
    .bss : {
    _bss_start = .;
    *(.bss .bss.*)
    *(COMMON)
    }  
    _bss_end = .;

    .stack : 
    {
    . = ALIGN(16);
    stack_bottom = .;
    . += 4096;
    stack_top = .;
    }
    
    /DISCARD/ : {
        *(.comment)
        *(.note*)
    }
}