/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       MCUCtrlTsk.c
    @ingroup

    @brief      MCU command via DxMCU interface.


    @note       Nothing.

    @version    V1.00.000
*/
#include <stdio.h>
#include "SxCmd.h"
#include "DxCfg.h"
#include "IOCfg.h"
#include "MCUCtrl.h"
#include "MCUCtrl_int.h"

#if defined(_MCU_ENABLE_)
#include "DxMCU.h"
#include "DxCommon.h"
#include "DxApi.h"
#include "uart_cmd.h"
#include "timer.h"
#include "DxPower.h"
#include "uart.h"


///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          MCUCtrl
#define __DBGLVL__          2 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define MCU_CMD_MAX_LEN      256

#define FLGMCUCMD_ACK           FLGPTN_BIT(0)
#define FLGMCUCMD_RSP           FLGPTN_BIT(1)
#define FLGMCUCMD_TIMEOUT       FLGPTN_BIT(2)
#define FLGMCUCMD_SND           FLGPTN_BIT(3)
#define FLGMCUCMD_RET           FLGPTN_BIT(4)
#define FLGMCUCMD_STOP          FLGPTN_BIT(5)
#define FLGMCUCMD_STOPED        FLGPTN_BIT(6)

#define CHK_MCU_FW_VER          DISABLE
#define MCU_FW_HEADER           0xD002
#define SKIP_SEND_CMD           DISABLE

//----- External funcs & variables declaration here  ------------------
#define MCU_RETRY_CNT    15
static BOOL g_bMCUCtrlOpened = FALSE;
static char szCmd[MCU_CMD_MAX_LEN] = {0};
static MCU_CMD_FMT gTskSndCmd = {0};
static UINT32 resp = 0;
static UINT32 gResult = 0;
static UINT32 bReceive = 0;
static UINT32 dbg_msg = FALSE;

#if (SKIP_SEND_CMD==ENABLE)
static UINT32 skip_snd = FALSE;
#endif

UINT32 MCUCtrl_Send(MCU_CMD_FMT *pSndCmd);
static CHAR MCUCtrl_Sum(MCU_CMD_FMT *pCmd)
{
	UINT32 i = 0;
	UINT32 sum = 0;
	UINT8 *pValue = (UINT8 *)pCmd;
	for (i = 0; i < sizeof(MCU_CMD_FMT); i++) {
		sum += *pValue;
		pValue++;
	}
	pCmd->m_sum = sum;
	return sum;
}

static UINT32 MCUCtrl_SendCmdDirect(UINT32 cmd, UINT32 param1, UINT32 param2, UINT32 param3)
{
	MCU_CMD_FMT sndCmd = {0};
	UINT32 result = 0;


	sndCmd.m_head = UART_CMD_HEAD;
	sndCmd.m_version = UART_CMD_VER;
	sndCmd.m_cmd = cmd;
	sndCmd.m_param1 = param1;
	sndCmd.m_param2 = param2;
	sndCmd.m_param3 = param3;
	sndCmd.m_sum = 0;
	sndCmd.m_tail = UART_CMD_TAIL;
	MCUCtrl_Sum(&sndCmd);

	MCUCtrl_Send(&sndCmd);
	return result;
}
static INT32 MCUCtrl_SendTsk(MCU_CMD_FMT *pCmd)
{
	FLGPTN  uiFlag = 0, waitFlag = 0;

	memcpy((CHAR *)&gTskSndCmd, pCmd, sizeof(MCU_CMD_FMT));
	set_flg(FLG_ID_MCUCtrl, FLGMCUCMD_SND);
	waitFlag = FLGMCUCMD_RET;
	wai_flg(&uiFlag, FLG_ID_MCUCtrl, waitFlag, TWF_ORW | TWF_CLR);
	return gResult;
}
static void MCUCtrl_Lock(void)
{
	DBG_IND("Lock\r\n");
	wai_sem(MCUCtrlTsk_SEM_ID);
}
static void MCUCtrl_Unlock(void)
{
	DBG_IND("Unlock\r\n");
	sig_sem(MCUCtrlTsk_SEM_ID);
}

static void MCUCtrl_SndLock(void)
{
	DBG_IND("Lock\r\n");
	wai_sem(MCUSNDCMD_SEM_ID);
}
static void MCUCtrl_SndUnlock(void)
{
	DBG_IND("Unlock\r\n");
	sig_sem(MCUSNDCMD_SEM_ID);
}

UINT32 MCUCtrl_Send(MCU_CMD_FMT *pSndCmd)
{
	UINT32 result;

	MCUCtrl_SndLock();
	result = DxMCU_Put((CHAR *)pSndCmd, sizeof(MCU_CMD_FMT));
	MCUCtrl_SndUnlock();

	return result;
}


static UINT32 MCUCtrl_Recv(MCU_CMD_FMT *pCmd)
{
	if (pCmd) {
		if ((pCmd->m_head == UART_CMD_HEAD) && (pCmd->m_tail == UART_CMD_TAIL)) {
			if (dbg_msg) {
				DBG_DUMP("#### rvc cmd 0x%02X ,0x%X ,0x%X ,0x%X #### \r\n", pCmd->m_cmd, pCmd->m_param1, pCmd->m_param2, pCmd->m_param3);
			}

			switch (pCmd->m_cmd) {
			case UART_CMD_MCU_ACK:
				set_flg(FLG_ID_MCUCtrl, FLGMCUCMD_ACK);
				break;
			case UART_CMD_POWER_ON_RESON_RSP:
				resp = pCmd->m_param1;
				set_flg(FLG_ID_MCUCtrl, FLGMCUCMD_RSP);
				break;
			case UART_CMD_FW_VERSION_RSP:
				resp = ((int)pCmd->m_param3) | (((int)pCmd->m_param2) << 8) | (((int)pCmd->m_param1) << 16);
				set_flg(FLG_ID_MCUCtrl, FLGMCUCMD_RSP);
				break;
			case UART_CMD_VBAT_DETECT_RSP:
				Dx_SetState(Dx_GetObject(DX_CLASS_POWER_EXT), DRVPWR_CTRL_BATTERY_ADC_VALUE, pCmd->m_param1);
				break;
			default:
				break;

			}

			//ack at the end
			if (pCmd->m_cmd != UART_CMD_MCU_ACK) {
				MCUCtrl_SendCmdDirect(UART_CMD_NVT_ACK, 0, 0, 0);
			}
			//MCUCtrl_SendCmd(UART_CMD_NVT_ACK,0,0,0);

		}
	}

	return 0;
}
static TIMER_ID     MCUCtrl_TID = 0;
#define TimerMs         1000  //1 ms
#define TimerDelay      300
static void MCUCtrl_TimeOutCB(UINT32 TimerId)
{
	set_flg(FLG_ID_MCUCtrl, FLGMCUCMD_TIMEOUT);
}

UINT32 MCUCtrl_SendCmd(UINT32 cmd, UINT32 param1, UINT32 param2, UINT32 param3)
{
	static MCU_CMD_FMT sndCmd = {0};
	UINT32 result = 0;

#if (SKIP_SEND_CMD==ENABLE)
	if (skip_snd) {
		return 0xFFFFFFFF;
	}
#endif

	MCUCtrl_Lock();

	sndCmd.m_head = UART_CMD_HEAD;
	sndCmd.m_version = UART_CMD_VER;
	sndCmd.m_cmd = cmd;
	sndCmd.m_param1 = param1;
	sndCmd.m_param2 = param2;
	sndCmd.m_param3 = param3;
	sndCmd.m_sum = 0;
	sndCmd.m_tail = UART_CMD_TAIL;
	MCUCtrl_Sum(&sndCmd);

	if (dbg_msg) {
		DBG_DUMP("#### snd cmd 0x%02X ,0x%X ,0x%X ,0x%X #### \r\n", cmd, param1, param2, param3);
	}
	result = MCUCtrl_SendTsk(&sndCmd);
	if (result < 0) {
		DBG_ERR("cmd %d,result %d\r\n", cmd, result);
	}
	MCUCtrl_Unlock();

	return result;
}

ER MCUCtrl_Open(void)
{
	DBG_FUNC_BEGIN("\r\n");
	if (g_bMCUCtrlOpened) {
		DBG_ERR("already opened!!! \r\n");
		return E_SYS;
	}

	DxMCU_Setcfgs(DXMCU_CFG_UART_BAUDRATE, UART_BAUDRATE_57600);
	DxMCU_Setcfgs(DXMCU_CFG_UART_LENGTH, UART_LEN_L8_S1);

	DxMCU_Open();

	// Start MCU Task
	sta_tsk(MCUCtrlRcvTsk_ID, 0);
	sta_tsk(MCUCtrlSndTsk_ID, 0);
	g_bMCUCtrlOpened = TRUE;

#if (SKIP_SEND_CMD==ENABLE)
	skip_snd = FALSE;
#endif

	if (timer_open(&MCUCtrl_TID, MCUCtrl_TimeOutCB) != E_OK) {
		DBG_ERR("open timer fail\r\n");
	}
	timer_cfg(MCUCtrl_TID, TimerDelay * TimerMs, TIMER_MODE_FREE_RUN | TIMER_MODE_ENABLE_INT, TIMER_STATE_PAUSE);

#if (CHK_MCU_FW_VER == ENABLE)
	UINT32 result = 0;
	result = MCUCtrl_SendCmd(UART_CMD_FW_VERSION, 0, 0, 0);
	if ((result >> 8) != MCU_FW_HEADER) {
		DBG_ERR("MCU FW version is not match = 0x%x\r\n", result);
#if (SKIP_SEND_CMD==ENABLE)
		DBG_ERR("Stop sending command to MCU!\r\n", result);
		skip_snd = TRUE;
#endif
	}
#endif

	DBG_FUNC_END("\r\n");
	return E_OK;
}

ER MCUCtrl_Close(void)
{
	DBG_FUNC_BEGIN("\r\n");
	FLGPTN  uiFlag = 0;

	if (g_bMCUCtrlOpened == FALSE) {
		DBG_ERR("already closed!!! \r\n");
		return E_SYS;
	}

	bReceive = FALSE;
	//#NT#2017/01/10#HM Tseng -begin
	//#NT#abort to exit get
	DxMCU_Get(0);
	//#NT#2017/01/10#HM Tseng -end

	set_flg(FLG_ID_MCUCtrl, FLGMCUCMD_STOP);
	wai_flg(&uiFlag, FLG_ID_MCUCtrl, FLGMCUCMD_STOPED, TWF_ORW | TWF_CLR);
	timer_close(MCUCtrl_TID);
	DxMCU_Close();

	g_bMCUCtrlOpened = FALSE;

	DBG_FUNC_END("\r\n");
	return E_OK;
}

void MCUCtrlSndTsk(void)
{
	FLGPTN  uiFlag = 0;
	BOOL    bContinue = 1;
	INT32   result = 0;
	kent_tsk();

	DBG_FUNC_BEGIN("\r\n");
	clr_flg(MCUCtrl_TID, 0xFFFFFFFF);
	while (bContinue) {
		wai_flg(&uiFlag, FLG_ID_MCUCtrl, FLGMCUCMD_SND | FLGMCUCMD_STOP, TWF_ORW | TWF_CLR);

		if (uiFlag & FLGMCUCMD_STOP) {
			bContinue = 0;
			continue;
		} else if (uiFlag & FLGMCUCMD_SND) {
			static UINT32 reSendCount = 0;
			MCU_CMD_FMT *pSndCmd = &gTskSndCmd;
			clr_flg(FLG_ID_MCUCtrl, FLGMCUCMD_ACK);
			result = MCUCtrl_Send(pSndCmd);

			if ((result == E_OK) && (pSndCmd->m_cmd != UART_CMD_NVT_ACK)) {
				FLGPTN  uiFlag = 0, waitFlag = 0;
				reSendCount = 0;
				while (1) {
					timer_pausePlay(MCUCtrl_TID, TIMER_STATE_PLAY);
					waitFlag = (FLGMCUCMD_ACK | FLGMCUCMD_TIMEOUT);
					wai_flg(&uiFlag, FLG_ID_MCUCtrl, waitFlag, TWF_ORW | TWF_CLR);
					if (uiFlag == FLGMCUCMD_ACK) {
						timer_pausePlay(MCUCtrl_TID, TIMER_STATE_PAUSE);
						result = MCUCTRL_RET_OK;
						break;
					} else {
						DBG_IND("send again %x count %d\r\n", pSndCmd->m_cmd, reSendCount);
						MCUCtrl_Send(pSndCmd);
						reSendCount++;

						if (reSendCount >= MCU_RETRY_CNT) {
							timer_pausePlay(MCUCtrl_TID, TIMER_STATE_PAUSE);
							DBG_ERR("cmd %x no ack\r\n", pSndCmd->m_cmd);
							result = MCUCTRL_RET_NO_ACK;
#if (SKIP_SEND_CMD==ENABLE)
							DBG_ERR("Stop sending command to MCU!\r\n", pSndCmd->m_cmd);
							skip_snd = TRUE;
#endif
							break;
						}

					}
				}
			}

			//wait power on reson response
			if ((result == MCUCTRL_RET_OK) && (pSndCmd->m_cmd == UART_CMD_POWER_ON_REASON)) {
				FLGPTN  uiFlag = 0, waitFlag = 0;

				waitFlag = (FLGMCUCMD_RSP);
				DBG_IND("wait rsp\r\n");
				wai_flg(&uiFlag, FLG_ID_MCUCtrl, waitFlag, TWF_ORW | TWF_CLR);
				result = resp ;
			}
			//wait fw version response
			if ((result == MCUCTRL_RET_OK) && (pSndCmd->m_cmd == UART_CMD_FW_VERSION)) {
				FLGPTN  uiFlag = 0, waitFlag = 0;

				waitFlag = (FLGMCUCMD_RSP);
				DBG_IND("wait rsp\r\n");
				wai_flg(&uiFlag, FLG_ID_MCUCtrl, waitFlag, TWF_ORW | TWF_CLR);
				result = resp ;
			}

			gResult = result ;
			set_flg(FLG_ID_MCUCtrl, FLGMCUCMD_RET);
		}
	}
	set_flg(FLG_ID_MCUCtrl, FLGMCUCMD_STOPED);

	DBG_FUNC_END("\r\n");
	ext_tsk();
}


//----- MCUCtrlRcvTsk  ------------------
void MCUCtrlRcvTsk(void)
{
	UINT32  cLen = 0;

	kent_tsk();
	DBG_FUNC_BEGIN("[TSK]\r\n");
	bReceive = 1;
	/* start up messages */
	while (bReceive) {
		DxMCU_Get((CHAR *)&szCmd[cLen]);
		//DBGH(szCmd[cLen]);

		//#NT#2017/05/05#HM Tseng -begin
		//#NT# receive remained command after received header
		if (cLen == 0 && szCmd[cLen] != UART_CMD_HEAD) {
			continue;
		}
		//#NT#2017/05/05#HM Tseng -end

		if ((cLen >= sizeof(MCU_CMD_FMT) - 1) && (szCmd[cLen] == UART_CMD_TAIL)) {
			if (cLen == sizeof(MCU_CMD_FMT) - 1) {
				MCUCtrl_Recv((MCU_CMD_FMT *)&szCmd[0]);
			}
			/*
			else
			{
			    DBG_WRN("cLen %d error\r\n",cLen);
			}
			*/
			cLen = 0;
			continue;
		}
		cLen++;
		if (cLen >= MCU_CMD_MAX_LEN) {
			DBG_ERR("command is larger than %d characters\r\n", MCU_CMD_MAX_LEN - 1);
			cLen = 0;
		}
	}

	DBG_FUNC_END("[TSK]\r\n");
	ext_tsk();


}
void MCUCtrl_DebugEnable(UINT32 enable)
{
	dbg_msg = enable;
}


#else
void MCUCtrlSndTsk(void)
{

}
void MCUCtrlRcvTsk(void)
{
}
UINT32 MCUCtrl_SendCmd(UINT32 cmd, UINT32 param1, UINT32 param2, UINT32 param3)
{
	return 0;
}
void MCUCtrl_DebugEnable(UINT32 enable)
{
}
#endif

#if defined(_MCU_ENABLE_)
static BOOL Cmd_MCU_test(CHAR *strCmd)
{
	UINT32 cmd;
	UINT32 par1, par2, par3;

	sscanf(strCmd, "%x %x %x %x", &cmd, &par1, &par2, &par3);

	DBGH(cmd);
	DBGH(par1);
	DBGH(par2);
	DBGH(par3);

	MCUCtrl_SendCmd(cmd, par1, par2, par3);

	return TRUE;
}

static BOOL Cmd_MCU_debug(CHAR *strCmd)
{
	UINT32 bEnable = 0;

	sscanf(strCmd, "%d", &bEnable);

	MCUCtrl_DebugEnable(bEnable);

	return TRUE;
}

static BOOL Cmd_MCU_pir(CHAR *strCmd)
{
	UINT32 bEnable = 0;

	sscanf(strCmd, "%d", &bEnable);

	Dx_SetState(Dx_GetObject(DX_CLASS_POWER_EXT), DRVPWR_CTRL_PIR_EN, bEnable);

	return TRUE;
}

#if 0
#include "FileSysTsk.h"
#define MCU_UPDATE_NAME "A:\\R5F117BA.bin"
static CHAR MCU_FILENAME[] = MCU_UPDATE_NAME;

static BOOL Cmd_MCU_update(CHAR *strCmd)
{
	UINT32  tmpBuf = SxCmd_GetTempMem(100000);
    UINT32  tmpBufSize = 100000;
    FST_FILE    pFile;

	if (!tmpBuf)
    {
        DBG_ERR("get buffer fail\r\n");
        return FALSE;
    }

    pFile = FileSys_OpenFile(MCU_FILENAME, FST_OPEN_READ);
    DBG_DUMP("^Yload mcu file\r\n");
    if (!pFile)
    {
        DBG_ERR("open file fail\r\n");
        return FALSE;
    }
    FileSys_ReadFile(pFile, (UINT8 *)tmpBuf, &tmpBufSize, 0, NULL);
    DBG_DUMP("^Ysize of mcu file = %d\r\n", tmpBufSize);
    if(tmpBufSize > 100000)
    {
        DBG_ERR("size is too large\r\n");
        return FALSE;
    } else if (tmpBufSize < 10000) {
		DBG_ERR("size is too small\r\n");
        return FALSE;
    }
    FileSys_CloseFile(pFile);
    DBG_DUMP("^Yupdate mcu file - begin\r\n");
    if(!MCU2Ctrl_UpdateFW((CHAR*)tmpBuf, tmpBufSize))
    {
        DBG_ERR("update fail\r\n");
        return FALSE;
    }
	return TRUE;
}
#endif
#endif

SXCMD_BEGIN(mcu, "mcu cmd")
#if defined(_MCU_ENABLE_)
SXCMD_ITEM("cmd %", Cmd_MCU_test, "MCU test cmd")
SXCMD_ITEM("dbg %", Cmd_MCU_debug, "on/off MCU debug msg")
SXCMD_ITEM("pir %", Cmd_MCU_pir, "on/off MCU PIR")
#endif
SXCMD_END()


void MCU_InstallCmd(void)
{
	SxCmd_AddTable(mcu);
}

