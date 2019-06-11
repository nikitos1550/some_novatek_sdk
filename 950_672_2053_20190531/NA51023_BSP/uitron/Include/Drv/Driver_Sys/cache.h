/**
    CPU module driver.

    This file is the driver of CPU module.

    @file       Cache.h
    @ingroup    mIDrvSys_Core
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _CACHE_H
#define _CACHE_H

#include "Type.h"


/**
    @addtogroup mIDrvSys_Core
*/
//@{
extern ER       cpu_lockDownDCache(UINT32 start, UINT32 end);
extern ER       cpu_lockDownICache(UINT32 start, UINT32 end);

#endif

//@}
