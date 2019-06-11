#include "SysKer.h"
#include "MCUCtrl.h"

/////////////////
// Services
#define PRI_MCU              8
#define STKSIZE_MCU         2048

extern void MCUCtrlRcvTsk(void);
extern void MCUCtrlSndTsk(void);

UINT32 MCUCtrlRcvTsk_ID = 0; // MCU Service: low priority
UINT32 MCUCtrlSndTsk_ID = 0; // MCU Service: low priority
UINT32 MCUCtrlTsk_SEM_ID = 0;
UINT32 MCUSNDCMD_SEM_ID = 0;
UINT32 FLG_ID_MCUCtrl    = 0;
void MCUCtrl_InstallID(void)
{
	OS_CONFIG_TASK(MCUCtrlRcvTsk_ID,     PRI_MCU,       STKSIZE_MCU,        MCUCtrlRcvTsk);
	OS_CONFIG_TASK(MCUCtrlSndTsk_ID,  PRI_MCU,       STKSIZE_MCU,        MCUCtrlSndTsk);
	OS_CONFIG_SEMPHORE(MCUCtrlTsk_SEM_ID, 0, 1, 1);
	OS_CONFIG_SEMPHORE(MCUSNDCMD_SEM_ID, 0, 1, 1);
	OS_CONFIG_FLAG(FLG_ID_MCUCtrl);
	MCU_InstallCmd();

}
