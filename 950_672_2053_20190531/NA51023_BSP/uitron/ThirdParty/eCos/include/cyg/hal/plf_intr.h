#ifndef CYGONCE_HAL_PLF_INTR_H
#define CYGONCE_HAL_PLF_INTR_H

//==========================================================================
//
//      plf_intr.h
//
//      HAL interrupt and clock support
//
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2006 eCosCentric Limited
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
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    nickg
// Contributors: nickg, jskov,
//               gthomas, jlarmour, dmoseley
// Date:         2001-03-20
// Purpose:      Define Interrupt support
// Description:  The macros defined here provide the HAL APIs for handling
//               interrupts and the real-time clock.
//
// Usage:
//              #include <cyg/hal/plf_intr.h>
//              ...
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>

// First an assembly safe part
#ifndef __ASSEMBLER__

//--------------------------------------------------------------------------
// Interrupt vectors.

#ifndef CYGHWR_HAL_INTERRUPT_VECTORS_DEFINED

// These are decoded via the interrupt controller ID
// register when an external interrupt is delivered.

#define CYGNUM_HAL_INTR_NULL                    0
#define CYGNUM_HAL_INTR_TIMER                   1
#define CYGNUM_HAL_INTR_SIE                     2
#define CYGNUM_HAL_INTR_SIE2                    3
#define CYGNUM_HAL_INTR_SIE3                    4
#define CYGNUM_HAL_INTR_SIE4                    5
#define CYGNUM_HAL_INTR_IPE                     6
#define CYGNUM_HAL_INTR_IME                     7
#define CYGNUM_HAL_INTR_DCE                     8
#define CYGNUM_HAL_INTR_IFE                     9
#define CYGNUM_HAL_INTR_IFE2                    10
#define CYGNUM_HAL_INTR_DIS                     11
#define CYGNUM_HAL_INTR_FDE                     12
#define CYGNUM_HAL_INTR_RDE                     13
#define CYGNUM_HAL_INTR_RHE                     14
#define CYGNUM_HAL_INTR_DRE                     15
#define CYGNUM_HAL_INTR_DAI                     16
#define CYGNUM_HAL_INTR_H264                    17
#define CYGNUM_HAL_INTR_JPEG                    18
#define CYGNUM_HAL_INTR_GRAPHIC                 19
#define CYGNUM_HAL_INTR_GRAPHIC2                20
#define CYGNUM_HAL_INTR_AFFINE                  21
#define CYGNUM_HAL_INTR_ISE                     22
#define CYGNUM_HAL_INTR_TGE                     23
#define CYGNUM_HAL_INTR_TSMUX                   24
#define CYGNUM_HAL_INTR_GPIO                    25
#define CYGNUM_HAL_INTR_REMOTE                  26
#define CYGNUM_HAL_INTR_PWM                     27
#define CYGNUM_HAL_INTR_USB                     28
#define CYGNUM_HAL_INTR_USB2                    29
#define CYGNUM_HAL_INTR_NAND                    30
#define CYGNUM_HAL_INTR_SDIO                    31
#define CYGNUM_HAL_INTR_SDIO2                   32
#define CYGNUM_HAL_INTR_SDIO3                   33
#define CYGNUM_HAL_INTR_DRAM                    34
#define CYGNUM_HAL_INTR_ETHERNET                35
#define CYGNUM_HAL_INTR_SPI                     36
#define CYGNUM_HAL_INTR_SPI2                    37
#define CYGNUM_HAL_INTR_SPI3                    38
#define CYGNUM_HAL_INTR_SPI4                    39
#define CYGNUM_HAL_INTR_SPI5                    40
#define CYGNUM_HAL_INTR_SIF                     41
#define CYGNUM_HAL_INTR_I2C                     42
#define CYGNUM_HAL_INTR_I2C2                    43
#define CYGNUM_HAL_INTR_UART                    44
#define CYGNUM_HAL_INTR_UART2                   45
#define CYGNUM_HAL_INTR_UART3                   46
#define CYGNUM_HAL_INTR_UART4                   47
#define CYGNUM_HAL_INTR_ADC                     48
#define CYGNUM_HAL_INTR_IDE                     49
#define CYGNUM_HAL_INTR_IDE2                    50
#define CYGNUM_HAL_INTR_MIPIDSI                 51
#define CYGNUM_HAL_INTR_MI                      52
#define CYGNUM_HAL_INTR_HDMI                    53
#define CYGNUM_HAL_INTR_VX1                     54
#define CYGNUM_HAL_INTR_LVDS_HISPI_MIPICSI      55
#define CYGNUM_HAL_INTR_LVDS_HISPI_MIPICSI2     56
#define CYGNUM_HAL_INTR_RTC                     57
#define CYGNUM_HAL_INTR_WDT                     58
#define CYGNUM_HAL_INTR_CG                      59
#define CYGNUM_HAL_INTR_CC                      60
#define CYGNUM_HAL_INTR_I2C3                    61


#define CYGNUM_HAL_INTR_SW0         62  //translate and append SW0 intr to end of HW0
#define CYGNUM_HAL_INTR_SW1         63  //translate and append SW1 intr to end of HW0
#define CYGNUM_HAL_INTR_HW1         64  //translate and append HW1 intr to end of HW0
#define CYGNUM_HAL_INTR_HW2         65  //translate and append HW2 intr to end of HW0
#define CYGNUM_HAL_INTR_HW3         66  //translate and append HW3 intr to end of HW0
#define CYGNUM_HAL_INTR_HW4         67  //translate and append HW4 intr to end of HW0
#define CYGNUM_HAL_INTR_HW5         68  //translate and append HW5 intr to end of HW0
#define CYGNUM_HAL_INTR_CPUCNT      CYGNUM_HAL_INTR_HW5


// Min/Max ISR numbers (Interrupt Controller)
#define HAL_INTR_MIN                    1
#define HAL_INTR_MAX                    61

// Virtual table
#define CYGNUM_HAL_ISR_MIN              0
#define CYGNUM_HAL_ISR_MAX              CYGNUM_HAL_INTR_CPUCNT
#define CYGNUM_HAL_ISR_COUNT            (CYGNUM_HAL_ISR_MAX - CYGNUM_HAL_ISR_MIN + 1)

#ifndef CYGNUM_HAL_INTERRUPT_RTC
#define CYGNUM_HAL_INTERRUPT_RTC        CYGNUM_HAL_INTR_CPUCNT
#endif

//#define CYGNUM_HAL_CPU2_INTERRUPT_TIMER     64

#define CYGHWR_HAL_INTERRUPT_VECTORS_DEFINED

#endif /* CYGHWR_HAL_INTERRUPT_VECTORS_DEFINED */

#include <cyg/infra/cyg_type.h>
#include <cyg/hal/plf_io.h>

//--------------------------------------------------------------------------
// Interrupt controller access.

#ifndef CYGHWR_HAL_INTERRUPT_CONTROLLER_ACCESS_DEFINED

// Array which stores the configured priority levels for the configured
// interrupts.
externC volatile CYG_BYTE hal_interrupt_level[CYGNUM_HAL_ISR_COUNT];

externC void hal_plf_interrupt_mask( cyg_uint32 vector );
#define HAL_INTERRUPT_MASK( _vector_ ) hal_plf_interrupt_mask( _vector_ )

externC void hal_plf_interrupt_unmask( cyg_uint32 vector );
#define HAL_INTERRUPT_UNMASK( _vector_ ) hal_plf_interrupt_unmask( _vector_ )

externC void hal_plf_interrupt_set_level( cyg_uint32 vector, cyg_uint32 prilevel );
#define HAL_INTERRUPT_SET_LEVEL( _vector_, _level_ ) hal_plf_interrupt_set_level( _vector_, _level_ )

externC void hal_plf_interrupt_acknowledge( cyg_uint32 vector );
#define HAL_INTERRUPT_ACKNOWLEDGE( _vector_ ) hal_plf_interrupt_acknowledge( _vector_ )

externC void hal_plf_interrupt_configure( cyg_uint32 vector, cyg_uint32 leveltriggered, cyg_uint32 up );
#define HAL_INTERRUPT_CONFIGURE( _vector_, _level_, _up_ ) hal_plf_interrupt_configure( _vector_, _level_, _up_ )

#define HAL_INTERRUPT_TRIGGER

#define CYGHWR_HAL_INTERRUPT_CONTROLLER_ACCESS_DEFINED

#endif /* CYGHWR_HAL_INTERRUPT_CONTROLLER_ACCESS_DEFINED */

//--------------------------------------------------------------------------
// Control-C support.

#if defined(CYGDBG_HAL_MIPS_DEBUG_GDB_CTRLC_SUPPORT)

# define CYGHWR_HAL_GDB_PORT_VECTOR CYGNUM_HAL_INTERRUPT_SER

externC cyg_uint32 hal_ctrlc_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data);

# define HAL_CTRLC_ISR hal_ctrlc_isr

#endif

//--------------------------------------------------------------------------
// Reset.
#ifndef CYGHWR_HAL_RESET_DEFINED
externC void hal_plf_reset( void );
#define CYGHWR_HAL_RESET_DEFINED
#define HAL_PLATFORM_RESET()             hal_plf_reset()

//#define HAL_PLATFORM_RESET_ENTRY 0xbfc00000
//#define HAL_PLATFORM_RESET_ENTRY 0x80000400 //???

#endif // CYGHWR_HAL_RESET_DEFINED

#endif // __ASSEMBLER__

//--------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_PLF_INTR_H
// End of plf_intr.h
