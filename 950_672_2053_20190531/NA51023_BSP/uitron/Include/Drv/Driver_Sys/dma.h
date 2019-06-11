/**
    Header file for DMA module.

    This file is the header file that define the API for DMA module.

    @file       DMA.h
    @ingroup    mIDrvDMA_DMA
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _DMA_H
#define _DMA_H

#include "Type.h"
#include "SysKer.h"

/**
    @addtogroup mIDrvDMA_DMA
*/

/**
    DMA controller ID

*/
typedef enum {
	DMA_ID_1,                           ///< DMA Controller

	DMA_ID_COUNT,                       //< DMA controller count

	ENUM_DUMMY4WORD(DMA_ID)
} DMA_ID;

/**
    Check DRAM address is cacheable or not

    Check DRAM address is cacheable or not.

    @param[in] addr     DRAM address

    @return Cacheable or not
        - @b TRUE   : Cacheable
        - @b FALSE  : Non-cacheable
*/
#define DMA_ADDR_TRANS_CHECK            ENABLE

#if (DMA_ADDR_TRANS_CHECK == DISABLE)
#define dma_isCacheAddr(addr)           (((UINT32)(addr) >= 0x40000000UL) ? FALSE : TRUE)
#else
#ifndef INREG8
#define INREG8(x)                       (*((volatile UINT8*)(x)))
#endif
#ifndef INREG32
#define INREG32(x)                      (*((volatile UINT32*)(x)))
#endif

#ifndef OUTREG8
#define OUTREG8(x, y)                   (*((volatile UINT8*)(x)) = (y))
#endif
static void uart_put_char(UINT8 ch)
{
#if _EMULATION_ON_CPU2_ == ENABLE
	while (!(INREG8(0xC0300014) & 0x20));
	OUTREG8(0xC0300000, ch);
#else
	while (!(INREG8(0xC0290014) & 0x20));
	OUTREG8(0xC0290000, ch);
#endif
}

static void uart_put_str(char *Str)
{
	char *cp;
	for (cp = Str; *cp != 0; cp++) {
		uart_put_char(*cp);
	}
}

static void dma_fatal_err_delay(UINT32 delay)
{
	UINT32  _delay = delay * 1000;
	UINT32 timeTick;
	UINT32 timeTick2;

	timeTick = INREG32(0xC0040108);

	if ((timeTick + _delay) <= 0x19999999) {
		while (INREG32(0xC0040108) < (timeTick + _delay));
	} else {
		timeTick2 = _delay - (0x19999999 - timeTick);
		while (INREG32(0xC0040108) <= 0x19999999);
		while (INREG32(0xC0040108) < timeTick2);
	}
}


static BOOL dma_isCacheAddr(UINT32 addr)
{
	UINT32 loop = 100;
	if (addr >= 0x80000000 && addr < 0xC0000000) {
		while (loop--) {
			uart_put_str("address violation =>should be 0x00000000 ~ 0x7FFFFFFC\r\n");
			OS_DumpCurrentBacktraceEx(0);
			dma_fatal_err_delay(100);
		}
	}
	return (((UINT32)(addr) >= 0x40000000UL) ? FALSE : TRUE);
}
#endif

/**
    Check (virtual) DRAM address is valid or not

    @param[in] addr     DRAM address

    @return valid or not
        - @b TRUE   : valid
        - @b FALSE  : Not valid
*/
#define dma_is_valid_addr(addr)     ((((UINT32)(addr))<0x80000000UL)?       \
									 ((addr)!=0?TRUE:FALSE)              \
									 :FALSE)

/**
    Translate DRAM address to cacheable address.

    Translate DRAM address to cacheable address.

    @param[in] addr     DRAM address

    @return Cacheable DRAM address
*/
#if (DMA_ADDR_TRANS_CHECK == DISABLE)
#define dma_getCacheAddr(addr)          ((((UINT32)(addr))>=0x40000000UL)?((UINT32)(addr)-0x40000000UL):(UINT32)(addr))
#else
static UINT32 dma_getCacheAddr(UINT32 addr)
{
	UINT32 loop = 100;
	if (addr >= 0x80000000 && addr < 0xC0000000) {
		while (loop--) {
			uart_put_str("address violation =>should be 0x00000000 ~ 0x7FFFFFFC\r\n");
			OS_DumpCurrentBacktraceEx(0);
			dma_fatal_err_delay(100);
		}
	}
	return ((((UINT32)(addr)) >= 0x40000000UL) ? ((UINT32)(addr) - 0x40000000UL) : (UINT32)(addr));
}
#endif

/**
    DRAM type encoding

    DRAM type encoding returned by dma_getDramType()
*/
typedef enum {
	DDR_TYPE_DDR2,          ///< DDR2 DRAM
	DDR_TYPE_DDR3,          ///< DDR3 DRAM
	ENUM_DUMMY4WORD(DMA_DDR_TYPE)
} DMA_DDR_TYPE;

extern DMA_DDR_TYPE     dma_getDramType(void);
extern UINT32           dma_getDramBaseAddr(DMA_ID id);

//@}

#endif
