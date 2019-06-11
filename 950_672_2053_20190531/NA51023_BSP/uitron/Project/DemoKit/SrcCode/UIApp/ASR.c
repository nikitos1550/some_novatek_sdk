#if defined(_ASR_ENABLE_)

#include "ASR.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          ASR
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#include "SysCfg.h"
#include "HwMem.h"

#define ASR_ECOS_VER        20190219

#define USE_CPU2            ENABLE

#define PRI_ASR             10
#define STKSIZE_ASR         2048

#if (USE_CPU2 == ENABLE)
static NVTIPC_I32 msqid = -1;
UINT32 ASR_TSK_ID = 0;
#endif

static UINT32 is_asr_opened = 0;
static UINT32 AudBufInAddr[5];       // 5 block to store audio in data
static UINT32 AudBufOutAddr[5];      // 5 block to send audio to cpu2
static UINT32 AudBufOutSize[5];      // 5 block to send audio to cpu2
static UINT32 AudBufIdx;
static UINT32 AudDataCnt;


void ASRTsk(void)
{
#if (USE_CPU2 == ENABLE)
	NVTIPC_I32 ipcErr = 0;
	ASR_RESULT ipcMsgRcv;

	kent_tsk();
	while (1) {
		ipcErr = NvtIPC_MsgRcv(msqid, &ipcMsgRcv, sizeof(ipcMsgRcv));
		// release buffer here!
		//DBG_DUMP("Rcv cnt%d, result=%d\r\n", ipcMsgRcv.count, ipcMsgRcv.result);
	}
	ext_tsk();
#endif
}

void ASR_InstallID(void)
{
#if (USE_CPU2 == ENABLE)
	OS_CONFIG_TASK(ASR_TSK_ID, PRI_ASR, STKSIZE_ASR, ASRTsk);
#endif
}

INT32 ASR_Init(void)
{
	UINT32 i;

	if (is_asr_opened == 1) {
		DBG_ERR("ASR is already opened!\n");
		return -1;
	}

#if (USE_CPU2 == ENABLE)
	CHAR ipccmd[64];
	NVTIPC_SYS_MSG sysMsg;
	NVTIPC_I32 ipcErr = 0;

	if (msqid < 0) {
		msqid = NvtIPC_MsgGet(NvtIPC_Ftok("asr"));
	}
	snprintf(ipccmd, sizeof(ipccmd) - 1, "asr -open %d", ASR_ECOS_VER);
	sysMsg.sysCmdID = NVTIPC_SYSCMD_SYSCALL_REQ;
	sysMsg.DataAddr = (UINT32)ipccmd;
	sysMsg.DataSize = strlen(ipccmd) + 1;
	if ((ipcErr = NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE2, &sysMsg, sizeof(sysMsg))) < 0) {
		DBG_ERR("Failed to NVTIPC_SYS_QUEUE_ID\r\n");
	}
	sta_tsk(ASR_TSK_ID, 0);
#endif

	for (i = 0; i < 5; i ++) {
		AudBufInAddr[i] = OS_GetMempoolAddr(POOL_ID_ASR) + i * OS_GetMempoolSize(POOL_ID_ASR) / 5;
		AudBufOutAddr[i] = OS_GetMempoolAddr(POOL_ID_ASR) + i * 1920;
		AudBufOutSize[i] = (i == 4) ? 2560 : 1920;
		//DBG_DUMP("AudBufInAddr[%d] =%x\r\n", i, AudBufInAddr[i]);
		//DBG_DUMP("AudBufOutAddr[%d]=%x, size=%x\r\n", i, AudBufOutAddr[i], AudBufOutSize[i]);
	}
	AudBufIdx = 0;
	AudDataCnt = 0;

	is_asr_opened = 1;

	return 0;
}

INT32 ASR_UnInit(void)
{
	if (is_asr_opened == 0) {
		DBG_ERR("ASR is already closed!\n");
		return -1;
	}
#if (USE_CPU2 == ENABLE)
	CHAR ipccmd[64];
	NVTIPC_SYS_MSG sysMsg;
	NVTIPC_I32 ipcErr = 0;

	snprintf(ipccmd, sizeof(ipccmd) - 1, "asr -close");
	sysMsg.sysCmdID = NVTIPC_SYSCMD_SYSCALL_REQ;
	sysMsg.DataAddr = (UINT32)ipccmd;
	sysMsg.DataSize = strlen(ipccmd) + 1;
	if ((ipcErr = NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE2, &sysMsg, sizeof(sysMsg))) < 0) {
		DBG_ERR("Failed to NVTIPC_SYS_QUEUE_ID\r\n");
	}
	ter_tsk(ASR_TSK_ID);

	if (msqid >= 0) {
		NvtIPC_MsgRel(msqid);
		msqid = -1;
	}
#endif
	is_asr_opened = 0;

	return 0;
}

USERPROC_TRIG_PROC_VAL ASR_UserProc_PushCB(ISF_PORT *pPort, ISF_DATA *pData)
{
#if (USE_CPU2 == ENABLE)
	UINT32 i;
	ISF_AUDIO_STREAM_BUF *pAudBuf = (ISF_AUDIO_STREAM_BUF *)&(pData->Desc[0]);
	NVTIPC_I32 ipcErr = 0;
	ASR_DATA ipcMsgSnd;

	for (i = 0; i < pAudBuf->DataSize / 2048; i ++) {
		hwmem_open();
		hwmem_memcpy(AudBufInAddr[AudBufIdx], pAudBuf->DataAddr, 2048);
		hwmem_close();
		ipcMsgSnd.count = AudDataCnt;
		ipcMsgSnd.addr = NvtIPC_GetPhyAddr(AudBufOutAddr[AudBufIdx]);
		ipcMsgSnd.size = AudBufOutSize[AudBufIdx];
		//DBG_DUMP("Get AudIn Sample %x(%x) (%d/%d)\r\n", AudBufInAddr[AudBufIdx], ipcMsgSnd.addr, AudBufIdx, AudDataCnt);
		ipcErr = NvtIPC_MsgSnd(msqid, NVTIPC_SENDTO_CORE2, &ipcMsgSnd, sizeof(ipcMsgSnd));
		AudBufIdx = (AudBufIdx + 1) % 5;
		AudDataCnt ++;
	}
#endif
	return USERPROC_TRIG_PROC_NONE;
}

#endif
