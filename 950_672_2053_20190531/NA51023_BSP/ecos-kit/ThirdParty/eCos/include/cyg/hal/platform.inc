#ifndef CYGONCE_HAL_PLATFORM_INC
#define CYGONCE_HAL_PLATFORM_INC
##=============================================================================
##
##  platform.inc
##
##  NT96660 DSC platform assembler header file
##
##=============================================================================
#####ECOSGPLCOPYRIGHTBEGIN####
## -------------------------------------------
## This file is part of eCos, the Embedded Configurable Operating System.
## Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
##
## eCos is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation; either version 2 or (at your option) any later version.
##
## eCos is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License along
## with eCos; if not, write to the Free Software Foundation, Inc.,
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
##
## As a special exception, if other files instantiate templates or use macros
## or inline functions from this file, or you compile this file and link it
## with other works to produce a work based on this file, this file does not
## by itself cause the resulting work to be covered by the GNU General Public
## License. However the source code for this file must still be made available
## in accordance with section (3) of the GNU General Public License.
##
## This exception does not invalidate any other reasons why a work based on
## this file might be covered by the GNU General Public License.
##
## Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
## at http://sources.redhat.com/ecos/ecos-license/
## -------------------------------------------
#####ECOSGPLCOPYRIGHTEND####
##=============================================================================
#######DESCRIPTIONBEGIN####
##
## Author(s):
## Contributors:
## Date:
## Purpose:      NT96660 DSC platform definitions.
## Description:  This file contains various definitions and macros that are
##               useful for writing assembly code for the NT96660 DSC board.
## Usage:
##      #include <cyg/hal/platform.inc>
##      ...
##
##
######DESCRIPTIONEND####
##
##=============================================================================

#include <cyg/hal/mips.inc>

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/plf_io.h>

#include <pkgconf/hal_mips.h>


#------------------------------------------------------------------------------
# Additional bits for status register.
# We set the IM[0] bit to accept all interrupts.

#define INITIAL_SR_PLF 0x00000400

#define EBase_CPUNUM_MASK       0x03FF      /* specifies the number of the CPU in a */
                                            /* multi-processor system               */
#define EBase_CPUNUM_CPU0       0x0f
#define EBase_CPUNUM_CPU1       0x78

#------------------------------------------------------------------------------

#define CYGPKG_HAL_RESET_VECTOR_FIRST_CODE
    .macro hal_reset_vector_first_code

    # Branch forward past the CPU number register.
    # Only one MIPS inside 96660 as main processor,
    # So, No need to detect CPU ID
#if 0
    mfc0 k0,$15,1
    andi k0,k0,EBase_CPUNUM_MASK
    li   k1,EBase_CPUNUM_CPU0
    beq  k0,k1,1f
    nop

    di
    wait    # halt the CPU1

    nop
    nop
    nop
    nop
    nop
1:
#endif
    .endm

#define CYGPKG_HAL_EARLY_INIT
    .macro  hal_early_init
    # Do these initializations early (rather than in hal_memc_init) so
    # we have access to the LEDs on the board for debugging purposes.

    .endm

#------------------------------------------------------------------------------
# Initialize CPU

#ifndef CYGPKG_HAL_MIPS_CPU_INIT_DEFINED
        .macro  hal_cpu_init


    #************************************************************************
    #* mapping 0xc0000000 via TLB
    #************************************************************************
    mtc0      $0, $2 # Clr EntryLo0
    mtc0        $0, $3 # Clr EntryLo1
    mtc0       $0, $5 # Clr PageMask
    mtc0       $0, $6 # Clr Wired

    lui     $15,0x1fff
    ori    $15,$15,0xe000 # $15=0x1fffe000
    mtc0       $15,$5,0 # Write to PageMask[28:13] set page size as 256MB

    # write EntryLo0($2) ,PFN=0xC0000,C=3, D=1,V=1,G=1
    lui     $15,0x0300
    ori    $15,$15,0x0017 # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017)
    #    ori         $15,$15,0x003f # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017), uncache accelerated=7 (0x003f)
    mtc0       $15,$2,0

    # write EntryLo1($3) ,PFN=0xD0000,C=3, D=1,V=1,G=1
    lui     $15,0x0340
    ori    $15,$15,0x0017 # $15=0x0140001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0       $15,$3,0

    # write EntryHi ,VPN=0, VPN2X=0, ASID=0;
    lui     $15, 0xC000
    mtc0       $15, $10, 0

    # set index register = 0
    mtc0 $0, $0, 0
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
    ori     $15,$15,0x001F # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$2,0

    # write EntryLo1($3) ,PFN=0x10000,C=3, D=1,V=1,G=1
    lui     $15,0x0040
    ori     $15,$15,0x001F # $15=0x0140001f, cache=3 (0x001f), uncache=2 (0x0017)
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
    ori     $15,$15,0x001F # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0    $15,$2,0

    # write EntryLo1($3) ,PFN=0x10000,C=3, D=1,V=1,G=1
    lui     $15,0x00C0
    ori     $15,$15,0x001F # $15=0x0140001f, cache=3 (0x001f), uncache=2 (0x0017)
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

    #************************************************************************
    #* end of mapping 0xc0000000 via TLB
    #************************************************************************

    # Read CCRes to check Counter divisor
    # li      $10, 0x00000008
    # mtc0    $10, $7, 0
    # rdhwr   a0, $3
    # nop

    # Clear Count register
    mtc0    zero, $9

    # Set compare to -1 to delay 1st count=compare
    # Also, clears timer interrupt
    li      $10, -1
    mtc0    $10, $11

    # Initialize/clear watchpoint registers
    mtc0    zero, $18               # C0_WATCHLO
    mtc0    zero, $19               # C0_WATCHHI

    la      t1, cyg_mem_size
    lw      a1, 0(t1)
    bnez    a1,1f
    nop
    li      t2, 0xC0000000          # DRAM controller configuration register
    lw      t3, 0(t2)
    andi    t2, t3, 7               # t2:0=128Mb,1=256Mb,2=512Mb,3=1Gb,4=2Gb
    li      t3, 128                 # 128Mb
    sll     a1, t3, t2              # Store memSize into cyg_mem_size
    sw      a1, 0(t1)
1:

        .endm
#define CYGPKG_HAL_MIPS_CPU_INIT_DEFINED
#endif // ifndef CYGPKG_HAL_MIPS_CPU_INIT_DEFINED


    # -------------------------------------------------------------------------#
    # Interrupt decode macros                                                  #
    # Only INTN[0] is connected on the NT96660 DSC board. We need to access the#
    # interrupt controller to get the actual vector number.                    #
    # -------------------------------------------------------------------------#
#ifndef CYGPKG_HAL_MIPS_INTC_INIT_DEFINED

    .macro  hal_intc_init

    # Reset interrupt controller, disable all interrupts
    li      t0, HAL_INTR_EN
    sw      zero, 0(t0) #EN1
    sw      zero, 4(t0) #EN2

    # .extern __ebase

    # Check if vectored interrupts & external interrupt controller is implemented.
    # Config3(VInt), Config3(VEIC), r
    # mfc0  v0,$16,3
    # nop
    # andi    v0,v0,0x60
    # ori       v1,zero,0x60
    # bne       v0,v1,1f        # for MIPS old version, compatible mode only
    # nop

    # Specify the base address of the exception vectors
    # EBase(Exception Base)
    mtc0  zero,$15,1
    la        v0,__ebase      # 0x80000200
    mtc0  v0,$15,1
    # where __ebase = 0x80000200, but bit[9..0] are read only. So, not necessary

/*
 ************************************************************************
 *          I N T C T L   R E G I S T E R   ( 1 2, SELECT 1 )           *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  I  |  I  |                               |         |         |
 * |  P  |  P  |         0                     |   VS    |   0     |  IntCtl
 * |  T  |  P  |                               |         |         |
 * |  I  |  C  |                               |         |         |
 * |     |  I  |                               |         |         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
    li      v0, 0x00000020          # Set VS as 32 bytes
    mtc0    v0,$12,1


    # -------------------------------------------------------------------------#
    # Set SSV2 as 1 ( vector 2 use shadow register1)                           #
    # -------------------------------------------------------------------------#
    # mfc0    t2, $12, 3
    # li      t3, 0x00000100    # Set SSV2 as 1 ( vector 2 use shadow register1)
    li      t3, 0x00000000    # Set SSV2 as 0 ( vector 2 use shadow register1)
    mtc0    t3, $12, 3
    nop
/*
 ************************************************************************
 *                C A U S E   R E G I S T E R   ( 1 3 )                 *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | | |   | | |   |I|W| | | | | | |I|I|I|I|I|I|I|I| |         |   |
 * |B|0| C |0|0| 0 |V|P| | | | | | |P|P|P|P|P|P|P|P|0| ExcCode | 0 | Cause
 * |D| | E | | |   | | | | | | | | |7|6|5|4|3|2|1|0| |         |   |
 * | | |   | | |   | | | | | | | | | | | | | | | | | |         |   |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    T     D P                    \---RIPL----/
 *    I     C C                      Release 2
 *            I
 */
    # Clear WP bit to avoid watch exception upon user code entry
    # Clear IV bit - Interrupts go to general exception vector
    # Clear software interrupts
    # mtc0    $0, $13
    # Indicates an interrupt exception uses the general exception vector.
    # Cause(IV) = 0, r/w
    //#NT#2013/07/26#Steven Wang -begin
    //#NT#Change to CAUSEiv = 1(bit23), and enable counter
#if 0
    mfc0    v0,cause
    lui     v1,0x0070           # Use the general interrupt vector(180h/380h)
                                # bit[31..28] are read only register, not necessary configured
    and     v0,v0,v1
    mtc0    v0,cause
#else
#   lui     v1,0x0880           # Use special interrupt vector (200h where BEV=0) and Disable Count register as well
    lui     v1,0x0080           # Use special interrupt vector (200h where BEV=0) and Enable Count register as well
    mtc0    v1,cause
#endif
    //#NT#2013/07/26#Steven Wang -end

1:

/*
 ************************************************************************
 *               S T A T U S   R E G I S T E R   ( 1 2 )                *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |C|C|C|C|R|F|R|M|P|B|T|S|M| | R |I|I|I|I|I|I|I|I|K|S|U|U|R|E|E|I|
 * |U|U|U|U|P|R|E|X|X|E|S|R|M| | s |M|M|M|M|M|M|M|M|X|X|X|M|s|R|X|E| Status
 * |3|2|1|0| | | | | |V| | |I| | v |7|6|5|4|3|2|1|0| | | | |v|L|L| |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
    # -------------------------------------------------------------------------#
    # Config status register                                                   #
    # -------------------------------------------------------------------------#
    # Controls the location of exception vectors normal.
    # Clear Interrupt Priority Level.
    # Status(BEV), Status(IPL), r/w
    # mfc0    v0,status
#if defined(CYG_HAL_STARTUP_RAM)
    # li      v1,0x1000FC00         # BEV = 0 (old value :0x1000FC00)
    //#NT#2013/07/26#Steven Wang -begin
    //#NT#unmask interrupt 2(660 INT) & 6(perf) &7(counter)
    # li      v1,0x10000000
    li      v1, 0x0100C400
#elif defined(CYG_HAL_STARTUP_ROM) || defined(CYG_HAL_STARTUP_ROMRAM)
    # li      v1,0x1040FC00        # BEV = 1
    li      v1,0x10400000
#endif
    //#NT#2013/07/26#Steven Wang -end
    # move    v0,v1
    # or     v0,v1

# #if 0
#   ori     v0,v0,0x0001         # Controls the enabling of the interrupt enable(IE)
# #endif
    mtc0    v1,status

    # Clear Count register
    mtc0    $0, $9

    # Set compare to -1 to delay 1st count=compare
    # Also, clears timer interrupt
    li      t2, -1
    mtc0    t2, $11

    .endm

#define CYGPKG_HAL_MIPS_INTC_INIT_DEFINED
#endif

#ifndef CYGPKG_HAL_MIPS_INTC_DECODE_DEFINED
    .macro  hal_intc_decode vnum
        mfc0    v1,status               # get status register (interrupt mask)
#       nop                             # delay slot
        mfc0    v0,cause                # get cause register
        and     v0,v0,v1                # apply interrupt mask
        srl     v0,v0,8                 # shift interrupt bits down
        andi    v1,v0,0x80              # test HW5
        beqz    v1,1f
        li      \vnum,68                # check if match with CYGNUM_HAL_INTR_CPUCNT value in plf_intr.h
        j       2f
        nop
1:
        li      v1,HAL_INTR_ID          # HAL_INTR_ID = 0xC0080104 interrupt ID - CPU2
        lw      v0,0(v1)
        srl     \vnum,v0,2              # TODO ID is already in 4 bytes unit
2:
    .endm

#define CYGPKG_HAL_MIPS_INTC_DECODE_DEFINED
#endif

#ifndef CYGPKG_HAL_MIPS_INTC_TRANSLATE_DEFINED
    .macro  hal_intc_translate inum,vnum
    move    \vnum,\inum         # Vector == interrupt number
    .endm
#define CYGPKG_HAL_MIPS_INTC_TRANSLATE_DEFINED
#endif

// We also define our own interrupt tables in platform.S...
#define CYG_HAL_MIPS_ISR_TABLES_DEFINED

#------------------------------------------------------------------------------
# Diagnostic macros

#ifndef CYGPKG_HAL_MIPS_DIAG_DEFINED

    .macro  hal_diag_init
    .endm

    .macro  hal_diag_excpt_start
    .endm

    .macro  hal_diag_intr_start
    .endm

    .macro  hal_diag_restore
    .endm

    .macro  hal_diag_writec char
    .endm

#define CYGPKG_HAL_MIPS_DIAG_DEFINED
#endif // ifndef CYGPKG_HAL_MIPS_DIAG_DEFINED

#------------------------------------------------------------------------------
# MEMC macros.
#

#define CYGARC_START_FUNC_UNCACHED

#------------------------------------------------------------------------------
# Make sure the jump to _start in vectors.S is done uncached
#

    .macro  hal_memc_sdr_init
    .endm

    .macro  hal_memc_ddr_init
    .endm

    .macro  hal_memc_ddr2_init
    .endm

#ifndef CYGPKG_HAL_MIPS_MEMC_DEFINED

    .macro  hal_memc_init
    .endm

#define CYGPKG_HAL_MIPS_MEMC_DEFINED
#endif // ifndef CYGPKG_HAL_MIPS_MEMC_DEFINED

#------------------------------------------------------------------------------
# Timer initialization.

#ifndef CYGPKG_HAL_MIPS_TIMER_DEFINED

    .macro  hal_timer_init
    .endm

#define CYGPKG_HAL_MIPS_TIMER_DEFINED
#endif // ifndef CYGPKG_HAL_MIPS_TIMER_DEFINED


#------------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_PLATFORM_INC
# end of platform.inc
