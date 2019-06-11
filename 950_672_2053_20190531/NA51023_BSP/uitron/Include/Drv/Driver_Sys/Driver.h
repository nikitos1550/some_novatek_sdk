/**
    Driver common part header file

    @file       Driver.h
    @ingroup    mIDriver
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _DRIVER_H
#define _DRIVER_H

/**
    @addtogroup mIDriver
*/
//@{

#include "Type.h"

typedef enum {
	DRV_VER_96650,
	DRV_VER_96660,
	DRV_VER_96680,
	DRV_VER_96510,

	ENUM_DUMMY4WORD(DRV_VER_INFO)
} DRV_VER_INFO;

/**
    Driver callback function

    uiEvent is bitwise event, please refer to each module's document.

*/
typedef void (*DRV_CB)(UINT32 uiEvent);

//#NT#2016/09/12#Lincy Lin -begin
//#NT#Porting Dual OS flow, the interrupt setting will be configured in loader
extern void     drv_set_init_int(BOOL enable);
//#NT#2016/09/12#Lincy Lin -end

extern void     drv_init(void);
extern void     drv_dumpInfo(void);
extern void 	idec_dumpInfo(UINT32 id);
extern void     pwm_dumpInfo(void);
extern void     drv_end_dump(void);
extern void     drv_begin_dump(void);
extern void     drv_dumpCPUInfo(void);
extern void     drv_dumpDRAMInfo(void);
extern void     drv_dumpIDEInfo(void);
extern void     drv_dumpSDIOInfo(void);
extern void     drv_dumpNANDInfo(void);
extern void     drv_dumpUSBInfo(void);
extern void     drv_dumpPWMInfo(void);
extern void     drv_dumpEACInfo(void);
extern void     drv_dumpSIFInfo(void);
extern void     drv_dumpSIEInfo(void);
extern void     drv_dumpCSIInfo(void);
extern void     drv_dumpCSI2Info(void);
extern void     drv_dumpLVDSInfo(void);
extern void     drv_dumpSensorInfo(void);
extern void     drv_dumpTVInfo(void);
extern void     drv_dumpPWBCInfo(void);
extern void     drv_dumpPllInfo(void);
extern void     drv_dumpMISCInfo(void);
//@}

#endif
