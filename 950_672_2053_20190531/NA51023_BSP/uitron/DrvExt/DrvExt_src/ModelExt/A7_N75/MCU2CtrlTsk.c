/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       MCU2CtrlTsk.c
    @ingroup

    @brief      MCU2 command via DxMCU2 interface.


    @note       Nothing.

    @version    V1.00.000
*/
#include <stdio.h>
#include "SxCmd.h"
#include "DxCfg.h"
#include "IOCfg.h"
#include "MCU2Ctrl.h"
#include "MCU2Ctrl_int.h"


#if defined(_MCU_ENABLE_)
#include "DxMCU.h"
#include "DxCommon.h"
#include "DxApi.h"
#include "uart_cmd2.h"
#include "timer.h"
#include "DxPower.h"
#include "Utility.h"
#include "uart.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          MCU2Ctrl
#define __DBGLVL__          2 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define MCU2_BUFFER_MAX_LEN     256

#define FLGMCU2UART_ACK         FLGPTN_BIT(0)
#define FLGMCU2UART_RSP         FLGPTN_BIT(1)
#define FLGMCU2UART_TIMEOUT     FLGPTN_BIT(2)
#define FLGMCU2UART_SND         FLGPTN_BIT(3)
#define FLGMCU2UART_RET         FLGPTN_BIT(4)
#define FLGMCU2UART_STOP        FLGPTN_BIT(5)
#define FLGMCU2UART_STOPED      FLGPTN_BIT(6)
#define FLGMCU2UART_RCV         FLGPTN_BIT(7)

#define GPIO_MCU_RESET              0
#define GPIO_MCU_TOOL0              0

#define MCU_BLOCKCNT                16
#define MCU_BLOCKSIZE               1024
#define MCU_CODESIZE                (1024*16)

//----- External funcs & variables declaration here  ------------------
#define MCU2_RETRY_CNT    15
static BOOL g_bMCU2CtrlOpened = FALSE;
static CHAR szSndBuffer[MCU2_BUFFER_MAX_LEN + 5] = {0};
static CHAR szRcvBuffer[MCU2_BUFFER_MAX_LEN + 5] = {0};
static UINT32 uiSndLen = 0;
static UINT32 uiRcvLen = 0;
static UINT32 uiSndRet = 0;
static UINT32 gSndResult = 0;
static UINT32 bRcvStart = 0;
static UINT32 dbg_msg = FALSE; //TRUE;

static void MCU2Ctrl_Lock(void)
{
	//DBG_IND("Lock\r\n");
	wai_sem(MCU2CtrlTsk_SEM_ID);
}
static void MCU2Ctrl_Unlock(void)
{
	//DBG_IND("Unlock\r\n");
	sig_sem(MCU2CtrlTsk_SEM_ID);
}

static void MCU2Ctrl_SndLock(void)
{
	//DBG_IND("Lock\r\n");
	wai_sem(MCU2SNDCMD_SEM_ID);
}
static void MCU2Ctrl_SndUnlock(void)
{
	//DBG_IND("Unlock\r\n");
	sig_sem(MCU2SNDCMD_SEM_ID);
}

UINT32 MCU2Ctrl_Send(CHAR *pCmd, UINT32 len);

static void MCU2Ctrl_Dump(CHAR *pCmd, UINT32 len)
{
	UINT32 i;
	DBG_DUMP("cmd #### ");
	for (i = 0; i < len + 4; i++) {
		DBG_DUMP("%02X ", pCmd[i]);
	}
	DBG_DUMP("####\r\n");
}

static CHAR MCU2Ctrl_Sum(CHAR *pCmd, UINT32 len)
{
	UINT32 i = 0;
	UINT32 sum = 0;
	CHAR *pValue = (CHAR *)pCmd;
	for (i = 0; i < len; i++) {
		sum -= *pValue;
		pValue++;
	}
	return sum;
}

UINT32 MCU2Ctrl_Send(CHAR *pCmd, UINT32 len)
{
	UINT32 result;
	if (dbg_msg) {
		DBG_DUMP("^Csnd ");
		MCU2Ctrl_Dump(pCmd, len);
	}

	result = DxMCU_Put((CHAR *)pCmd, len + 4);

	return result;
}

static INT32 MCU2Ctrl_SendTsk(CHAR *pCmd, UINT32 len)
{
	FLGPTN  uiFlag = 0, waitFlag = 0;
	if (len > MCU2_BUFFER_MAX_LEN) {
		DBG_ERR("Out of cmd length!\r\n");
		return 0;
	}
	MCU2Ctrl_SndLock();
	uiSndLen = len;
	memcpy(szSndBuffer, pCmd, uiSndLen + 4);
	if (0) {
		DBG_DUMP("^Csnd ");
		MCU2Ctrl_Dump(szSndBuffer, uiSndLen);
	}
	set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_SND);

	Delay_DelayUs(550); //tCS = tDS = 550 us (66 bits)

	waitFlag = FLGMCU2UART_RET;
	wai_flg(&uiFlag, FLG_ID_MCU2Ctrl, waitFlag, TWF_ORW | TWF_CLR);
	MCU2Ctrl_SndUnlock();
	return gSndResult;
}

static UINT32 MCU2Ctrl_Recv(CHAR *pCmd, UINT32 len)
{
	if (pCmd) {
		if (dbg_msg) {
			DBG_DUMP("^Mrcv ");
			MCU2Ctrl_Dump(pCmd, len);
		}
		if ((pCmd[0] == UART_RCV_STSHEAD) && (pCmd[UART_RCV_OFFSET + len + 1] == UART_RCV_STSTAIL)) {
			if (pCmd[UART_RCV_OFFSET + len] != MCU2Ctrl_Sum(pCmd + UART_RCV_OFFSET - 1, len + 1)) {
				DBG_DUMP("^Rret: SUM error!\r\n");
				while (1);
			}

			if (len == 1) { //return ST1, return 1 bytes
				uiSndRet = pCmd[UART_RCV_OFFSET + 0];
				if (pCmd[UART_RCV_OFFSET + 0] == UART_RET_ACK) {
					DBG_DUMP("ret: ACK\r\n");
					set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_ACK);
				} else {
					DBG_DUMP("^Rret: ST1 = %02X\r\n", pCmd[UART_RCV_OFFSET + 0]);
					set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_ACK);
					if (pCmd[UART_RCV_OFFSET + 0] != UART_RET_INTERNALERR)
						while (1);
				}
			} else if (len == 3) { //return ST1+r1+r2  //UART_CMD_BAUDRATESET send cmd, return 3 bytes
				uiSndRet = pCmd[UART_RCV_OFFSET + 0];
				if (pCmd[UART_RCV_OFFSET + 0] == UART_RET_ACK) {
					DBG_DUMP("ret: ACK (D01 D02 = %02X %02X)\r\n", pCmd[UART_RCV_OFFSET + 1], pCmd[UART_RCV_OFFSET + 2]);
					set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_ACK);
				} else {
					DBG_DUMP("^Rret: ST1 = %02X\r\n", pCmd[UART_RCV_OFFSET + 0]);
					set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_ACK);
					while (1);
				}
			} else if (len == 2) { //return ST2 //UART_CMD_PROGRAMMING send data, return 2 bytes
				uiSndRet = pCmd[UART_RCV_OFFSET + 0];
				if ((pCmd[UART_RCV_OFFSET + 0] == UART_RET_ACK) && (pCmd[UART_RCV_OFFSET + 1] == UART_RET_ACK)) {
					DBG_DUMP("ret: ACK\r\n", pCmd[UART_RCV_OFFSET + 0], pCmd[UART_RCV_OFFSET + 1]);
					set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_ACK);
				} else {
					uiSndRet = (pCmd[UART_RCV_OFFSET + 0]) | (pCmd[UART_RCV_OFFSET + 1] << 8);
					DBG_DUMP("^Rret: ST1 ST2 = %02X %02X\r\n", pCmd[UART_RCV_OFFSET + 0], pCmd[UART_RCV_OFFSET + 1]);
					set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_ACK);
					while (1);
				}
			} else if (len == 2) { //return x //UART_CMD_CHECKSUM recv data, return 2 bytes
				{
					uiSndRet = UART_RET_ACK;
					DBG_DUMP("ret: CK1 CK2 = %02X %02X\r\n", pCmd[UART_RCV_OFFSET + 0], pCmd[UART_RCV_OFFSET + 1]);
					set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_ACK);
				}
			} else if (len == 0x08) { //return x //UART_CMD_SECURITYGET recv data, return 8 bytes
				{
					uiSndRet = UART_RET_ACK;
					DBG_DUMP("ret: SECURITY FLG=%02X, BOT=%02X, b_start=%02X%02X, b_end=%02X%02X, RES=%02X%02X\r\n",
							 pCmd[UART_RCV_OFFSET + 0], pCmd[UART_RCV_OFFSET + 1],
							 pCmd[UART_RCV_OFFSET + 2], pCmd[UART_RCV_OFFSET + 3],
							 pCmd[UART_RCV_OFFSET + 4], pCmd[UART_RCV_OFFSET + 5],
							 pCmd[UART_RCV_OFFSET + 6], pCmd[UART_RCV_OFFSET + 7]);
					set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_ACK);
				}
			} else if (len == 0x16) { //return x //UART_CMD_SILICONSIGNATURE recv data, return 22 bytes
				{
					uiSndRet = UART_RET_ACK;
					DBG_DUMP("ret: SIGNATURE dev=%02X%02X%02X ",
							 pCmd[UART_RCV_OFFSET + 0], pCmd[UART_RCV_OFFSET + 1], pCmd[UART_RCV_OFFSET + 2]);
					DBG_DUMP("name=[%s] ",
							 &(pCmd[UART_RCV_OFFSET + 3]));
					DBG_DUMP("codesize=0x%02X%02X%02X ",
							 pCmd[UART_RCV_OFFSET + 15], pCmd[UART_RCV_OFFSET + 14], pCmd[UART_RCV_OFFSET + 13]);
					DBG_DUMP("datasize=0x%02X%02X%02X ",
							 pCmd[UART_RCV_OFFSET + 18], pCmd[UART_RCV_OFFSET + 17], pCmd[UART_RCV_OFFSET + 16]);
					DBG_DUMP("ver=%d.%d%d\r\n",
							 pCmd[UART_RCV_OFFSET + 19], pCmd[UART_RCV_OFFSET + 20], pCmd[UART_RCV_OFFSET + 21]);
					set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_ACK);
				}
			}
		}
	}

	return 0;
}


static INT32 MCU2Ctrl_RecvTsk(CHAR *pCmd, UINT32 len)
{
	FLGPTN  uiFlag = 0, waitFlag = 0;
	if (len > MCU2_BUFFER_MAX_LEN) {
		DBG_ERR("Out of cmd length!\r\n");
		return 0;
	}
	MCU2Ctrl_SndLock();
	set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_RCV);

	Delay_DelayUs(550); //tCS = tDS = 550 us (66 bits)

	waitFlag = FLGMCU2UART_RET;
	wai_flg(&uiFlag, FLG_ID_MCU2Ctrl, waitFlag, TWF_ORW | TWF_CLR);
#if 0
	uiSndLen = len;
	memcpy(szSndBuffer, pCmd, uiSndLen + 4);
	if (0) {
		DBG_DUMP("^Csnd ");
		MCU2Ctrl_Dump(szSndBuffer, uiSndLen);
	}
#endif
	MCU2Ctrl_SndUnlock();
	return gSndResult;
}

static TIMER_ID     MCU2Ctrl_TID = 0;
#define TimerMs         1000  //1 ms
#define TimerDelay      1000
static void MCU2Ctrl_TimeOutCB(UINT32 TimerId)
{
	set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_TIMEOUT);
}

static CHAR tmpSndCmd[MCU2_BUFFER_MAX_LEN + 5] = {0};
UINT32 MCU2Ctrl_SendCmd(UINT32 cmd, UINT32 p1, UINT32 p2, UINT32 p3)
{
	UINT32 result = 0;
	UINT32 len = 0;
	UINT32 rdlen = 0;
	UINT32 sdlen = 0;
	CHAR *pCmd;
	UINT32 b_sta;
	UINT32 b_end;

	MCU2Ctrl_Lock();
	pCmd = (CHAR *)tmpSndCmd;
	switch (cmd) {
	case UART_CMD_RESET:
	case UART_CMD_SILICONSIGNATURE:
	case UART_CMD_SECURITYSET:
	case UART_CMD_SECURITYGET:
	case UART_CMD_SECURITYRELEASE:
		pCmd[UART_SND_OFFSET + 0] = cmd;
		len = 1;
		if (cmd == UART_CMD_SILICONSIGNATURE) {
			rdlen = 22;
		}
		if (cmd == UART_CMD_SECURITYGET) {
			rdlen = 8;
		}
		break;
	case UART_CMD_BAUDRATESET:
		len = 1 + 2;
		pCmd[UART_SND_OFFSET + 0] = cmd;
		pCmd[UART_SND_OFFSET + 1] = p1;
		pCmd[UART_SND_OFFSET + 2] = p2;
		break;
	case UART_CMD_BLOCKERASE:
		len = 1 + 3;
		b_sta = p1;
		pCmd[UART_SND_OFFSET + 0] = cmd;
		pCmd[UART_SND_OFFSET + 1] = (b_sta & 0x0000ff);
		pCmd[UART_SND_OFFSET + 2] = (b_sta & 0x00ff00) >> 8;
		pCmd[UART_SND_OFFSET + 3] = (b_sta & 0xff0000) >> 16;
		break;
	case UART_CMD_PROGRAMMING:
	case UART_CMD_VERIFY:
	case UART_CMD_CHECKSUM:
		len = 1 + 6;
		if (cmd == UART_CMD_SILICONSIGNATURE) {
			rdlen = 2;
		}
		if ((cmd == UART_CMD_PROGRAMMING) || (cmd == UART_CMD_VERIFY)) {
			sdlen = MCU_BLOCKSIZE;
		}
		b_sta = p1;
		b_end = p1 + MCU_BLOCKSIZE - 1;
		pCmd[UART_SND_OFFSET + 0] = cmd;
		pCmd[UART_SND_OFFSET + 1] = (b_sta & 0x0000ff);
		pCmd[UART_SND_OFFSET + 2] = (b_sta & 0x00ff00) >> 8;
		pCmd[UART_SND_OFFSET + 3] = (b_sta & 0xff0000) >> 16;
		pCmd[UART_SND_OFFSET + 4] = (b_end & 0x0000ff);
		pCmd[UART_SND_OFFSET + 5] = (b_end & 0x00ff00) >> 8;
		pCmd[UART_SND_OFFSET + 6] = (b_end & 0xff0000) >> 16;
		break;
	case UART_CMD_BLOCKBLANKCHECK:
		len = 1 + 7;
		pCmd[UART_SND_OFFSET + 0] = cmd;
		b_sta = p1;
		b_end = p1 + MCU_BLOCKSIZE - 1;
		pCmd[UART_SND_OFFSET + 1] = (b_sta & 0x0000ff);
		pCmd[UART_SND_OFFSET + 2] = (b_sta & 0x00ff00) >> 8;
		pCmd[UART_SND_OFFSET + 3] = (b_sta & 0xff0000) >> 16;
		pCmd[UART_SND_OFFSET + 4] = (b_end & 0x0000ff);
		pCmd[UART_SND_OFFSET + 5] = (b_end & 0x00ff00) >> 8;
		pCmd[UART_SND_OFFSET + 6] = (b_end & 0xff0000) >> 16;
		pCmd[UART_SND_OFFSET + 7] = UART_PAR_CHECKONE; //always check 1 block
		break;
	}
	pCmd[0] = UART_SND_CMDHEAD;
	pCmd[1] = len;
	pCmd[UART_SND_OFFSET + len] = MCU2Ctrl_Sum(pCmd + UART_SND_OFFSET - 1, len + 1);
	pCmd[UART_SND_OFFSET + len + 1] = UART_SND_CMDTAIL;
	if (0) {
		DBG_DUMP("^Csnd ");
		MCU2Ctrl_Dump(tmpSndCmd, len);
	}
	result = MCU2Ctrl_SendTsk(tmpSndCmd, len);
	//if(result != UART_RET_ACK)
	//    DBG_ERR("send cmd %2X fail, result %d\r\n",cmd,result);
	MCU2Ctrl_Unlock();

	if (rdlen != 0) { //receive data
		MCU2Ctrl_RecvData(0, rdlen);
		if (cmd == UART_CMD_CHECKSUM) { //return 2 bytes
			Delay_DelayMs(135);    //tSD = 1350000 us (48+15564 bits)
		} else if (cmd == UART_CMD_SECURITYGET) { //return 8 bytes
			Delay_DelayUs(1200);    //tSD = 1200 us (139 bits)
		} else if (len == UART_CMD_SILICONSIGNATURE) { //return 22 bytes
			Delay_DelayMs(3);    //tSD = 3000 us (340 bits)
		}
	}

	if (sdlen != 0) { //send data
		MCU2Ctrl_SendData((CHAR *)p3, MCU_BLOCKSIZE); //pData size = 256 bytes, return 2 bytes
		if (cmd == UART_CMD_PROGRAMMING) {
			Delay_DelayMs(12);    //tSS = 12000 us (1287 bits)
		}
	}

	if (cmd == UART_CMD_BAUDRATESET) {
		Delay_DelayUs(67);    //tSNx = 67 us
	} else {
		Delay_DelayUs(550);    //tSNx = 550 us (66 bits)
	}

	return result;
}

UINT32 MCU2Ctrl_SendData(CHAR *pData, UINT32 len)
{
	UINT32 result = 0;
	CHAR *pCmd;
	UINT32 clen = len;
	CHAR *cData = pData;
	MCU2Ctrl_Lock();
	pCmd = (CHAR *)tmpSndCmd;
	while (clen > 0) {
		UINT32 dlen = (clen <= 256) ? (clen) : (256);
		//DBG_DUMP("begin data %d %d\r\n", clen, dlen);
		pCmd[0] = UART_SND_DATAHEAD;
		pCmd[1] = dlen;
		memcpy(pCmd + UART_SND_OFFSET, cData, dlen);
		pCmd[UART_SND_OFFSET + dlen] = MCU2Ctrl_Sum(pCmd + UART_SND_OFFSET, dlen);
		if (clen <= 256) {
			pCmd[UART_SND_OFFSET + dlen + 1] = (UART_SND_DATATAIL);
		} else {
			pCmd[UART_SND_OFFSET + dlen + 1] = (UART_SND_DATANEXT);
		}
		if (0) {
			DBG_DUMP("^Csnd ");
			MCU2Ctrl_Dump(tmpSndCmd, dlen);
		}
		result = MCU2Ctrl_SendTsk(tmpSndCmd, dlen);
		//if(result != UART_RET_ACK)
		//    DBG_ERR("send data len(%d) fail,result %d\r\n",len,result);

		Delay_DelayUs(550); //tSD = 550 us (66 bits)

		cData += dlen;
		clen -= dlen;
		//DBG_DUMP("end data %d\r\n", clen);
	}
	MCU2Ctrl_Unlock();

	return result;
}

UINT32 MCU2Ctrl_RecvData(CHAR *pData, UINT32 len)
{
	UINT32 result = 0;

	MCU2Ctrl_Lock();
	result = MCU2Ctrl_RecvTsk(pData, len);
	//if(result != UART_RET_ACK)
	//    DBG_ERR("receive data fail, result %d\r\n",result);
	MCU2Ctrl_Unlock();

	return result;
}

ER MCU2Ctrl_Open(void)
{
	DBG_FUNC_BEGIN("\r\n");
	if (g_bMCU2CtrlOpened) {
		DBG_ERR("already opened!!! \r\n");
		return E_SYS;
	}

	DxMCU_Setcfgs(DXMCU_CFG_UART_BAUDRATE, UART_BAUDRATE_115200);
	DxMCU_Setcfgs(DXMCU_CFG_UART_LENGTH, UART_LEN_L8_S2);

	DxMCU_Open();

	// Start MCU2 Task
	sta_tsk(MCU2CtrlRcvTsk_ID, 0);
	sta_tsk(MCU2CtrlSndTsk_ID, 0);
	g_bMCU2CtrlOpened = TRUE;

	if (timer_open(&MCU2Ctrl_TID, MCU2Ctrl_TimeOutCB) != E_OK) {
		DBG_ERR("open timer fail\r\n");
	}
	timer_cfg(MCU2Ctrl_TID, TimerDelay * TimerMs, TIMER_MODE_FREE_RUN | TIMER_MODE_ENABLE_INT, TIMER_STATE_PAUSE);

	DBG_FUNC_END("\r\n");
	return E_OK;
}

ER MCU2Ctrl_Close(void)
{
	DBG_FUNC_BEGIN("\r\n");
	FLGPTN  uiFlag = 0;

	if (g_bMCU2CtrlOpened == FALSE) {
		DBG_ERR("already closed!!! \r\n");
		return E_SYS;
	}

	bRcvStart = FALSE;
	//#NT#2017/01/10#HM Tseng -begin
	//#NT#abort to exit get
	DxMCU_Get(0);
	//#NT#2017/01/10#HM Tseng -end

	set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_STOP);
	wai_flg(&uiFlag, FLG_ID_MCU2Ctrl, FLGMCU2UART_STOPED, TWF_ORW | TWF_CLR);
	timer_close(MCU2Ctrl_TID);
	DxMCU_Close();

	g_bMCU2CtrlOpened = FALSE;

	DBG_FUNC_END("\r\n");
	return E_OK;
}

void MCU2CtrlSndTsk(void)
{
	FLGPTN  uiFlag = 0;
	BOOL    bContinue = 1;
	INT32   result = 0;
	kent_tsk();

	DBG_FUNC_BEGIN("\r\n");
	clr_flg(MCU2Ctrl_TID, 0xFFFFFFFF);
	while (bContinue) {
		wai_flg(&uiFlag, FLG_ID_MCU2Ctrl, FLGMCU2UART_SND | FLGMCU2UART_RCV | FLGMCU2UART_STOP, TWF_ORW | TWF_CLR);

		if (uiFlag & FLGMCU2UART_STOP) {
			bContinue = 0;
			continue;
		} else if (uiFlag & (FLGMCU2UART_SND | FLGMCU2UART_RCV)) {
			static UINT32 reSendCount = 0;
			uiSndRet = 0;
			if (uiFlag & FLGMCU2UART_SND) {
				result = MCU2Ctrl_Send(szSndBuffer, uiSndLen);
			} else { //(uiFlag & FLGMCU2UART_RCV)
				szSndBuffer[0] = 0; //NO HEAD
				result = E_OK;
			}

			if (result == E_OK) {
				FLGPTN  uiFlag = 0, waitFlag = 0;
				reSendCount = 0;
				while (1) {
					timer_pausePlay(MCU2Ctrl_TID, TIMER_STATE_PLAY);
					waitFlag = (FLGMCU2UART_ACK | FLGMCU2UART_TIMEOUT);
					clr_flg(FLG_ID_MCU2Ctrl, waitFlag);
					wai_flg(&uiFlag, FLG_ID_MCU2Ctrl, waitFlag, TWF_ORW | TWF_CLR);

					if (uiFlag == FLGMCU2UART_ACK) {
						timer_pausePlay(MCU2Ctrl_TID, TIMER_STATE_PAUSE);
						result = uiSndRet;
						break;
					} else {
						if (szSndBuffer[0] == UART_SND_CMDHEAD) {
							DBG_WRN("[retry] send cmd %02X again, count %d\r\n", szSndBuffer[UART_SND_OFFSET], reSendCount);
						} else if (szSndBuffer[0] == UART_SND_DATAHEAD) {
							DBG_WRN("[retry] send data again, count %d\r\n", reSendCount);
						} else if (szSndBuffer[0] == 0) {
							DBG_WRN("[retry] receive data again, count %d\r\n", reSendCount);
						}

						if (szSndBuffer[0] != 0) {
							MCU2Ctrl_Send(szSndBuffer, uiSndLen);
						}
						reSendCount++;

						if (reSendCount >= MCU2_RETRY_CNT) {
							timer_pausePlay(MCU2Ctrl_TID, TIMER_STATE_PAUSE);
							if (szSndBuffer[0] == UART_SND_CMDHEAD) {
								DBG_ERR("send cmd %02X no ack\r\n", szSndBuffer[UART_SND_OFFSET]);
							} else if (szSndBuffer[0] == UART_SND_DATAHEAD) {
								DBG_ERR("send data no ack\r\n");
							} else if (szSndBuffer[0] == 0) {
								DBG_ERR("receive data no ack\r\n");
							}
							result = UART_RET_NACK;
							break;
						}
					}
				}
			}
			gSndResult = result ;
			set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_RET);
		}
	}
	set_flg(FLG_ID_MCU2Ctrl, FLGMCU2UART_STOPED);

	DBG_FUNC_END("\r\n");
	ext_tsk();
}


//----- MCU2CtrlRcvTsk  ------------------
void MCU2CtrlRcvTsk(void)
{
	UINT32  cLen = 0;
	UINT32  len = 0;

	kent_tsk();
	DBG_FUNC_BEGIN("[TSK]\r\n");
	bRcvStart = 1;
	while (bRcvStart) {
		DxMCU_Get((CHAR *)&szRcvBuffer[cLen]);
		cLen++;

		if (cLen < 2) {
		} else if (cLen == 2) {
			len = szRcvBuffer[1];
		} else { //cLen > 2
			uiRcvLen = cLen;
			if (cLen == len + 4) {
				MCU2Ctrl_Recv(szRcvBuffer, len);
				cLen = 0;
				continue;
			} else if (cLen == MCU2_BUFFER_MAX_LEN - 1) {
				DBG_ERR("rcv cmd len is too large!\r\n");
				cLen = 0;
				continue;
			}
		}
	}

	DBG_FUNC_END("[TSK]\r\n");
	ext_tsk();
}
/*
void MCU2Ctrl_DebugEnable(UINT32 enable)
{
    dbg_msg = enable;
}
*/

extern ER MCUCtrl_Close(void);

UINT32 MCU2Ctrl_UpdateFW(CHAR *pBuf, UINT32 len)
{
	CHAR *pData;
	//CHAR* pData2;
	UINT32 clen, coff, dlen;
	//UINT32 plen, qlen;
	UINT32 b_sta, b_end;

	//Shutdown Normal MCU operation!
	MCUCtrl_Close();
	Delay_DelayMs(1000);

	// UART (Universal Asynchronous Receiver Transmitter)
	//
	// Buad rate = 115200
	// Start bits     = 1 (fixed)
	// Data bits      = 8
	// Parity bits    = 0 (optional)
	// Stop bits      = 1
	// => Total bits  = 10
	//
	// Performance = (1000000 / (Buad_rate/Total_bits)) us
	// => Performance = (1000000 / (115200/1)) = 8.68 us/bit
	// => Performance = (1000000 / (115200/10)) = 86.8 us/char

	//NOTE:

	//this procedure is following the spec in MCU document "RL78 Protocol A"
	//but value of #RESET pin is reversed, value of #TOOL0 pin is also reversed.

	//DBG_DUMP("app target - begin\r\n");
	gpio_setDir(GPIO_MCU_RESET, GPIO_DIR_OUTPUT);
	gpio_setDir(GPIO_MCU_TOOL0, GPIO_DIR_OUTPUT);

	gpio_setPin(GPIO_MCU_RESET);
	gpio_setPin(GPIO_MCU_TOOL0);
	//expect VDD is on
	//Delay_DelayUs(9); //tTR = 8.68 us + 135 ns
	Delay_DelayMs(2); //tTR = 8.68 us + 135 ns
	gpio_clearPin(GPIO_MCU_RESET);
	//Delay_DelayUs(724); //tRT = 723 us + 305 ns
	Delay_DelayMs(2); //tRT = 723 us + 305 ns
	gpio_clearPin(GPIO_MCU_TOOL0);
	Delay_DelayUs(16); //tTM = 16 us
	//DBG_DUMP("app target - end\r\n");

	//------------------------------
	//DBG_DUMP("mode setting - begin\r\n");
	gpio_setPin(GPIO_MCU_TOOL0);
	//Delay_DelayUs(87);//tM = 86.8 us (send 1 char 0x00 to select "Two-wire UART Communication Mode")
	Delay_DelayUs(78);//tM = 78.12 us - 9 bit 0 (send 1 char 0x00 to select "Two-wire UART Communication Mode")
	gpio_clearPin(GPIO_MCU_TOOL0);
	Delay_DelayUs(18); //tM = 17.36 us - 2 bit 1
	//Delay_DelayUs(62); //tMB = 62 us
	Delay_DelayUs(62); //tMB = 62 us
	//DBG_DUMP("mode setting - end\r\n");

	//------------------------------

	//DBG_DUMP("uart open - begin\r\n");
	MCU2Ctrl_Open(); //Must start with Baudrate 115200 for MCU
	//DBG_DUMP("uart open - end\r\n");
	//------------------------------

	DBG_DUMP("set BAUDRATE\r\n");
	MCU2Ctrl_SendCmd(UART_CMD_BAUDRATESET, UART_PAR_BAUDRATESET_115200, UART_PAR_BAUDRATESET_V211, 0);
	DBG_DUMP("get SIGNATURE\r\n");
	MCU2Ctrl_SendCmd(UART_CMD_SILICONSIGNATURE, 0, 0, 0); //return 22 bytes
#if 1
	DBG_DUMP("write BLOCK\r\n");
	b_sta = 0;
	b_end = 0;
	pData = pBuf;
	clen = len;
	coff = 0;
	for (; clen > 0;) { // total 16K, each block size = 1K bytes
		UINT32 r;
		if (clen > MCU_BLOCKSIZE) {
			dlen = MCU_BLOCKSIZE;
		} else {
			dlen = clen;
		}
		DBG_DUMP("write BLOCK %d [%06X~%06X]\r\n", b_end, coff, coff + dlen - 1);
		//DBG_DUMP("C");
		DBG_DUMP("- is BLANK\r\n");
		r = MCU2Ctrl_SendCmd(UART_CMD_BLOCKBLANKCHECK, coff, 0, 0);
		if (r == UART_RET_INTERNALERR) { //not blank, need to do erase
			//DBG_DUMP("\x1B[DE");
			DBG_DUMP("- do ERASE\r\n");
			MCU2Ctrl_SendCmd(UART_CMD_BLOCKERASE, coff, 0, 0);
		}
		//DBG_DUMP("\x1B[DP");
		DBG_DUMP("- do PROGRAMMING\r\n");
		MCU2Ctrl_SendCmd(UART_CMD_PROGRAMMING, coff, 0, (UINT32)pData);
		//DBG_DUMP("\x1B[DV");
		DBG_DUMP("- do VERIFY\r\n");
		MCU2Ctrl_SendCmd(UART_CMD_VERIFY, coff, 0, (UINT32)pData);
		//DBG_DUMP("\x1B[DW");
		pData += dlen;
		coff += dlen;
		clen -= dlen;
		b_end++;
	}
	b_end--;
	DBG_DUMP("finish!\r\n");
#endif
#if 0
	{
		UINT32 b_boot;

		DBG_DUMP("set SECURITY\r\n");
		static CHAR szSecuritySet[8] = {0};
		MCU2Ctrl_SendCmd(UART_CMD_SECURITYSET, 0, 0, 0);
		// total 16 blocks
		b_boot = 4; //boot loader is fixed in first 4 blocks (0~3) <--- cause f/w symbol address is start from 0x010000
		b_start = 4;
		b_end = 15;  //code/data is located other blocks (4~15)
		szSecuritySet[0] = 0xff; //FLG: Security flag, 0xff = allow all
		szSecuritySet[1] = b_boot; //BOT: Boot block cluster block number
		szSecuritySet[2] = (b_sta & 0x00ff);
		szSecuritySet[3] = (b_sta & 0xff00) >> 8;
		szSecuritySet[4] = (b_end & 0x00ff);
		szSecuritySet[5] = (b_end & 0xff00) >> 8;
		szSecuritySet[6] = 0;
		szSecuritySet[7] = 0;
		MCU2Ctrl_SendData(szSecuritySet, 8);
	}
#endif

	//------------------------------

	//DBG_DUMP("uart close - begin\r\n");
	MCU2Ctrl_Close();
	//DBG_DUMP("uart close - end\r\n");

	//------------------------------

	//DBG_DUMP("shutdown target - begin\r\n");
	Delay_DelayUs(10); //add by jeah
	gpio_setPin(GPIO_MCU_RESET);
	//expect VDD will off
	//DBG_DUMP("shutdown target - end\r\n");

	return 1;
}



#else
void MCU2CtrlSndTsk(void)
{

}
void MCU2CtrlRcvTsk(void)
{
}
UINT32 MCU2Ctrl_SendCmd(UINT32 cmd, UINT32 p1, UINT32 p2, UINT32 p3)
{
	return 0;
}
UINT32 MCU2Ctrl_SendData(CHAR *pData, UINT32 len)
{
	return 0;
}
UINT32 MCU2Ctrl_RecvData(CHAR *pData, UINT32 len)
{
	return 0;
}
/*
void MCU2Ctrl_DebugEnable(UINT32 enable)
{
}
*/
UINT32 MCU2Ctrl_UpdateFW(CHAR *pBuf, UINT32 len)
{
	return 0;
}
#endif

#if 0
static BOOL Cmd_MCU2_debug(CHAR *strCmd)
{
	UINT32 bEnable = 0;

	sscanf(strCmd, "%d", &bEnable);

	MCU2Ctrl_DebugEnable(bEnable);

	return TRUE;
}
#endif

