/*
	Interrupt module driver.

	This file is the driver of Interrupt module.

	@file		interrupt.c
	@ingroup
	@note		Nothing.

	Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#include <common.h>
#include <asm/io.h>
#include <modelext/interrupt.h>
#include <asm/arch/nvt_common.h>
#include <asm/arch/IOAddress.h>
#include <asm/arch/rcw_macro.h>

/**
	Configure interrupt controller

	Configure interrupt controller by upper layer

	@param[in] config_id	Configuration ID
	@param[in] config		Configuration value
	@return void
*/
void int_set_config(INT_CONFIG_ID config_id, UINT32 config)
{
	switch (config_id) {
	case INT_CONFIG_ID_INT_DESTINATION: {
			PINT_DST int_dst = (PINT_DST)config;
			UINT32   addr[CC_CORE_NUM];

			addr[CC_CORE_MIPS1] = (UINT32)&int_dst->int_id_dst[CC_CORE_MIPS1].Reg;
			addr[CC_CORE_MIPS2] = (UINT32)&int_dst->int_id_dst[CC_CORE_MIPS2].Reg;
			addr[CC_CORE_DSP] = (UINT32)&int_dst->int_id_dst[CC_CORE_DSP].Reg;

			OUTW(IOADDR_INTERRUPT_REG_BASE + (CC_CORE_MIPS1 << 4) + 0x0, *(UINT32 *)addr[CC_CORE_MIPS1]);
			OUTW(IOADDR_INTERRUPT_REG_BASE + (CC_CORE_MIPS1 << 4) + 0x4, *(UINT32 *)(addr[CC_CORE_MIPS1] + 4));

			OUTW(IOADDR_INTERRUPT_REG_BASE + (CC_CORE_MIPS2 << 4) + 0x0, *(UINT32 *)addr[CC_CORE_MIPS2]);
			OUTW(IOADDR_INTERRUPT_REG_BASE + (CC_CORE_MIPS2 << 4) + 0x4, *(UINT32 *)(addr[CC_CORE_MIPS2] + 4));

			OUTW(IOADDR_INTERRUPT_REG_BASE + (CC_CORE_DSP << 4) + 0x0, *(UINT32 *)addr[CC_CORE_DSP]);
			OUTW(IOADDR_INTERRUPT_REG_BASE + (CC_CORE_DSP << 4) + 0x4, *(UINT32 *)(addr[CC_CORE_DSP] + 4));


			debug("MIPS1[0x%08x][0x%08x]\r\n", INW(IOADDR_INTERRUPT_REG_BASE + (CC_CORE_MIPS1 << 4) + 0x0), INW(IOADDR_INTERRUPT_REG_BASE + (CC_CORE_MIPS1 << 4) + 0x4));
			debug("MIPS2[0x%08x][0x%08x]\r\n", INW(IOADDR_INTERRUPT_REG_BASE + (CC_CORE_MIPS2 << 4) + 0x0), INW(IOADDR_INTERRUPT_REG_BASE + (CC_CORE_MIPS2 << 4) + 0x4));
			debug("  DSP[0x%08x][0x%08x]\r\n", INW(IOADDR_INTERRUPT_REG_BASE + (CC_CORE_DSP << 4) + 0x0), INW(IOADDR_INTERRUPT_REG_BASE + (CC_CORE_DSP << 4) + 0x4));

		}
		break;

	default:
		printf("Unknow interrupt set config ID%d\r\n", config_id);
		break;
	}
}