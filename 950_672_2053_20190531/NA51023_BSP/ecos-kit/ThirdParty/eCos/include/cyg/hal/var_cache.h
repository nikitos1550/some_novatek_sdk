#ifndef CYGONCE_IMP_CACHE_H
#define CYGONCE_IMP_CACHE_H

//=============================================================================
//
//      var_cache.h
//
//      HAL cache control API
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2005, 2006 eCosCentric Limited
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
// Purpose:     Cache control API
// Description: The macros defined here provide the HAL APIs for handling
//              cache control operations.
// Usage:
//              #include <cyg/hal/var_cache.h>
//              ...
//
//
//####DESCRIPTIONEND####
//
//=============================================================================

#include <pkgconf/hal.h>
#include <cyg/infra/cyg_type.h>

#include <cyg/hal/mips-regs.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/plf_cache.h>
#include <cyg/hal/var_arch.h>


//-----------------------------------------------------------------------------
// Cache dimensions

#if defined(CYGHWR_HAL_MIPS_NT96660_CACHE_8K8K)

// Data cache
#define HAL_DCACHE_SIZE                 8192    // Size of cache in bytes
#define HAL_DCACHE_LINE_SIZE            32      // Size of a cache line
#define HAL_DCACHE_WAYS                 4       // Associativity of the cache

// Instruction cache
#define HAL_ICACHE_SIZE                 8192    // Size of cache in bytes
#define HAL_ICACHE_LINE_SIZE            32      // Size of a cache line
#define HAL_ICACHE_WAYS                 4       // Associativity of the cache

#define HAL_DCACHE_SETS (HAL_DCACHE_SIZE/(HAL_DCACHE_LINE_SIZE*HAL_DCACHE_WAYS))
#define HAL_ICACHE_SETS (HAL_ICACHE_SIZE/(HAL_ICACHE_LINE_SIZE*HAL_ICACHE_WAYS))

#define HAL_DCACHE_WRITETHRU_MODE       0
#define HAL_DCACHE_WRITEBACK_MODE       1

// This CPU supports the Index (Writeback) Invalidate operation on the
// cache instruction. This is not the case for all MIPS32 variants.
//#define HAL_DCACHE_SUPPORTS_INDEX_INVALIDATE
//#define HAL_ICACHE_SUPPORTS_INDEX_INVALIDATE

#elif defined(CYGHWR_HAL_MIPS_NT96660_CACHE_16K16K)

// Data cache
#define HAL_DCACHE_SIZE                 16384   // Size of cache in bytes
#define HAL_DCACHE_LINE_SIZE            32      // Size of a cache line
#define HAL_DCACHE_WAYS                 4       // Associativity of the cache

// Instruction cache
#define HAL_ICACHE_SIZE                 16384   // Size of cache in bytes
#define HAL_ICACHE_LINE_SIZE            32      // Size of a cache line
#define HAL_ICACHE_WAYS                 4       // Associativity of the cache

#define HAL_DCACHE_SETS (HAL_DCACHE_SIZE/(HAL_DCACHE_LINE_SIZE*HAL_DCACHE_WAYS))
#define HAL_ICACHE_SETS (HAL_ICACHE_SIZE/(HAL_ICACHE_LINE_SIZE*HAL_ICACHE_WAYS))

#define HAL_DCACHE_WRITETHRU_MODE       0
#define HAL_DCACHE_WRITEBACK_MODE       1

// This CPU supports the Index (Writeback) Invalidate operation on the
// cache instruction. This is not the case for all MIPS32 variants.
//#define HAL_DCACHE_SUPPORTS_INDEX_INVALIDATE
//#define HAL_ICACHE_SUPPORTS_INDEX_INVALIDATE

#elif defined(CYGHWR_HAL_MIPS_NT96660_CACHE_32K32K)

// Data cache
#define HAL_DCACHE_SIZE                 32768   // Size of cache in bytes
#define HAL_DCACHE_LINE_SIZE            32      // Size of a cache line
#define HAL_DCACHE_WAYS                 4       // Associativity of the cache

// Instruction cache
#define HAL_ICACHE_SIZE                 32768   // Size of cache in bytes
#define HAL_ICACHE_LINE_SIZE            32      // Size of a cache line
#define HAL_ICACHE_WAYS                 4       // Associativity of the cache

#define HAL_DCACHE_SETS (HAL_DCACHE_SIZE/(HAL_DCACHE_LINE_SIZE*HAL_DCACHE_WAYS))
#define HAL_ICACHE_SETS (HAL_ICACHE_SIZE/(HAL_ICACHE_LINE_SIZE*HAL_ICACHE_WAYS))

#define HAL_DCACHE_WRITETHRU_MODE       0
#define HAL_DCACHE_WRITEBACK_MODE       1

// This CPU supports the Index (Writeback) Invalidate operation on the
// cache instruction. This is not the case for all MIPS32 variants.
//#define HAL_DCACHE_SUPPORTS_INDEX_INVALIDATE
//#define HAL_ICACHE_SUPPORTS_INDEX_INVALIDATE

#elif defined(CYGHWR_HAL_MIPS_NT96660_CACHE_64K64K)

// Data cache
#define HAL_DCACHE_SIZE                 65536   // Size of cache in bytes
#define HAL_DCACHE_LINE_SIZE            32      // Size of a cache line
#define HAL_DCACHE_WAYS                 4       // Associativity of the cache

// Instruction cache
#define HAL_ICACHE_SIZE                 65536   // Size of cache in bytes
#define HAL_ICACHE_LINE_SIZE            32      // Size of a cache line
#define HAL_ICACHE_WAYS                 4       // Associativity of the cache

#define HAL_DCACHE_SETS (HAL_DCACHE_SIZE/(HAL_DCACHE_LINE_SIZE*HAL_DCACHE_WAYS))
#define HAL_ICACHE_SETS (HAL_ICACHE_SIZE/(HAL_ICACHE_LINE_SIZE*HAL_ICACHE_WAYS))

#define HAL_DCACHE_WRITETHRU_MODE       0
#define HAL_DCACHE_WRITEBACK_MODE       1

// This CPU supports the Index (Writeback) Invalidate operation on the
// cache instruction. This is not the case for all MIPS32 variants.
//#define HAL_DCACHE_SUPPORTS_INDEX_INVALIDATE
//#define HAL_ICACHE_SUPPORTS_INDEX_INVALIDATE

#else

#error Unknown MIPS32 Variant

#endif

// Set default cache enable and disable values for CP0 Config0
// register K0 field. The precise values supported by the CPU here can
// vary, and on some CPUs, selecting the wrong value can be fatal.

#ifndef HAL_MIPS_NT96660_KSEG0_CACHE_ENABLE
#define HAL_MIPS_NT96660_KSEG0_CACHE_ENABLE      3
#endif

#ifndef HAL_MIPS_NT96660_KSEG0_CACHE_DISABLE
#define HAL_MIPS_NT96660_KSEG0_CACHE_DISABLE     2
#endif

//-----------------------------------------------------------------------------
// General cache defines.
#define HAL_CLEAR_ITAGLO()  asm volatile (" mtc0 $0,$28,0;" \
                                             " nop;"      \
                                             " nop;"      \
                                             " nop;")
#define HAL_CLEAR_IDATALO()  asm volatile (" mtc0 $0,$28,1;" \
                                             " nop;"      \
                                             " nop;"      \
                                             " nop;")
#define HAL_CLEAR_IDATAHI()  asm volatile (" mtc0 $0,$29,1;" \
                                             " nop;"      \
                                             " nop;"      \
                                             " nop;")

#define HAL_CLEAR_DTAGLO()  asm volatile (" mtc0 $0,$28,2;" \
                                             " nop;"      \
                                             " nop;"      \
                                             " nop;")
#define HAL_CLEAR_DDATALO()  asm volatile (" mtc0 $0,$28,3;" \
                                             " nop;"      \
                                             " nop;"      \
                                             " nop;")

#define HAL_CLEAR_L23TAGLO()  asm volatile (" mtc0 $0,$28,4;" \
                                             " nop;"      \
                                             " nop;"      \
                                             " nop;")
#define HAL_CLEAR_L23DATALO()  asm volatile (" mtc0 $0,$28,5;" \
                                             " nop;"      \
                                             " nop;"      \
                                             " nop;")
#define HAL_CLEAR_L23DATAHI()  asm volatile (" mtc0 $0,$29,5;" \
                                             " nop;"      \
                                             " nop;"      \
                                             " nop;")

/* Cache instruction opcodes */
#define HAL_CACHE_OP(which, op)             (which | (op << 2))

#define HAL_WHICH_ICACHE                    0x0	// I cache
#define HAL_WHICH_DCACHE                    0x1	// D cache
#define HAL_WHICH_TCACHE                    0x2	// Tertiary cache (Not supported)
#define HAL_WHICH_L23CACHE                  0x3	// Secondary cache

#define HAL_INDEX_INVALIDATE                0x0	// I
#define HAL_INDEX_WRITEBACK_INVALIDATE      0x0	// D, S, T
#define HAL_INDEX_LOAD_TAG                  0x1	// I, D, S
#define HAL_INDEX_STORE_TAG                 0x2	// All
#define HAL_INDEX_STORE_DATA                0x3	// S
#define HAL_HIT_INVALIDATE                  0x4	// All
#define HAL_ICACHE_FILL                     0x5	// I
#define HAL_DCACHE_HIT_INVALIDATE           0x5	// D, S, T
#define HAL_DCACHE_HIT_WRITEBACK            0x6	// D, S, T
#define HAL_FETCH_AND_LOCK                  0x7	// All

//-----------------------------------------------------------------------------
// Global control of data cache

__externC void hal_dcache_disable( void );
#define HAL_DCACHE_DISABLE_DEFINED
#define HAL_DCACHE_DISABLE() hal_dcache_disable()

__externC void hal_dcache_enable( void );
#define HAL_DCACHE_ENABLE_DEFINED
#define HAL_DCACHE_ENABLE() hal_dcache_enable()

__externC cyg_uint32 hal_dcache_is_enabled( void );
#define HAL_DCACHE_IS_ENABLED_DEFINED
#define HAL_DCACHE_IS_ENABLED(_state_) (_state_) = hal_dcache_is_enabled()

#ifndef HAL_DCACHE_SUPPORTS_INDEX_INVALIDATE

// Invalidate the entire cache
__externC void hal_dcache_invalidate_all( void );
#define HAL_DCACHE_INVALIDATE_ALL_DEFINED
#define HAL_DCACHE_INVALIDATE_ALL() hal_dcache_invalidate_all()

// Synchronize the contents of the cache with memory.
__externC void hal_dcache_sync(void);
#define HAL_DCACHE_SYNC_DEFINED
#define HAL_DCACHE_SYNC() hal_dcache_sync()

#else
// Use Architecture implementation
#endif

// Set the data cache refill burst size
//#define HAL_DCACHE_BURST_SIZE(_asize_)

// Set the data cache write mode
//#define HAL_DCACHE_WRITE_MODE( _mode_ )

// Load the contents of the given address range into the data cache
// and then lock the cache so that it stays there.
__externC void hal_dcache_lock( CYG_ADDRESS base, CYG_WORD  asize);
#define HAL_DCACHE_LOCK_DEFINED
#define HAL_DCACHE_LOCK(_base_, _asize_) hal_dcache_lock( (CYG_ADDRESS)_base_, _asize_ )

// Undo a previous lock operation
__externC void hal_dcache_unlock( CYG_ADDRESS base, CYG_WORD asize);
#define HAL_DCACHE_UNLOCK_DEFINED
#define HAL_DCACHE_UNLOCK(_base_, _asize_) hal_dcache_unlock( (CYG_ADDRESS)_base_, _asize_ )

// Unlock entire cache
#define HAL_DCACHE_UNLOCK_ALL_DEFINED
#define HAL_DCACHE_UNLOCK_ALL() HAL_DCACHE_UNLOCK(0,HAL_DCACHE_SIZE)


//-----------------------------------------------------------------------------
// Data cache line control

// Allocate cache lines for the given address range without reading its
// contents from memory.
//#define HAL_DCACHE_ALLOCATE( _base_ , _asize_ )

// Write dirty cache lines to memory and invalidate the cache entries
// for the given address range.
#define HAL_DCACHE_FLUSH_DEFINED
#if HAL_DCACHE_WRITETHRU_MODE == 1
// No need to flush a writethrough cache
#define HAL_DCACHE_FLUSH( _base_ , _asize_ )
#elif HAL_DCACHE_WRITEBACK_MODE == 1
// To Do Someting
__externC void hal_dcache_flush( CYG_ADDRESS base , CYG_WORD asize );
#define HAL_DCACHE_FLUSH( _base_ , _asize_ )  hal_dcache_flush( (CYG_ADDRESS)_base_, _asize_ )
#else
#error HAL_DCACHE_FLUSH undefined for NT96660 writeback cache
#endif

// Write dirty cache lines to memory for the given address range.
#define HAL_DCACHE_STORE_DEFINED
#if HAL_DCACHE_WRITETHRU_MODE == 1
// No need to store a writethrough cache
#define HAL_DCACHE_STORE( _base_ , _asize_ )
#elif HAL_DCACHE_WRITEBACK_MODE == 1
// To Do Someting
__externC void hal_dcache_store( CYG_ADDRESS base , CYG_WORD asize );
#define HAL_DCACHE_STORE( _base_ , _asize_ )    hal_dcache_store( (CYG_ADDRESS)_base_, _asize_ )
#else
#error HAL_DCACHE_STORE undefined for NT96660 writeback cache
#endif

// Invalidate cache lines in the given range without writing to memory.
__externC void hal_dcache_invalidate( CYG_ADDRESS base , CYG_WORD asize );
#define HAL_DCACHE_INVALIDATE_DEFINED
#define HAL_DCACHE_INVALIDATE( _base_ , _asize_ ) hal_dcache_invalidate( (CYG_ADDRESS)_base_, _asize_ )


//-----------------------------------------------------------------------------
// Global control of Instruction cache

// We can only enable/disable cacheability for a region, and not choose what type,
// so in line with the way UCACHE macros work, we just map to the DCACHE ones.
#define HAL_ICACHE_ENABLE_DEFINED
#define HAL_ICACHE_DISABLE_DEFINED
#define HAL_ICACHE_IS_ENABLED_DEFINED
#define HAL_ICACHE_ENABLE()            HAL_DCACHE_ENABLE()
#define HAL_ICACHE_DISABLE()           HAL_DCACHE_DISABLE()
#define HAL_ICACHE_IS_ENABLED(_state_) HAL_DCACHE_IS_ENABLED(_state_)

#ifndef HAL_ICACHE_SUPPORTS_INDEX_INVALIDATE

// Invalidate the entire cache
__externC void hal_icache_invalidate_all( void );
#define HAL_ICACHE_INVALIDATE_ALL_DEFINED
#define HAL_ICACHE_INVALIDATE_ALL() hal_icache_invalidate_all()

// Synchronize the contents of the cache with memory.
__externC void hal_icache_sync(void);
#define HAL_ICACHE_SYNC_DEFINED
#define HAL_ICACHE_SYNC() hal_icache_sync()

#else
// Use Architecture implementation
#endif

// Set the instruction cache refill burst size
//#define HAL_ICACHE_BURST_SIZE(_asize_)

// Load the contents of the given address range into the data cache
// and then lock the cache so that it stays there.
__externC void hal_icache_lock( CYG_ADDRESS base, CYG_WORD asize);
#define HAL_ICACHE_LOCK_DEFINED
#define HAL_ICACHE_LOCK(_base_, _asize_) hal_icache_lock( (CYG_ADDRESS)_base_, _asize_ )

// Undo a previous lock operation
__externC void hal_icache_unlock( CYG_ADDRESS base, CYG_WORD asize);
#define HAL_ICACHE_UNLOCK_DEFINED
#define HAL_ICACHE_UNLOCK(_base_, _asize_) hal_icache_unlock( (CYG_ADDRESS)_base_, _asize_ )

// Unlock entire cache
#define HAL_ICACHE_UNLOCK_ALL_DEFINED
#define HAL_ICACHE_UNLOCK_ALL() HAL_ICACHE_UNLOCK(0,HAL_ICACHE_SIZE)

//-----------------------------------------------------------------------------
// Instruction cache line control

// Invalidate cache lines in the given range without writing to memory.
__externC void hal_icache_invalidate( CYG_ADDRESS base , CYG_WORD asize );
#define HAL_ICACHE_INVALIDATE_DEFINED
#define HAL_ICACHE_INVALIDATE( _base_ , _asize_ ) hal_icache_invalidate( (CYG_ADDRESS)_base_, _asize_ )

//-----------------------------------------------------------------------------
#endif // ifndef CYGONCE_IMP_CACHE_H
// End of var_cache.h
