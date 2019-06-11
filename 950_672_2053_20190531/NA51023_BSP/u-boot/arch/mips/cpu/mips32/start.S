/*
 *  Startup Code for MIPS32 CPU-core
 *
 *  Copyright (c) 2003	Wolfgang Denk <wd@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm-offsets.h>
#include <config.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>

#ifdef CONFIG_NA51023
	.extern CONFIG_SYS_TEXT_BASE
#endif

#ifndef CONFIG_SYS_MIPS_CACHE_MODE
#define CONFIG_SYS_MIPS_CACHE_MODE CONF_CM_CACHABLE_NONCOHERENT
#endif

	/*
	 * For the moment disable interrupts, mark the kernel mode and
	 * set ST0_KX so that the CPU does not spit fire when using
	 * 64-bit addresses.
	 */
	.macro	setup_c0_status set clr
	.set	push
	mfc0	t0, CP0_STATUS
	or	t0, ST0_CU0 | \set | 0x1f | \clr
	xor	t0, 0x1f | \clr
	mtc0	t0, CP0_STATUS
	.set	noreorder
	sll	zero, 3				# ehb
	.set	pop
	.endm

# -------------------------------------------------------------------------#
# Loader  code                                                             #
# -------------------------------------------------------------------------#
#define FUNC_START(name)    \
    .type name,@function;   \
    .set push ;             \
    .globl name;        \
    .ent   name;        \
    .set noreorder ;        \
name:

#define FUNC_END(name)      \
name##_end:         \
    .set pop;               \
    .end name       \


#define RVECENT(f,n) \
   b f; nop
#define XVECENT(f,bev) \
   b f     ;           \
   li k0,bev

    .section ".text.startup","ax"
FUNC_START(_start_nvt)
#ifdef CONFIG_NA51023
	mfc0	t5, CP0_CONFIG
	andi	t5, 0xFFF8
	ori	t5, CONF_CM_UNCACHED
	mtc0	t5, CP0_CONFIG
	# -------------------------------------------------------------------------#
    # (1). mapping 0xc0000000 via TLB start                                    #
    # Only MIPS2 should configured these procedure                             #
    # -------------------------------------------------------------------------#
    mtc0    $0, $2 # Clr EntryLo0
    mtc0    $0, $3 # Clr EntryLo1
    mtc0    $0, $5 # Clr PageMask
    mtc0    $0, $6 # Clr Wired

    lui     $15,0x1fff
    ori     $15,$15,0xe000 # $15=0x1fffe000
    mtc0    $15,$5,0 # Write to PageMask[28:13] set page size as 256MB

    # write EntryLo0($2) ,PFN=0xC0000,C=3, D=1,V=1,G=1
    lui     $15,0x0300
    ori     $15,$15,0x0017 # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017)
#   ori     $15,$15,0x003f # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017), uncache accelerated=7 (0x003f)
    mtc0    $15,$2,0

    # write EntryLo1($3) ,PFN=0xD0000,C=3, D=1,V=1,G=1
    lui     $15,0x0340
    ori     $15,$15,0x0017 # $15=0x0140001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$3,0

    # write EntryHi ,VPN=0, VPN2X=0, ASID=0;
    lui     $15, 0xC000
    mtc0    $15, $10, 0

    # set index register = 0
    mtc0    $0, $0, 0

    # write to TLB
    ssnop
    ssnop
    tlbwi # Update TLB register
    nop
    # -------------------------------------------------------------------------#
    # (0)mapping 0xc0000000 via TLB end                                       #
    # -------------------------------------------------------------------------#
    # -------------------------------------------------------------------------#
    #               |___________|                                              #
    # 0x7FFF_FFFC   |           |                                              #
    #               | un-cache  |                                              #
    # 0x4000_0000   |___________|                                              #
    # 0x3FFF_FFFC   |           |                                              #
    #               | cacheable |                                              #
    # 0x0000_0000   |___________|                                              #
    #                virtual            |   physical                           #
    # Idx(0) 0xC000_0000 ~ 0xDFFF_FFFF --> 0xC000_0000 ~ 0xDFFF_FFFF (un-cache)#
    # Idx(1) 0x0000_0000 ~ 0x1FFF_FFFF --> 0x0000_0000 ~ 0x1FFF_FFFF (cachable)#
    # Idx(2) 0x2000_0000 ~ 0x3FFF_FFFF --> 0x2000_0000 ~ 0x3FFF_FFFF (cachable)#
    # Idx(3) 0x4000_0000 ~ 0x5FFF_FFFF --> 0x0000_0000 ~ 0x1FFF_FFFF (un-cache)#
    # Idx(4) 0x6000_0000 ~ 0x7FFF_FFFF --> 0x2000_0000 ~ 0x3FFF_FFFF (un-cache)#
    # -------------------------------------------------------------------------#

    # -------------------------------------------------------------------------#
    # (1). mapping 0x00000000~0x1FFFFFFF -> 0x00000000~0x1FFFFFFF via TLB start#
    # -------------------------------------------------------------------------#
    lui     $15,0x1fff
    ori     $15,$15,0xe000 # $15=0x1fffe000
    mtc0    $15,$5,0 # Write to PageMask[28:13] set page size as 256MB

    # write EntryLo0($2) ,PFN=0x00000,C=3, D=1,V=1,G=1
    lui     $15,0x0000
    ori     $15,$15,0x0017 # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$2,0

    # write EntryLo1($3) ,PFN=0x10000,C=3, D=1,V=1,G=1
    lui     $15,0x0040
    ori     $15,$15,0x0017 # $15=0x0140001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$3,0

    # write EntryHi ,VPN=0, VPN2X=0, ASID=0;
    lui     $15, 0x0000
    mtc0    $15, $10, 0

    # set index register = 0
    li      t7, 0x1
    mtc0    t7, $0, 0
    ehb
    # write to TLB
    ssnop
    ssnop
    tlbwi # Update TLB register
    nop
    ehb

    # -------------------------------------------------------------------------#
    # (1). mapping 0x00000000~0x1FFFFFFF -> 0x00000000~0x1FFFFFFF via TLB end  #
    # -------------------------------------------------------------------------#

    # -------------------------------------------------------------------------#
    # (2). mapping 0x20000000~0x3FFFFFFF -> 0x20000000~0x3FFFFFFF via TLB start#
    # -------------------------------------------------------------------------#
    lui     $15,0x1fff
    ori     $15,$15,0xe000 # $15=0x1fffe000
    mtc0    $15,$5,0 # Write to PageMask[28:13] set page size as 256MB

    # write EntryLo0($2) ,PFN=0x00000,C=3, D=1,V=1,G=1
    lui     $15,0x0080
    ori     $15,$15,0x0017 # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$2,0

    # write EntryLo1($3) ,PFN=0x10000,C=3, D=1,V=1,G=1
    lui     $15,0x00C0
    ori     $15,$15,0x0017 # $15=0x0140001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$3,0

    # write EntryHi ,VPN=0, VPN2X=0, ASID=0;
    lui     $15, 0x2000
    mtc0    $15, $10, 0

    # set index register = 0
    li      t7, 0x2
    mtc0    t7, $0, 0
    ehb
    # write to TLB
    ssnop
    ssnop

    tlbwi # Update TLB register
    nop
    ehb

    # -------------------------------------------------------------------------#
    # (2). mapping 0x20000000~0x3FFFFFFF -> 0x20000000~0x3FFFFFFF via TLB end  #
    # -------------------------------------------------------------------------#
    # -------------------------------------------------------------------------#
    # (3). mapping 0x40000000~0x5FFFFFFF -> 0x00000000~0x1FFFFFFF via TLB start#
    # -------------------------------------------------------------------------#
    lui     $15,0x1fff
    ori     $15,$15,0xe000 # $15=0x1fffe000
    mtc0    $15,$5,0 # Write to PageMask[28:13] set page size as 256MB

    # write EntryLo0($2) ,PFN=0x00000,C=3, D=1,V=1,G=1
    lui     $15,0x0000
    ori     $15,$15,0x0017 # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$2,0

    # write EntryLo1($3) ,PFN=0x10000,C=3, D=1,V=1,G=1
    lui     $15,0x0040
    ori     $15,$15,0x0017 # $15=0x0140001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$3,0

    # write EntryHi ,VPN=0, VPN2X=0, ASID=0;
    lui     $15, 0x4000
    mtc0    $15, $10, 0

    # set index register = 0
    li      t7, 0x3
    mtc0    t7, $0, 0

    # write to TLB
    ssnop
    ssnop
    tlbwi # Update TLB register
    nop
    ehb

    # -------------------------------------------------------------------------#
    # (3). mapping 0x40000000~0x5FFFFFFF -> 0x00000000~0x1FFFFFFF via TLB end  #
    # -------------------------------------------------------------------------#
    # -------------------------------------------------------------------------#
    # (4). mapping 0x60000000~0x7FFFFFFF -> 0x20000000~0x3FFFFFFF via TLB start#
    # -------------------------------------------------------------------------#
    lui     $15,0x1fff
    ori     $15,$15,0xe000 # $15=0x1fffe000
    mtc0    $15,$5,0 # Write to PageMask[28:13] set page size as 256MB

    # write EntryLo0($2) ,PFN=0x00000,C=3, D=1,V=1,G=1
    lui     $15,0x0080
    ori     $15,$15,0x0017 # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$2,0

    # write EntryLo1($3) ,PFN=0x10000,C=3, D=1,V=1,G=1
    lui     $15,0x00C0
    ori     $15,$15,0x0017 # $15=0x0140001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$3,0

    # write EntryHi ,VPN=0, VPN2X=0, ASID=0;
    lui     $15, 0x6000
    mtc0    $15, $10, 0

    # set index register = 0
    li      t7, 0x4
    mtc0    t7, $0, 0

    # write to TLB
    ssnop
    ssnop
    tlbwi # Update TLB register
    nop
    ehb
    # -------------------------------------------------------------------------#
    # (4). mapping 0x60000000~0x7FFFFFFF -> 0x20000000~0x3FFFFFFF via TLB end  #
    # -------------------------------------------------------------------------#
#endif
	RVECENT(reset,0)			# U-boot entry point
    nop
FUNC_END( _start_nvt )
	.set noreorder

	.globl _start
	.text
_start:
	/* U-boot entry point */
	b	reset
	 nop

#ifndef CONFIG_NA51023  /* for reducing code size */
	.org 0x10
#if defined(CONFIG_SYS_XWAY_EBU_BOOTCFG)
	/*
	 * Almost all Lantiq XWAY SoC devices have an external bus unit (EBU) to
	 * access external NOR flashes. If the board boots from NOR flash the
	 * internal BootROM does a blind read at address 0xB0000010 to read the
	 * initial configuration for that EBU in order to access the flash
	 * device with correct parameters. This config option is board-specific.
	 */
	.word CONFIG_SYS_XWAY_EBU_BOOTCFG
	.word 0x0
#elif defined(CONFIG_MALTA)
	/*
	 * Linux expects the Board ID here.
	 */
	.word 0x00000420	# 0x420 (Malta Board with CoreLV)
	.word 0x00000000
#endif

	.org 0x200
	/* TLB refill, 32 bit task */
1:	b	1b
	 nop

	.org 0x280
	/* XTLB refill, 64 bit task */
1:	b	1b
	 nop

	.org 0x300
	/* Cache error exception */
1:	b	1b
	 nop

	.org 0x380
	/* General exception */
1:	b	1b
	 nop

	.org 0x400
	/* Catch interrupt exceptions */
1:	b	1b
	 nop

	.org 0x480
	/* EJTAG debug exception */
1:	b	1b
	 nop
#endif
	.align 4
reset:

	/* Clear watch registers */
	mtc0	zero, CP0_WATCHLO
	mtc0	zero, CP0_WATCHHI

	/* WP(Watch Pending), SW0/1 should be cleared */
	mtc0	zero, CP0_CAUSE

	setup_c0_status 0 0

	/* Init Timer */
	mtc0	zero, CP0_COUNT
	mtc0	zero, CP0_COMPARE

#ifndef CONFIG_SKIP_LOWLEVEL_INIT
	/* CONFIG0 register */
	li	t0, CONF_CM_UNCACHED
	mtc0	t0, CP0_CONFIG
#endif

	/* Initialize $gp */
	bal	1f
	 nop
	.word	_gp
1:
	lw	gp, 0(ra)

#ifndef CONFIG_SKIP_LOWLEVEL_INIT
	/* Initialize any external memory */
	la	t9, lowlevel_init
	jalr	t9
	 nop

	/* Initialize caches... */
	la	t9, mips_cache_reset
	jalr	t9
	 nop

    # -------------------------------------------------------------------------#
    # (0)mapping 0xc0000000 via TLB end                                       #
    # -------------------------------------------------------------------------#
    # -------------------------------------------------------------------------#
    #               |___________|                                              #
    # 0x7FFF_FFFC   |           |                                              #
    #               | un-cache  |                                              #
    # 0x4000_0000   |___________|                                              #
    # 0x3FFF_FFFC   |           |                                              #
    #               | cacheable |                                              #
    # 0x0000_0000   |___________|                                              #
    #                virtual            |   physical                           #
    # Idx(0) 0xC000_0000 ~ 0xDFFF_FFFF --> 0xC000_0000 ~ 0xDFFF_FFFF (un-cache)#
    # Idx(1) 0x0000_0000 ~ 0x1FFF_FFFF --> 0x0000_0000 ~ 0x1FFF_FFFF (cachable)#
    # Idx(2) 0x2000_0000 ~ 0x3FFF_FFFF --> 0x2000_0000 ~ 0x3FFF_FFFF (cachable)#
    # Idx(3) 0x4000_0000 ~ 0x5FFF_FFFF --> 0x0000_0000 ~ 0x1FFF_FFFF (un-cache)#
    # Idx(4) 0x6000_0000 ~ 0x7FFF_FFFF --> 0x2000_0000 ~ 0x3FFF_FFFF (un-cache)#
    # -------------------------------------------------------------------------#

    # -------------------------------------------------------------------------#
    # (1). mapping 0x00000000~0x1FFFFFFF -> 0x00000000~0x1FFFFFFF via TLB start#
    # -------------------------------------------------------------------------#
    lui     $15,0x1fff
    ori     $15,$15,0xe000 # $15=0x1fffe000
    mtc0    $15,$5,0 # Write to PageMask[28:13] set page size as 256MB

    # write EntryLo0($2) ,PFN=0x00000,C=3, D=1,V=1,G=1
    lui     $15,0x0000
    ori     $15,$15,0x001f # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$2,0

    # write EntryLo1($3) ,PFN=0x10000,C=3, D=1,V=1,G=1
    lui     $15,0x0040
    ori     $15,$15,0x001f # $15=0x0140001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$3,0

    # write EntryHi ,VPN=0, VPN2X=0, ASID=0;
    lui     $15, 0x0000
    mtc0    $15, $10, 0

    # set index register = 0
    li      t7, 0x1
    mtc0    t7, $0, 0
    ehb
    # write to TLB
    ssnop
    ssnop
    tlbwi # Update TLB register
    nop
    ehb

    # -------------------------------------------------------------------------#
    # (1). mapping 0x00000000~0x1FFFFFFF -> 0x00000000~0x1FFFFFFF via TLB end  #
    # -------------------------------------------------------------------------#
	/* ... and enable them */
	li	t0, CONFIG_SYS_MIPS_CACHE_MODE
	mtc0	t0, CP0_CONFIG
#endif

	/* Set up temporary stack */
#ifdef CONFIG_NA51023
	/* li	t0, CONFIG_SYS_SDRAM_BASE + CONFIG_SYS_INIT_SP_OFFSET
	   replace with:
	   CONFIG_SYS_SDRAM_BASE + (CONFIG_SYS_TEXT_BASE + 0x00200000 - CONFIG_SYS_SDRAM_BASE)
       = CONFIG_SYS_TEXT_BASE + 0x00200000
	*/
	li t0, 0x00200000
	la t1, CONFIG_SYS_TEXT_BASE


	addu t0, t0, t1
	la	sp, 0(t0)
#else
	li	sp, CONFIG_SYS_SDRAM_BASE + CONFIG_SYS_INIT_SP_OFFSET
#endif

	la	t9, board_init_f
	jr	t9
	 nop

/*
 * void relocate_code (addr_sp, gd, addr_moni)
 *
 * This "function" does not return, instead it continues in RAM
 * after relocating the monitor code.
 *
 * a0 = addr_sp
 * a1 = gd
 * a2 = destination address
 */
	.globl	relocate_code
	.ent	relocate_code
relocate_code:
	move	sp, a0			# set new stack pointer

	move	s0, a1			# save gd in s0
	move	s2, a2			# save destination address in s2
#ifdef CONFIG_NA51023
	la	t0, CONFIG_SYS_TEXT_BASE
#else
	li	t0, CONFIG_SYS_MONITOR_BASE
#endif
	sub	s1, s2, t0		# s1 <-- relocation offset

	la	t3, in_ram
	lw	t2, -12(t3)		# t2 <-- __image_copy_end
	move	t1, a2

	add	gp, s1			# adjust gp

	/*
	 * t0 = source address
	 * t1 = target address
	 * t2 = source end address
	 */
1:
	lw	t3, 0(t0)
	sw	t3, 0(t1)
	addu	t0, 4
	blt	t0, t2, 1b
	 addu	t1, 4

	/* If caches were enabled, we would have to flush them here. */
	sub	a1, t1, s2		# a1 <-- size
	la	t9, flush_cache
	jalr	t9
	 move	a0, s2			# a0 <-- destination address

	/* Jump to where we've relocated ourselves */
	#ifdef CONFIG_NA51023
	la      t0, in_ram
	#else
	addi	t0, s2, in_ram - _start
	#endif
	jr	t0
	 nop

	.word	__rel_dyn_end
	.word	__rel_dyn_start
	.word	__image_copy_end
	.word	_GLOBAL_OFFSET_TABLE_
	.word	num_got_entries

in_ram:
	/*
	 * Now we want to update GOT.
	 *
	 * GOT[0] is reserved. GOT[1] is also reserved for the dynamic object
	 * generated by GNU ld. Skip these reserved entries from relocation.
	 */
	lw	t3, -4(t0)		# t3 <-- num_got_entries
	lw	t8, -8(t0)		# t8 <-- _GLOBAL_OFFSET_TABLE_
	add	t8, s1			# t8 now holds relocated _G_O_T_
	addi	t8, t8, 8		# skipping first two entries
	li	t2, 2
1:
	lw	t1, 0(t8)
	beqz	t1, 2f
	 add	t1, s1
	sw	t1, 0(t8)
2:
	addi	t2, 1
	blt	t2, t3, 1b
	 addi	t8, 4

	/* Update dynamic relocations */
	lw	t1, -16(t0)		# t1 <-- __rel_dyn_start
	lw	t2, -20(t0)		# t2 <-- __rel_dyn_end

	b	2f			# skip first reserved entry
	 addi	t1, 8

1:
	lw	t8, -4(t1)		# t8 <-- relocation info

	li	t3, 3
	bne	t8, t3, 2f		# skip non R_MIPS_REL32 entries
	 nop

	lw	t3, -8(t1)		# t3 <-- location to fix up in FLASH

	lw	t8, 0(t3)		# t8 <-- original pointer
	add	t8, s1			# t8 <-- adjusted pointer

	add	t3, s1			# t3 <-- location to fix up in RAM
	sw	t8, 0(t3)

2:
	blt	t1, t2, 1b
	 addi	t1, 8			# each rel.dyn entry is 8 bytes

	/*
	 * Clear BSS
	 *
	 * GOT is now relocated. Thus __bss_start and __bss_end can be
	 * accessed directly via $gp.
	 */
	la	t1, __bss_start		# t1 <-- __bss_start
	la	t2, __bss_end		# t2 <-- __bss_end

1:
	sw	zero, 0(t1)
	blt	t1, t2, 1b
	 addi	t1, 4

	move	a0, s0			# a0 <-- gd
	la	t9, board_init_r
	jr	t9
	 move	a1, s2

	.end	relocate_code