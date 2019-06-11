/*
 * MIPS register definitions, originally from:
 *
 * include/asm-mips/regdefs.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License, Version 2.  See the file "COPYING" in the main directory of this
 * archive for more details.
 *
 * Copyright (C) 1994, 1995 by Ralf Baechle
 */

/*
 * Symbolic register names for 32 bit ABI
 */
.equ zero,  $0      /* wired zero */
.equ AT,    $1      /* assembler temp  - uppercase because of ".set at" */
.equ v0,    $2      /* return value */
.equ v1,    $3
.equ a0,    $4      /* argument registers */
.equ a1,    $5
.equ a2,    $6
.equ a3,    $7
.equ t0,    $8      /* caller saved */
.equ t1,    $9
.equ t2,    $10
.equ t3,    $11
.equ t4,    $12
.equ t5,    $13
.equ t6,    $14
.equ t7,    $15
.equ s0,    $16     /* callee saved */
.equ s1,    $17
.equ s2,    $18
.equ s3,    $19
.equ s4,    $20
.equ s5,    $21
.equ s6,    $22
.equ s7,    $23
.equ t8,    $24     /* caller saved */
.equ t9,    $25
.equ jp,    $25     /* PIC jump register */
.equ k0,    $26     /* kernel scratch */
.equ k1,    $27
.equ gp,    $28     /* global pointer */
.equ sp,    $29     /* stack pointer */
.equ fp,    $30     /* frame pointer */
.equ ra,    $31     /* return address */

.equ C0_Status,     $12
.equ C0_Cause,      $13
.equ C0_EPC,        $14
.equ C0_Config,     $16

.equ S_ConfigK0,    0                       /* Kseg0 coherency algorithm (R/W) */
.equ M_ConfigK0, (0x7 << S_ConfigK0)

.equ S_StatusEXL,   1                       /* Denotes exception level (R/W) */
.equ M_StatusEXL, (0x1 << S_StatusEXL)

.equ S_StatusERL,   2                       /* Denotes error level (R/W) */
.equ M_StatusERL, (0x1 << S_StatusERL)


.equ S_StatusKSU,   3                       /* Two-bit current mode (R/W) */
.equ M_StatusKSU, (0x3 << S_StatusKSU)

.equ S_StatusIE,    0                       /* Enables interrupts (R/W) */
.equ M_StatusIE, (0x1 << S_StatusIE)

.equ S_StatusBEV,   22                      /* Enable Boot Exception Vectors (R/W) */
.equ M_StatusBEV, (0x1 << S_StatusBEV)


/*
 * Cache attribute values in the C field of EntryLo and the
 * K0 field of Config
 */
.equ K_CacheAttrCWTnWA,       0                       /* Cacheable, write-thru, no write allocate */
.equ K_CacheAttrCWTWA,        1                       /* Cacheable, write-thru, write allocate */
.equ K_CacheAttrU,            2                       /* Uncached */
.equ K_CacheAttrC,            3                       /* Cacheable */
.equ K_CacheAttrCN,           3                       /* Cacheable, non-coherent */
.equ K_CacheAttrCCE,          4                       /* Cacheable, coherent, exclusive */
.equ K_CacheAttrCCS,          5                       /* Cacheable, coherent, shared */
.equ K_CacheAttrCCU,          6                       /* Cacheable, coherent, update */
.equ K_CacheAttrUA,           7                       /* Uncached accelerated */

.equ S_EntryLo0PFN,           S_EntryLoPFN
.equ M_EntryLo0PFN,           M_EntryLoPFN


.equ R_C0_SRSCtl,             12
.equ R_C0_SelSRSCtl,          2

.equ S_SRSCtlHSS,             26                      /* Highest shadow set (R) */
.equ W_SRSCtlHSS,             4
.equ M_SRSCtlHSS, (0xf << S_SRSCtlHSS)
.equ S_SRSCtlEICSS,           18                      /* Exception shadow set (R/W) */
.equ W_SRSCtlEICSS,           4
.equ M_SRSCtlEICSS, (0xf << S_SRSCtlEICSS)
.equ S_SRSCtlESS,             12                      /* Exception shadow set (R/W) */
.equ W_SRSCtlESS,             4
.equ M_SRSCtlESS, (0xf << S_SRSCtlESS)
.equ S_SRSCtlPSS,             6                       /* Previous shadow set (R/W) */
.equ W_SRSCtlPSS,             4
.equ M_SRSCtlPSS, (0xf << S_SRSCtlPSS)
.equ S_SRSCtlCSS,             0
.equ W_SRSCtlCSS,             4
.equ M_SRSCtlCSS, (0xf << S_SRSCtlCSS)                /* Current Shadow set (R/W) */

.equ M_SRSCtl0Fields,         0xc3c30c30
.equ M_SRSCtlRFields,         0x3c3c000f

.equ INT_ENABLE_ADDR,         0xB0080000
.equ INT_IRQ_STATUS_ADDR,     0xB0080028

.equ KUSEG_MSK,                 0x80000000
.equ KSEG_MSK,                  0xE0000000
.equ KUSEGBASE,                 0x00000000
.equ KSEG0BASE,                 0x80000000
.equ KSEG1BASE,                 0xA0000000
.equ KSSEGBASE,                 0xC0000000
.equ KSEG3BASE,                 0xE0000000

