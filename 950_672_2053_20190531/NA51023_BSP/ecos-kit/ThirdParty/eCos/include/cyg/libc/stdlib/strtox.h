#ifndef CYGONCE_LIBC_STDLIB_STRTOX_H
#define CYGONCE_LIBC_STDLIB_STRTOX_H
/*===========================================================================
//
//      strtox.h
//
//      Header definitions for the strtof(), strtod(), atof() standard
//      utility functions.
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


/* FUNCTION PROTOTYPES */

/* ISO C 7.10.1 - String conversion functions */

#ifdef __cplusplus
extern "C" {
#endif

extern double
atof( const char * /* double_str */ );

extern double
strtod( const char * /* double_str */, char ** /* endptr */ );

/* C99 */
extern float
strtof( const char * /* nptr */, char ** /* endptr */ );

#ifdef __cplusplus
} /* extern "C" */
#endif 

#include <cyg/libc/stdlib/strtof.inl>

#endif /* CYGONCE_LIBC_STDLIB_STRTOX_H multiple inclusion protection */

/* EOF strtox.h */
