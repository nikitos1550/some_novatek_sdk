/**
    Novatek memory query function

    @file       NvtMem.h
    @ingroup    mIUtilNvtMem
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

#ifndef _NVTMEM_H
#define _NVTMEM_H

#include "Type.h"

/**
    @addtogroup mIUtilNvtMem
*/
//@{


extern UINT32 NvtMem_GetNonCacheAddr(UINT32 addr);
extern UINT32 NvtMem_GetPhyAddr(UINT32 addr);

//@}

#endif //_HWMEM_H

