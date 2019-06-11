#include "PrjCfg.h"
#include "HwMem.h"
#include "timer.h"
#include "dma.h"
#include "lviewd.h"
#include "LviewNvtAPI.h"

#if (HTTP_LIVEVIEW_FUNC == ENABLE)


#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppHttp
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


static BOOL bStartHttp = FALSE;

static int UIAppHttp_getJpg(int *jpgAddr, int *jpgSize)
{
	MEM_RANGE  JpgBuf;
	int        ret;

	if (bStartHttp) {
		ret = LviewNvt_getCurDecJpgBuf(&JpgBuf);
		if (ret == E_OK) {
			*jpgAddr = JpgBuf.Addr;
			*jpgSize = JpgBuf.Size;
			ret = TRUE;
		} else {
			*jpgAddr = 0;
			*jpgSize = 0;
			ret = FALSE;
		}
		DBG_IND("jpgAddr=0x%x,jpgSize=0x%x\r\n ", JpgBuf.Addr, JpgBuf.Size);
	} else {
		*jpgAddr = 0;
		*jpgSize = 0;
		ret = FALSE;
	}
	return ret;
}

static UINT32 UIAppHttp_getCurrTime(void)
{
	return timer_getCurrentCount(timer_getSysTimerID());
}


static void UIAppHttp_notifyStatus(int status)
{
	static  UINT32 preClientTime = 0;
	switch (status) {
	case CYG_LVIEW_STATUS_CLIENT_REQUEST:
		DBG_DUMP("client request %d ms\r\n", UIAppHttp_getCurrTime() / 1000);
		preClientTime = UIAppHttp_getCurrTime();
		break;
	case CYG_LVIEW_STATUS_SERVER_RESPONSE_START:
		DBG_IND("send data start, time= %05d ms\r\n", UIAppHttp_getCurrTime() / 1000);
		break;
	case CYG_LVIEW_STATUS_SERVER_RESPONSE_END:
		DBG_IND("send data end, time= %05d ms\r\n", UIAppHttp_getCurrTime() / 1000);
		break;
	case CYG_LVIEW_STATUS_CLIENT_DISCONNECT:
		DBG_DUMP("client disconnect, time= %05d ms\r\n", UIAppHttp_getCurrTime() / 1000);
		break;
	}
}

void UIAppHttp_Open(void)
{
	extern UINT32 g_lviewPort, g_lviewTaskPri, g_maxJpgSize, g_lviewframeRate;

	LVIEWNVT_DAEMON_INFO   lviewObj;
	// start http daemon
	DBG_IND("start lviewd\r\n");
	// start client thread

	lviewObj.getJpg = (LVIEWD_GETJPGCB)UIAppHttp_getJpg;
	lviewObj.serverEvent = (LVIEWD_SERVER_EVENT_CB)UIAppHttp_notifyStatus;
	lviewObj.portNum = g_lviewPort;
	// set http live view server thread priority
	lviewObj.threadPriority = g_lviewTaskPri;
	// set the maximum JPG size that http live view server to support
	lviewObj.maxJpgSize = g_maxJpgSize;
	// live view streaming frame rate
	lviewObj.frameRate = g_lviewframeRate;
	// socket buffer size
	lviewObj.sockbufSize = 40960;
	// reserved parameter , no use now, just fill NULL.
	lviewObj.arg = NULL;
	LviewNvt_StartDaemon(&lviewObj);

}

void UIAppHttp_Close(void)
{
	LviewNvt_StopDaemon();
}

void UIAppHttp_Start(void)
{
	bStartHttp = TRUE;
}
void UIAppHttp_Stop(void)
{
	bStartHttp = FALSE;
}
#endif
