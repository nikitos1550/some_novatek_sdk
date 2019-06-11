/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       MCUCtrl.h
    @ingroup    mIMCUCtrl

    @brief      Header file of MCUCtrl.c
                Header file of MCUCtrl.c

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIMCUCtrl */
//@{

#ifndef _MCUCTRL_H
#define _MCUCTRL_H

#include "Type.h"
#include "uart_cmd.h"
extern void MCUCtrl_InstallID(void) _SECTION(".kercfg_text");

#define MCUCTRL_RET_OK          ( 0)
#define MCUCTRL_RET_NO_ACK      (-1)


ER MCUCtrl_Open(void);
ER MCUCtrl_Close(void);
UINT32 MCUCtrl_SendCmd(UINT32 cmd, UINT32 param1, UINT32 param2, UINT32 param3);
void MCUCtrl_DebugEnable(UINT32 enable);
void MCU_InstallCmd(void);

#endif //_MCUCTRL_H

//@}

