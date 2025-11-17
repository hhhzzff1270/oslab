OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS {
    /* 代码从0x80000000开始 */
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
        _bss_end = .;
    }
    
    /* 栈空间 */
    .stack : {
        . = ALIGN(16);
        stack_top = .;
        . += 4096;  /* 4KB栈空间 */
    }
    
    /DISCARD/ : {
        *(.comment)
        *(.note*)
    }
    end = .;
}