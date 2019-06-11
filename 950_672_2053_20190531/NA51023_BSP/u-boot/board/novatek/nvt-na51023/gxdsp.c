/**
    NVT utilities for command customization

    @file       gxdsp.c
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#include <common.h>
#include <fs.h>
#include <asm/io.h>
#include <nvt_type.h>
#include "gxdsp.h"
#include <asm/arch/rcw_macro.h>
#include "drv/dma.h"
#include "dsp/dsp.h"
#include <asm/arch/IOAddress.h>
#include "nvtpack.h"
#include <modelext/modelext_parser.h>
#include <modelext/dram_partition_info.h>

BOOL GxDSP_Open(GXDSP_OPEN *pOpen)
{
	/* ToDo */
	return TRUE;
}
