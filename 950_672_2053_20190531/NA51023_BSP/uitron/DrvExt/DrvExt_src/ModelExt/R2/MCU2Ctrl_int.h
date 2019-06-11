#include "Type.h"

#ifndef _MCU2CTRL_INT_H
#define _MCU2CTRL_INT_H

extern UINT32 _SECTION(".kercfg_data") MCU2CtrlRcvTsk_ID; // MCU2 Service: low priority
extern UINT32 _SECTION(".kercfg_data") MCU2CtrlSndTsk_ID; // MCU2 Service: low priority
extern UINT32 _SECTION(".kercfg_data") MCU2CtrlTsk_SEM_ID;
extern UINT32 _SECTION(".kercfg_data") MCU2SNDCMD_SEM_ID;
extern UINT32 _SECTION(".kercfg_data") FLG_ID_MCU2Ctrl;

void MCU2CtrlRcvTsk(void);
void MCU2CtrlSndTsk(void);

#endif //_MCU2CTRL_INT_H
