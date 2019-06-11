/* Memory sub-system initialization code */

#include <config.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>

#include <asm/arch/na51023_regs.h>

	.text
	.set noreorder

	.globl	lowlevel_init
lowlevel_init:
	/*
************************************************************************
* mapping 0xc0000000 via TLB
************************************************************************
*/


/*
************************************************************************
* mapping 0xc0000000 via TLB
************************************************************************
*/
	jr	ra
	nop
