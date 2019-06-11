#ifndef CYGONCE_PLF_IO_H
#define CYGONCE_PLF_IO_H

//=============================================================================
//
//      plf_io.h
//
//      Platform specific IO support
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2006 eCosCentric Ltd.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):
// Contributors:
// Date:
// Purpose:      HAL IO support
// Description:
// Usage:        #include <cyg/hal/plf_io.h>
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <pkgconf/hal.h>
#include <cyg/hal/hal_misc.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/plf_intr.h>

#ifdef __ASSEMBLER__
#define HAL_REG(x)                      x
#define HAL_REG8(x)                     x
#define HAL_REG16(x)                    x
#define HAL_REG32(x)                    x
#else
#define HAL_REG(x)                      (volatile CYG_WORD *)(x)
#define HAL_REG8(x)                     (volatile CYG_BYTE *)(x)
#define HAL_REG16(x)                    (volatile CYG_WORD16 *)(x)
#define HAL_REG32(x)                    (volatile CYG_WORD32 *)(x)
#endif

/**
 * NT96660 Uart module
 */
#if 0
#define HAL_UART_BASE                   0xB0290000

#define HAL_UART_THR                    (HAL_UART_BASE + 0x00)
#define HAL_UART_RBR                    (HAL_UART_BASE + 0x00)
#define HAL_UART_DLABLO                 (HAL_UART_BASE + 0x00)
#define HAL_UART_DLABHI                 (HAL_UART_BASE + 0x04)
#define HAL_UART_IER                    (HAL_UART_BASE + 0x04)
#define HAL_UART_IIR                    (HAL_UART_BASE + 0x08)
#define HAL_UART_IFCR                   (HAL_UART_BASE + 0x08)
#define HAL_UART_LCR                    (HAL_UART_BASE + 0x0C)
#define HAL_UART_LSR                    (HAL_UART_BASE + 0x14)
#define HAL_UART_SCRATCH                (HAL_UART_BASE + 0x1C)

#define HAL_UART_THR_OFFSET             0x00
#define HAL_UART_RBR_OFFSET             0x00
#define HAL_UART_DLABLO_OFFSET          0x00
#define HAL_UART_DLABHI_OFFSET          0x04
#define HAL_UART_IER_OFFSET             0x04
#define HAL_UART_IIR_OFFSET             0x08
#define HAL_UART_IFCR_OFFSET            0x08
#define HAL_UART_LCR_OFFSET             0x0C
#define HAL_UART_LSR_OFFSET             0x14
#define HAL_UART_SCRATCH_OFFSET         0x1C

#define HAL_UART_FCR_RXTRIGGER_LV       0x000000c0UL

#define FCR_RX_SHIFBIT                  6
#define FCR_RXTRIGGER_LV1               (0x00 << FCR_RX_SHIFBIT)
#define FCR_RXTRIGGER_LV4               (0x01 << FCR_RX_SHIFBIT)
#define FCR_RXTRIGGER_LV8               (0x02 << FCR_RX_SHIFBIT)
#define FCR_RXTRIGGER_LV14              (0x03 << FCR_RX_SHIFBIT)

// bit mapping of LCR register
#define HAL_UART_LCR_CHARLENGTH         0x00000003UL
#define HAL_UART_LCR_STOPBIT            0x00000004UL
#define HAL_UART_LCR_PARITY_ENABLE      0x00000008UL
#define HAL_UART_LCR_PARITY_EVEN        0x00000010UL
#define HAL_UART_LCR_PARITY_STICK       0x00000020UL
#define HAL_UART_LCR_MODE_LOOPBACK      0x00000040UL
#define HAL_UART_LCR_DLAB               0x00000080UL

#define LCR_CHAR_LEN5                   0x00000000UL
#define LCR_CHAR_LEN6                   0x00000001UL
#define LCR_CHAR_LEN7                   0x00000002UL
#define LCR_CHAR_LEN8                   0x00000003UL

#define LCR_STOP1                       0x00000000UL
#define LCR_STOP2                       0x00000004UL

#define LCR_PARITY_DISABLE              0x00000000UL
#define LCR_PARITY_ENABLE               0x00000008UL

#define LCR_PARITY_STICK_DISABLE        0x00000000UL
#define LCR_PARITY_STICK_ENABLE         0x00000004UL

#define LCR_EVEN_PARITY                 0x00000010
#define LCR_STICK_PARITY                0x00000020

// bit mapping of LSR register
#define HAL_UART_LSR_DATAREADY          0x00000001UL
#define HAL_UART_LSR_ERR_OVERRUN        0x00000002UL
#define HAL_UART_LSR_ERR_PARITY         0x00000004UL
#define HAL_UART_LSR_ERR_FRAMING        0x00000008UL
#define HAL_UART_LSR_THREMPTY           0x00000020UL
#define HAL_UART_LSR_TXEMPTY            0x00000040UL
#define HAL_UART_LSR_ERR_RXFIFO         0x00000080UL
#endif

/**
 * NT96660 Timer module
 */

#if 0
#define HAL_TIMER_BASE                  0xB0040000

#define HAL_TIMER1_INTEN                (HAL_TIMER_BASE + 0x00000000)
#define HAL_TIMER1_INTST                (HAL_TIMER_BASE + 0x00000004)
#define HAL_TIMER1_DIV                  (HAL_TIMER_BASE + 0x00000008)

#define HAL_TIMER1_CNT                  (HAL_TIMER_BASE + 0x00000018)
#define HAL_TIMER1_CTRL                 (HAL_TIMER_BASE + 0x00000010)
#define HAL_TIMER1_TVAL                             (HAL_TIMER_BASE + 0x00000014)
#define HAL_TIMER1_PAUSE                                (HAL_TIMER_BASE + 0x0000001C)   //???not exist in NT96660

#define HAL_TIMER2_CNT                  (HAL_TIMER_BASE + 0x00000028)
#define HAL_TIMER2_CTRL                 (HAL_TIMER_BASE + 0x00000020)
#define HAL_TIMER2_TVAL                             (HAL_TIMER_BASE + 0x00000024)
#define HAL_TIMER2_PAUSE                (HAL_TIMER_BASE + 0x0000002C)   //???not exist in NT96660

#define HAL_TIMER3_CNT                  (HAL_TIMER_BASE + 0x00000038)
#define HAL_TIMER3_CTRL                 (HAL_TIMER_BASE + 0x00000030)
#define HAL_TIMER3_TVAL                             (HAL_TIMER_BASE + 0x00000034)
#define HAL_TIMER3_PAUSE                (HAL_TIMER_BASE + 0x0000003C)   //???not exist in NT96660
#define HAL_TIMER3_UPDATE               (HAL_TIMER_BASE + 0x0000003C)   //???not exist in NT96660

#define HAL_TIMER4_CNT                  (HAL_TIMER_BASE + 0x00000048)
#define HAL_TIMER4_CTRL                 (HAL_TIMER_BASE + 0x00000040)
#define HAL_TIMER4_TVAL                             (HAL_TIMER_BASE + 0x00000044)
#define HAL_TIMER4_PAUSE                (HAL_TIMER_BASE + 0x0000004C)   //???not exist in NT96660
#define HAL_TIMER4_UPDATE               (HAL_TIMER_BASE + 0x0000004C)   //???not exist in NT96660

#define HAL_TIMER5_CNT                  (HAL_TIMER_BASE + 0x00000058)
#define HAL_TIMER5_CTRL                 (HAL_TIMER_BASE + 0x00000050)
#define HAL_TIMER5_TVAL                             (HAL_TIMER_BASE + 0x00000054)
#define HAL_TIMER5_PAUSE                (HAL_TIMER_BASE + 0x0000005C)   //???not exist in NT96660
#define HAL_TIMER5_UPDATE               (HAL_TIMER_BASE + 0x0000005C)   //???not exist in NT96660

// bit mapping of TIMER divider register
#define HAL_TIMER_DIVISOR               0x0000000FUL

// bit mapping of TIMER control register
#define HAL_TIMER_FREEMODE              0x00000002UL
#define HAL_TIMER_ENABLE                0x00000001UL
#define HAL_TIMER_SRC_CLK               0x00000004UL

// bit mapping of TIMER pause register //???not exist in NT96660
#define HAL_TIMER_PAUSE                 0x00000000UL    //??? 0x00000001UL

#endif

/**
 * NT96660 Interrupt Controller module
 */

#define HAL_INTR_BASE                   0xC0080000

#define NVT_INT_USE_CPU2 0 //set 1 indicate to use CPU2, otherwise to use CPU1

#if (NVT_INT_USE_CPU2)
#define HAL_INTR_EN                     (HAL_INTR_BASE + 0x00000070)
#define HAL_INTR_STS                    (HAL_INTR_BASE + 0x00000040)
#define HAL_INTR_ID                     (HAL_INTR_BASE + 0x00000104)
#else
#define HAL_INTR_EN                     (HAL_INTR_BASE + 0x00000060)
#define HAL_INTR_STS                    (HAL_INTR_BASE + 0x00000030)
#define HAL_INTR_ID                     (HAL_INTR_BASE + 0x00000100)
#endif


//-----------------------------------------------------------------------------

#endif // CYGONCE_PLF_IO_H
// end of plf_io.h
