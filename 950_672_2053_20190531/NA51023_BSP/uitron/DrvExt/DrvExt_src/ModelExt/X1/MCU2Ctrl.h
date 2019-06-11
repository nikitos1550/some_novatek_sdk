/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       MCU2Ctrl.h
    @ingroup    mIMCU2Ctrl

    @brief      Header file of MCU2Ctrl.c
                Header file of MCU2Ctrl.c

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIMCUCtrl */
//@{

#ifndef _MCU2CTRL_H
#define _MCU2CTRL_H

#include "Type.h"
#include "uart_cmd2.h"
extern void MCU2Ctrl_InstallID(void) _SECTION(".kercfg_text");

#define MCU2CTRL_RET_OK          ( 0)
#define MCU2CTRL_RET_NO_ACK      (-1)


ER MCU2Ctrl_Open(void);
ER MCU2Ctrl_Close(void);
UINT32 MCU2Ctrl_SendCmd(UINT32 cmd, UINT32 p1, UINT32 p2, UINT32 p3);
UINT32 MCU2Ctrl_SendData(CHAR *pData, UINT32 len);
UINT32 MCU2Ctrl_RecvData(CHAR *pData, UINT32 len);
void MCU2Ctrl_DebugEnable(UINT32 enable);
//void MCU2_InstallCmd(void);

UINT32 MCU2Ctrl_UpdateFW(CHAR *pBuf, UINT32 len);

#endif //_MCU2CTRL_H

//@}

