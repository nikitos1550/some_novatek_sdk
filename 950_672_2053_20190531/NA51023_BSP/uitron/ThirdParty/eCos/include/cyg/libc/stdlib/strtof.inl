#ifndef CYGONCE_LIBC_STDLIB_STRTOF_INL
#define CYGONCE_LIBC_STDLIB_STRTOF_INL
/*===========================================================================
//
//      strtof.inl
//
//      Inline implementation for the ISO C99 standard utility function
//      strtof()
//
//===========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2005 eCosCentric Ltd.
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
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//===========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    jlarmour
// Contributors: 
// Date:         2005-02-02
// Purpose:     
// Description: 
// Usage:        Do not include this file directly - include <stdlib.h> instead
//
//####DESCRIPTIONEND####
//
//=========================================================================*/

/* CONFIGURATION */

#include <pkgconf/isoinfra.h>

/* INCLUDES */

#include <cyg/infra/cyg_trac.h>    /* Tracing support */
#include <cyg/libc/stdlib/strtox.h>
#include <errno.h>
#include <math.h>

/* INLINE FUNCTIONS */

#ifndef CYGPRI_LIBC_STDLIB_STRTOF_INLINE
# define CYGPRI_LIBC_STDLIB_STRTOF_INLINE extern __inline__
#endif

CYGPRI_LIBC_STDLIB_STRTOF_INLINE float
strtof( const char * __nptr, char **__endptr )
{
    float __retval;
    double __tmpret, __abstmpret;

    CYG_REPORT_FUNCNAMETYPE( "strtof", "returning 0x%08x" );

    __tmpret = strtod( __nptr, __endptr );

    __abstmpret = fabs(__tmpret);

    /* We need to allow for the value being able to fit into a double
       but not a float */
    if ( __abstmpret > (double)FLT_MAX )
    {
        /* overflow */
        __retval = HUGE_VALF;
        errno = ERANGE;
    }
    else if ( (__abstmpret != 0.0) && (__abstmpret < (double)FLT_MIN) )
    {
        /* underflow */
        __retval = 0.0;
        errno = ERANGE;
    }
    else
    {
        __retval = (float) __tmpret;
    }

    CYG_REPORT_RETVAL( __retval );

    return __retval;
} /* strtof() */

#endif /* CYGONCE_LIBC_STDLIB_STRTOF_INL multiple inclusion protection */

/* EOF strtof.inl */
