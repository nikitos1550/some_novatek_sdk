#ifndef CYGONCE_PLF_CLOCK_H
#define CYGONCE_PLF_CLOCK_H

//=============================================================================
//
//      plf_clock.h
//
//      HAL clock support
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
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
// Author(s):   nickg, gthomas, hmt
// Contributors:        nickg, gthomas, hmt
// Date:        1999-01-28
// Purpose:     Define clock support
// Description: The macros defined here provide the HAL APIs for handling
//              the clock.
//
// Usage:
//              #include <cyg/hal/hal_intr.h> // which includes this file
//              ...
//
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <cyg/hal/hal_io.h>
#include <cyg/hal/plf_io.h>

#ifndef __ASSEMBLER__

#ifndef CYGHWR_HAL_CLOCK_CONTROL_DEFINED

typedef int (*hal_clock_reset_callback_t)(cyg_uint32, cyg_uint32); /* 0 for OK, non-null for error */

externC CYG_WORD32 cyg_hal_clock_period;
#define CYGHWR_HAL_CLOCK_PERIOD_DEFINED

__externC void hal_plf_clock_initialize( cyg_uint32 period );
#define HAL_CLOCK_INITIALIZE( _period_ ) hal_plf_clock_initialize(_period_)

__externC cyg_uint32 hal_plf_clock_read( void );
#define HAL_CLOCK_READ( _pvalue_ ) *(_pvalue_) = hal_plf_clock_read()

__externC void hal_plf_clock_write( cyg_uint32 clock );
#define HAL_CLOCK_WRITE( _pvalue_ ) hal_plf_clock_write(_pvalue_)

__externC void hal_plf_clock_installResetCB(hal_clock_reset_callback_t resetCB);

__externC void hal_plf_clock_reset( cyg_uint32 vector, cyg_uint32 period );
#define HAL_CLOCK_RESET( _vector_, _period_ ) hal_plf_clock_reset( _vector_, _period_ )

#define HAL_CLOCK_LATENCY( _pvalue_ )   HAL_CLOCK_READ( _pvalue_)

__externC void hal_plf_delay_us( cyg_uint32 delay );
#define HAL_DELAY_US(_delay_) hal_plf_delay_us(_delay_)

#define CYGHWR_HAL_CLOCK_CONTROL_DEFINED

#endif /* CYGHWR_HAL_CLOCK_CONTROL_DEFINED */

#endif /* __ASSEMBLER__ */

//-----------------------------------------------------------------------------
#endif // ifndef CYGONCE_PLF_CLOCK_H
// End of plf_clock.h
