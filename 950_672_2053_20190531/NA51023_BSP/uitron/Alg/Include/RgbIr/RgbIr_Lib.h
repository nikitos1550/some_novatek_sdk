#ifndef _RGBIR_LIB_H_
#define _RGBIR_LIB_H_

#include "sie_lib.h"
#include "sie1_lib.h"

typedef struct {
	UINT16 IrSub_R;
	UINT16 IrSub_G;
	UINT16 IrSub_B;
	UINT16 IrLSub_LB;
	UINT16 IrLSub;
	UINT16 IrLSubRang;
	UINT16 IRSatGain;
} RGBIR_PARMS_INFO;

extern void RgbIr_calParms(SIE_STCS_CA_RSLT_INFO *CARst, RGBIR_PARMS_INFO *pParms);
extern void RgbIr_GetCaStcs(SIE_STCS_CA_RSLT_INFO *CARst);
extern UINT32 RgbIr_calcIrLevel(SIE_STCS_CA_RSLT_INFO *CARst, SIE_CA_WIN_INFO CaWinInfo, SIE_CA_IRSUB_INFO CaIrSubInfo);
extern void RgbIr_setAutoSatgainEn(UINT32 uiEn);

extern void RgbIr_InstallCmd(void);         // Uart Cmd

#endif