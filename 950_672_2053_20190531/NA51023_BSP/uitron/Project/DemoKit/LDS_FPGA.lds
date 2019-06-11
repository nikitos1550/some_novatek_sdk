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
ENTRY(__start)
SEARCH_DIR(.)

SECTIONS
{
    .SPECIAL_RO $BOARD_UITRON_ADDR:
    {
    	_section_01_addr = ABSOLUTE(.);
        KEEP(BinInfo.o(.data*));
    }
    .EXCEPTION_TABLE $BOARD_UITRON_ADDR + 0x180:
    {
        KEEP(Startup/exception_MIPS.o(.text*));
    }
    .VECTOR_TABLE $BOARD_UITRON_ADDR + 0x200:
    {
        __start_vector_table = ABSOLUTE(.);
        KEEP(Startup/Isr_MIPS.o(.text*));
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
        Startup/Loader_MIPS.o(.text*)
        Startup/Startup_MIPS.o (.text* .data* .data1* .rodata* .rodata1* .reginfo*)
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
    /* kernel common */
    . = ALIGN(4);
    .SECTION_01 :
    {
        /*============================ before os start ===============================  */
        *(.kercfg_data*) /*collect install ID process of all modules*/
        *(.kercfg_text*) /*collect install ID process of all modules*/
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
    }

    /* others */
    . = ALIGN(4);
    .SECTION_10 :
    {
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
        *(.sdata*) /* __fdlib_version in libm.a */
    }

    _section_10_size = ABSOLUTE(.) - _section_10_addr;
    _etext = ABSOLUTE(.);

    /* NOTE:
    if using -fcommon compiler option, un-initialized global data will be isolated to .common section.
    if using -fno-common compiler option, un-initialized global data will be merged to .bss section.
    memory between _image_general_zi_zi_base and _image_general_zi_zi_limit will be fill to zero before _main().
    */
    . = ALIGN(4);
    .bss :
    {
        _image_general_zi_zi_base = ABSOLUTE(.);
        /* collect zero initialized global data (mark "B" in sym file) */
        /* collect static local data (mark "b" in sym file) */
        *(.bss*)
    }
    . = ALIGN(4);
    .common :
    {
        __common_base = ABSOLUTE(.);
        /* collect "small" un-initialized global data (mark "C" in sym file) */
        *(.scommon*)
        /* collect "large" un-initialized global data (mark "B" in sym file) */
        *(COMMON)
        __common_limit = ABSOLUTE(.);
    }

    . = ALIGN(16);
        _image_general_zi_zi_limit = ABSOLUTE(.);
        _section_all_size = ABSOLUTE(.) - _section_01_addr;
    _stack_start = .;

}

EXTERN($SYMBOL_IPL)
EXTERN($SYMBOL_NETWORK)
EXTERN($SYMBOL_PLUG)
