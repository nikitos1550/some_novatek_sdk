# ifndef _CHIPSET_H
# define _CHIPSET_H

/* Copyright (c) 2005  Novatek Microelectronics Corporation

COMMENT Module Name:
COMMENT     Chipset.h

COMMENT Abstract:
COMMENT     Definitions for CPU Mode, Interrupt and Stack
COMMENT     This is a shared header file for ASM and C
COMMENT     For more information, please refer to http://www.arm.com/support/faqdev/1208.html
COMMENT     Only support "# define" now

COMMENT Notes:
COMMENT     Copyright (c) 2005 Novatek Microelectronics Corporation.  All Rights Reserved.

COMMENT Revision History:
COMMENT     03/11/05: Created by Chris Hsu */

/* CPU */

#if (_ARCH_ == _ARCH_ARM_)
# define Mode_USR               0x10        /* M[4:0] = 01010 */
# define Mode_FIQ               0x11        /* M[4:0] = 10001 */
# define Mode_IRQ               0x12        /* M[4:0] = 10010 */
# define Mode_SVC               0x13        /* M[4:0] = 10011 */
# define Mode_ABT               0x17        /* M[4:0] = 10111 */
# define Mode_UNDEF             0x1B        /* M[4:0] = 11011 */
# define Mode_SYS               0x1F        /* M[4:0] = 11111 */
# define Mode_MSK               0x1F        /* M[4:0] = 11111 */
# define I_Bit                  0x80        /* 7  6  5  4  3  2  1  0 */
# define F_Bit                  0x40        /* I  F  T M4 M3 M2 M1 M0 */

# define INT_IRQ_STATUS_ADDR    0x98800028
# define INT_FIQ_STATUS_ADDR    0x98800024
# define INT_ENABLE_ADDR        0x98800000

#endif

# endif
