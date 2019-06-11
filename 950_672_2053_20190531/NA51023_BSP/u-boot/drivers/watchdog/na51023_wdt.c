/*
 *  watchdog/na51000_wdt.c
 *
 *  Author:	Howard Chang
 *  Created:	Aug 27, 2018
 *  Copyright:	Novatek Inc.
 *
 */
#include <common.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <watchdog.h>
#include <asm/arch/rcw_macro.h>
#include <asm/arch/IOAddress.h>

#define WDT_REG_ADDR(ofs)       (IOADDR_WDT_REG_BASE+(ofs))
#define WDT_GETREG(ofs)         INW(WDT_REG_ADDR(ofs))
#define WDT_SETREG(ofs,value)   OUTW(WDT_REG_ADDR(ofs), (value))

#define CG_REG_ADDR(ofs)       (IOADDR_CG_REG_BASE+(ofs))
#define CG_GETREG(ofs)         INW(CG_REG_ADDR(ofs))
#define CG_SETREG(ofs,value)   OUTW(CG_REG_ADDR(ofs), (value))

#define CG_ENABLE_OFS 0x74
#define CG_RESET_OFS 0x84
#define WDT_POS (1 << 17)

#define WDT_CTRL_OFS 0x0
#define WDT_TRIG_OFS 0x8

void wdt_init(unsigned long ignored)
{
	u32 reg_value;

	reg_value = CG_GETREG(CG_ENABLE_OFS);
	CG_SETREG(CG_ENABLE_OFS, reg_value | WDT_POS);

	reg_value = CG_GETREG(CG_RESET_OFS);
	CG_SETREG(CG_RESET_OFS, reg_value | WDT_POS);

	WDT_SETREG(WDT_CTRL_OFS, 0x5A96FF12);

	udelay(80);

	WDT_SETREG(WDT_CTRL_OFS, 0x5A96FF13);
}

void watchdog_reset(void)
{
	/*Kick the dog*/
	WDT_SETREG(WDT_TRIG_OFS, 0x1);
}