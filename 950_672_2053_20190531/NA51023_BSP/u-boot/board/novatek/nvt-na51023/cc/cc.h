/**
    Header file for CC (Core Communicator) module.

    This file is the header file that define the API and data type
    for CC module.

    @file       cc.h
    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#ifndef _CC_X_H
#define _CC_X_H

typedef unsigned long       CC_U32;     ///< Unsigned 32 bits data type
typedef   signed long       CC_I32;     ///< Unsigned 32 bits data type

#define CC_RES_ID_MAX       32

typedef enum {
    CC_RES_ID_0 = 0x0,
    CC_RES_ID_1,
    CC_RES_ID_2,
    CC_RES_ID_3,
    CC_RES_ID_4,
    CC_RES_ID_NUM,
} CC_RES_ID;

STATIC_ASSERT(CC_RES_ID_NUM <= CC_RES_ID_MAX);

#define CC_RESOURCE_IPC             CC_RES_ID_0     //< IPC resource between multi-cpu
#define CC_RESOURCE_RTC             CC_RES_ID_1     //< RTC resource between multi-cpu
#define CC_RESOURCE_CKG             CC_RES_ID_2     //< CKG resource between multi-cpu
#define CC_RESOURCE_TOP             CC_RES_ID_3     //< TOP resource between multi-cpu
#define CC_RESOURCE_GPIO            CC_RES_ID_4     //< TOP resource between multi-cpu

typedef enum _CC_ER {
    CC_ER_OK    =  0x00000000, ///< no error
    CC_ER_PARM  =  -1,         ///< parameter error
    CC_ER_TMOUT =  -2,         ///< cc timeout
} CC_ER;

extern CC_ER  cc_sendCore1CMD(PCC_CMD pCMD);
extern CC_ER  cc_hwLockResource(CC_CORE_ID uiCoreID, CC_RES_ID uiResource);
extern CC_ER  cc_hwUnlockResource(CC_CORE_ID uiCoreID, CC_RES_ID uiResource);
extern CC_I32 cc_getHwResGrant(CC_CORE_ID uiCoreID);

#endif /* _CC_X_H */
