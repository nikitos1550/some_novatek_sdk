#include "SysKer.h"
#include "MCU2Ctrl.h"

/////////////////
// Services
#define PRI_MCU2            8
#define STKSIZE_MCU2        2048

extern void MCU2CtrlRcvTsk(void);
extern void MCU2CtrlSndTsk(void);

UINT32 MCU2CtrlRcvTsk_ID = 0; // MCU Service: low priority
UINT32 MCU2CtrlSndTsk_ID = 0; // MCU Service: low priority
UINT32 MCU2CtrlTsk_SEM_ID = 0;
UINT32 MCU2SNDCMD_SEM_ID = 0;
UINT32 FLG_ID_MCU2Ctrl    = 0;
void MCU2Ctrl_InstallID(void)
{
	OS_CONFIG_TASK(MCU2CtrlRcvTsk_ID,  PRI_MCU2,       STKSIZE_MCU2,        MCU2CtrlRcvTsk);
	OS_CONFIG_TASK(MCU2CtrlSndTsk_ID,  PRI_MCU2,       STKSIZE_MCU2,        MCU2CtrlSndTsk);
	OS_CONFIG_SEMPHORE(MCU2CtrlTsk_SEM_ID, 0, 1, 1);
	OS_CONFIG_SEMPHORE(MCU2SNDCMD_SEM_ID, 0, 1, 1);
	OS_CONFIG_FLAG(FLG_ID_MCU2Ctrl);
	//MCU2_InstallCmd();

}
