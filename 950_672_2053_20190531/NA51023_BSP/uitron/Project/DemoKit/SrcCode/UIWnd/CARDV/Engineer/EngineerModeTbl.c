/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       EngineerModeTbl.c
    @ingroup    mIPRJAPTest

    @brief      Engineer Mode item table
                Engineer Mode item table

    @note       Nothing.

    @date       2007/12/18
*/

/** \addtogroup mIPRJAPTest */
//@{
#include <string.h>
#include "SysCommon.h"
#include "AppCommon.h"
#include "UICommon.h"
#include "CalibrationWnd.h"
#include "DynamicMenu.h"
#include "FileSysTsk.h"
#include "SysCfg.h"
//#include "CalCIPA.h"
//#include "CalMITF.h"
//#include "CalBurnIn.h"
//#include "CalSerNum.h"
//#include "CalEmptyVolt.h"
//#include "CalRtc.h"
//#include "CalLangSet.h"
//#include "CalMisc.h"
//#include "CalGyro.h"
#include "PStore.h"
//#include "ae_task.h"
//#include "awb_task.h"
//#include "Lens.h"
//#include "LensCtrlTsk.h"
//#include "af_api.h"
#include "EngineerMode.h"
#if (CALIBRATION_FUNC == ENABLE)
//#include "CalRunScript.h"
#include "calibration_item.h"
#endif
//extern void IQ_GetAdjustRst(CalIQInfo *CalData);
#include "UserCmd.h"

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          EngTbl
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

UIDMenuInfo UIFlowInfoEngMode =
{
    0,
    0,
    0,
    ITEMS_PER_PAGE
};
UIDMenuInfo *pEngModeDMenuInfo = &UIFlowInfoEngMode;
volatile BOOL   g_bCalbirationOpened = FALSE;
CHAR*           g_pExt;
UINT32          g_uiResult;
volatile BOOL   g_bCalibrationModeChange = FALSE;
BOOL            g_bIsEngineerMode = FALSE;

// Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
/*
static UINT32 ChkSelfTestStatus(void)
{
    return  (gCalSysData.RTC_Test_Status == _RTC_TEST_STATUS);
}

static UINT32 ChkBatEmptyVolt(void)
{
    return (gCalSysData.EmptyVolt_Test_Status == _EMPTYVOLT_STATUS);
}

static UINT32 ChkBurnInSts(void)
{
    return (gCalSysData.Burnin_Status== _BURNIN_STATUS);
}

static UINT32 ChkLangSts(void)
{
    return (gCalSysData.Language_Status == _LANGUAGE_STATUS);
}

static UINT32 ChkSNTestSts(void)
{
    return (gCalSysData.SerialNumber_Test_Status == _SERNUM_STATUS);
}

static UINT32 ChkGryoZeroAdjust(void)
{
    return (gCalSysData.Gyro_Zero_Adjust_Status == _GYRO_ZERO_ADJUST_STATUS);
}

static UINT32 ChkOrientAdjust(void)
{
    return (gCalSysData.Orient_Adjust_Status == _ORIENT_ADJUST_STATUS);
}


static UINT32 ChkDNPCalSts(void)
{
    return (DNP_Status_chk(NULL));
}

static UINT32 ChkISOCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.ISO_STATUS == _CAL_OK);
}

static UINT32 ChkAWBCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.AWB_STATUS == _CAL_OK);
}

static UINT32 ChkDPCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.DP_STATUS == _CAL_OK);
}

static UINT32 ChkDP_Dark_CalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return ((CalIQData.Cal_Status.DP_STATUS == _CAL_UnderFlow) || (CalIQData.Cal_Status.DP_STATUS == _CAL_OK));
}

static UINT32 ChkDP_Bright_CalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.DP_STATUS == _CAL_OK);
}

static UINT32 ChkECSCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.ECS_STATUS == _CAL_OK);
}

static UINT32 ChkAECHKCalSts(void)
{
    CalIQInfo CalIQData;

    IQ_GetAdjustRst(&CalIQData);

    return (CalIQData.Cal_Status.AECHK_STATUS == _CAL_OK);
}
*/

void Self_Test(void)
{
    CHAR     StringBuf[30];
    //URECT    tempRECT={20,60,300,40};
    snprintf(StringBuf,sizeof(StringBuf)-1,"Test OK.");
    //Cal_ShowString(StringBuf, &tempRECT, TRUE);
    //Cal_SetStringColor(_OSD_INDEX_WHITE);
}

// Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
DMENU_ITEM g_EngineerItem[]    =
{
    //Main Items, Intergrated Items
    {" 0. Self Test",                   Self_Test                ,NULL,     NULL},
#if CALIBRATION_FUNC
    {" 1. DP Adjust",                   Cal_SenDP                ,NULL,     NULL},
    {" 2. DP Bright Adjust",            Cal_SenDP_Bright         ,NULL,     NULL},
    {" 3. DP Dark Adjust",              Cal_SenDP_Dark           ,NULL,     NULL},
    {" 4. AE Check",                    Cal_SenAE_Check          ,NULL,     NULL},
    {" 5. ISO Adjust",                  Cal_SenISO               ,NULL,     NULL},
    {" 6. MShutter Adjust",             Cal_SenISO               ,NULL,     NULL},
    {" 7. AWB Adjust",                  Cal_SenAWB               ,NULL,     NULL},
    {" 8. AWB GS",                      Cal_SenAWBGS             ,NULL,     NULL},
    {" 9. ECS Adjust",                  Cal_SenECS               ,NULL,     NULL},
    {"10. DNP Adjust",                  Cal_SenDNP               ,NULL,     NULL},
    {"11. LENSCEN Adjust",              Cal_LENSCEN              ,NULL,     NULL},
    {"12. CapOnePic",                   Cal_Capture              ,NULL,     NULL},
#endif
    {"13. Exit",                        NULL                     ,NULL,     NULL},
};

UINT32 g_EngineerMaxItemNum      = sizeof(g_EngineerItem) / sizeof(DMENU_ITEM) - 1;


static void CalRoot_ScanCB(FIND_DATA *findData, BOOL *bContinue, UINT16 *cLongname, UINT32 Param)
{
    CHAR    orgFilename[9];
    UINT32  i;
    if (!M_IsDirectory(findData->attrib))
    {
        if (!strncmp(findData->FATExtName , g_pExt, 3))
        {
            memcpy(orgFilename, findData->FATMainName, 8);
            orgFilename[8] = '\0';
            sscanf_s(orgFilename, "%08X", &g_uiResult);
            for (i=0; i<8; i++)
            {
                if (isxdigit(*(orgFilename + i)) == 0)
                {
                    g_uiResult = NO_DEFAULT_RUN;
                    break;
                }
            }
        }
    }
}

/**
  Find file that filename with extension in root folder

  Find file that filename with extension in root folder and return the main
  filename (convert to integer of UINT32)
  [Calibration internal API]

  @param INT8 *pExt: extension
  @return UINT32: Filename of integer
*/

UINT32 Cal_GetDefaultInRoot(CHAR * pExt)
{
    g_uiResult = NO_DEFAULT_RUN;
    g_pExt = pExt;
    if(FileSys_ScanDir("A:\\", CalRoot_ScanCB, FALSE, 0)!=FST_STA_OK)
    {
        DBG_ERR("Failed to FileSys_ScanDir.\r\n");
    }
    return g_uiResult;
}

BOOL EngineerMode_CheckEng(void)
{
    BOOL ret = FALSE;
    FST_FILE filehdl = NULL;

    if (UI_GetData(FL_CardStatus) == CARD_INSERTED)
    {
        filehdl = FileSys_OpenFile(ENG_MODE_FILE,FST_OPEN_READ);
        if (filehdl!=NULL)
        {
           FileSys_CloseFile(filehdl);
           ret = TRUE;
        }
    }
    return ret;
}

void EngineerMode_Open(void)
{
    if (g_bCalbirationOpened == TRUE)
    {
        return;
    }
    g_bCalbirationOpened = TRUE;

	//#NT#2016/10/14#KCHong -begin
	//#NT#Forced use 320x240 UI for engineer mode
	// Toggle UI_Show twice to trigger UIDisplay_Init()
	UI_Show(UI_SHOW_WINDOW, FALSE);
	UI_Show(UI_SHOW_WINDOW, TRUE);
	// Forced set UI layout size to 320x240
	UI_SetLayoutSize(320, 240);
	//#NT#2016/10/14#KCHong -end

    Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_RELEASE, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_CONTINUE, FLGKEY_KEY_MASK_DEFAULT);

    //Ux_OpenWindow((VControl *)(&EngModeWndCtrl), 3, g_EngineerItem,g_EngineerMaxItemNum,NO_DEFAULT_RUN);
    pEngModeDMenuInfo->menuItem = g_EngineerItem;
    pEngModeDMenuInfo->uiMaxItem = g_EngineerMaxItemNum;
    //pEngModeDMenuInfo->uiSelectNum = NO_DEFAULT_RUN;
    pEngModeDMenuInfo->uiSelectNum = Cal_GetDefaultInRoot("CAL");
    //temp use CalibrationWndCtrl UI
    Ux_OpenWindow((VControl *)(&CalibrationWndCtrl), 1, pEngModeDMenuInfo);
}

BOOL IsEngineerModeOpened(void)
{
    return g_bCalbirationOpened;
}

void EngineerMode_WndOpen(void)
{
    Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_RELEASE, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_CONTINUE, FLGKEY_KEY_MASK_DEFAULT);

    //Ux_OpenWindow((VControl *)(&EngModeWndCtrl), 3, g_EngineerItem,g_EngineerMaxItemNum,NO_DEFAULT_RUN);
    pEngModeDMenuInfo->menuItem = g_EngineerItem;
    pEngModeDMenuInfo->uiMaxItem = g_EngineerMaxItemNum;
    //pEngModeDMenuInfo->uiSelectNum = NO_DEFAULT_RUN;
    pEngModeDMenuInfo->uiSelectNum = Cal_GetDefaultInRoot("CAL");
    //temp use CalibrationWndCtrl UI
    Ux_OpenWindow((VControl *)(&CalibrationWndCtrl), 1, pEngModeDMenuInfo);
}

void EngineerMode_WndClose(void)
{
    Ux_CloseWindow((VControl *)(&CalibrationWndCtrl), 0);
}

BOOL IsEngineerModeChange(void)
{
    return g_bCalibrationModeChange;
}

void SetEngineerModeChange(BOOL b)
{
    g_bCalibrationModeChange = b;
}

BOOL IsEngineerMode(void)
{
    return g_bIsEngineerMode;
}

//@}
