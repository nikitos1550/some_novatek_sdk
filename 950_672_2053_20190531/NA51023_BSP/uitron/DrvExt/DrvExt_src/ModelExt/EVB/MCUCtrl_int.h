#include "Type.h"

#ifndef _MCUCTRL_INT_H
#define _MCUCTRL_INT_H

extern UINT32 _SECTION(".kercfg_data") MCUCtrlRcvTsk_ID; // MCU Service: low priority
extern UINT32 _SECTION(".kercfg_data") MCUCtrlSndTsk_ID; // MCU Service: low priority
extern UINT32 _SECTION(".kercfg_data") MCUCtrlTsk_SEM_ID;
extern UINT32 _SECTION(".kercfg_data") MCUSNDCMD_SEM_ID;
extern UINT32 _SECTION(".kercfg_data") FLG_ID_MCUCtrl;

void MCUCtrlRcvTsk(void);
void MCUCtrlSndTsk(void);

#endif //_MCUCTRL_INT_H
