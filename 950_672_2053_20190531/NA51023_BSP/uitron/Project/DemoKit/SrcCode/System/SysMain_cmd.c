//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"

#include "SysCfg.h"
#include "SysCommon.h"

#include "AppLib.h"

#if (UI_FUNC == ENABLE)
#include "UISetup.h"
#endif

#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "nvtmpp.h"
#include "ProjectInfo.h"
#include "SysMain.h"

#if (defined(_MODEL_DVCAM1_EVB_) || defined(_MODEL_DVCAM2_EVB_) || defined(_MODEL_DVCAM_64M_EVB_))
#include "UIAppMovie.h"
#endif

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysMainCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// sys state
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "PStore.h"
#include "FileSysTsk.h"
//#include "DxPower.h"

CHAR sys_usage_str[17] = {0};
CHAR sys_usage_str2[17] = {0};
int sys_dump_cnt = 0;
BOOL sys_keeptop = FALSE;
BOOL sys_dotrace = FALSE;

void sys_usage_dump(BOOL bTrace)
{
	//fix for CID 62610,62611 - begin
	UINT32 dma_usage = 0;
	UINT32 cpu_usage = 0;
	//fix for CID 62610,62611 - end
	UINT32 mem_free, mem_total;
	#if (PST_FUNC == ENABLE)
	UINT32 stg_pst_free, stg_pst_total;
	#endif
	#if (FS_FUNC == ENABLE)
	UINT64 stg_fat_free, stg_fat_total;
	#endif
	//UINT32 pwr_lvl_usage;
	//UINT32 pwr_vot_usage;
	//UINT32 pwr_tmp_usage;
	profile_get(PROFILE_DMA, &dma_usage);
	if (dma_usage > 100) {
		dma_usage = 0;
	}
	profile_get(PROFILE_CPU, &cpu_usage);
	if (cpu_usage > 100) {
		cpu_usage = 0;
	}
	mem_free = SxCmd_GetTempMem(TEMPBUF_GET_MAXSIZE); //not release,it get size
	mem_total = OS_GetMemSize(MEM_TOTAL);
	#if (PST_FUNC == ENABLE)
	stg_pst_free = PStore_GetInfo(PS_INFO_FREE_SPACE);
	stg_pst_total = PStore_GetInfo(PS_INFO_TOT_SIZE);
	#endif
	#if (FS_FUNC == ENABLE)
	stg_fat_free = FileSys_GetDiskInfoEx('A', FST_INFO_FREE_SPACE);
	stg_fat_total = FileSys_GetDiskInfoEx('A', FST_INFO_DISK_SIZE);
	#endif
	//pwr_lvl_usage = DrvPower_GetControl(DRVPWR_CTRL_BATTERY_LEVEL); //level
	//if(pwr_lvl_usage>4)pwr_lvl_usage=0;
	//pwr_vot_usage = DrvPower_GetControl(DRVPWR_CTRL_BATTERY_ADC_VALUE); //votiage
	//pwr_tmp_usage = DrvPower_GetControl(DRVPWR_CTRL_BATTERY_TEMPERATURE); //temparature
	if (!bTrace) {
		profile_msg("resource |  total |  usage\r\n");
		profile_msg("=================================\r\n");
		profile_msg("     DMA |  %5u |  %5u (%%)\r\n", 100, dma_usage);
		profile_msg("     CPU |  %5u |  %5u (%%)\r\n", 100, cpu_usage);
		profile_msg("     MEM |  %5u |  %5u (MB)\r\n", (mem_total) / 1024 / 1024, (mem_total - mem_free) / 1024 / 1024);
		#if (PST_FUNC == ENABLE)
		profile_msg("   INMEM |  %5u |  %5u (MB)\r\n", (stg_pst_total) / 1024 / 1024, (stg_pst_total - stg_pst_free) / 1024 / 1024);
		#endif
		#if (FS_FUNC == ENABLE)
		profile_msg("   EXMEM |  %5u |  %5u (MB)\r\n", (UINT32)((stg_fat_total) / 1024 / 1024), (UINT32)((stg_fat_total - stg_fat_free) / 1024 / 1024));
		#endif
		//profile_msg("     PWR |  %5u |  %5u\r\n", 4, pwr_lvl_usage);
		//profile_msg("PWR_VOT: %u/%u\r\n", pwr_vot_usage, 1000);
		//profile_msg("PWR_TMP: %u/%u\r\n", pwr_tmp_usage, 100);
	} else {
		if (sys_dump_cnt == 0) {
			profile_msg("\r\n");
			profile_msg("             DMA             CPU             MEM           INMEM           EXMEM\r\n");
			profile_msg("================================================================================\r\n");
		} else {
			snprintf(sys_usage_str, sizeof(sys_usage_str), "%u/%u", dma_usage, 100);
			profile_msg("%16s", sys_usage_str);
			snprintf(sys_usage_str, sizeof(sys_usage_str), "%u/%u", cpu_usage, 100);
			profile_msg("%16s", sys_usage_str);
			snprintf(sys_usage_str, sizeof(sys_usage_str), "%u/%u", (mem_total - mem_free) / 1024 / 1024, (mem_total) / 1024 / 1024);
			profile_msg("%16s", sys_usage_str);
			#if (PST_FUNC == ENABLE)
			snprintf(sys_usage_str, sizeof(sys_usage_str), "%u/%u", (stg_pst_total - stg_pst_free) / 1024 / 1024, (stg_pst_total) / 1024 / 1024);
			profile_msg("%16s", sys_usage_str);
			#endif
			#if (FS_FUNC == ENABLE)
			snprintf(sys_usage_str, sizeof(sys_usage_str), "%u/%u", (UINT32)((stg_fat_total - stg_fat_free) / 1024 / 1024), (UINT32)((stg_fat_total) / 1024 / 1024));
			profile_msg("%16s\r\n", sys_usage_str);
			#endif
		}
	}
}

void sys_profile_process(void)
{
	//fix for CID 62612 - begin
	UINT32 mode = 0;
	//fix for CID 62612 - end
	profile_get(PROFILE_MODE, &mode);
	switch (mode) {
	case 1: { //"sys task" or "sys task top"
			if (sys_keeptop) { //"sys task top"
				profile_msg("\x1B[2J\x1B[H"); //cls
				profile_cmd(PROFILE_TASK, TRUE, 0);
			} else { //"sys task"
				if (sys_dump_cnt > 9) {
					profile_msg("\r\n");
					profile_cmd(PROFILE_TASK, TRUE, 0);
					SxCmd_DoCommand("tprof stop");
				} else {
					profile_cmd(PROFILE_TASK, FALSE, 0);
					profile_msg(".");
				}
			}
		}
		sys_dump_cnt++;
		break;
	case 2: //"sys cpu" or "sys cpu top" or "sys cpu trace"
		if (!sys_dotrace) { //"sys cpu" or "sys cpu top"
			if (sys_keeptop) { //"sys cpu top"
				profile_msg("\x1B[2J\x1B[H"); //cls
				profile_cmd(PROFILE_CPU, TRUE, 0);
			} else { //"sys cpu"
				if (sys_dump_cnt > 9) {
					profile_msg("\r\n");
					profile_cmd(PROFILE_CPU, TRUE, 0);
					SxCmd_DoCommand("tprof stop");
				} else {
					profile_cmd(PROFILE_CPU, FALSE, 0);
					profile_msg(".");
				}
			}
		} else { //"sys cpu trace"
			profile_cmd(PROFILE_CPU, TRUE, 0);
		}
		sys_dump_cnt++;
		break;
	case 3: //"sys usage" or "sys usage top" or "sys usage trace"
		if (!sys_dotrace) { //"sys usage" or "sys usage top"
			if (sys_keeptop) { //"sys cpu top"
				profile_msg("\x1B[2J\x1B[H"); //cls
				sys_usage_dump(FALSE);
			} else { //"sys cpu"
				if (sys_dump_cnt > 0) {
					profile_msg("\r\n");
					sys_usage_dump(FALSE);
					SxCmd_DoCommand("tprof stop");
				} else {
					profile_msg(".");
				}
			}
		} else { //"sys usage trace"
			sys_usage_dump(TRUE);
		}
		sys_dump_cnt++;
		break;
	}
}


///////////////////////////////////////////////////////////////////////////////
// cmd filter
///////////////////////////////////////////////////////////////////////////////

//FILTER for KEY EMULATION
BOOL User_CommandFilter(CHAR *pcCmdStr, int len)
{
	int slen = strlen(pcCmdStr);

	switch (*pcCmdStr) {
	// Key Macro
	case 'k':
		if (!strncmp(pcCmdStr, "koff", 4) && (slen == 4)) {
			strncpy(pcCmdStr, "key power", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "kdel", 4) && (slen == 4)) {
			strncpy(pcCmdStr, "key delete", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "km", 2) && (slen == 2)) {
			strncpy(pcCmdStr, "key menu", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "kmd", 3) && (slen == 3)) {
			strncpy(pcCmdStr, "key mode", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "kmp", 3) && (slen == 3)) {
			strncpy(pcCmdStr, "key playback", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "kmv", 3) && (slen == 3)) {
			strncpy(pcCmdStr, "key movie", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "ku", 2) && (slen == 2)) {
			strncpy(pcCmdStr, "key up", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "kd", 2) && (slen == 2)) {
			strncpy(pcCmdStr, "key down", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "kr", 2) && (slen == 2)) {
			strncpy(pcCmdStr, "key right", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "kl", 2) && (slen == 2)) {
			strncpy(pcCmdStr, "key left", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "ke", 2) && (slen == 2)) {
			strncpy(pcCmdStr, "key enter", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "kzi", 3) && (slen == 3)) {
			strncpy(pcCmdStr, "key zoomin", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "kzo", 3) && (slen == 3)) {
			strncpy(pcCmdStr, "key zoomout", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "kzr", 3) && (slen == 3)) {
			strncpy(pcCmdStr, "key zoom release", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "ks1", 3) && (slen == 3)) {
			strncpy(pcCmdStr, "key shutter1", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "ks2", 3) && (slen == 3)) {
			strncpy(pcCmdStr, "key shutter2", len);
			pcCmdStr[len] = 0;
		} else if (!strncmp(pcCmdStr, "kc1", 3) && (slen == 3)) {
			strncpy(pcCmdStr, "key custom1", len);
			pcCmdStr[len] = 0;
		}
		if (!strncmp(pcCmdStr, "kn", 2) && (slen == 2)) {
			strncpy(pcCmdStr, "key next", len);
			pcCmdStr[len] = 0;
		}
		//#NT#2016/03/16#Niven Cho -begin
		//#NT#add key previous
		else if (!strncmp(pcCmdStr, "kp", 2) && (slen == 2)) {
			strncpy(pcCmdStr, "key prev", len);
			pcCmdStr[len] = 0;
		}
		//#NT#2016/03/16#Niven Cho -end
		else if (!strncmp(pcCmdStr, "ks", 2) && (slen == 2)) {
			strncpy(pcCmdStr, "key select", len);
			pcCmdStr[len] = 0;
		}
		if (!strncmp(pcCmdStr, "ki", 2) && (slen == 2)) {
			strncpy(pcCmdStr, "key i", len);
			pcCmdStr[len] = 0;
		}
		break;
	case 'x':
		if (!strncmp(pcCmdStr, "x", 1) && (slen == 1)) {
			if (sys_keeptop || sys_dotrace) {
				strncpy(pcCmdStr, "tprof stop", len);
				pcCmdStr[len] = 0;
			}
		}
		break;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
// sys cmd
///////////////////////////////////////////////////////////////////////////////

static BOOL Cmd_sys_mem(CHAR *strCmd)
{
#if 1
	char str1[32];

	sscanf_s(strCmd, "%s", str1, sizeof(str1)); //fix for CID 44998

	if (strncmp(str1, "su", 31) == 0) { //sort by unit
		DBG_DUMP("[System]\r\n");
		Install_SystemMEM();
		DBG_DUMP("\r\n");
	} else { //sort by addr
		SxCmd_DoCommand("ker dumpmem");
	}
	{
		nvtmpp_dump_mem_range(debug_msg);
	}

	if (1) {
		DBG_DUMP("[Summary]\r\n");
		DBG_DUMP("=============================================================\r\n");
		DBG_DUMP("^Y   Total     : Range[0x%08X~0x%08X] Size=0x%08X\r\n",
				 OS_GetMemAddr(MEM_TOTAL),
				 OS_GetMemAddr(MEM_TOTAL) + OS_GetMemSize(MEM_TOTAL),
				 OS_GetMemSize(MEM_TOTAL)
				);
	}
#endif
	return TRUE;
}
static BOOL Cmd_sys_boot(CHAR *strCmd)
{
	DBG_DUMP("Boot\r\n");
	//DscMain();
	// Start Dsc System
	//System_PowerOn(SYS_POWERON_NORMAL);
	GxSystem_PowerOn(); //boot start
	return TRUE;
}
static BOOL Cmd_sys_shutdown(CHAR *strCmd)
{
	DBG_DUMP("Shutdown\r\n");
	//System_PowerOff(SYS_POWEROFF_NORMAL);
	Ux_SendEvent(0, NVTEVT_SYSTEM_SHUTDOWN, 1, 0); //shutdown start
	return TRUE;
}
static BOOL Cmd_sys_sleep(CHAR *strCmd)
{
	DBG_DUMP("Sleep\r\n");
	return TRUE;
}
static BOOL Cmd_sys_wakeup(CHAR *strCmd)
{
	DBG_DUMP("Wakeup\r\n");
	return TRUE;
}

extern UINT32 UI_EventTraceLvl;

static BOOL Cmd_sys_event(CHAR *strCmd)
{
	UINT32 lvl = 0;

	sscanf_s(strCmd, "%d", &lvl);
	if (lvl > 5) {
		lvl = 5;
	}

	UI_EventTraceLvl = lvl;
	DBG_DUMP("event trace level = %d!\r\n", UI_EventTraceLvl);
	return TRUE;
}

static BOOL Cmd_sys_tm(CHAR *strCmd)
{
	char str1[32];

	sscanf_s(strCmd, "%s", str1, sizeof(str1)); //fix for CID 45000

	if (strncmp(str1, "boot", 31) == 0) {
		DBG_MSG("Dump TM results of Boot:\r\n");
		TM_Dump("*");
	}
	return TRUE;
}

static BOOL Cmd_sys_usage(CHAR *strCmd)
{
	char str1[32];

	sscanf_s(strCmd, "%s", str1, sizeof(str1));

	sys_keeptop = FALSE;
	sys_dotrace = FALSE;
	sys_dump_cnt = 0;
	if (strncmp(str1, "top", 31) == 0) {
		sys_keeptop = TRUE;
	}
	if (strncmp(str1, "trace", 31) == 0) {
		sys_dotrace = TRUE;
	}
	SxCmd_DoCommand("tprof start 3");
	return TRUE;
}

static BOOL Cmd_sys_cpu(CHAR *strCmd)
{
	char str1[32];

	sscanf_s(strCmd, "%s", str1, sizeof(str1));

	sys_keeptop = FALSE;
	sys_dotrace = FALSE;
	sys_dump_cnt = 0;
	if (strncmp(str1, "top", 31) == 0) {
		sys_keeptop = TRUE;
	}
	if (strncmp(str1, "trace", 31) == 0) {
		//sys_dotrace = TRUE;
		return TRUE; //not support
	}
	SxCmd_DoCommand("tprof start 2");
	return TRUE;
}

static BOOL Cmd_sys_task(CHAR *strCmd)
{
	char str1[32];

	sscanf_s(strCmd, "%s", str1, sizeof(str1));

	sys_keeptop = FALSE;
	sys_dotrace = FALSE;
	sys_dump_cnt = 0;
	if (strncmp(str1, "top", 31) == 0) {
		sys_keeptop = TRUE;
	}
	if (strncmp(str1, "trace", 31) == 0) {
		return TRUE; //not support
	}
	SxCmd_DoCommand("tprof start 1");
	return TRUE;
}
//#NT#2016/06/23#Niven Cho -begin
//#NT#Enter calibration by cgi event or command event
static BOOL Cmd_sys_calibration(CHAR *strCmd)
{
#if (CALIBRATION_FUNC == ENABLE)
	Ux_PostEvent(NVTEVT_KEY_CALIBRATION, 0);
#else
	DBG_ERR("CALIBRATION_FUNC must be ENABLE.\r\n");
#endif
	return TRUE;
}
//#NT#2016/06/23#Niven Cho -end
BOOL Cmd_sys_reset(CHAR *strCmd)
{
#if (UI_FUNC == ENABLE)
	Ux_SendEvent(0, NVTEVT_EXE_SYSRESET, 0);
#endif
	return TRUE;
}

BOOL Cmd_sys_gitver(CHAR *strCmd)
{
	DBG_DUMP("GitVer=%s\r\n",Prj_GetGitRevision());
	return TRUE;
}

static BOOL Cmd_sys_evtfps(CHAR *strCmd)
{
	UINT32 bEn =0;
	UINT32 unit =0;
	sscanf_s(strCmd, "%d %d", &bEn,&unit);

	//consume fps
	UserCalConsumeFps(bEn,unit);
	//post fps
	Ux_CalPostFps(bEn,unit);
	return TRUE;

}

SXCMD_BEGIN(sys, "system command")
SXCMD_ITEM("usage %", Cmd_sys_usage, "usage")
SXCMD_ITEM("cpu %", Cmd_sys_cpu, "cpu")
SXCMD_ITEM("task %", Cmd_sys_task, "task")
SXCMD_ITEM("mem %", Cmd_sys_mem, "mem")
SXCMD_ITEM("boot", Cmd_sys_boot, "boot")
SXCMD_ITEM("shutdown", Cmd_sys_shutdown, "shutdown")
SXCMD_ITEM("sleep", Cmd_sys_sleep, "sleep")
SXCMD_ITEM("wakeup", Cmd_sys_wakeup, "wakeup")
SXCMD_ITEM("reset", Cmd_sys_reset, "reset to default setting")
SXCMD_ITEM("event %", Cmd_sys_event, "event trace level")
SXCMD_ITEM("tm %", Cmd_sys_tm, "time mark")
//#NT#2016/06/23#Niven Cho -begin
//#NT#Enter calibration by cgi event or command event
SXCMD_ITEM("calibration", Cmd_sys_calibration, "enter calibration window")
//#NT#2016/06/23#Niven Cho -end
SXCMD_ITEM("gitver", Cmd_sys_gitver, "get git fw version")
SXCMD_ITEM("evtfps %", Cmd_sys_evtfps, "cal evt consume fps")
SXCMD_END()



///////////////////////////////////////////////////////////////////////////////
// temp memory for user cmd
///////////////////////////////////////////////////////////////////////////////
#define      MAX_BUF_NUM        1

UINT32 gBufNum = 0;

UINT32 User_GetTempBuffer(UINT32 uiSize)
{
	NVTMPP_VB_POOL pool;
	NVTMPP_VB_BLK  blk;
	UINT32         blk_size = uiSize, addr;
    CHAR           func_name[]= "UsrTmp";

	if (uiSize == TEMPBUF_GET_MAXSIZE) {
		return POOL_SIZE_APP;    //return max size
	}
    if(gBufNum>=MAX_BUF_NUM) {
    	DBG_ERR("over MAX num %d,need release buf\r\n",MAX_BUF_NUM);
        return 0;
    }
    pool = nvtmpp_vb_create_pool(func_name, blk_size, 1, NVTMPP_DDR_1);

    if (NVTMPP_VB_INVALID_POOL == pool) {
    	DBG_ERR("create private pool err\r\n");
    	return 0;
    }

    blk = nvtmpp_vb_get_block(0, pool, blk_size, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_BLK == blk) {
		DBG_ERR("get vb block err\r\n");
		return 0;
	}
	addr = nvtmpp_vb_block2addr(blk);
	DBG_IND("%s get noncache buf addr = 0x%08X, size=0x%08X\r\n", func_name, addr, blk_size);

    if(addr) {
        gBufNum++;
    }
	return (UINT32)addr;
}

UINT32 User_RelTempBuffer(UINT32 addr)
{
	NVTMPP_VB_POOL pool;

    pool = nvtmpp_vb_addr2pool(addr);
	if (pool == NVTMPP_VB_INVALID_POOL){
		DBG_ERR("invalid block addr 0x%08X\r\n",addr);
        return FALSE;
	} else {
        gBufNum--;
		return nvtmpp_vb_destroy_pool(pool);
	}

}

//#NT#2016/03/18#Niven Cho -begin
//#NT#improve the using of gprof
#if defined(_GPROF_TOOL_ON_)
_ALIGNED(4) static UINT8 m_gProfBuf[0x100000];
UINT32 System_GetGprofBuffer(UINT32 uiSize)
{
	void *pBuf = 0;
	if (uiSize == TEMPBUF_GET_MAXSIZE) {
		return 0x100000;    //return max size
	}
	if (uiSize <= sizeof(m_gProfBuf)) {
		pBuf = (void *)m_gProfBuf;
	}
	if (pBuf == NULL) {
		DBG_ERR("get gprof buffer fail\r\n");
	}
	return (UINT32)pBuf;
}
#endif
//#NT#2016/03/18#Niven Cho -end


///////////////////////////////////////////////////////////////////////////////
// user cmd
///////////////////////////////////////////////////////////////////////////////

static BOOL Cmd_user_cmd(CHAR *strCmd)
{
	DBG_DUMP("this is sample cmd!\r\n");
	return TRUE;
}

//#NT#2016/03/03#Ben Wang -begin
//#NT#Add debug message for script test.
BOOL Cmd_dbg(CHAR *strCmd)
{
	UINT32 ShowMsg = 0;
	sscanf_s(strCmd, "%d", &ShowMsg);
	g_bDbgTest = ShowMsg;
	return TRUE;
}


static BOOL Cmd_user_movie_mode_d2d_func_set(CHAR *strCmd)
{
#if (defined(_MODEL_DVCAM1_EVB_) || defined(_MODEL_DVCAM2_EVB_) || defined(_MODEL_DVCAM_64M_EVB_))
	UINT32 uiEnable = 0;

	sscanf_s(strCmd, "%d", &uiEnable);

	DBGD(uiEnable);

	MovieExe_SetMovieD2DModeEn(uiEnable);
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE)); // Current mode should be Movie or Photo mode.
#endif

	return TRUE;
}

#include "WavStudioTsk.h"
#include "wsp_wavfile.h"
#include "Dx.h"
#include "nvtmpp.h"
#include "DCF.h"
#include "timer.h"

UINT32 Wav_GetTempBuffer(UINT32 uiSize)
{
	NVTMPP_VB_POOL pool;
	NVTMPP_VB_BLK  blk;
	UINT32         blk_size = uiSize, addr;
    CHAR           func_name[]= "WavTmp";

    pool = nvtmpp_vb_create_pool(func_name, blk_size, 1, NVTMPP_DDR_1);

    if (NVTMPP_VB_INVALID_POOL == pool) {
    	DBG_ERR("create private pool err\r\n");
    	return 0;
    }

    blk = nvtmpp_vb_get_block(0, pool, blk_size, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_BLK == blk) {
		DBG_ERR("get vb block err\r\n");
		return 0;
	}
	addr = nvtmpp_vb_block2addr(blk);
	DBG_IND("%s get noncache buf addr = 0x%08X, size=0x%08X\r\n", func_name, addr, blk_size);

	return (UINT32)addr;
}

UINT32 Wav_RelTempBuffer(UINT32 addr)
{
	NVTMPP_VB_POOL pool;

    pool = nvtmpp_vb_addr2pool(addr);
	if (pool == NVTMPP_VB_INVALID_POOL){
		DBG_ERR("invalid block addr 0x%08X\r\n",addr);
        return FALSE;
	} else {
		return nvtmpp_vb_destroy_pool(pool);
	}

}

BOOL WavStudio_SaveCB(PAUDIO_BUF_QUEUE pAudBufQue, UINT32 id, UINT64 timestamp)
{
	BOOL ret = 0;
	ret = WavFile_SaveCB(pAudBufQue, id, timestamp);
	WavStudio_SetConfig(WAVSTUD_CFG_UNLOCK_ADDR, WAVSTUD_ACT_REC, pAudBufQue->uiAddress);

	return ret;
}

WAVSTUD_APPOBJ wavObj = {0};
WAVSTUD_INFO_SET wsis = {0};
static UINT32   g_uiPlayTime;
CHAR gWavExamFile[64] = {'A', ':', '\\', 'T', 'e', 's', 't', '.', 'W', 'A', 'V', 0};

void WavStudio_CallBack(WAVSTUD_ACT act, UINT32 p1, UINT32 p2)
{
    //DBG_DUMP("^C WavStudioCB: %d\r\n", uiEventID);
    switch(p1)
    {
    case WAVSTUD_CB_EVT_STOPPED:
        g_uiPlayTime = 0;
        debug_msg("WAVSTUD_CB_EVT_STOPPED\r\n");
        //DBG_DUMP("WAVSTUDIO_EVENT_STOPPED\r\n");
        break;

    case WAVSTUD_CB_EVT_TCHIT:
        g_uiPlayTime++;
        DBGD(g_uiPlayTime);
        //DBG_DUMP("WAVSTUDIO_EVENT_DURATION\r\n");
        break;
    }
}


//Open
static BOOL WavStudio_UserOpen(CHAR *strCmd)
{

	DX_HANDLE devAudObj = Dx_GetObject(DX_CLASS_AUDIO_EXT);


	wsis.objCount			= 1;
	wsis.dataMode			= WAVSTUD_DATA_MODE_PUSH;

	wsis.audInfo.audSR		= AUDIO_SR_32000;
	wsis.audInfo.audCh		= AUDIO_CH_STEREO;
	wsis.audInfo.bitPerSamp = 16;
	wsis.audInfo.ch_num		= 2;

	wsis.bufCount			= 100;
	wsis.unitTime			= 10;


	wavObj.mem.uiSize = WavStudio_GetConfig(WAVSTUD_CFG_MEM, (UINT32)&wsis, WAVSTUD_ACT_REC);
	wavObj.mem.uiAddr = Wav_GetTempBuffer(wavObj.mem.uiSize);
    wavObj.pWavStudCB = WavStudio_CallBack;

	WavStudio_Open(WAVSTUD_ACT_REC, &wavObj, devAudObj, &wsis);

    return TRUE;
}

//Start record
static BOOL WavStudio_UserRecord(CHAR *strCmd)
{
	MEM_RANGE usrData;
	MEM_RANGE hdrMemRec;
	WAVFILE_FILENAME gRecFileName;
	WAVFILE_AUD_INFO gWAVFILE_RecCodec = {0};
	UINT64 gStopCount = 0;
    UINT32 dirID,fileID;

    // make dcf file fullpath
    if (FALSE == DCF_GetNextID(&dirID,&fileID))
    {
        DBG_ERR("DCF_GetNextID:%d,%d\r\n",dirID,fileID);
        return FALSE;
    }
    if (FALSE == DCF_MakeObjPath(dirID,fileID,DCF_FILE_TYPE_WAV,gWavExamFile))
    {
        DBG_ERR("DCF_MakeObjPath:%d,%d\r\n",dirID,fileID);
        return FALSE;
    }

    DCF_AddDBfile(gWavExamFile); //必須確保file create成功再加入DB
    debug_msg("gWavExamFile:%s\r\n",gWavExamFile);

	gRecFileName.fileNameLen = 63;
	gRecFileName.pFileName = (unsigned char *)&gWavExamFile[0];

	usrData.Addr = 0;
	usrData.Size = 0;

	hdrMemRec.Addr = WavStudio_GetConfig(WAVSTUD_CFG_HDR_ADDR, WAVSTUD_ACT_REC, 0);
	hdrMemRec.Size = WAVFILE_HEADER_BUF_SIZE;

	if (hdrMemRec.Addr == 0) {
		DBG_ERR("Rec 1 header address is NULL\r\n");
        return FALSE;
	}

	//Rec
	if (FALSE == WavFile_SaveOpen(&gRecFileName, usrData)) {
		DBG_ERR(":Set File=%s Fail\r\n", gWavExamFile);
        return FALSE;
	}

	gWAVFILE_RecCodec.audCh = wsis.audInfo.audCh;
	gWAVFILE_RecCodec.audSR = wsis.audInfo.audSR;
	gWAVFILE_RecCodec.bitPerSamp = wsis.audInfo.bitPerSamp;

	if (FALSE == WavFile_SaveHeader(&gWAVFILE_RecCodec, &gStopCount, hdrMemRec)) {
		DBG_ERR(":Write header File=%s Fail\r\n", gWavExamFile);
        return FALSE;
	}

	// Set Volume
    WavStudio_SetConfig(WAVSTUD_CFG_VOL, WAVSTUD_ACT_REC, 100);

	//Rec start
	if (FALSE == WavStudio_Start(WAVSTUD_ACT_REC, &wsis.audInfo, WAVSTUD_NON_STOP_COUNT, &WavStudio_SaveCB)) {
		DBG_ERR(":Rec File=%s Fail\r\n", gWavExamFile);
        return FALSE;
	}

    return TRUE;
}

//Stop record
static BOOL WavStudio_UserRecordStop(CHAR *strCmd)
{
	if (FALSE == WavStudio_Stop(WAVSTUD_ACT_REC)) {
		DBG_ERR("Fail:file=%s,sts=0x%x\r\n", gWavExamFile, WavStudio_GetConfig(WAVSTUD_CFG_GET_STATUS, 0, 0));
        return FALSE;
	}

	WavStudio_WaitStop(WAVSTUD_ACT_REC);
	WavFile_SaveClose();

    return TRUE;
}

//Close
static BOOL WavStudio_UserClose(CHAR *strCmd)
{
	if (E_OK != WavStudio_Close(WAVSTUD_ACT_REC)) {
		DBG_ERR("WavStudio already Closed\r\n");
        return FALSE;
	}

	Wav_RelTempBuffer(wavObj.mem.uiAddr);
    return TRUE;
}
//=============================================================================

// WavStudio Play API
//TIMER_ID gWavTimerID;
UINT64   gStopCount = 0;
//INT16    gPlay_Buff[AUDIO_SR_48000] = {0};
WAVFILE_AUD_INFO gWAVFILE_PlayCodec = {0};

#if 0
static void Wav_TriggerTimerHdlR(UINT32 nouse)
{
	AUDIO_BUF_QUEUE AudBufQue;

	AudBufQue.uiAddress = (UINT32)gPlay_Buff;
	AudBufQue.uiSize = WavStudio_GetConfig(WAVSTUD_CFG_BUFF_UNIT_SIZE, WAVSTUD_ACT_PLAY, 0);
	WavFile_LoadCB(&AudBufQue, 0, 0);
	WavStudio_PushBufToQueue(AudBufQue.uiAddress, &AudBufQue.uiSize);
}
#endif

void WavStudio_PlayCB(WAVSTUD_ACT act, UINT32 p1, UINT32 p2)
{
	switch (p1) {
	case WAVSTUD_CB_EVT_STOPPED:
        WavFile_LoadClose();
//        timer_close(gWavTimerID);
       // CHKPNT;
		break;
	case WAVSTUD_CB_EVT_STARTED:
#if 0
		{
			AUDIO_BUF_QUEUE AudBufQue;
CHKPNT;
			AudBufQue.uiAddress = (UINT32)gPlay_Buff;
			AudBufQue.uiSize = WavStudio_GetConfig(WAVSTUD_CFG_BUFF_UNIT_SIZE, WAVSTUD_ACT_PLAY, 0);
#if 0
			if (E_OK != timer_open(&gWavTimerID, Wav_TriggerTimerHdlR)) {
				DBG_ERR("Timer open failed\r\n");
				break;
			}
			if (E_OK != timer_cfg(gWavTimerID, wsis.unitTime * 100000, TIMER_MODE_FREE_RUN | TIMER_MODE_ENABLE_INT, TIMER_STATE_PLAY)) {
				DBG_ERR("Timer config failed\r\n");
				break;
			}
#endif
			WavFile_LoadCB(&AudBufQue, 0, 0);
			WavStudio_PushBufToQueue(AudBufQue.uiAddress, &AudBufQue.uiSize);
		}
#endif
		break;
    case WAVSTUD_CB_EVT_TCHIT:
        debug_msg("WAVSTUD_CB_EVT_TCHIT\r\n");
        break;
	default:
		break;
	}
}


static BOOL WavStudio_UserPlayOpen(CHAR *strCmd)
{

	DX_HANDLE devAudObj = Dx_GetObject(DX_CLASS_AUDIO_EXT);


	wsis.objCount			= 1;
	wsis.dataMode			= WAVSTUD_DATA_MODE_PULL;

	wsis.audInfo.audSR		= AUDIO_SR_32000;
	wsis.audInfo.audCh		= AUDIO_CH_STEREO;
	wsis.audInfo.bitPerSamp = 16;
	wsis.audInfo.ch_num		= 2;

	wsis.bufCount			= 100;
	wsis.unitTime			= 10;


	wavObj.mem.uiSize = WavStudio_GetConfig(WAVSTUD_CFG_MEM, (UINT32)&wsis, WAVSTUD_ACT_PLAY);
	wavObj.mem.uiAddr = Wav_GetTempBuffer(wavObj.mem.uiSize);
    wavObj.pWavStudCB = WavStudio_PlayCB;

	WavStudio_Open(WAVSTUD_ACT_PLAY, &wavObj, devAudObj, &wsis);

    return TRUE;
}

BOOL WavStudio_UserPlay(CHAR *strCmd)
{
    WAVFILE_FILENAME gPlayFileName;
	MEM_RANGE hdrMemPlay;
    UINT32      DirID = 0, FileID = 0, Index = 0, uiThisFileFormat = 0;


	hdrMemPlay.Addr = WavStudio_GetConfig(WAVSTUD_CFG_HDR_ADDR, WAVSTUD_ACT_PLAY, 0);
	hdrMemPlay.Size = WAVFILE_HEADER_BUF_SIZE;

    DCF_ScanObj();
    Index = DCF_GetDBInfo(DCF_INFO_CUR_INDEX);
	DCF_GetObjInfo(Index, &DirID, &FileID, &uiThisFileFormat);
	DCF_GetObjPath(Index, uiThisFileFormat, gWavExamFile);
    debug_msg("gWavExamFile:%s\r\n",gWavExamFile);

	if (hdrMemPlay.Addr == 0) {
		DBG_ERR("Play header address is NULL\r\n");
		return FALSE;
	}

    gPlayFileName.fileNameLen = 63;
    gPlayFileName.pFileName = (unsigned char *)&gWavExamFile[0];

	if (FALSE == WavFile_LoadOpen(&gPlayFileName)) {
		DBG_ERR(":File=%s Set Fail\r\n", gWavExamFile);
		return FALSE;
	}

   	gWAVFILE_PlayCodec.audCh = wsis.audInfo.audCh;
	gWAVFILE_PlayCodec.audSR = wsis.audInfo.audSR;
	gWAVFILE_PlayCodec.bitPerSamp = wsis.audInfo.bitPerSamp;

	if (FALSE == WavFile_LoadHeader(&gWAVFILE_PlayCodec, &gStopCount, hdrMemPlay)) {
		DBG_ERR(":Read File=%s Fail\r\n", gWavExamFile);
		return FALSE;
	}

    // Set play Vol
    WavStudio_SetConfig(WAVSTUD_CFG_VOL, WAVSTUD_ACT_PLAY, 100);

    // Start play audio
	if (FALSE == WavStudio_Start(WAVSTUD_ACT_PLAY, &wsis.audInfo, gStopCount, &WavFile_LoadCB)) {
		DBG_ERR(":Play File=%s Fail\r\n", gWavExamFile);
		return FALSE;
	}

	return TRUE;
}
BOOL WavStudio_UserPausePlay(CHAR *strCmd)
{
	if (FALSE == WavStudio_Pause(WAVSTUD_ACT_PLAY)) {
		DBG_ERR("Fail:file=%s,sts=0x%x\r\n", gWavExamFile, WavStudio_GetConfig(WAVSTUD_CFG_GET_STATUS, 0, 0));
		return FALSE;
	}
	return TRUE;
}
BOOL WavStudio_UserResumePlay(CHAR *strCmd)
{
	if (FALSE == WavStudio_Resume(WAVSTUD_ACT_PLAY)) {
		DBG_ERR("Fail:file=%s,sts=0x%x\r\n", gWavExamFile, WavStudio_GetConfig(WAVSTUD_CFG_GET_STATUS, 0, 0));
		return FALSE;
	}
	return TRUE;
}
BOOL WavStudio_UserStopPlay(CHAR *strCmd)
{
	if (FALSE == WavStudio_Stop(WAVSTUD_ACT_PLAY)) {
		DBG_ERR("Fail:file=%s,sts=0x%x\r\n", gWavExamFile, WavStudio_GetConfig(WAVSTUD_CFG_GET_STATUS, 0, 0));
		return FALSE;
	}
	return TRUE;
}

static BOOL WavStudio_UserPlayClose(CHAR *strCmd)
{
	if (E_OK != WavStudio_Close(WAVSTUD_ACT_PLAY)) {
		DBG_ERR("WavStudio already Closed\r\n");
        return FALSE;
	}

	Wav_RelTempBuffer(wavObj.mem.uiAddr);
    return TRUE;
}


SXCMD_BEGIN(user, "user command")
SXCMD_ITEM("cmd", Cmd_user_cmd, "sample code")
SXCMD_ITEM("dbg %", Cmd_dbg, "show/hide DBG_TEST() message")
SXCMD_ITEM("movie_mode_d2d_func_set %", Cmd_user_movie_mode_d2d_func_set, "enable d2d func")
SXCMD_ITEM("wavopen", WavStudio_UserOpen, "wavstudio open")
SXCMD_ITEM("wavclose", WavStudio_UserClose, "wavstudio close")
SXCMD_ITEM("wavrecord", WavStudio_UserRecord, "wavstudio start record")
SXCMD_ITEM("wavstop", WavStudio_UserRecordStop, "wavstudio stop record")

SXCMD_ITEM("wavpopen", WavStudio_UserPlayOpen, "wavstudio play open")
SXCMD_ITEM("wavpclose", WavStudio_UserPlayClose, "wavstudio play close")
SXCMD_ITEM("wavplay", WavStudio_UserPlay, "wavstudio start play")
SXCMD_ITEM("wavpstop", WavStudio_UserStopPlay, "wavstudio stop play")
SXCMD_ITEM("wavppause", WavStudio_UserPausePlay, "wavstudio pause play")
SXCMD_ITEM("wavpresume", WavStudio_UserResumePlay, "wavstudio resume play")

SXCMD_END()
//#NT#2016/03/03#Ben Wang -end
