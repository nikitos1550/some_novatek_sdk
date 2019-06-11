/**
    HW memory manipulate function

    Use Hardware to do memory copy and set.
    The maximum size that HW can handle is 159.99MB. Sample operation flow:\n
    hwmem_open();\n
    hwmem_memcpy();\n
    hwmem_memset();\n
    hwmem_close();

    @file       HwMem.h
    @ingroup    mIUtilHWMem
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _HWMEM_H
#define _HWMEM_H

#include "Type.h"

/**
    @addtogroup mIUtilHWMem
*/
//@{


extern ER       hwmem_open(void);
extern ER       hwmem_close(void);
extern void     hwmem_memcpy(UINT32 uiDst, UINT32 uiSrc, UINT32 uiSize);
extern void     hwmem_memset(UINT32 uiDst, UINT8 ui8Value, UINT32 uiSize);
extern void     hwmem_memset16(UINT32 uiDst, UINT16 ui16Value, UINT32 uiSize);
extern void     hwmem_memset32(UINT32 uiDst, UINT32 ui32Value, UINT32 uiSize);

extern ER       hwmem2_open(void);
extern ER       hwmem2_close(void);
extern void     hwmem2_memcpy(UINT32 uiDst, UINT32 uiSrc, UINT32 uiSize);

#if (_EMULATION_ == ENABLE) //This part only for emulation usage now
extern ER       hwmem_tse_open(void);
extern ER       hwmem_tse_close(void);
extern void     hwmem_tse_memcpy(UINT32 uiDst, UINT32 uiSrc, UINT32 uiSize);

extern ER       hwmem_noe_open(void);
extern ER       hwmem_noe_close(void);
extern void     hwmem_noe_memcpy(UINT32 uiDst, UINT32 uiSrc, UINT32 uiSize);

#endif
//@}

#endif //_HWMEM_H
