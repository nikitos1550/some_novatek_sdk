#ifndef _ASR_H
#define _ASR_H
#include "PrjCfg.h"
#if defined(_ASR_ENABLE_)
#include "ImageUnit_UserProc.h"
#include "NvtIpcAPI.h"

typedef struct {
	NVTIPC_U32 count;
	NVTIPC_U32 addr;
	NVTIPC_U32 size;
} ASR_DATA;

typedef struct {
	NVTIPC_U32 count;
	NVTIPC_U32 result;
} ASR_RESULT;

extern void ASR_InstallID(void);
extern INT32 ASR_Init(void);
extern INT32 ASR_UnInit(void);
extern USERPROC_TRIG_PROC_VAL ASR_UserProc_PushCB(ISF_PORT *pPort, ISF_DATA *pData);
#endif
#endif // #ifndef _ASR_H