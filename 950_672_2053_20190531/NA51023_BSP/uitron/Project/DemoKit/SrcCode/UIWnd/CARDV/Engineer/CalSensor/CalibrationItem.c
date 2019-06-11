/**
    Calibration item

    @file       CalibrationItem.c
    @ingroup    calibration
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "PrjCfg.h"
#if (CALIBRATION_FUNC == ENABLE)
#include "SxCmd.h"
#include "Cal_Header_Int.h"
#include "CalibrationItem.h"
#include "IPL_Cal_KeyStrSample_Int.h"
#include "ae_task.h"
#include "awb_task.h"
#include "IPL_CAL_Debug.h"
#include "IPL_CalRW.h"
#include "DxSensor.h"
#include "IPL_Cmd.h"
#include "ae_api.h"
#include "awb_api.h"
#include "SysCfg.h"
#include "Utility.h"
#include "IPL_CalApi.h"
#include "IPL_AlgInfor.h"

#include "SysKer.h"
#include "GxSystem.h"
#include "IPL_Cmd.h"
#include "SysCfg.h"
#include "ImgCaptureAPI.h"
#include "GxDisplay.h"
#include "IPL_Utility.h"
#include "stdio.h"
#include "IPL_Display.h"
#include "Sensor.h"
#include "PStore.h"

#if isDemokit
#include "UIAppPhoto.h"
//#include "CalibrationAPI.h"
#include "CalibrationWnd.h"
#include "SysCommon.h"
#include "UIResource.h"
#include "IPL_Cmd.h"

#endif
#include "VControl.h"
#include "FileSysTsk.h"
#include "GxVideo.h"
//#include "AppView.h"
//#include "NvtAppPhotoApi.h"

#include "NvtUser.h"
#include "Dx.h"
#include "DCF.h"

#include "SysKer.h"
#include "GxSystem.h"
#include "GxVideo.h"
#include "DxSensor.h"
#include "IPL_Cmd.h"
#include "Delay.h"
#include "SysCfg.h"
#include "ImgCaptureAPI.h"
#include "GxDisplay.h"
#include "IPL_Utility.h"
#include "IPL_AlgInfor.h"
#include "PhotoTask.h"
#include "stdio.h"
#include "ExamSrvApi.h"
#include "FileSysTsk.h"
#include "IPL_Display.h"
#include "dis_alg.h"
#include "IPL_Ctrl.h"
#include "IPL_CalRW.h"
#include "EngineerMode.h"
#include "ImgCapInfor.h"
#include "Delay.h"

//#define AUTO_BACKLIGHT_FUNCTION         DISABLE
#define CalEndPrv DISABLE // set to preview after every calibration station
#define FailToEnd DISABLE
#define DNPCAL    DISABLE

#include "adc.h"

#if 0
- par
#endif
static BOOL m_CalOpen[IPL_ID_MAX_NUM] = {FALSE, FALSE, FALSE, FALSE};
static BOOL m_CalInit[IPL_ID_MAX_NUM] = {FALSE, FALSE, FALSE, FALSE};
static BOOL bInit = FALSE;

#if DNPCAL
static CAL_STATUS DNP_STATUS[IPL_ID_MAX_NUM][5] =
{/*  ECS         ISO       AWB      DP Dark   DP Bright*/
 {_CAL_NONE, _CAL_NONE, _CAL_NONE, _CAL_NONE, _CAL_NONE},
 {_CAL_NONE, _CAL_NONE, _CAL_NONE, _CAL_NONE, _CAL_NONE},
 {_CAL_NONE, _CAL_NONE, _CAL_NONE, _CAL_NONE, _CAL_NONE},
 {_CAL_NONE, _CAL_NONE, _CAL_NONE, _CAL_NONE, _CAL_NONE},};
#endif

static UINT32 m_Buf_StartAddr = 0;
static UINT32 m_Buf_Size = 0;
static UINT32 m_Header_Addr = 0;
static UINT32 m_Header_Size = 0;

UINT32 CalRwStoreOp_OnOpen(char *pSecName, CAL_RW_OPEN_SEL OpenSel);
ER CalRwStoreOp_OnClose(UINT32 pSectionHandleAddr);
ER CalRwStoreOp_OnRead(UINT8 *pcBuffer,UINT32 ulStartAddress, UINT32 ulDataLength, UINT32 pSectionHandleAddr);
ER CalRwStoreOp_OnWrite(UINT8 *pcBuffer, UINT32 ulStartAddress, UINT32 ulDataLength, UINT32 pSectionHandleAddr);
ER CalRwStoreOp_OnDelete(char *pSecName);
static CAL_RW_STORE_OP m_CalRwStoreOp = {CalRwStoreOp_OnOpen, CalRwStoreOp_OnClose, CalRwStoreOp_OnRead, CalRwStoreOp_OnWrite, CalRwStoreOp_OnDelete};

char m_CalDbgMsg[100];

#if 0
- static api
#endif
/**
    read header and all sections from pstore to dram,
    if header not exit, init dram for header
*/
static CAL_DMA_RST _PS_R(UINT32 dmaAddr,UINT32 dmaSize)
{
    CAL_DMA_RST ps_rst = {0};
    ps_rst = CAL_RW()->fpSectionAllRead(dmaAddr,dmaSize);
    if(ps_rst.erCode != E_OK)
    {
        if(ps_rst.erCode == E_NOMEM)
            DBG_IND("buf not enougth\r\n");
        //pstore header
        ps_rst = CAL_RW()->fpSectionRead_Header(dmaAddr,dmaSize);
        if(ps_rst.erCode != E_OK)
        {
            DBG_IND("no header\r\n");
            //init should only be executed once at the very first execution of calibration
            ps_rst = CAL_RW()->fpInit(dmaAddr,dmaSize,NULL);
            if(ps_rst.erCode != E_OK)
            {
                DBG_ERR("pstore/dram init fail!\r\n");
            }
            else
            {
                //inorder to let all following calibration functions can be executed correctly.
                //because all cailbration functions' 1st operation is to read header
                if(CAL_RW()->fpSectionWrite_Header() != E_OK)
                    DBG_ERR("header write 2 PS fail\r\n");
            }
        }
    }
    return ps_rst;
}



#if 0
- extern api
#endif
/**
    calibration input ipl ID which need to do calibrate
*/
BOOL m_CalInput[IPL_ID_MAX_NUM] = {TRUE, FALSE, FALSE, FALSE};

/**
    open all ipl calibration resource
    requirement:
    1. sensor need to be closed.
    2. display ctrl task need to be closed.
    3. ipl_ctrl task need to be closed.

    calibration ipl open
    1. read cal data from PStore
    2. set2preview
*/
ER Cal_IplOpen(IPL_PROC_ID Id, UINT32 iplNum, UINT32 BufAddr, UINT32 BufSize)
{
    CAL_DMA_RST ps_rst;
    UINT32 iplID;

    if(!m_CalInit[Id])
    {
        DBG_ERR("calibration sensor %d need to init\r\n", Id);
        return E_SYS;
    }

    if(m_CalOpen[Id])
    {
        DBG_WRN("calibration sensor %d ipl is open\r\n", Id);
        return E_OK;
    }

    ps_rst = _PS_R(BufAddr,BufSize);

    if(ps_rst.erCode != E_OK)
    {
        DBG_ERR("read header and all sections from pstore to dram\r\n");
        return E_SYS;
    }
    m_Header_Addr = BufAddr;
    m_Header_Size = BufSize - ps_rst.RemainSize;
    m_Buf_StartAddr = ps_rst.RemainAddr;
    m_Buf_Size = ps_rst.RemainSize;


    if(Set2Preview)
    {
       if(Set2Preview(Id, iplNum, m_Buf_StartAddr,m_Buf_Size))
       {
           DBG_ERR("Set2Preview fail\r\n");
           return E_SYS;
       }
    }
    else
       DBG_ERR("No Set2Preview function\r\n");

    /*
        only support open (preview) single sensor at same time
        close other sensor except open sensor
    */
    for (iplID = 0; iplID < IPL_ID_MAX_NUM; iplID++)
    {
        if (iplID == Id)
            m_CalOpen[iplID] = TRUE;
        else
            m_CalOpen[iplID] = FALSE;
    }

    return E_OK;
}

/**
    initial calibration ipl
    1.check IPL close
    2.open sensor
    3.hook R/W function point
*/
ER Cal_IplInit (void)
{
    IPL_PROC_ID Id;
    SENSOR_INIT_OBJ SenObj;
    SENSOR_DRVTAB *pSenTab;

    // check debug cfg in SD card
    CAL_ReadDebugConfig();

    /*
        check IPL close
    */
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(m_CalInput[Id] == TRUE)
        {
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SIZE, 1, PHOTO_SIZE_12M);

            IPL_MODE_INFOR Info;
            Info.Id = Id;
            IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

            if(Info.Mode != IPL_MODE_OFF)
            {
                IPL_MODE_DATA CmdData = {0};

                CmdData.Mode = IPL_MODE_OFF;
                IPL_SetCmd(IPL_SET_CHGMODE, (void *)&CmdData);
                IPL_WaitCmdFinish();
            }

            //open IPL
            if(IPL_IsOpen())
            {
                DBG_IND("IPL is open\r\n");
                //return E_SYS;
            }
            else
                IPL_Open();
        }
    }

    /*
        not support sensor 3&4
    */
    if ((m_CalInput[IPL_ID_3] == TRUE)||(m_CalInput[IPL_ID_4] == TRUE))
    {
        m_CalInput[IPL_ID_3] = FALSE;
        m_CalInput[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    /*
        open sensor
    */
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(m_CalInput[Id] == TRUE)
        {
            //check calibration IPL init
            if(m_CalInit[Id])
            {
                DBG_ERR("calibration sensor %d is init\r\n", Id);
                continue;
            }

            //open sensor driver
            SENSOR_ID senId = IPL_UTI_CONV2_SEN_ID(Id);
            if (Sensor_IsOpen(senId))
            {
                Sensor_Close(senId);
            }

            SenObj = DrvSensor_GetObj(senId);
            pSenTab = DrvSensor_GetTab(senId);
            DrvSensor_PowerTurnOn(senId);
            Sensor_Open(senId, &SenObj, pSenTab);
            m_CalInit[Id] = TRUE;
        }
    }

    /*
        hook R/W function point
    */
    CAL_RegRWFunc(&m_CalRwStoreOp);

    //ensure R/W store function point OK
    if (PCalRWOBJ == NULL)
    {
        DBG_ERR("no R/W store function point\r\n");
        return E_SYS;
    }

    return E_OK;
}
#if 0
static ER Cal_IplClose(void)
{
    /*
        Stop CAL PipeLine Status
    */
    IPL_PROC_ID Id;

    if ((m_CalInput[IPL_ID_3] == TRUE)||(m_CalInput[IPL_ID_4] == TRUE))
    {
        m_CalInput[IPL_ID_3] = FALSE;
        m_CalInput[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(m_CalInput[Id] == TRUE)
        {
            //stop all pipeline
            CalApi()->fpSetMode(Id,CAL_IPL_ALLSTOP,NULL,NULL);
            //uninitial preview buffer
            CalApi()->fpBufEvent()->fpUnInit(Id);

            m_CalOpen[Id] = FALSE;
        }
    }
    return E_OK;
}
#endif

ER Cal_IplUnInit(void)
{
    /*ER bCalClose = Cal_IplClose();
    return bCalClose;*/

    IPL_PROC_ID Id;

    /*
        close IPL
    */
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(m_CalInput[Id] == TRUE)
        {
            IPL_MODE_INFOR Info;
            Info.Mode = IPL_MODE_OFF;
            Info.Id = Id;
            IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

            if(Info.Mode != IPL_MODE_OFF)
            {
                IPL_MODE_DATA CmdData = {0};

                CmdData.Mode = IPL_MODE_OFF;
                IPL_SetCmd(IPL_SET_CHGMODE, (void *)&CmdData);
                IPL_WaitCmdFinish();
            }

            //open IPL
            if(!IPL_IsOpen())
            {
                DBG_IND("IPL is close\r\n");
            }
            else
            {
                IPL_Close();
            }
        }
    }

    /*
        not support sensor 3&4
    */
    if ((m_CalInput[IPL_ID_3] == TRUE)||(m_CalInput[IPL_ID_4] == TRUE))
    {
        m_CalInput[IPL_ID_3] = FALSE;
        m_CalInput[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    /*
        close sensor
    */
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(m_CalInput[Id] == TRUE)
        {
            //check calibration IPL init
            if(!m_CalInit[Id])
            {
                DBG_ERR("calibration sensor %d is uninit\r\n", Id);
                continue;
            }

            //open sensor driver
            SENSOR_ID senId = IPL_UTI_CONV2_SEN_ID(Id);
            if (Sensor_IsOpen(senId))
            {
                Sensor_Close(senId);
            }
            m_CalInit[Id] = FALSE;
            bInit = FALSE;
        }
    }

    return E_OK;
}

void Cal_Capture(void)
{
    #if 0
    CHAR   StringBuf[30];
    URECT  tempRECT1={20,100,300,40};
    IPL_PROC_ID Id;
    ER CalOpen;
    UINT32 iplNum = 0;
    if ((CalInput_IPL_ID[IPL_ID_3] == TRUE)||(CalInput_IPL_ID[IPL_ID_4] == TRUE))
    {
        CalInput_IPL_ID[IPL_ID_3] = FALSE;
        CalInput_IPL_ID[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    Cal_IplClose(CalInput_IPL_ID);
    //#NT#2015/02/03#Silvia Wu -begin
    //#NT# need to add capture flow
    /*pStrg = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_NAND1);
    ret = FileSys_ChangeDisk(pStrg); // change disk
    if(ret == FST_STA_OK)
    {
        DCF_ScanObj(); // scan DCF
    }

    DscPhoto_Open();
    AWB_SetAlgMode(1);  //Set AWB to Gray World
    Delay_DelayMs(1200); //delay for AE/AWB converge
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_IMAGE_RATIO,1,IMAGERATIO_4_3);
    Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SIZE, 1, SIZE_14M);
    FlowPhoto_DoCapture();
    Delay_DelayMs(4000);

    pStrg = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);
    ret = FileSys_ChangeDisk(pStrg); // change disk
    if(ret == FST_STA_OK)
    {
        DCF_ScanObj(); // scan DCF
    }

    m_CalOpen = FALSE;*/
    //#NT#2015/02/03#Silvia Wu -end

    /*
        set calibration ipl num
    */
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(CalInput_IPL_ID[Id] == TRUE)
        {
            iplNum++;
        }
    }

    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if((CalInput_IPL_ID[Id] == TRUE) && (!m_CalOpen[Id]))
        {
            //Capture with Fix ISO
            IPL_AlgSetUIInfo(Id, IPL_SEL_ISOINDEX,SEL_ISOINDEX_100);
            // need to check !!!!!
            CalOpen = Cal_IplOpen(Id, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
            if (CalOpen != E_OK)
            {
                DBG_ERR("Calibration IPL open fail\r\n");
                return;
            }
        }
    }



    DBG_ERR("Capture End\r\n");

    #endif
}

static ER Cal_SaveData(CHAR* CalDbgMsg, BOOL bDeletFile)
{
    CHAR pcFilename[32];
    //#NT#2016/06/07#Silvia Wu -begin
    //#NT# fixed coverity warning
    //UINT32 RW_OpenResult1 = {0};
    //FST_FILE RW_OpenResult = &RW_OpenResult1;
    FST_FILE RW_OpenResult;
    //#NT#2016/06/07#Silvia Wu -end
    UINT32 size;
    ER ER_Code = E_OK;
    UINT32 buf_cnt = 0;

    // file name
    buf_cnt = snprintf(NULL, 0, "A:\\Calibration_Result.txt");
    snprintf(pcFilename, buf_cnt+1, "A:\\Calibration_Result.txt");

    if (bDeletFile) //if exist, delete it; if not exist, create new
    {
        RW_OpenResult = FileSys_OpenFile(pcFilename, FST_CREATE_ALWAYS|FST_OPEN_WRITE);
    }
    else //if exist, open; if not exist, create new
    {
        RW_OpenResult = FileSys_OpenFile(pcFilename, FST_OPEN_ALWAYS|FST_OPEN_READ|FST_OPEN_WRITE);
        FileSys_SeekFile(RW_OpenResult, 0, FST_SEEK_END);
    }

    if (RW_OpenResult== NULL)
        return E_SYS;

    size = strlen(m_CalDbgMsg);
    ER_Code = FileSys_WriteFile(RW_OpenResult, (UINT8*)&m_CalDbgMsg, &size, 0, NULL);
    if (ER_Code != E_OK)
        return E_SYS;

    ER_Code = FileSys_CloseFile(RW_OpenResult);
    if (ER_Code != E_OK)
        return E_SYS;

    return E_OK;
}


void Cal_GetCalRst(IPL_PROC_ID Id, CAL_DEBUG_INFO CalDebugInfor)
{
    CAL_ITEM(Id)->fpGetAdjustRst(Id,NULL,&CalDebugInfor);
}

void Cal_SenDP_Bright(void)
{
    CAL_DMA_RST ps_rst;
    IPL_PROC_ID Id;
    ER CalOpen;
    UINT32 iplNum = 0;
    IPL_PROC_ID lastCalId = IPL_ID_1;
    static CHAR m_CalMsg[100];
    UINT32 buf_cnt = 0;

    // open sensor & hook R/W function point
    if (bInit == FALSE)
    {
        if(Cal_IplInit())
        {
            DBG_WRN("calibration init fail!\r\n");
            return;
        }
        else
        {
            bInit = TRUE;
        }

    }

    //check cal sensor id init
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if((m_CalInput[Id] == TRUE)&&(!m_CalInit[Id]))
        {
            DBG_WRN("calibration sensor %d need to init\r\n", Id);
            return;
        }
    }

    if ((m_CalInput[IPL_ID_3] == TRUE)||(m_CalInput[IPL_ID_4] == TRUE))
    {
        m_CalInput[IPL_ID_3] = FALSE;
        m_CalInput[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    /*
        set calibration ipl num
    */
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(m_CalInput[Id] == TRUE)
        {
            iplNum++;
        }
    }

    if(CAL_ITEM)
    {
        /*
            Cailbration
        */
        BOOL CalRstOK[IPL_ID_MAX_NUM] = {TRUE, TRUE, TRUE, TRUE};
        BOOL CalRstBrightOK[IPL_ID_MAX_NUM] = {TRUE, TRUE, TRUE, TRUE};
        for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
        {
            if(m_CalInput[Id] == TRUE)
            {
                /*
                    open sensor (read pstore data & set2preview)
                    which sensor Id need to calibrate
                */
                if(!m_CalOpen[Id])
                {
                    CalOpen = Cal_IplOpen(Id, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
                    if (CalOpen != E_OK)
                    {
                        DBG_ERR("Calibration IPL open fail\r\n");
                        return;
                    }
                }

                /*
                    show OSD & wait key
                */
#if DNPCAL
#else
                CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);
                CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
#endif
                buf_cnt = snprintf(NULL, 0, "DP(BRIGHT) IN PROCESSING");
                snprintf((char*)m_CalMsg, buf_cnt, "DP(BRIGHT) IN PROCESSING");
                CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_BLACK);

                /*
                    calibration for sensor Id (no preview)
                */
                if(CAL_ITEM(Id)) //uninit sensor Id buffer & get calibration item
                {
                    m_CalOpen[Id] = FALSE; // preview buffer be uninit
                    if(CAL_ITEM(Id)->fpDP_Adj_Bright(Id,CAL_UICB()->DP_W_UI,m_Buf_StartAddr,m_Buf_Size) == E_OK)
                    {
                        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
                            DBG_ERR("sensor %d write header fail\r\n", Id);

                        // re-read all section because of maybe pstore have new section information after calibration function.
                        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));

                        // must be return E_OK at each calibration function
                        if(ps_rst.erCode != E_OK)
                        {
                            DBG_ERR("sensor %d read header and section fail\r\n", Id);
                            return;
                        }
                        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
                        m_Buf_StartAddr = ps_rst.RemainAddr;
                        m_Buf_Size = ps_rst.RemainSize;
                    }
                    else
                        DBG_ERR("sensor %d DP Bright Adjust fail\r\n", Id);
                }
                else
                    DBG_ERR("sensor %d get calibration item fail\r\n");

                lastCalId = Id;

                /*
                    1. get calibration result
                    2. (save result in txt) depend on eng.cfg
                    3. get calibration final result to show on OSD
                */
                CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
                CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
                ER ER_Code = E_OK;
                UINT32 SenModeCondiCnt = 0;
                // get calibration result
                CAL_ITEM(Id)->fpGetAdjustRst(Id, NULL, CalDebugInfor_DNP);
                if(CAL_ITEM(Id)->fpGetDebugLevel(CALDEBUG_INFO)) //save result in txt & get calibration final result
                {
                    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
                    {
                        if (CalDebugInfor_DNP->CalDebugDP->CalAeRstBright[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
                        {
                            // station name + sensor ID
                            buf_cnt = snprintf(NULL, 0, "DPB%d_", Id);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "DPB%d_", Id);
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] == _CAL_DEBUG_OK)
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "OK%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "OK%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            }
                            else if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] == _CAL_DEBUG_DP_B_OK)
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "OK(B)%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "OK(B)%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                                CalRstOK[Id] = FALSE;
                            }
                            else
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "NG%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "NG%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                                CalRstOK[Id] = FALSE;
                                CalRstBrightOK[Id] = FALSE;
                            }
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            // calibration result (DP count)
                            buf_cnt = snprintf(NULL, 0, "%d_", CalDebugInfor_DNP->CalDebugDP->BCnt[SenModeCondiCnt]);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "%d_", CalDebugInfor_DNP->CalDebugDP->BCnt[SenModeCondiCnt]);
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            // ae information
                            buf_cnt = snprintf(NULL, 0, "%d_%d_%d_%d\r\n"
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstBright[SenModeCondiCnt].Lum
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstBright[SenModeCondiCnt].ISO
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstBright[SenModeCondiCnt].ExpT
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstBright[SenModeCondiCnt].AeTimeout);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "%d_%d_%d_%d\r\n"
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstBright[SenModeCondiCnt].Lum
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstBright[SenModeCondiCnt].ISO
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstBright[SenModeCondiCnt].ExpT
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstBright[SenModeCondiCnt].AeTimeout);

                            // save data in SD card
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);

                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }
                        }
                    }
                }
                else // get calibration final result
                {
                    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
                    {
                        if (CalDebugInfor_DNP->CalDebugDP->CalAeRstBright[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
                        {
                            if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] == _CAL_DEBUG_OK)
                            {
                                // DP dark & DP bright results are OK
                            }
                            else if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] == _CAL_DEBUG_DP_B_OK)
                            {
                                // only DP bright OK
                                CalRstOK[Id] = FALSE;
                            }
                            else
                            {
                                // DP FAIL
                                CalRstOK[Id] = FALSE;
                                CalRstBrightOK[Id] = FALSE;
                            }
                        }
                    }
                }

            } // calibration for input sensor ID

        } //per sensor ID

        /*
            show calibration final result on OSD
        */
        CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_RED);
#if DNPCAL
        for (Id = 0; Id <= lastCalId; Id++)
        {
            if (CalRstOK[Id] == FALSE)
            {
                DNP_STATUS[Id][4] = _CAL_OverFlow;
            }
            else
            {
                DNP_STATUS[Id][4] = _CAL_OK;
            }
        }
#else
        if (lastCalId == IPL_ID_1) // Only one sensor does not need to show sensor Id on OSD
        {
            if ((CalRstOK[IPL_ID_1] == FALSE)&&(CalRstBrightOK[IPL_ID_1] == FALSE))
            {
                // DP FAIL
                buf_cnt = snprintf(NULL, 0, "DP FAIL");
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP FAIL");
                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }
            else if (CalRstOK[IPL_ID_1] == FALSE)
            {
                // only DP bright OK
                buf_cnt = snprintf(NULL, 0, "DP BRIGHT PASS");
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP BRIGHT PASS");
                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
            else
            {
                // DP dark & DP bright results are OK
                buf_cnt = snprintf(NULL, 0, "DP PASS");
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP PASS");
                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
        }
        else //muti-sensor need to show sensor Id on OSD
        {
            for (Id = 0; Id <= lastCalId; Id++)
            {
                if ((CalRstOK[Id] == FALSE)&&(CalRstBrightOK[Id] == FALSE))
                {
                    // DP FAIL
                    buf_cnt = snprintf(NULL, 0, "DP %d FAIL", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "DP %d FAIL", Id);
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                }
                else if (CalRstOK[Id] == FALSE)
                {
                    // only DP bright OK
                    buf_cnt = snprintf(NULL, 0, "DP BRIGHT %d PASS", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "DP BRIGHT %d PASS", Id);
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
                }
                else
                {
                    // DP dark & DP bright results are OK
                    buf_cnt = snprintf(NULL, 0, "DP %d PASS", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "DP %d PASS", Id);
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
                }
            }
        }

        buf_cnt = snprintf(NULL, 0, "FINISH!!!");
        snprintf((char*)m_CalMsg, buf_cnt+1, "FINISH!!!");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
        CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
        CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);
#endif

        /*
            open last cal sensor (read pstore data & set2preview)
        */
#if CalEndPrv
        if(!m_CalOpen[lastCalId])
        {
            CalOpen = Cal_IplOpen(lastCalId, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
            if (CalOpen != E_OK)
            {
                DBG_ERR("Calibration IPL open fail\r\n");
                return;
            }
        }
#endif
    }
    else
        DBG_ERR("no calibration item\r\n");
    Cal_IplUnInit();
    DBG_IND("===== DP Bright finish =====\r\n");
}

void Cal_SenDP_Dark(void)
{
    CAL_DMA_RST ps_rst;
    IPL_PROC_ID Id;
    ER CalOpen;
    UINT32 iplNum = 0;
    IPL_PROC_ID lastCalId = IPL_ID_1;
    static CHAR m_CalMsg[100];
    UINT32 buf_cnt = 0;

    // open sensor & hook R/W function point
    if (bInit == FALSE)
    {
        if(Cal_IplInit())
        {
            DBG_WRN("calibration init fail!\r\n");
            return;
        }
        else
        {
            bInit = TRUE;
        }

    }

    //check cal sensor id init
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if((m_CalInput[Id] == TRUE)&&(!m_CalInit[Id]))
        {
            DBG_WRN("calibration sensor %d need to init\r\n", Id);
            return;
        }
    }

    if ((m_CalInput[IPL_ID_3] == TRUE)||(m_CalInput[IPL_ID_4] == TRUE))
    {
        m_CalInput[IPL_ID_3] = FALSE;
        m_CalInput[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    /*
        set calibration ipl num
    */
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(m_CalInput[Id] == TRUE)
        {
            iplNum++;
        }
    }

    if(CAL_ITEM)
    {
        /*
            Cailbration
        */
        BOOL CalRstOK[IPL_ID_MAX_NUM] = {TRUE, TRUE, TRUE, TRUE};
        BOOL CalRstDarkOK[IPL_ID_MAX_NUM] = {TRUE, TRUE, TRUE, TRUE};
        for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
        {
            if(m_CalInput[Id] == TRUE)
            {
                /*
                    open sensor (read pstore data & set2preview)
                    which sensor Id need to calibrate
                */
                if(!m_CalOpen[Id])
                {
                    CalOpen = Cal_IplOpen(Id, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
                    if (CalOpen != E_OK)
                    {
                        DBG_ERR("Calibration IPL open fail\r\n");
                        return;
                    }
                }

                /*
                    show OSD & wait key
                */
#if DNPCAL
#else
                CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);
                CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
#endif
                buf_cnt = snprintf(NULL, 0, "DP(DARK) IN PROCESSING");
                snprintf((char*)m_CalMsg, buf_cnt, "DP(DARK) IN PROCESSING");
                CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_BLACK);

                /*
                    calibration for sensor Id (no preview)
                */
                if(CAL_ITEM(Id)) //uninit sensor Id buffer & get calibration item
                {
                    m_CalOpen[Id] = FALSE; // preview buffer be uninit
                    if(CAL_ITEM(Id)->fpDP_Adj_Dark(Id,CAL_UICB()->DP_D_UI,m_Buf_StartAddr,m_Buf_Size) == E_OK)
                    {
                        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
                            DBG_ERR("sensor %d write header fail\r\n", Id);

                        // re-read all section because of maybe pstore have new section information after calibration function.
                        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));

                        // must be return E_OK at each calibration function
                        if(ps_rst.erCode != E_OK)
                        {
                            DBG_ERR("sensor %d read header and section fail\r\n", Id);
                            return;
                        }
                        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
                        m_Buf_StartAddr = ps_rst.RemainAddr;
                        m_Buf_Size = ps_rst.RemainSize;
                    }
                    else
                        DBG_ERR("sensor %d DP Dark Adjust fail\r\n", Id);
                }
                else
                    DBG_ERR("sensor %d get calibration item fail\r\n");

                lastCalId = Id;

                /*
                    1. get calibration result
                    2. (save result in txt) depend on eng.cfg
                    3. get calibration final result to show on OSD
                */
                CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
                CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
                ER ER_Code = E_OK;
                UINT32 SenModeCondiCnt = 0;
                // get calibration result
                CAL_ITEM(Id)->fpGetAdjustRst(Id, NULL, CalDebugInfor_DNP);
                if(CAL_ITEM(Id)->fpGetDebugLevel(CALDEBUG_INFO)) //save result in txt & get calibration final result
                {
                    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
                    {
                        if (CalDebugInfor_DNP->CalDebugDP->CalAeRstDark[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
                        {
                            // station name + sensor ID
                            buf_cnt = snprintf(NULL, 0, "DPD%d_", Id);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "DPD%d_", Id);
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] == _CAL_DEBUG_OK)
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "OK%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "OK%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            }
                            else if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] == _CAL_DEBUG_DP_D_OK)
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "OK(D)%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "OK(D)%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                                CalRstOK[Id] = FALSE;
                            }
                            else
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "NG%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "NG%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                                CalRstOK[Id] = FALSE;
                                CalRstDarkOK[Id] = FALSE;
                            }
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            // calibration result (DP count)
                            buf_cnt = snprintf(NULL, 0, "%d_", CalDebugInfor_DNP->CalDebugDP->DCnt[SenModeCondiCnt]);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "%d_", CalDebugInfor_DNP->CalDebugDP->DCnt[SenModeCondiCnt]);
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            // ae information
                            buf_cnt = snprintf(NULL, 0, "%d_%d_%d_%d\r\n"
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstDark[SenModeCondiCnt].Lum
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstDark[SenModeCondiCnt].ISO
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstDark[SenModeCondiCnt].ExpT
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstDark[SenModeCondiCnt].AeTimeout);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "%d_%d_%d_%d\r\n"
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstDark[SenModeCondiCnt].Lum
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstDark[SenModeCondiCnt].ISO
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstDark[SenModeCondiCnt].ExpT
                                , CalDebugInfor_DNP->CalDebugDP->CalAeRstDark[SenModeCondiCnt].AeTimeout);

                            // save data in SD card
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);

                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }
                        }
                    }
                }
                else // get calibration final result
                {
                    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
                    {
                        if (CalDebugInfor_DNP->CalDebugDP->CalAeRstDark[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
                        {
                            if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] == _CAL_DEBUG_OK)
                            {
                                // DP dark & DP bright results are OK
                            }
                            else if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst[SenModeCondiCnt] == _CAL_DEBUG_DP_D_OK)
                            {
                                // DP dark OK
                                CalRstOK[Id] = FALSE;
                            }
                            else
                            {
                                // DP FAIL
                                CalRstOK[Id] = FALSE;
                                CalRstDarkOK[Id] = FALSE;
                            }
                        }
                    }
                }

            } // calibration for input sensor ID

        } //per sensor ID

        /*
            show calibration final result on OSD
        */
        CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_RED);
#if DNPCAL
        for (Id = 0; Id <= lastCalId; Id++)
        {
            if (CalRstOK[Id] == FALSE)
            {
                DNP_STATUS[Id][3] = _CAL_OverFlow;
            }
            else
            {
                DNP_STATUS[Id][3] = _CAL_OK;
            }
        }
#else
        if (lastCalId == IPL_ID_1) // Only one sensor does not need to show sensor Id on OSD
        {
            if ((CalRstOK[IPL_ID_1] == FALSE)&&(CalRstDarkOK[IPL_ID_1] == FALSE))
            {
                // DP FAIL
                buf_cnt = snprintf(NULL, 0, "DP FAIL");
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP FAIL");
                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }
            else if (CalRstOK[IPL_ID_1] == FALSE)
            {
                // only DP dark OK
                buf_cnt = snprintf(NULL, 0, "DP DARK PASS");
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP DARK PASS");
                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
            else
            {
                // DP dark & DP bright results are OK
                buf_cnt = snprintf(NULL, 0, "DP PASS");
                snprintf((char*)m_CalMsg, buf_cnt+1, "DP PASS");
                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
        }
        else //muti-sensor need to show sensor Id on OSD
        {
            for (Id = 0; Id <= lastCalId; Id++)
            {
                if ((CalRstOK[Id] == FALSE)&&(CalRstDarkOK[Id] == FALSE))
                {
                    // DP FAIL
                    buf_cnt = snprintf(NULL, 0, "DP %d FAIL", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "DP %d FAIL", Id);
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                }
                else if (CalRstOK[Id] == FALSE)
                {
                    // only DP dark OK
                    buf_cnt = snprintf(NULL, 0, "DP DARK %d PASS", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "DP DARK %d PASS", Id);
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
                }
                else
                {
                    // DP dark & DP bright results are OK
                    buf_cnt = snprintf(NULL, 0, "DP %d PASS", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "DP %d PASS", Id);
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
                }
            }
        }

        buf_cnt = snprintf(NULL, 0, "FINISH!!!");
        snprintf((char*)m_CalMsg, buf_cnt+1, "FINISH!!!");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
        CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
        CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);
#endif

        /*
            open last cal sensor (read pstore data & set2preview)
        */
#if CalEndPrv
        if(!m_CalOpen[lastCalId])
        {
            CalOpen = Cal_IplOpen(lastCalId, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
            if (CalOpen != E_OK)
            {
                DBG_ERR("Calibration IPL open fail\r\n");
                return;
            }
        }
#endif
    }
    else
        DBG_ERR("no calibration item\r\n");
    Cal_IplUnInit();
    DBG_IND("===== DP Dark finish =====\r\n");

}

void Cal_SenDP(void)
{
    #if 0
    CalIQInfo CalIQData;
    IPL_PROC_ID Id;

    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(CalInput_IPL_ID[Id] == TRUE)
        {
            CAL_ITEM(Id)->fpGetAdjustRst(Id,NULL,&CalIQData);
        }
    }

    if (CalIQData.Cal_Status.DP_STATUS != _CAL_DP_D_OK)
    {
        Cal_SenDP_Dark(CalInput_IPL_ID);
    }

    if (CalIQData.Cal_Status.DP_STATUS != _CAL_DP_B_OK)
    {
        Cal_SenDP_Bright(CalInput_IPL_ID);
    }
    #endif
}
#if DNPCAL
static BOOL showDNP(void)
{
    IPL_PROC_ID Id = IPL_ID_1;
    UINT32 StationCnt = 0;
    static CHAR m_CalMsg[100];
    UINT32 buf_cnt = 0;
    BOOL CALRst = TRUE;

    CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);
    for (Id = 0; Id <= IPL_ID_MAX_NUM; Id++)
    {
        for (StationCnt = 0; StationCnt < 5; StationCnt++)
        {

            if (DNP_STATUS[Id][StationCnt] == _CAL_OK)
            {
                switch(StationCnt)
                {
                    case 0://ECS
                        buf_cnt = snprintf(NULL, 0, "ECS OK", Id);
                        snprintf((char*)m_CalMsg, buf_cnt+1, "ECS OK", Id);
                        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
                        break;
                    case 1://ISO
                        buf_cnt = snprintf(NULL, 0, "ISO OK", Id);
                        snprintf((char*)m_CalMsg, buf_cnt+1, "ISO OK", Id);
                        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
                         break;
                    case 2://AWB
                        buf_cnt = snprintf(NULL, 0, "AWB OK", Id);
                        snprintf((char*)m_CalMsg, buf_cnt+1, "AWB OK", Id);
                        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
                         break;
                    case 3://DP dark
                        buf_cnt = snprintf(NULL, 0, "DP dark OK", Id);
                        snprintf((char*)m_CalMsg, buf_cnt+1, "DP dark OK", Id);
                        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
                         break;
                    case 4://DP bright
                        buf_cnt = snprintf(NULL, 0, "DP bright OK", Id);
                        snprintf((char*)m_CalMsg, buf_cnt+1, "DP bright OK", Id);
                        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
                         break;
                    default:
                        break;
                }
            }
            else if (DNP_STATUS[Id][StationCnt] == _CAL_OverFlow)
            {
                CALRst = FALSE;
                switch(StationCnt)
                {
                    case 0://ECS
                        buf_cnt = snprintf(NULL, 0, "ECS FAIL", Id);
                        snprintf((char*)m_CalMsg, buf_cnt+1, "ECS FAIL", Id);
                        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                        break;
                    case 1://ISO
                        buf_cnt = snprintf(NULL, 0, "ISO FAIL", Id);
                        snprintf((char*)m_CalMsg, buf_cnt+1, "ISO FAIL", Id);
                        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                         break;
                    case 2://AWB
                        buf_cnt = snprintf(NULL, 0, "AWB FAIL", Id);
                        snprintf((char*)m_CalMsg, buf_cnt+1, "AWB FAIL", Id);
                        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                         break;
                    case 3://DP dark
                        buf_cnt = snprintf(NULL, 0, "DP dark FAIL", Id);
                        snprintf((char*)m_CalMsg, buf_cnt+1, "DP dark FAIL", Id);
                        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                         break;
                    case 4://DP bright
                        buf_cnt = snprintf(NULL, 0, "DP bright FAIL", Id);
                        snprintf((char*)m_CalMsg, buf_cnt+1, "DP bright FAIL", Id);
                        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                         break;
                    default:
                        break;
                }
            }
            else
            {
            }
        }
    }
    return CALRst;
}
#endif
void Cal_SenDNP(void)
{
#if DNPCAL
    static CHAR m_CalMsg[100];
    UINT32 buf_cnt = 0;
    BOOL CALRst = TRUE;

    CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);
    CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);

    /*
        ECS calibration
    */
    Cal_SenECS();
    CALRst = showDNP();
#if FailToEnd
    if(!CALRst)
        goto finish;
#endif

    /*
        ISO calibration
    */
    Cal_SenISO();
    CALRst = showDNP();
#if FailToEnd
    if(!CALRst)
        goto finish;
#endif

    /*
        AWB calibration
    */
    Cal_SenAWB();
    CALRst = showDNP();
#if FailToEnd
    if(!CALRst)
        goto finish;
#endif

    /*
        DP dark calibration
    */
    Cal_SenDP_Dark();
    CALRst = showDNP();
#if FailToEnd
    if(!CALRst)
        goto finish;
#endif

    /*
        DP bright calibration
    */
    CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
    Cal_SenDP_Bright();
    CALRst = showDNP();
#if FailToEnd
    if(!CALRst)
        goto finish;
#endif

#if FailToEnd
    finish:
#endif
    buf_cnt = snprintf(NULL, 0, "FINISH!!!");
    snprintf((char*)m_CalMsg, buf_cnt+1, "FINISH!!!");
    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
    CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
    CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);



#endif






#if 0
    IPL_PROC_ID Id = IPL_ID_1;
    static CHAR m_CalMsg[100];
    CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
    CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
    ER ER_Code = E_OK;

    //Delay_DelayMs(5000);


    CAL_UICB()->AWB_UI(UICB_CLEAN,NULL,UICOL_GREEN);
    sprintf((char*)m_CalMsg, " ");
    CAL_UICB()->AWB_UI(UICB_WAITKEY,(CHAR*)m_CalMsg,UICOL_GREEN);
    CAL_UICB()->AWB_UI(UICB_CLEAN,NULL,UICOL_GREEN);
    /*sprintf((char*)m_CalDbgMsg, "StationC,");
    Cal_SaveData(m_CalDbgMsg, TRUE);*/

    /*
        ECS calibration
    */
    sprintf((char*)m_CalMsg, "ECS IN PROCESSING");
    CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_BLACK);

    //calibration
    Cal_SenECS();

    //get result & save result if cal result is fail
    CAL_ITEM(Id)->fpGetAdjustRst(Id, NULL, CalDebugInfor_DNP);
    if (CalDebugInfor_DNP->CalDebugECS->CalStatusRst != _CAL_DEBUG_OK)
    {
        sprintf((char*)m_CalDbgMsg, "ECS_NG_");
        ER_Code = Cal_SaveData(m_CalDbgMsg, TRUE);

        sprintf((char*)m_CalDbgMsg, "%d_%d_%d,", CalDebugInfor_DNP->CalDebugECS->CalAeRst.Lum
        , CalDebugInfor_DNP->CalDebugECS->CalAeRst.ISO, CalDebugInfor_DNP->CalDebugECS->CalAeRst.AeTimeout);
        ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalMsg, "ECS FAIL");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);

        if (ER_Code != E_OK)
        {
            sprintf((char*)m_CalMsg, "SD Card Full : STOP");
            CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            goto Stop;
        }

        #if FailToEnd
        goto Exit;
        #endif
    }
    else
    {
        sprintf((char*)m_CalDbgMsg, "ECS_OK_");
        ER_Code = Cal_SaveData(m_CalDbgMsg, TRUE);

        sprintf((char*)m_CalDbgMsg, "%d_%d_%d,", CalDebugInfor_DNP->CalDebugECS->CalAeRst.Lum
        , CalDebugInfor_DNP->CalDebugECS->CalAeRst.ISO, CalDebugInfor_DNP->CalDebugECS->CalAeRst.AeTimeout);
        ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalMsg, "ECS PASS");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);

        if (ER_Code != E_OK)
        {
            sprintf((char*)m_CalMsg, "SD Card Full : STOP");
            CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            goto Stop;
        }
    }

    /*
        ISO calibration
    */
    sprintf((char*)m_CalMsg, "ISO IN PROCESSING");
    CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_BLACK);

    //calibration
    Cal_SenISO();

    //get result & save result if cal result is fail
    CAL_ITEM(Id)->fpGetAdjustRst(Id, NULL, CalDebugInfor_DNP);
    if (CalDebugInfor_DNP->CalDebugISO->CalStatusRst != _CAL_DEBUG_OK)
    {
        sprintf((char*)m_CalDbgMsg, "ISO_NG_");
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_", CalDebugInfor_DNP->CalDebugISO->CalAeRst.ISOBase);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_%d_%d,", CalDebugInfor_DNP->CalDebugISO->CalAeRst.Lum
        , CalDebugInfor_DNP->CalDebugISO->CalAeRst.ISO, CalDebugInfor_DNP->CalDebugISO->CalAeRst.AeTimeout);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalMsg, "ISO FAIL");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);

        if (ER_Code != E_OK)
        {
            sprintf((char*)m_CalMsg, "SD Card Full : STOP");
            CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            goto Stop;
        }

        #if FailToEnd
        goto Exit;
        #endif
    }
    else
    {
        sprintf((char*)m_CalDbgMsg, "ISO_OK_");
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_", CalDebugInfor_DNP->CalDebugISO->CalAeRst.ISOBase);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_%d_%d,", CalDebugInfor_DNP->CalDebugISO->CalAeRst.Lum
        , CalDebugInfor_DNP->CalDebugISO->CalAeRst.ISO, CalDebugInfor_DNP->CalDebugISO->CalAeRst.AeTimeout);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalMsg, "ISO PASS");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);

        if (ER_Code != E_OK)
        {
            sprintf((char*)m_CalMsg, "SD Card Full : STOP");
            CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            goto Stop;
        }
    }

    /*
        AWB calibration
    */
    sprintf((char*)m_CalMsg, "AWB IN PROCESSING");
    CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_BLACK);

    //calibration
    Cal_SenAWB();

    //get result & save result if cal result is fail
    CAL_ITEM(Id)->fpGetAdjustRst(Id, NULL, CalDebugInfor_DNP);
    if (CalDebugInfor_DNP->CalDebugAWB->CalStatusRst != _CAL_DEBUG_OK)
    {
        sprintf((char*)m_CalDbgMsg, "AWB_NG_");
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_%d_", CalDebugInfor_DNP->CalDebugAWB->CalRgain, CalDebugInfor_DNP->CalDebugAWB->CalBgain);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_%d_%d,", CalDebugInfor_DNP->CalDebugAWB->CalAeRst.Lum
        , CalDebugInfor_DNP->CalDebugAWB->CalAeRst.ISO, CalDebugInfor_DNP->CalDebugAWB->CalAeRst.AeTimeout);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalMsg, "AWB FAIL");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);

        if (ER_Code != E_OK)
        {
            sprintf((char*)m_CalMsg, "SD Card Full : STOP");
            CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            goto Stop;
        }

        #if FailToEnd
        goto Exit;
        #endif
    }
    else
    {
        sprintf((char*)m_CalDbgMsg, "AWB_OK_");
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_%d_", CalDebugInfor_DNP->CalDebugAWB->CalRgain, CalDebugInfor_DNP->CalDebugAWB->CalBgain);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_%d_%d,", CalDebugInfor_DNP->CalDebugAWB->CalAeRst.Lum
        , CalDebugInfor_DNP->CalDebugAWB->CalAeRst.ISO, CalDebugInfor_DNP->CalDebugAWB->CalAeRst.AeTimeout);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalMsg, "AWB PASS");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);

        if (ER_Code != E_OK)
        {
            sprintf((char*)m_CalMsg, "SD Card Full : STOP");
            CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            goto Stop;
        }
    }

    /*
        DP dark calibration
    */
    sprintf((char*)m_CalMsg, "DP(DARK) IN PROCESSING");
    CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_BLACK);

    //calibration
    Cal_SenDP_Dark();

    //get result & save result if cal result is fail
    CAL_ITEM(Id)->fpGetAdjustRst(Id, NULL, CalDebugInfor_DNP);
    if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst != _CAL_DEBUG_DP_D_OK)
    {
        sprintf((char*)m_CalDbgMsg, "DP_D_NG_");
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_", CalDebugInfor_DNP->CalDebugDP->DCnt);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_%d_%d_", CalDebugInfor_DNP->CalDebugDP->CalAeRstDark.Lum
        , CalDebugInfor_DNP->CalDebugDP->CalAeRstDark.ISO, CalDebugInfor_DNP->CalDebugDP->CalAeRstDark.AeTimeout);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalMsg, "DP(DARK) FAIL");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);

        if (ER_Code != E_OK)
        {
            sprintf((char*)m_CalMsg, "SD Card Full : STOP");
            CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            goto Stop;
        }

        #if FailToEnd
        goto Exit;
        #endif
    }
    else
    {
        sprintf((char*)m_CalDbgMsg, "DP_D_OK_");
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_", CalDebugInfor_DNP->CalDebugDP->DCnt);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_%d_%d_", CalDebugInfor_DNP->CalDebugDP->CalAeRstDark.Lum
        , CalDebugInfor_DNP->CalDebugDP->CalAeRstDark.ISO, CalDebugInfor_DNP->CalDebugDP->CalAeRstDark.AeTimeout);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalMsg, "DP(DARK) PASS");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);

        if (ER_Code != E_OK)
        {
            sprintf((char*)m_CalMsg, "SD Card Full : STOP");
            CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            goto Stop;
        }
    }

    //CAL_UICB()->AWB_UI(UICB_CLEAN,NULL,UICOL_GREEN);
    CAL_UICB()->AWB_UI(UICB_WAITKEY,NULL,UICOL_GREEN);


    /*
        DP bright calibration
    */
    sprintf((char*)m_CalMsg, "DP(BRIGHT) IN PROCESSING");
    CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_BLACK);

    //calibration
    Cal_SenDP_Bright();

    //get result & save result if cal result is fail
    CAL_ITEM(Id)->fpGetAdjustRst(Id, NULL, CalDebugInfor_DNP);
    if (CalDebugInfor_DNP->CalDebugDP->CalStatusRst != _CAL_DEBUG_OK)
    {
        sprintf((char*)m_CalDbgMsg, "B_NG_");
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_", CalDebugInfor_DNP->CalDebugDP->BCnt);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_%d_%d_", CalDebugInfor_DNP->CalDebugDP->CalAeRstBright.Lum
        , CalDebugInfor_DNP->CalDebugDP->CalAeRstBright.ISO, CalDebugInfor_DNP->CalDebugDP->CalAeRstBright.AeTimeout);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "NG_%d", CalDebugInfor_DNP->CalDebugDP->BCnt+CalDebugInfor_DNP->CalDebugDP->DCnt);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalMsg, "DP(BRIGHT) FAIL");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);

        if (ER_Code != E_OK)
        {
            sprintf((char*)m_CalMsg, "SD Card Full : STOP");
            CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            goto Stop;
        }

        #if FailToEnd
        goto Exit;
        #endif
    }
    else
    {
        sprintf((char*)m_CalDbgMsg, "B_OK_");
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_", CalDebugInfor_DNP->CalDebugDP->BCnt);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "%d_%d_%d_", CalDebugInfor_DNP->CalDebugDP->CalAeRstBright.Lum
        , CalDebugInfor_DNP->CalDebugDP->CalAeRstBright.ISO, CalDebugInfor_DNP->CalDebugDP->CalAeRstBright.AeTimeout);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalDbgMsg, "OK_%d", CalDebugInfor_DNP->CalDebugDP->BCnt+CalDebugInfor_DNP->CalDebugDP->DCnt);
        Cal_SaveData(m_CalDbgMsg, FALSE);

        sprintf((char*)m_CalMsg, "DP(BRIGHT) PASS");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);

        if (ER_Code != E_OK)
        {
            sprintf((char*)m_CalMsg, "SD Card Full : STOP");
            CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            goto Stop;
        }
    }

    #if FailToEnd
    Exit:
    #endif

    sprintf((char*)m_CalMsg, "FINISH!!!");
    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);

    Stop:
    sprintf((char*)m_CalDbgMsg, ";");
    Cal_SaveData(m_CalDbgMsg, FALSE);


    while(1)
    {
        if(GxKey_GetData(GXKEY_NORMAL_KEY) & 0x80000000)
        {
            System_PowerOff(0);
            break;
        }
        else
        {

        }
    }
#endif
}

void Cal_SenAE_Check(void)
{
    #if 0
    CAL_DMA_RST ps_rst;
    IPL_PROC_ID Id;
    ER CalOpen;

    if ((CalInput_IPL_ID[IPL_ID_3] == TRUE)||(CalInput_IPL_ID[IPL_ID_4] == TRUE))
    {
        CalInput_IPL_ID[IPL_ID_3] = FALSE;
        CalInput_IPL_ID[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if((CalInput_IPL_ID[Id] == TRUE) && (!m_CalOpen[Id]))
        {
            CalOpen = Cal_IplOpen(Id, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
            if (CalOpen != E_OK)
            {
                DBG_ERR("Calibration IPL open fail\r\n");
                return;
            }
        }
    }

    if(CAL_ITEM)
    {
        for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
        {
            if(CalInput_IPL_ID[Id] == TRUE)
            {
                if(CAL_ITEM(Id))
                {
                    if(CAL_ITEM(Id)->fpAE_Check(Id,NULL,m_Buf_StartAddr,m_Buf_Size) == E_OK)
                    {
                        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
                            DBG_ERR("sensor %d write header fail\r\n", Id);

                        //re-read all section because of maybe pstore have new section information after calibration function.
                        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));
                        //must be return E_OK at each calibration function excluding Cal_IplOpen()
                        if(ps_rst.erCode != E_OK)
                        {
                            DBG_ERR("sensor %d read header and section fail\r\n", Id);
                            return;
                        }
                        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
                        m_Buf_StartAddr = ps_rst.RemainAddr;
                        m_Buf_Size = ps_rst.RemainSize;
                    }
                    else
                        DBG_ERR("sensor %d AE check fail\r\n", Id);
                }
                else
                    DBG_ERR("sensor %d no calibration item\r\n");
            }
        }
    }

    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(CalInput_IPL_ID[Id] == TRUE)
        {
            if(Set2Preview)
                if(Set2Preview(Id, m_Buf_StartAddr,m_Buf_Size))
                    DBG_ERR("Set2Preview fail\r\n");
        }
    }
    #endif
}

void Cal_SenISO(void)
{
    CAL_DMA_RST ps_rst;
    IPL_PROC_ID Id;
    ER CalOpen;
    UINT32 iplNum = 0;
    IPL_PROC_ID lastCalId = IPL_ID_1;
    static CHAR m_CalMsg[100];
    UINT32 buf_cnt = 0;

    // open sensor & hook R/W function point
    if (bInit == FALSE)
    {
        if(Cal_IplInit())
        {
            DBG_WRN("calibration init fail!\r\n");
            return;
        }
        else
        {
            bInit = TRUE;
        }

    }

    //check cal sensor id init
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if((m_CalInput[Id] == TRUE)&&(!m_CalInit[Id]))
        {
            DBG_WRN("calibration sensor %d need to init\r\n", Id);
            return;
        }
    }

    if ((m_CalInput[IPL_ID_3] == TRUE)||(m_CalInput[IPL_ID_4] == TRUE))
    {
        m_CalInput[IPL_ID_3] = FALSE;
        m_CalInput[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    /*
        set calibration ipl num
    */
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(m_CalInput[Id] == TRUE)
        {
            iplNum++;
        }
    }

    if(CAL_ITEM)
    {
        /*
            Cailbration
        */
        BOOL CalRstOK[IPL_ID_MAX_NUM] = {TRUE, TRUE, TRUE, TRUE};
        for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
        {
            if(m_CalInput[Id] == TRUE)
            {
                /*
                    open sensor (read pstore data & set2preview)
                    which sensor Id need to calibrate
                */
                if(!m_CalOpen[Id])
                {
                    CalOpen = Cal_IplOpen(Id, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
                    if (CalOpen != E_OK)
                    {
                        DBG_ERR("Calibration IPL open fail\r\n");
                        return;
                    }
                }

                /*
                    show OSD & wait key
                */
#if DNPCAL
#else
                CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);
                CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
#endif
                buf_cnt = snprintf(NULL, 0, "ISO IN PROCESSING");
                snprintf((char*)m_CalMsg, buf_cnt, "ISO IN PROCESSING");
                CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_BLACK);

                /*
                    calibration for sensor Id (no preview)
                */
                if(CAL_ITEM(Id)) //uninit sensor Id buffer & get calibration item
                {
                    m_CalOpen[Id] = FALSE; // preview buffer be uninit
                    if(CAL_ITEM(Id)->fpISOAdj(Id,CAL_UICB()->ISO_UI,m_Buf_StartAddr,m_Buf_Size) == E_OK)
                    {
                        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
                            DBG_ERR("sensor %d write header fail\r\n", Id);

                        // re-read all section because of maybe pstore have new section information after calibration function.
                        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));

                        // must be return E_OK at each calibration function
                        if(ps_rst.erCode != E_OK)
                        {
                            DBG_ERR("sensor %d read header and section fail\r\n", Id);
                            return;
                        }
                        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
                        m_Buf_StartAddr = ps_rst.RemainAddr;
                        m_Buf_Size = ps_rst.RemainSize;
                    }
                    else
                        DBG_ERR("sensor %d ISO Adjust fail\r\n", Id);
                }
                else
                    DBG_ERR("sensor %d get calibration item fail\r\n");

                lastCalId = Id;

                /*
                    1. get calibration result
                    2. (save result in txt) depend on eng.cfg
                    3. get calibration final result to show on OSD
                */
                CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
                CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
                ER ER_Code = E_OK;
                UINT32 SenModeCondiCnt = 0;
                // get calibration result
                CAL_ITEM(Id)->fpGetAdjustRst(Id, NULL, CalDebugInfor_DNP);
                if(CAL_ITEM(Id)->fpGetDebugLevel(CALDEBUG_INFO)) //save result in txt & get calibration final result
                {
                    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
                    {
                        if (CalDebugInfor_DNP->CalDebugISO->CalAeRst[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
                        {
                            // station name + sensor ID
                            buf_cnt = snprintf(NULL, 0, "ISO%d_", Id);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "ISO%d_", Id);
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            if (CalDebugInfor_DNP->CalDebugISO->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "NG%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "NG%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                                CalRstOK[Id] = FALSE;
                            }
                            else
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "OK%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "OK%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            }
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            // calibration result (isobase)
                            buf_cnt = snprintf(NULL, 0, "%d_", CalDebugInfor_DNP->CalDebugISO->CalISOBase[SenModeCondiCnt]);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "%d_", CalDebugInfor_DNP->CalDebugISO->CalISOBase[SenModeCondiCnt]);
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            // ae information
                            buf_cnt = snprintf(NULL, 0, "%d_%d_%d_%d\r\n"
                                , CalDebugInfor_DNP->CalDebugISO->CalAeRst[SenModeCondiCnt].Lum
                                , CalDebugInfor_DNP->CalDebugISO->CalAeRst[SenModeCondiCnt].ISO
                                , CalDebugInfor_DNP->CalDebugISO->CalAeRst[SenModeCondiCnt].ExpT
                                , CalDebugInfor_DNP->CalDebugISO->CalAeRst[SenModeCondiCnt].AeTimeout);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "%d_%d_%d_%d\r\n"
                                , CalDebugInfor_DNP->CalDebugISO->CalAeRst[SenModeCondiCnt].Lum
                                , CalDebugInfor_DNP->CalDebugISO->CalAeRst[SenModeCondiCnt].ISO
                                , CalDebugInfor_DNP->CalDebugISO->CalAeRst[SenModeCondiCnt].ExpT
                                , CalDebugInfor_DNP->CalDebugISO->CalAeRst[SenModeCondiCnt].AeTimeout);

                            // save data in SD card
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);

                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }
                        }
                    }
                }
                else // get calibration final result
                {
                    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
                    {
                        if (CalDebugInfor_DNP->CalDebugISO->CalAeRst[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
                        {
                            if (CalDebugInfor_DNP->CalDebugISO->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
                            {
                                CalRstOK[Id] = FALSE;
                            }
                        }
                    }
                }

            } // calibration for input sensor ID

        } //per sensor ID

        /*
            show calibration final result on OSD
        */
        CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_RED);
#if DNPCAL
        for (Id = 0; Id <= lastCalId; Id++)
        {
            if (CalRstOK[Id] == FALSE)
            {
                DNP_STATUS[Id][1] = _CAL_OverFlow;
            }
            else
            {
                DNP_STATUS[Id][1] = _CAL_OK;
            }
        }
#else
        if (lastCalId == IPL_ID_1) // Only one sensor does not need to show sensor Id on OSD
        {
            if (CalRstOK[IPL_ID_1] == FALSE)
            {
                buf_cnt = snprintf(NULL, 0, "ISO FAIL");
                snprintf((char*)m_CalMsg, buf_cnt+1, "ISO FAIL");
                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }
            else
            {
                buf_cnt = snprintf(NULL, 0, "ISO PASS");
                snprintf((char*)m_CalMsg, buf_cnt+1, "ISO PASS");
                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
        }
        else //muti-sensor need to show sensor Id on OSD
        {
            for (Id = 0; Id <= lastCalId; Id++)
            {
                if (CalRstOK[Id] == FALSE)
                {
                    buf_cnt = snprintf(NULL, 0, "ISO %d FAIL", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "ISO %d FAIL", Id);
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                }
                else
                {
                    buf_cnt = snprintf(NULL, 0, "ISO %d PASS", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "ISO %d PASS", Id);
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
                }
            }
        }

        buf_cnt = snprintf(NULL, 0, "FINISH!!!");
        snprintf((char*)m_CalMsg, buf_cnt+1, "FINISH!!!");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
        CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
        CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);
#endif

        /*
            open last cal sensor (read pstore data & set2preview)
        */
#if CalEndPrv
        if(!m_CalOpen[lastCalId])
        {
            CalOpen = Cal_IplOpen(lastCalId, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
            if (CalOpen != E_OK)
            {
                DBG_ERR("Calibration IPL open fail\r\n");
                return;
            }
        }
#endif
    }
    else
        DBG_ERR("no calibration item\r\n");
    Cal_IplUnInit();
    DBG_IND("===== ISO finish =====\r\n");

}

void Cal_SenMShutter(void)
{
    #if 0
    CAL_DMA_RST ps_rst;
    IPL_PROC_ID Id;
    ER CalOpen;

    if ((CalInput_IPL_ID[IPL_ID_3] == TRUE)||(CalInput_IPL_ID[IPL_ID_4] == TRUE))
    {
        CalInput_IPL_ID[IPL_ID_3] = FALSE;
        CalInput_IPL_ID[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if((CalInput_IPL_ID[Id] == TRUE) && (!m_CalOpen[Id]))
        {
            CalOpen = Cal_IplOpen(Id, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
            if (CalOpen != E_OK)
            {
                DBG_ERR("Calibration IPL open fail\r\n");
                return;
            }
        }
    }

    if(CAL_ITEM)
    {
        for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
        {
            if(CalInput_IPL_ID[Id] == TRUE)
            {
                if(CAL_ITEM(Id))
                {
                    if(CAL_ITEM(Id)->fpMSHUTAdj(Id,CAL_UICB()->MSHUT_UI,m_Buf_StartAddr,m_Buf_Size) == E_OK)
                    {
                        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
                            DBG_ERR("sensor %d write header fail\r\n", Id);

                        //re-read all section because of maybe pstore have new section information after calibration function.
                        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));
                        //must be return E_OK at each calibration function excluding Cal_IplOpen()
                        if(ps_rst.erCode != E_OK)
                        {
                            DBG_ERR("sensor %d read header and section fail\r\n", Id);
                            return;
                        }
                        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
                        m_Buf_StartAddr = ps_rst.RemainAddr;
                        m_Buf_Size = ps_rst.RemainSize;
                    }
                    else
                        DBG_ERR("sensor %d M-Shutter Adjust fail\r\n", Id);
                }
                else
                    DBG_ERR("sensor %d no calibration item\r\n");
            }
        }
    }
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(CalInput_IPL_ID[Id] == TRUE)
        {
            if(Set2Preview)
                if(Set2Preview(Id, m_Buf_StartAddr,m_Buf_Size))
                    DBG_ERR("Set2Preview fail\r\n");
        }
    }
    #endif
}

void Cal_SenAWB(void)
{
    CAL_DMA_RST ps_rst;
    IPL_PROC_ID Id;
    ER CalOpen;
    UINT32 iplNum = 0;
    IPL_PROC_ID lastCalId = IPL_ID_1;
    static CHAR m_CalMsg[100];
    UINT32 buf_cnt = 0;

    // open sensor & hook R/W function point
    if (bInit == FALSE)
    {
        if(Cal_IplInit())
        {
            DBG_WRN("calibration init fail!\r\n");
            return;
        }
        else
        {
            bInit = TRUE;
        }

    }

    //check cal sensor id init
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if((m_CalInput[Id] == TRUE)&&(!m_CalInit[Id]))
        {
            DBG_WRN("calibration sensor %d need to init\r\n", Id);
            return;
        }
    }

    if ((m_CalInput[IPL_ID_3] == TRUE)||(m_CalInput[IPL_ID_4] == TRUE))
    {
        m_CalInput[IPL_ID_3] = FALSE;
        m_CalInput[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    /*
        set calibration ipl num
    */
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(m_CalInput[Id] == TRUE)
        {
            iplNum++;
        }
    }

    if(CAL_ITEM)
    {
        /*
            Cailbration
        */
        BOOL CalRstOK[IPL_ID_MAX_NUM] = {TRUE, TRUE, TRUE, TRUE};
        for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
        {
            if(m_CalInput[Id] == TRUE)
            {
                /*
                    open sensor (read pstore data & set2preview)
                    which sensor Id need to calibrate
                */
                if(!m_CalOpen[Id])
                {
                    CalOpen = Cal_IplOpen(Id, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
                    if (CalOpen != E_OK)
                    {
                        DBG_ERR("Calibration IPL open fail\r\n");
                        return;
                    }
                }
                /*
                    show OSD & wait key
                */
#if DNPCAL
#else
                CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);
                CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
#endif
                buf_cnt = snprintf(NULL, 0, "AWB IN PROCESSING");
                snprintf((char*)m_CalMsg, buf_cnt, "AWB IN PROCESSING");
                CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_BLACK);

                /*
                    calibration for sensor Id (no preview)
                */
                if(CAL_ITEM(Id)) //uninit sensor Id buffer & get calibration item
                {
                    m_CalOpen[Id] = FALSE; // preview buffer be uninit
                    if(CAL_ITEM(Id)->fpAWBAdj(Id,CAL_UICB()->AWB_UI,m_Buf_StartAddr,m_Buf_Size) == E_OK)
                    {
                        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
                            DBG_ERR("sensor %d write header fail\r\n", Id);

                        // re-read all section because of maybe pstore have new section information after calibration function.
                        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));

                        // must be return E_OK at each calibration function
                        if(ps_rst.erCode != E_OK)
                        {
                            DBG_ERR("sensor %d read header and section fail\r\n", Id);
                            return;
                        }
                        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
                        m_Buf_StartAddr = ps_rst.RemainAddr;
                        m_Buf_Size = ps_rst.RemainSize;
                    }
                    else
                        DBG_ERR("sensor %d AWB Adjust fail\r\n", Id);
                }
                else
                    DBG_ERR("sensor %d get calibration item fail\r\n");

                lastCalId = Id;

                /*
                    1. get calibration result
                    2. (save result in txt) depend on eng.cfg
                    3. get calibration final result to show on OSD
                */
                CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
                CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
                ER ER_Code = E_OK;
                UINT32 SenModeCondiCnt = 0;
                // get calibration result
                CAL_ITEM(Id)->fpGetAdjustRst(Id, NULL, CalDebugInfor_DNP);
                if(CAL_ITEM(Id)->fpGetDebugLevel(CALDEBUG_INFO)) //save result in txt & get calibration final result
                {
                    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
                    {
                        if (CalDebugInfor_DNP->CalDebugAWB->CalAeRst[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
                        {
                            // station name + sensor ID
                            buf_cnt = snprintf(NULL, 0, "AWB%d_", Id);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "AWB%d_", Id);
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            if (CalDebugInfor_DNP->CalDebugAWB->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "NG%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "NG%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                                CalRstOK[Id] = FALSE;
                            }
                            else
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "OK%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "OK%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            }
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            // calibration result (Rgain/Bgain)
                            buf_cnt = snprintf(NULL, 0, "%d_%d_", CalDebugInfor_DNP->CalDebugAWB->CalRgain[SenModeCondiCnt], CalDebugInfor_DNP->CalDebugAWB->CalBgain[SenModeCondiCnt]);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "%d_%d_", CalDebugInfor_DNP->CalDebugAWB->CalRgain[SenModeCondiCnt], CalDebugInfor_DNP->CalDebugAWB->CalBgain[SenModeCondiCnt]);
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            // ae information
                            buf_cnt = snprintf(NULL, 0, "%d_%d_%d_%d\r\n"
                                , CalDebugInfor_DNP->CalDebugAWB->CalAeRst[SenModeCondiCnt].Lum
                                , CalDebugInfor_DNP->CalDebugAWB->CalAeRst[SenModeCondiCnt].ISO
                                , CalDebugInfor_DNP->CalDebugAWB->CalAeRst[SenModeCondiCnt].ExpT
                                , CalDebugInfor_DNP->CalDebugAWB->CalAeRst[SenModeCondiCnt].AeTimeout);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "%d_%d_%d_%d\r\n"
                                , CalDebugInfor_DNP->CalDebugAWB->CalAeRst[SenModeCondiCnt].Lum
                                , CalDebugInfor_DNP->CalDebugAWB->CalAeRst[SenModeCondiCnt].ISO
                                , CalDebugInfor_DNP->CalDebugAWB->CalAeRst[SenModeCondiCnt].ExpT
                                , CalDebugInfor_DNP->CalDebugAWB->CalAeRst[SenModeCondiCnt].AeTimeout);

                            // save data in SD card
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);

                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }
                        }
                    }
                }
                else // get calibration final result
                {
                    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
                    {
                        if (CalDebugInfor_DNP->CalDebugAWB->CalAeRst[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
                        {
                            if (CalDebugInfor_DNP->CalDebugAWB->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
                            {
                                CalRstOK[Id] = FALSE;
                            }
                        }
                    }
                }

            } // calibration for input sensor ID

        } //per sensor ID

        /*
            show calibration final result on OSD
        */
        CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_RED);
#if DNPCAL
        for (Id = 0; Id <= lastCalId; Id++)
        {
            if (CalRstOK[Id] == FALSE)
            {
                DNP_STATUS[Id][2] = _CAL_OverFlow;
            }
            else
            {
                DNP_STATUS[Id][2] = _CAL_OK;
            }
        }
#else
        if (lastCalId == IPL_ID_1) // Only one sensor does not need to show sensor Id on OSD
        {
            if (CalRstOK[IPL_ID_1] == FALSE)
            {
                buf_cnt = snprintf(NULL, 0, "AWB FAIL");
                snprintf((char*)m_CalMsg, buf_cnt+1, "AWB FAIL");
                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }
            else
            {
                buf_cnt = snprintf(NULL, 0, "AWB PASS");
                snprintf((char*)m_CalMsg, buf_cnt+1, "AWB PASS");
                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
        }
        else //muti-sensor need to show sensor Id on OSD
        {
            for (Id = 0; Id <= lastCalId; Id++)
            {
                if (CalRstOK[Id] == FALSE)
                {
                    buf_cnt = snprintf(NULL, 0, "AWB %d FAIL", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "AWB %d FAIL", Id);
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                }
                else
                {
                    buf_cnt = snprintf(NULL, 0, "AWB %d PASS", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "AWB %d PASS", Id);
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
                }
            }
        }

        buf_cnt = snprintf(NULL, 0, "FINISH!!!");
        snprintf((char*)m_CalMsg, buf_cnt+1, "FINISH!!!");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
        CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
        CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);
#endif

        /*
            open last cal sensor (read pstore data & set2preview)
        */
#if CalEndPrv
        if(!m_CalOpen[lastCalId])
        {
            CalOpen = Cal_IplOpen(lastCalId, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
            if (CalOpen != E_OK)
            {
                DBG_ERR("Calibration IPL open fail\r\n");
                return;
            }
        }
#endif
    }
    else
        DBG_ERR("no calibration item\r\n");
    Cal_IplUnInit();
    DBG_IND("===== AWB finish =====\r\n");
}


void Cal_SenAWBGS(void)
{
    CAL_DMA_RST ps_rst;
    IPL_PROC_ID Id;
    ER CalOpen;
    UINT32 iplNum = 0;
    IPL_PROC_ID lastCalId = IPL_ID_1;

    // open sensor & hook R/W function point
    if (bInit == FALSE)
    {
        if(Cal_IplInit())
        {
            DBG_WRN("calibration init fail!\r\n");
            return;
        }
        else
        {
            bInit = TRUE;
        }

    }

    //check cal sensor id has init
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if((m_CalInput[Id] == TRUE)&&(!m_CalInit[Id]))
        {
            DBG_WRN("calibration sensor %d need to init\r\n", Id);
            return;
        }
    }

    if ((m_CalInput[IPL_ID_3] == TRUE)||(m_CalInput[IPL_ID_4] == TRUE))
    {
        m_CalInput[IPL_ID_3] = FALSE;
        m_CalInput[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    /*
        set calibration ipl num
    */
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(m_CalInput[Id] == TRUE)
        {
            iplNum++;
        }
    }

    if(CAL_ITEM)
    {
        for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
        {
            if(m_CalInput[Id] == TRUE)
            {
                /*
                    open sensor (read pstore data & set2preview)
                    which sensor Id need to calibrate
                */
                if(!m_CalOpen[Id])
                {
                    CalOpen = Cal_IplOpen(Id, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
                    if (CalOpen != E_OK)
                    {
                        DBG_ERR("Calibration IPL open fail\r\n");
                        return;
                    }
                }

                /*
                    calibration for sensor Id (no preview)
                */
                if(CAL_ITEM(Id)) //uninit sensor Id buffer & get calibration item
                {
                    m_CalOpen[Id] = FALSE; // preview buffer be uninit
                    if(CAL_ITEM(Id)->fpAWBGSAdj(Id,CAL_UICB()->AWB_UI,m_Buf_StartAddr,m_Buf_Size) == E_OK)
                    {
                        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
                            DBG_ERR("sensor %d write header fail\r\n", Id);

                        // re-read all section because of maybe pstore have new section information after calibration function.
                        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));

                        // must be return E_OK at each calibration function
                        if(ps_rst.erCode != E_OK)
                        {
                            DBG_ERR("sensor %d read header and section fail\r\n", Id);
                            return;
                        }
                        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
                        m_Buf_StartAddr = ps_rst.RemainAddr;
                        m_Buf_Size = ps_rst.RemainSize;
                    }
                    else
                        DBG_ERR("sensor %d AWB GS Adjust fail\r\n", Id);
                }
                else
                    DBG_ERR("sensor %d get calibration item fail\r\n");

                lastCalId = Id;

            } // calibration for input sensor ID

        } //per sensor ID

        /*
            open last cal sensor (read pstore data & set2preview)
        */
#if CalEndPrv
        if(!m_CalOpen[lastCalId])
        {
            CalOpen = Cal_IplOpen(lastCalId, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
            if (CalOpen != E_OK)
            {
                DBG_ERR("Calibration IPL open fail\r\n");
                return;
            }
        }
#endif
    }
    else
        DBG_ERR("no calibration item\r\n");
}

void Cal_SenECS(void)
{
    CAL_DMA_RST ps_rst;
    IPL_PROC_ID Id;
    ER CalOpen;
    UINT32 iplNum = 0;
    IPL_PROC_ID lastCalId = IPL_ID_1;
    static CHAR m_CalMsg[100];
    UINT32 buf_cnt = 0;

    // open sensor & hook R/W function point
    if (bInit == FALSE)
    {
        if(Cal_IplInit())
        {
            DBG_WRN("calibration init fail!\r\n");
            return;
        }
        else
        {
            bInit = TRUE;
        }

    }

    //check cal sensor id init
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if((m_CalInput[Id] == TRUE)&&(!m_CalInit[Id]))
        {
            DBG_WRN("calibration sensor %d need to init\r\n", Id);
            return;
        }
    }

    if ((m_CalInput[IPL_ID_3] == TRUE)||(m_CalInput[IPL_ID_4] == TRUE))
    {
        m_CalInput[IPL_ID_3] = FALSE;
        m_CalInput[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    /*
        set calibration ipl num
    */
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(m_CalInput[Id] == TRUE)
        {
            iplNum++;
        }
    }

    if(CAL_ITEM)
    {
        /*
            Cailbration
        */
        BOOL CalRstOK[IPL_ID_MAX_NUM] = {TRUE, TRUE, TRUE, TRUE};
        for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
        {
            if(m_CalInput[Id] == TRUE)
            {
                /*
                    open sensor (read pstore data & set2preview)
                    which sensor Id need to calibrate
                */
                if(!m_CalOpen[Id])
                {
                    CalOpen = Cal_IplOpen(Id, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
                    if (CalOpen != E_OK)
                    {
                        DBG_ERR("Calibration IPL open fail\r\n");
                        return;
                    }
                }
                /*
                    show OSD & wait key
                */
#if DNPCAL
#else
                CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);
                CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
#endif
                buf_cnt = snprintf(NULL, 0, "ECS IN PROCESSING");
                snprintf((char*)m_CalMsg, buf_cnt, "ECS IN PROCESSING");
                CAL_UICB()->AWB_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_BLACK);

                /*
                    calibration for sensor Id (no preview)
                */
                if(CAL_ITEM(Id)) //uninit sensor Id buffer & get calibration item
                {
                    m_CalOpen[Id] = FALSE; // preview buffer be uninit
                    if(CAL_ITEM(Id)->fpECSAdj(Id,CAL_UICB()->ECS_UI,m_Buf_StartAddr,m_Buf_Size) == E_OK)
                    {
                        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
                            DBG_ERR("sensor %d write header fail\r\n", Id);

                        // re-read all section because of maybe pstore have new section information after calibration function.
                        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));

                        // must be return E_OK at each calibration function
                        if(ps_rst.erCode != E_OK)
                        {
                            DBG_ERR("sensor %d read header and section fail\r\n", Id);
                            return;
                        }
                        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
                        m_Buf_StartAddr = ps_rst.RemainAddr;
                        m_Buf_Size = ps_rst.RemainSize;
                    }
                    else
                        DBG_ERR("sensor %d ECS Adjust fail\r\n", Id);
                }
                else
                    DBG_ERR("sensor %d get calibration item fail\r\n");

                lastCalId = Id;

                /*
                    1. get calibration result
                    2. (save result in txt) depend on eng.cfg
                    3. get calibration final result to show on OSD
                */
                CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
                CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
                ER ER_Code = E_OK;
                UINT32 SenModeCondiCnt = 0;
                // get calibration result
                CAL_ITEM(Id)->fpGetAdjustRst(Id, NULL, CalDebugInfor_DNP);
                if(CAL_ITEM(Id)->fpGetDebugLevel(CALDEBUG_INFO)) //save result in txt & get calibration final result
                {
                    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
                    {
                        if (CalDebugInfor_DNP->CalDebugECS->CalAeRst[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
                        {
                            // station name + sensor ID
                            buf_cnt = snprintf(NULL, 0, "ECS%d_", Id);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "ECS%d_", Id);
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            if (CalDebugInfor_DNP->CalDebugECS->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "NG%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "NG%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                                CalRstOK[Id] = FALSE;
                            }
                            else
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "OK%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "OK%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            }
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            // ae information
                            buf_cnt = snprintf(NULL, 0, "%d_%d_%d_%d\r\n"
                                , CalDebugInfor_DNP->CalDebugECS->CalAeRst[SenModeCondiCnt].Lum
                                , CalDebugInfor_DNP->CalDebugECS->CalAeRst[SenModeCondiCnt].ISO
                                , CalDebugInfor_DNP->CalDebugECS->CalAeRst[SenModeCondiCnt].ExpT
                                , CalDebugInfor_DNP->CalDebugECS->CalAeRst[SenModeCondiCnt].AeTimeout);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "%d_%d_%d_%d\r\n"
                                , CalDebugInfor_DNP->CalDebugECS->CalAeRst[SenModeCondiCnt].Lum
                                , CalDebugInfor_DNP->CalDebugECS->CalAeRst[SenModeCondiCnt].ISO
                                , CalDebugInfor_DNP->CalDebugECS->CalAeRst[SenModeCondiCnt].ExpT
                                , CalDebugInfor_DNP->CalDebugECS->CalAeRst[SenModeCondiCnt].AeTimeout);

                            // save data in SD card
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);

                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }
                        }
                    }
                }
                else // get calibration final result
                {
                    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
                    {
                        if (CalDebugInfor_DNP->CalDebugECS->CalAeRst[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
                        {
                            if (CalDebugInfor_DNP->CalDebugECS->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
                            {
                                CalRstOK[Id] = FALSE;
                            }
                        }
                    }
                }

            } // calibration for input sensor ID

        } //per sensor ID

        /*
            show calibration final result on OSD
        */
        CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_RED);
#if DNPCAL
        for (Id = 0; Id <= lastCalId; Id++)
        {
            if (CalRstOK[Id] == FALSE)
            {
                DNP_STATUS[Id][0] = _CAL_OverFlow;
            }
            else
            {
                DNP_STATUS[Id][0] = _CAL_OK;
            }
        }
#else
        if (lastCalId == IPL_ID_1) // Only one sensor does not need to show sensor Id on OSD
        {
            if (CalRstOK[IPL_ID_1] == FALSE)
            {
                buf_cnt = snprintf(NULL, 0, "ECS FAIL");
                snprintf((char*)m_CalMsg, buf_cnt+1, "ECS FAIL");
                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }
            else
            {
                buf_cnt = snprintf(NULL, 0, "ECS PASS");
                snprintf((char*)m_CalMsg, buf_cnt+1, "ECS PASS");
                CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
        }
        else //muti-sensor need to show sensor Id on OSD
        {
            for (Id = 0; Id <= lastCalId; Id++)
            {
                if (CalRstOK[Id] == FALSE)
                {
                    buf_cnt = snprintf(NULL, 0, "ECS %d FAIL", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "ECS %d FAIL", Id);
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                }
                else
                {
                    buf_cnt = snprintf(NULL, 0, "ECS %d PASS", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "ECS %d PASS", Id);
                    CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
                }
            }
        }

        buf_cnt = snprintf(NULL, 0, "FINISH!!!");
        snprintf((char*)m_CalMsg, buf_cnt+1, "FINISH!!!");
        CAL_UICB()->AWB_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
        CAL_UICB()->AWB_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
        CAL_UICB()->AWB_UI(UICB_CLEAN, NULL, UICOL_GREEN);
#endif

        /*
            open last cal sensor (read pstore data & set2preview)
        */
#if CalEndPrv
        if(!m_CalOpen[lastCalId])
        {
            CalOpen = Cal_IplOpen(lastCalId, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
            if (CalOpen != E_OK)
            {
                DBG_ERR("Calibration IPL open fail\r\n");
                return;
            }
        }
#endif
    }
    else
        DBG_ERR("no calibration item\r\n");

    Cal_IplUnInit();


    DBG_IND("===== ECS finish =====\r\n");
}

//#NT#2016/09/12#Silvia Wu -begin
//#NT# support lens center calibration
void Cal_LENSCEN(void)
{
    CAL_DMA_RST ps_rst;
    IPL_PROC_ID Id;
    ER CalOpen;
    UINT32 iplNum = 0;
    IPL_PROC_ID lastCalId = IPL_ID_1;
    static CHAR m_CalMsg[100];
    UINT32 buf_cnt = 0;

    // open sensor & hook R/W function point
    if (bInit == FALSE)
    {
        if(Cal_IplInit())
        {
            DBG_WRN("calibration init fail!\r\n");
            return;
        }
        else
        {
            bInit = TRUE;
        }

    }

    //check cal sensor id init
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if((m_CalInput[Id] == TRUE)&&(!m_CalInit[Id]))
        {
            DBG_WRN("calibration sensor %d need to init\r\n", Id);
            return;
        }
    }

    if ((m_CalInput[IPL_ID_3] == TRUE)||(m_CalInput[IPL_ID_4] == TRUE))
    {
        m_CalInput[IPL_ID_3] = FALSE;
        m_CalInput[IPL_ID_4] = FALSE;
        DBG_ERR("not support sensor 3&4\r\n");
    }

    /*
        set calibration ipl num
    */
    for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
    {
        if(m_CalInput[Id] == TRUE)
        {
            iplNum++;
        }
    }

    if(CAL_ITEM)
    {
        /*
            Cailbration
        */
        BOOL CalRstOK[IPL_ID_MAX_NUM] = {TRUE, TRUE, TRUE, TRUE};
        for (Id = 0; Id < IPL_ID_MAX_NUM; Id++)
        {
            if(m_CalInput[Id] == TRUE)
            {
                /*
                    open sensor (read pstore data & set2preview)
                    which sensor Id need to calibrate
                */
                if(!m_CalOpen[Id])
                {
                    CalOpen = Cal_IplOpen(Id, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
                    if (CalOpen != E_OK)
                    {
                        DBG_ERR("Calibration IPL open fail\r\n");
                        return;
                    }
                }
                /*
                    show OSD & wait key
                */
#if DNPCAL
#else
                CAL_UICB()->LENSCEN_UI(UICB_CLEAN, NULL, UICOL_GREEN);
                CAL_UICB()->LENSCEN_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
#endif
                buf_cnt = snprintf(NULL, 0, "LensCenter IN PROCESSING");
                snprintf((char*)m_CalMsg, buf_cnt, "LensCenter IN PROCESSING");
                CAL_UICB()->LENSCEN_UI(UICB_START,(CHAR*)m_CalMsg,UICOL_BLACK);

                /*
                    calibration for sensor Id (no preview)
                */
                if(CAL_ITEM(Id)) //uninit sensor Id buffer & get calibration item
                {
                    m_CalOpen[Id] = FALSE; // preview buffer be uninit
                    if(CAL_ITEM(Id)->fpLENSCENAdj(Id,CAL_UICB()->LENSCEN_UI,m_Buf_StartAddr,m_Buf_Size) == E_OK)
                    {
                        if(CAL_RW()->fpSectionWrite_Header() != E_OK)
                            DBG_ERR("sensor %d write header fail\r\n", Id);

                        // re-read all section because of maybe pstore have new section information after calibration function.
                        ps_rst = _PS_R(m_Header_Addr,(m_Header_Size + m_Buf_Size));

                        // must be return E_OK at each calibration function
                        if(ps_rst.erCode != E_OK)
                        {
                            DBG_ERR("sensor %d read header and section fail\r\n", Id);
                            return;
                        }
                        m_Header_Size = (m_Header_Size + m_Buf_Size) - ps_rst.RemainSize;
                        m_Buf_StartAddr = ps_rst.RemainAddr;
                        m_Buf_Size = ps_rst.RemainSize;
                    }
                    else
                        DBG_ERR("sensor %d LensCenter Adjust fail\r\n", Id);
                }
                else
                    DBG_ERR("sensor %d get calibration item fail\r\n");
                lastCalId = Id;
                /*
                    1. get calibration result
                    2. (save result in txt) depend on eng.cfg
                    3. get calibration final result to show on OSD
                */
                CAL_DEBUG_INFO CalDebugInfor_DNP1 = {0};
                CAL_DEBUG_INFO *CalDebugInfor_DNP = &CalDebugInfor_DNP1;
                ER ER_Code = E_OK;
                UINT32 SenModeCondiCnt = 0;
                // get calibration result
                CAL_ITEM(Id)->fpGetAdjustRst(Id, NULL, CalDebugInfor_DNP);
                if(CAL_ITEM(Id)->fpGetDebugLevel(CALDEBUG_INFO)) //save result in txt & get calibration final result
                {
                    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
                    {
                        if (CalDebugInfor_DNP->CalDebugLENSCEN->CalAeRst[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
                        {
                            // station name + sensor ID
                            buf_cnt = snprintf(NULL, 0, "LENSCEN%d_", Id);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "LENSCEN%d_", Id);
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->LENSCEN_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            if (CalDebugInfor_DNP->CalDebugLENSCEN->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "NG%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "NG%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                                CalRstOK[Id] = FALSE;
                            }
                            else
                            {
                                // result + sensor mode/condition
                                buf_cnt = snprintf(NULL, 0, "OK%d_", SenModeCondiCnt);
                                snprintf((char*)m_CalDbgMsg, buf_cnt+1, "OK%d_", SenModeCondiCnt);
                                ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            }
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->LENSCEN_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            // calibration result (center shift pixel)
                            buf_cnt = snprintf(NULL, 0, "%d_%d_%d_%d_%d_"
                            , CalDebugInfor_DNP->CalDebugLENSCEN->CalCenSftX[SenModeCondiCnt]
                            , CalDebugInfor_DNP->CalDebugLENSCEN->CalCenSftY[SenModeCondiCnt]
                            , CalDebugInfor_DNP->CalDebugLENSCEN->CalActStrX[SenModeCondiCnt]
                            , CalDebugInfor_DNP->CalDebugLENSCEN->CalActStrY[SenModeCondiCnt]
                            , CalDebugInfor_DNP->CalDebugLENSCEN->CalReliability[SenModeCondiCnt]);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "%d_%d_%d_%d_%d_"
                            , CalDebugInfor_DNP->CalDebugLENSCEN->CalCenSftX[SenModeCondiCnt]
                            , CalDebugInfor_DNP->CalDebugLENSCEN->CalCenSftY[SenModeCondiCnt]
                            , CalDebugInfor_DNP->CalDebugLENSCEN->CalActStrX[SenModeCondiCnt]
                            , CalDebugInfor_DNP->CalDebugLENSCEN->CalActStrY[SenModeCondiCnt]
                            , CalDebugInfor_DNP->CalDebugLENSCEN->CalReliability[SenModeCondiCnt]);
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);
                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->LENSCEN_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }

                            // ae information
                            buf_cnt = snprintf(NULL, 0, "%d_%d_%d_%d\r\n"
                                , CalDebugInfor_DNP->CalDebugLENSCEN->CalAeRst[SenModeCondiCnt].Lum
                                , CalDebugInfor_DNP->CalDebugLENSCEN->CalAeRst[SenModeCondiCnt].ISO
                                , CalDebugInfor_DNP->CalDebugLENSCEN->CalAeRst[SenModeCondiCnt].ExpT
                                , CalDebugInfor_DNP->CalDebugLENSCEN->CalAeRst[SenModeCondiCnt].AeTimeout);
                            snprintf((char*)m_CalDbgMsg, buf_cnt+1, "%d_%d_%d_%d\r\n"
                                , CalDebugInfor_DNP->CalDebugLENSCEN->CalAeRst[SenModeCondiCnt].Lum
                                , CalDebugInfor_DNP->CalDebugLENSCEN->CalAeRst[SenModeCondiCnt].ISO
                                , CalDebugInfor_DNP->CalDebugLENSCEN->CalAeRst[SenModeCondiCnt].ExpT
                                , CalDebugInfor_DNP->CalDebugLENSCEN->CalAeRst[SenModeCondiCnt].AeTimeout);

                            // save data in SD card
                            ER_Code = Cal_SaveData(m_CalDbgMsg, FALSE);

                            if (ER_Code != E_OK)
                            {
                                buf_cnt = snprintf(NULL, 0, "SD Card Full : STOP");
                                snprintf((char*)m_CalMsg, buf_cnt+1, "SD Card Full : STOP");
                                CAL_UICB()->LENSCEN_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                            }
                        }
                    }
                }
                else // get calibration final result
                {
                    for (SenModeCondiCnt=0; SenModeCondiCnt < Max_SenMode_Condition; SenModeCondiCnt++)
                    {
                        if (CalDebugInfor_DNP->CalDebugLENSCEN->CalAeRst[SenModeCondiCnt].ISO != 0)// check which sensor mode have been made to the calibration
                        {
                            if (CalDebugInfor_DNP->CalDebugLENSCEN->CalStatusRst[SenModeCondiCnt] != _CAL_DEBUG_OK)
                            {
                                CalRstOK[Id] = FALSE;
                            }
                        }
                    }
                }

            } // calibration for input sensor ID

        } //per sensor ID

        /*
            show calibration final result on OSD
        */
        CAL_UICB()->LENSCEN_UI(UICB_CLEAN, NULL, UICOL_RED);
#if 0 //DNPCAL
        for (Id = 0; Id <= lastCalId; Id++)
        {
            if (CalRstOK[Id] == FALSE)
            {
                DNP_STATUS[Id][0] = _CAL_OverFlow;
            }
            else
            {
                DNP_STATUS[Id][0] = _CAL_OK;
            }
        }
#else
        if (lastCalId == IPL_ID_1) // Only one sensor does not need to show sensor Id on OSD
        {
            if (CalRstOK[IPL_ID_1] == FALSE)
            {
                buf_cnt = snprintf(NULL, 0, "LensCenter FAIL");
                snprintf((char*)m_CalMsg, buf_cnt+1, "LensCenter FAIL");
                CAL_UICB()->LENSCEN_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
            }
            else
            {
                buf_cnt = snprintf(NULL, 0, "LensCenter PASS");
                snprintf((char*)m_CalMsg, buf_cnt+1, "LensCenter PASS");
                CAL_UICB()->LENSCEN_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
            }
        }
        else //muti-sensor need to show sensor Id on OSD
        {
            for (Id = 0; Id <= lastCalId; Id++)
            {
                if (CalRstOK[Id] == FALSE)
                {
                    buf_cnt = snprintf(NULL, 0, "LensCenter %d FAIL", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "LensCenter %d FAIL", Id);
                    CAL_UICB()->LENSCEN_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_RED);
                }
                else
                {
                    buf_cnt = snprintf(NULL, 0, "LensCenter %d PASS", Id);
                    snprintf((char*)m_CalMsg, buf_cnt+1, "LensCenter %d PASS", Id);
                    CAL_UICB()->LENSCEN_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_BLUE);
                }
            }
        }

        buf_cnt = snprintf(NULL, 0, "FINISH!!!");
        snprintf((char*)m_CalMsg, buf_cnt+1, "FINISH!!!");
        CAL_UICB()->LENSCEN_UI(UICB_PROCESSING,(CHAR*)m_CalMsg,UICOL_GREEN);
        CAL_UICB()->LENSCEN_UI(UICB_WAITKEY, NULL, UICOL_GREEN);
        CAL_UICB()->LENSCEN_UI(UICB_CLEAN, NULL, UICOL_GREEN);
#endif

        /*
            open last cal sensor (read pstore data & set2preview)
        */
#if CalEndPrv
        if(!m_CalOpen[lastCalId])
        {
            CalOpen = Cal_IplOpen(lastCalId, iplNum, OS_GetMempoolAddr(POOL_ID_APP)+(POOL_SIZE_APP/2),(POOL_SIZE_APP/2));
            if (CalOpen != E_OK)
            {
                DBG_ERR("Calibration IPL open fail\r\n");
                return;
            }
        }
#endif
    }
    else
        DBG_ERR("no calibration item\r\n");

    Cal_IplUnInit();


    DBG_IND("===== LensCenter finish =====\r\n");
}
//#NT#2016/09/12#Silvia Wu -end

UINT32 CalRwStoreOp_OnOpen(char *pSecName, CAL_RW_OPEN_SEL OpenSel)
{
    UINT32 hHandle = 0;
#if (PST_FUNC)
    UINT32 RWOperation = 0;
    RWOperation |= (OpenSel&_CAL_RD_ONLY)?PS_RDONLY:0;
    RWOperation |= (OpenSel&_CAL_WR_ONLY)?PS_WRONLY:0;
    RWOperation |= (OpenSel&_CAL_CREATE)?PS_CREATE:0;
    hHandle = (UINT32)PStore_OpenSection(pSecName, RWOperation);
#endif
    return hHandle;
}
//#NT#2016/06/07#Silvia Wu -begin
//#NT# fixed coverity warning
ER CalRwStoreOp_OnClose(UINT32 pSectionHandleAddr)
{
    ER er;
#if (PST_FUNC)
    er = PStore_CloseSection((PSTORE_SECTION_HANDLE*) pSectionHandleAddr);
#else
    er = E_SYS;
#endif
    return er;
}

ER CalRwStoreOp_OnRead(UINT8 *pcBuffer,UINT32 ulStartAddress, UINT32 ulDataLength, UINT32 pSectionHandleAddr)
{
    ER er;
#if (PST_FUNC)
    er = PStore_ReadSection(pcBuffer, ulStartAddress, ulDataLength, (PSTORE_SECTION_HANDLE*) pSectionHandleAddr);
#else
    er = E_SYS;
#endif
    return er;
}

ER CalRwStoreOp_OnWrite(UINT8 *pcBuffer, UINT32 ulStartAddress, UINT32 ulDataLength, UINT32 pSectionHandleAddr)
{
    ER er;
#if (PST_FUNC)
    er = PStore_WriteSection(pcBuffer, ulStartAddress, ulDataLength, (PSTORE_SECTION_HANDLE*) pSectionHandleAddr);
#else
    er = E_SYS;
#endif
    return er;
}

ER CalRwStoreOp_OnDelete(char *pSecName)
{
    ER er;
#if (PST_FUNC)
    er = PStore_DeleteSection(pSecName);
#else
    er = E_SYS;
#endif
    return er;
}
//#NT#2016/06/07#Silvia Wu -end
//@}
#endif
