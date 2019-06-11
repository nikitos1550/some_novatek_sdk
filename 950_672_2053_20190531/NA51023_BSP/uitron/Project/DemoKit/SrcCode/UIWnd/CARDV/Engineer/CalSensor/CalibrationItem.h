/** \addtogroup mIPRJAPTest */
//@{

#ifndef _CALIBRATIONITEM_H
#define _CALIBRATIONITEM_H

#include "Type.h"


extern void     Cal_SenDP(void);
extern void     Cal_SenDP_Bright(void);
extern void     Cal_SenDP_Dark(void);
extern void     Cal_SenAE_Check(void);
extern void     Cal_SenISO(void);
extern void     Cal_SenMShutter(void);
extern void     Cal_SenAWB(void);
extern void     Cal_SenAWBGS(void);
extern void     Cal_SenECS(void);
extern void     Cal_SenDNP(void);
extern void     Cal_Capture(void);
//#NT#2016/09/12#Silvia Wu -begin
//#NT# support lens center calibration
extern void     Cal_LENSCEN(void);
//#NT#2016/09/12#Silvia Wu -end

extern ER Cal_IplOpen(IPL_PROC_ID Id, UINT32 iplNum, UINT32 BufAddr, UINT32 BufSize);
extern ER Cal_IplInit (void);
extern ER Cal_IplUnInit (void);
extern SX_CMD_ENTRY* UiCalTest_GetCmdEntry(void);
extern void CalTest_InstallCmd(void);
#endif

//@}
