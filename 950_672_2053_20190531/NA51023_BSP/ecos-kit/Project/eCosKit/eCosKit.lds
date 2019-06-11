/* Linker script for malta
 *
 * Version: Sourcery G++ Lite 4.3-221
 * Support: https://support.codesourcery.com/GNUToolchain/
 *
 * Copyright (c) 2007, 2008, 2009 CodeSourcery, Inc.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions.  No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 * */

/* Rule:
 * 1. to use .o in project layer just type the .o name with file extension. (UiFlow.o)
 * 2. to use .o in library layer have to add prefix module name. (DscSystem/SysTask.o)
 * */

OUTPUT_ARCH(mips)
ENTRY(_start)

SECTIONS
{
    .START_UP $BOARD_ECOS_ADDR:
    {
        _section_01_addr = ABSOLUTE(.);
        _load_entry_start_base = ABSOLUTE(.);
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.text.startup));
        _load_entry_end_base = ABSOLUTE(.);
    }
    .EXCEPTION_TABLE $BOARD_ECOS_ADDR + 0x180:
    {
        /* KEEP(OS.a(.text.exp)); */
        /* KEEP(Startup.a(.other_vector)); */
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.other_vector));
    }
    .VECTOR_TABLE $BOARD_ECOS_ADDR + 0x200:
    {
        __ebase = ABSOLUTE(.);	/* ??? TODO ??? */
        __start_vector_table = ABSOLUTE(.);
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.interrupt_vector*));
        __end_vector_table = ABSOLUTE(.);
    }
    .CODE_INFO $BOARD_ECOS_ADDR + 0x2F0:
    {
        KEEP(CodeInfo.o(.data*));
    }

    /* _stext = .; */	/* ??? TODO ??? */

    .BIN_INFO $BOARD_ECOS_ADDR + 0x400:
    {
        KEEP(BinInfo.o(.data*));
    }

    /* kernel common */
    . = ALIGN(4);
    .SECTION_01 :
    {
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.text.start));
    }

    /* device common */
    . = ALIGN(4);
    .SECTION_02 :
    {
        _section_01_size = ABSOLUTE(.) - _section_01_addr;
        _section_02_addr = ABSOLUTE(.);
    }

    /* device photo */
    . = ALIGN(4);
    .SECTION_03 :
    {
        _section_02_size = ABSOLUTE(.) - _section_02_addr;
        _section_03_addr = ABSOLUTE(.);
    }

    /* device movie */
    . = ALIGN(4);
    .SECTION_04 :
    {
        _section_03_size = ABSOLUTE(.) - _section_03_addr;
        _section_04_addr = ABSOLUTE(.);
    }

    /* device playback */
    . = ALIGN(4);
    .SECTION_05 :
    {
        _section_04_size = ABSOLUTE(.) - _section_04_addr;
        _section_05_addr = ABSOLUTE(.);
    }

    /* application common */
    . = ALIGN(4);
    .SECTION_06 :
    {
        _section_05_size = ABSOLUTE(.) - _section_05_addr;
        _section_06_addr = ABSOLUTE(.);
    }

    /* application photo */
    . = ALIGN(4);
    .SECTION_07 :
    {
        _section_06_size = ABSOLUTE(.) - _section_06_addr;
        _section_07_addr = ABSOLUTE(.);
    }

    /* application movie */
    . = ALIGN(4);
    .SECTION_08 :
    {
        _section_07_size = ABSOLUTE(.) - _section_07_addr;
        _section_08_addr = ABSOLUTE(.);
    }

    /* application playback */
    . = ALIGN(4);
    .SECTION_09 :
    {
        _section_08_size = ABSOLUTE(.) - _section_08_addr;
        _section_09_addr = ABSOLUTE(.);

	KEEP (*(.nvtversioninfo.begin*))
	_section_verinfostart = ABSOLUTE(.);
        KEEP(*(.version.info.*))
	_section_verinfoend = ABSOLUTE(.);
        KEEP (*(.nvtversioninfo.end*))
    }

    /* others */
    . = ALIGN(4);
    /*
    .SECTION_10 :
    {
    */
    	_section_09_size = ABSOLUTE(.) - _section_09_addr;
        _section_10_addr = ABSOLUTE(.);

       .init ALIGN (0x4) : { . = .; KEEP (*(.init*)) }
       .text ALIGN (0x4) : { _stext = .; _ftext = . ; *(.text*) *(.text.*) *(.stub) *(.gnu.warning) *(.gnu.linkonce.t*) *(.mips16.fn.*) *(.mips16.call.*) } _etext = .; PROVIDE (etext = .);

       .fini ALIGN (0x4) : { . = .; KEEP (*(.fini*)) }
       .rodata ALIGN (0x8) : { . = .; *(.rodata*) *(.rodata.*) *(.gnu.linkonce.r*) }
       .rodata1 ALIGN (0x8) : { . = .; *(.rodata1*) *(.rodata1.*) }
       .data ALIGN (0x8) : { __ram_data_start = ABSOLUTE (.); _fdata = . ; *(.data*) *(.data.*) *(.gnu.linkonce.d*) *( .2ram.*) . = ALIGN (8); SORT(CONSTRUCTORS) } __rom_data_start = LOADADDR(.data);
       .data1 ALIGN (0x8) : { . = .; *(.data1*) *(.data1.*) }
       .eh_frame_hdr ALIGN (0x4) : { . = .; __EH_FRAME_BEGIN__ = .; KEEP(*(.eh_frame_hdr*)) __FRAME_END__ = .; . = . + 8; }
       .eh_frame ALIGN (0x4) : { . = .; __EH_FRAME_BEGIN__ = .; KEEP(*(.eh_frame*)) __FRAME_END__ = .; . = . + 8; }
       .gcc_except_table ALIGN (0x4) : { . = .; KEEP(*(.gcc_except_table*)) *(.gcc_except_table.*) }

       .ctors ALIGN (0x4) : { . = .; __CTOR_LIST__ = .; PROVIDE (__CTOR_LIST__ = .); KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors*)) __CTOR_END__ = .; PROVIDE (__CTOR_END__ = .); }
       .dtors ALIGN (0x4) : { . = .; __DTOR_LIST__ = .; PROVIDE (__DTOR_LIST__ = .); KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors*)) __DTOR_END__ = .; PROVIDE (__DTOR_END__ = .); }
       .devtab ALIGN (0x4) : { . = .; KEEP(*( SORT (.ecos.table.*))) ; }
       .maintab ALIGN (0x4) : { . = .; KEEP(*( SORT (.main.table.*))) ; }

       _gp = ALIGN(16) + 0x7ff0; .got ALIGN (0x4) : { . = .; *(.got.plt) *(.got*) }
       .dynamic ALIGN (0x4) : { . = .; *(.dynamic*) }
       .sdata ALIGN (0x4) : { . = .; *(.sdata*) *(.sdata.*) *(.gnu.linkonce.s*) }
       .lit8 ALIGN (0x8) : { . = .; *(.lit8*) }
       .lit4 : AT ((LOADADDR (.lit8) + SIZEOF (.lit8) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; *(.lit4) } __ram_data_end = .; _edata = . ; PROVIDE (edata = .);

       _section_10_size = ABSOLUTE(.) - _section_10_addr;
       ecos_bin_size = ABSOLUTE(.) - _section_01_addr;

       /* collect zero initialized global data (mark "B" in sym file) */
       /* collect static local data (mark "b" in sym file) */
       _section_zi_addr = ABSOLUTE(.);
       _image_general_zi_zi_base = ABSOLUTE(.);

       __bss_start = .; _fbss = .; .sbss ALIGN (0x8) : { . = .; *(.dynsbss*) *(.sbss) *(.sbss.*) *(.scommon*) *(.gnu.linkonce.sb.*) }
       .bss ALIGN (0x8) : { *(.dynbss*) *(.bss*) *(.bss.*) *(COMMON) *(.gnu.linkonce.b.*) } __bss_end = .;

       /* NOTE:
       if using -fcommon compiler option, un-initialized global data will be isolated to .common section.
       if using -fno-common compiler option, un-initialized global data will be merged to .bss section.
       memory between _image_general_zi_zi_base and _image_general_zi_zi_limit will be fill to zero before _main().
       */
       . = ALIGN(4);
       .common : {
          __common_base = ABSOLUTE(.);
          /* collect "small" un-initialized global data (mark "C" in sym file) */
          *(.scommon*)
          /* collect "large" un-initialized global data (mark "B" in sym file) */
          *(COMMON)
          __common_limit = ABSOLUTE(.);
        }

        /* _image_general_zi_zi_limit = ABSOLUTE(.); */

    /*
    }
    */

    . = ALIGN(16);

    /* heap area start for eCos */
    . = ALIGN(16);
    __heap1 = .; 	/* All rest of RAM reserved for heap. !!!TODO!!!manage kernel memory pool and stack */
    . = . + 0x00100000;
    . = ALIGN(0x20000); /* because _ecos_mem_sizembit must align to 1MBit (128K Bytes) */
    __heap1_end = .;

    _ecos_mem_end = .;
    _ecos_mem_size = . - _section_01_addr;
    _ecos_mem_sizembit = _ecos_mem_size* 8 / (1024*1024) ;

    /* end of code and eCos stuff */
    _end = .; PROVIDE (end = .);

    /* stack area start for DSP-FreeRTOS firmware */
    _image_general_zi_zi_limit = ABSOLUTE(.);
    _stack_start = ALIGN(16);

}

EXTERN($SYMBOL_NETWORK)
EXTERN(NVT_HoughLink)

