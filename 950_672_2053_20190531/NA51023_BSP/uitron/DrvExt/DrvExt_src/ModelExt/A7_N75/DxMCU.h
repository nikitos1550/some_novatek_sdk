/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DxMCU.h
    @ingroup    mIDxMCU

    @brief      Header file of DxMCU.c
                Header file of DxMCU.c

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIDxMCU */
//@{

#ifndef _DXMCU_H
#define _DXMCU_H

#include "Type.h"
#include "Dx.h"
#include "DxCommon.h"

#define DXMCU_VER                 0x00010000

typedef enum {
	DXMCU_CFG_UART_BAUDRATE,
	DXMCU_CFG_UART_LENGTH,
	ENUM_DUMMY4WORD(DXMCU_CFG_ID)
} DXMCU_CFG_ID, *PDXMCU_CFG_ID;

extern UINT32 DxMCU_Open(void);
extern UINT32 DxMCU_Get(CHAR *pChar);
extern UINT32 DxMCU_Put(CHAR* pChar,UINT32 len);
extern UINT32 DxMCU_Close(void);
extern UINT32 DxMCU_Setcfgs(DXMCU_CFG_ID CfgID, UINT32 Param1);
#endif

//@}
