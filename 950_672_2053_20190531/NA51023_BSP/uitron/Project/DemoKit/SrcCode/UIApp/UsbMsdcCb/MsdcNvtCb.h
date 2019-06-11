#ifndef _MSDCNVTCB_H
#define _MSDCNVTCB_H
#include "UMSDC.h"

typedef struct _MSDCNVTCB_OPEN {
	void (*fpUSBMakerInit)(USB_MSDC_INFO *pUSBMSDCInfo);
} MSDCNVTCB_OPEN, *PMSDCNVTCB_OPEN;

extern void MsdcNvtCb_Open(MSDCNVTCB_OPEN *pOpen);
extern void MsdcNvtCb_Close(void);
extern void MsdcNvtCb_Attach(USB_MSDC_INFO *pInfo);
extern void MsdcNvtCb_Net(BOOL bTurnOn);
extern BOOL MsdcNvtCb_IsUsbConnected(void);


#endif