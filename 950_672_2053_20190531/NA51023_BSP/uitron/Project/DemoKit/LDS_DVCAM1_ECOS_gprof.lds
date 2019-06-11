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
    .SPECIAL_RO $BOARD_UITRON_ADDR:
    {
    	_section_01_addr = ABSOLUTE(.);
        KEEP(BinInfo.o(.data*));
    }
    .EXCEPTION_TABLE $BOARD_UITRON_ADDR + 0x180:
    {
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.other_vector*));
    }
    .VECTOR_TABLE $BOARD_UITRON_ADDR + 0x200:
    {
        __ebase = ABSOLUTE(.);
        __start_vector_table = ABSOLUTE(.);
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.interrupt_vector*));
        KEEP(Startup.a(.text.isr*));
        __end_vector_table = ABSOLUTE(.);
    }
    .CODE_INFO $BOARD_UITRON_ADDR + 0x2F0:
    {
       	_code_info_start_base = ABSOLUTE(.);
        KEEP(CodeInfo.o(.data*));
        _code_info_end_base = ABSOLUTE(.);
    }
    .START_UP $BOARD_UITRON_ADDR + 0x400:
    {
        _load_entry_start_base = ABSOLUTE(.);
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.text.start*));
        _load_entry_end_base = ABSOLUTE(.);
    }
    .MODELEXT_INFO $BOARD_UITRON_ADDR + 0x1000:
    {
    	_section_modelext_info_addr = ABSOLUTE(.);
    	KEEP($MODELEXT/modelext_info.o (modelext_header.modelext_info));
    	KEEP(*(modelext_data.modelext_info));
    	KEEP($MODELEXT/bin_info.o (modelext_header.bin_info));
    	KEEP(*(modelext_data.bin_info));
    	KEEP($MODELEXT/pinmux_cfg.o (modelext_header.pinmux_cfg));
    	KEEP(*(modelext_data.pinmux_cfg));
    	KEEP($MODELEXT/intdir_cfg.o (modelext_header.intdir_cfg));
    	KEEP(*(modelext_data.intdir_cfg));
    	KEEP($MODELEXT/emb_partition_info.o (modelext_header.emb_partition_info));
    	KEEP(*(modelext_data.emb_partition_info));
    	KEEP($MODELEXT/gpio_info.o (modelext_header.gpio_info));
    	KEEP(*(modelext_data.gpio_info));
    	KEEP($MODELEXT/dram_partition_info.o (modelext_header.dram_partition_info));
    	KEEP(*(modelext_data.dram_partition_info));
    	KEEP($MODELEXT/model_cfg.o (modelext_header.model_cfg));
    	KEEP(*(modelext_data.model_cfg));
    	_section_modelext_info_size = ABSOLUTE(.) - _section_modelext_info_addr;
    }
    . = ALIGN(4);
    .DOCHG_DMA_CLK :
    {
        _change_dma_clock_text_start = .;
		    KEEP(Driver_Sys/clock_power_down.o(.chgdma_text*));
		    *(.chgdma_text*) /* collect functions used in change DMA flow */
		    _change_dma_clock_text_end = .;
        _change_dma_clock_data_start = .;
        KEEP(*(.chgdma_data*));
		    *(.chgdma_data*) /* collect variables used in change DMA flow */
		    _change_dma_clock_data_end = .;
    }
    _stext = ABSOLUTE(.);
    /* all global C++ constructors */
    . = ALIGN(4);

       .ctors ALIGN (0x4) : { . = .; __CTOR_LIST__ = .; PROVIDE (__CTOR_LIST__ = .); KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors*)) __CTOR_END__ = .; PROVIDE (__CTOR_END__ = .); }
       .dtors ALIGN (0x4) : { . = .; __DTOR_LIST__ = .; PROVIDE (__DTOR_LIST__ = .); KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors*)) __DTOR_END__ = .; PROVIDE (__DTOR_END__ = .); }
       .devtab ALIGN (0x4) : { . = .; KEEP(*( SORT (.ecos.table.*))) ; }

    .init ALIGN (0x4) : { . = .; KEEP (*(.init*)) }
    . = ALIGN(4);

    /* kernel common */
    . = ALIGN(4);
    .text :
    {
        _ftext = . ; 
        /*============================ before os start ===============================  */
        *(.kercfg_data*) /*collect install ID process of all modules*/
        *(.kercfg_text*) /*collect install ID process of all modules*/
    	_section_01_size = ABSOLUTE(.) - _section_01_addr;
        _section_02_addr = ABSOLUTE(.);
    	_section_02_size = ABSOLUTE(.) - _section_02_addr;
        _section_03_addr = ABSOLUTE(.);
    	_section_03_size = ABSOLUTE(.) - _section_03_addr;
        _section_04_addr = ABSOLUTE(.);
    	_section_04_size = ABSOLUTE(.) - _section_04_addr;
        _section_05_addr = ABSOLUTE(.);
    	_section_05_size = ABSOLUTE(.) - _section_05_addr;
        _section_06_addr = ABSOLUTE(.);
    	_section_06_size = ABSOLUTE(.) - _section_06_addr;
        _section_07_addr = ABSOLUTE(.);
    	_section_07_size = ABSOLUTE(.) - _section_07_addr;
        _section_08_addr = ABSOLUTE(.);
    	_section_08_size = ABSOLUTE(.) - _section_08_addr;
        _section_09_addr = ABSOLUTE(.);
    	_section_09_size = ABSOLUTE(.) - _section_09_addr;
	KEEP (*(.nvtversioninfo.begin*))
        _section_10_addr = ABSOLUTE(.);
	_section_verinfostart = _section_10_addr;
        KEEP(*(.version.info.*))
	_section_verinfoend = ABSOLUTE(.);
        KEEP (*(.nvtversioninfo.end*))
        *(.text*)
        *(.got*)
        *(.rodata*)
        *(.data*)
        *(.data1*)
        *(.rodata1*)
        *(.reginfo*)
        *(.rel.dyn*)
        *(.sdata*) /* __fdlib_version in libm.a */
        *(.text*) *(.text.*) *(.stub) *(.gnu.warning) *(.gnu.linkonce.t*) *(.mips16.fn.*) *(.mips16.call.*)  /* for C++ */
    }
    _etext = .; PROVIDE (etext = .);
    _section_10_size = ABSOLUTE(.) - _section_10_addr;

    .fini ALIGN (0x4) : { . = .; KEEP (*(.fini*)) }
    .rodata ALIGN (0x8) : { . = .; *(.rodata*) *(.rodata.*) *(.gnu.linkonce.r*) }
    .rodata1 ALIGN (0x8) : { . = .; *(.rodata1*) *(.rodata1.*) }
    .data ALIGN (0x8) : { __ram_data_start = ABSOLUTE (.); _fdata = . ; *(.data*) *(.data.*) *(.gnu.linkonce.d*) *( .2ram.*) . = ALIGN (8); SORT(CONSTRUCTORS) } __rom_data_start = LOADADDR(.data);
    .data1 ALIGN (0x8) : { . = .; *(.data1*) *(.data1.*) }
    .eh_frame_hdr ALIGN (0x4) : { . = .; __EH_FRAME_BEGIN__ = .; KEEP(*(.eh_frame_hdr*)) __FRAME_END__ = .; . = . + 8; }
    .eh_frame ALIGN (0x4) : { . = .; __EH_FRAME_BEGIN__ = .; KEEP(*(.eh_frame*)) __FRAME_END__ = .; . = . + 8; }
    .gcc_except_table ALIGN (0x4) : { . = .; KEEP(*(.gcc_except_table*)) *(.gcc_except_table.*) }

    _gp = ALIGN(16) + 0x7ff0; .got ALIGN (0x4) : { . = .; *(.got.plt) *(.got*) }
    .dynamic ALIGN (0x4) : { . = .; *(.dynamic*) }
    .sdata ALIGN (0x4) : { . = .; *(.sdata*) *(.sdata.*) *(.gnu.linkonce.s*) }
    .lit8 ALIGN (0x8) : { . = .; *(.lit8*) }
    .lit4 : AT ((LOADADDR (.lit8) + SIZEOF (.lit8) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; *(.lit4) } __ram_data_end = .; _edata = . ; PROVIDE (edata = .);

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

    . = ALIGN(16);

    /* heap area start for eCos */
    __heap1 = ALIGN(16);	/* All rest of RAM reserved for heap. !!!TODO!!!manage kernel memory pool and stack */
    __heap1_end = . + 0x00300000;   /* reserved 3MB for ecos heap memory, 3MB is enough for EVB ecos network using
                                     , this value is project dependent should be adjusted by project*/
    . = __heap1_end;
    _ecos_mem_end = .;
    _ecos_mem_size = . - $BOARD_UITRON_ADDR;
    _ecos_mem_sizembit = _ecos_mem_size / (1024*1024) * 8;

    /* end of code and eCos stuff */
    _end = .; PROVIDE (end = .);

    /* stack area start for RTOS tasks */
    _image_general_zi_zi_limit = ABSOLUTE(.);
    _section_all_size = ABSOLUTE(.) - _section_01_addr;
    _stack_start = ALIGN(16);
}

EXTERN($SYMBOL_IPL)
EXTERN($SYMBOL_NETWORK)
EXTERN($SYMBOL_PLUG)
