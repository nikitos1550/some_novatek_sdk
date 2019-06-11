#ifndef CYGONCE_ISO_SYS_TYPES_H
#define CYGONCE_ISO_SYS_TYPES_H
/*========================================================================
//
//      sys/types.h
//
//      POSIX types
//
//========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2002 Nick Garnett
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
//========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     jlarmour
// Contributors:
// Date:          2000-04-14
// Purpose:       This file provides various types required by POSIX 1003.1.
// Description:   The real contents of this file get set from the
//                configuration (set by the implementation)
// Usage:         #include <sys/types.h>
//
//####DESCRIPTIONEND####
//
//======================================================================
*/

/* CONFIGURATION */

#include <pkgconf/system.h>
#include <pkgconf/isoinfra.h>          /* Configuration header */

/* INCLUDES */

#include "cyg/infra/cyg_type.h"

/* This is the "standard" way to get size_t from stddef.h,
 * which is the canonical location of the definition.
 */
#define __need_size_t
#include <stddef.h>

#ifdef CYGBLD_ISO_SSIZET_HEADER
# include CYGBLD_ISO_SSIZET_HEADER
#else
typedef long ssize_t;
#endif

#ifdef CYGBLD_ISO_FSTYPES_HEADER
# include CYGBLD_ISO_FSTYPES_HEADER
#else
typedef short dev_t;
typedef unsigned int ino_t;
typedef unsigned int mode_t;
typedef unsigned short nlink_t;
typedef long off_t;
typedef long long off64_t;
typedef unsigned int blksize_t;
typedef long blkcnt_t;
typedef unsigned long fsblkcnt_t;
typedef unsigned long fsfilcnt_t;
#endif

#define uint8_t  cyg_uint8
#define uint16_t cyg_uint16
#define uint32_t cyg_uint32
#define uint64_t cyg_uint64

#define int8_t  cyg_int8
#define int16_t cyg_int16
#define int32_t cyg_int32
#define int64_t cyg_int64

#if 1	//for netlink.h
typedef uint32_t __u32;
typedef int32_t __s32;
typedef uint16_t __u16;
typedef int16_t __s16;
typedef uint8_t __u8;
#endif

#if 1	//for usleep() in unistd.h
typedef unsigned long useconds_t;
typedef long suseconds_t;
#endif

#ifdef CYGBLD_ISO_SCHEDTYPES_HEADER
# include CYGBLD_ISO_SCHEDTYPES_HEADER
#else
typedef unsigned int gid_t;
typedef unsigned int uid_t;
typedef int pid_t;
#endif

#if CYGINT_ISO_PMUTEXTYPES
# include CYGBLD_ISO_PMUTEXTYPES_HEADER
#endif

#if CYGINT_ISO_PTHREADTYPES
# include CYGBLD_ISO_PTHREADTYPES_HEADER
#endif

/* Include <sys/select.h> for backward compatibility for now */
#define __NEED_FD_SETS_ONLY
#include <sys/select.h>
#undef __NEED_FD_SETS_ONLY

#if !defined(_POSIX_SOURCE)
# if CYGINT_ISO_BSDTYPES
#  ifdef CYGBLD_ISO_BSDTYPES_HEADER
#   include CYGBLD_ISO_BSDTYPES_HEADER
#  endif
# endif
#endif // !defined(_POSIX_SOURCE)

#endif /* CYGONCE_ISO_SYS_TYPES_H multiple inclusion protection */

/* EOF sys/types.h */
