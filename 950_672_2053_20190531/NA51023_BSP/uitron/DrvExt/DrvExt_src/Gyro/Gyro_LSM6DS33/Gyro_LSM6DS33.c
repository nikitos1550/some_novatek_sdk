/*
    Gyro Driver ST LSM6DS33.

    @file       GyroDrv_LSM6DS33.c
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

#include <stdio.h>
#include <string.h>
#include "Type.h"
#include "ErrorNo.h"
#include "GyroDrv.h"
#include "spi.h"
#include "Delay.h"
#include "time.h"

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          GyroDrv
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define GDCONST_GD_NUM_PER_FRM (8)
// gyro auto calibate on gyro_open
#define AUTO_GYRO_CALIBRATION   0

typedef enum
{
    GYRO_IDLE   = 0,        ///< idle
    GYRO_READY  = 1,        ///< ready for read/write
    GYRO_RUN    = 2,        ///< running
    ENUM_DUMMY4WORD(GYRO_STATUS)
} GYRO_STATUS;


static GYRO_STATUS g_GyroStatus = GYRO_IDLE;
static GYRO_OP_MODE g_GyroOpMode = GYRO_OP_MODE_SINGLE_ACCESS;
static GYRO_FREE_RUN_PARAM  gyroFreeRunParm={0};
static SPI_ID g_TestSpiID = SPI_ID_3;
const static UINT32 g_uiReadFrequency  = 1000000; // 1 MHz for read
const static UINT32 g_uiWriteFrequency = 1000000; // 1 MHz for write
static INT32  g_xOffset = 0;
static INT32  g_yOffset = 0;
static INT32  g_zOffset = 0;
static void (*pGyroCB)(void) = NULL;
static GYRO_CFGINFO  _SECTION(".kercfg_data") g_GyroCfgInfo={0, //RSC_GYRO_AXIS_X;
                                    1, //RSC_GYRO_AXIS_Y;
                                    2, //RSC_GYRO_AXIS_Z;
                                    0, //RSC_DIR_POS
                                    0, //RSC_DIR_POS
                                    0};//RSC_DIR_POS

#define GDI2_GB_NUM (32)
static UINT32 puiGyroBuf[GDI2_GB_NUM][5]={0};
static UINT32 puiAccelBuf[GDI2_GB_NUM][5]={0};
static UINT32 uiGyroBufInIdx = 0, uiGyroBufOutIdx = 0, uiGyroQueueCount = 0;


#define REG_ACCEL_XOUT_L            (0x28)
#define REG_ACCEL_XOUT_H            (0x29)
#define REG_ACCEL_YOUT_L            (0x2A)
#define REG_ACCEL_YOUT_H            (0x2B)
#define REG_ACCEL_ZOUT_L            (0x2C)
#define REG_ACCEL_ZOUT_H            (0x2D)

#define REG_GYRO_XOUT_L             (0x22)
#define REG_GYRO_XOUT_H             (0x23)
#define REG_GYRO_YOUT_L             (0x24)
#define REG_GYRO_YOUT_H             (0x25)
#define REG_GYRO_ZOUT_L             (0x26)
#define REG_GYRO_ZOUT_H             (0x27)


#define REG_WHO_AM_I                (0x0F)
#define REG_WRITE                   (0x00)
#define REG_READ                    (0x80)

#define REG_DELAY                   (0xFF)

#define GYRO_ID                     (0x69)

typedef struct
{
    UINT32  uiAddress;                      // Panel Register Address
    UINT32  uiValue;                        // Panel Register config value
}tGYRO_CMD;

tGYRO_CMD cmd_gyro_init[]=
{
    0x12,    0x05,      // Reset
    0xFF,    0x01,      // delay 1 ms
    0x10,    0x37,      // Accelerometer Configuration +-16g, 50Hz
    0x11,    0x80,      // Gyro Configuration        245 dps, 1.66KHz
    0x18,    0x38,      // Enable Accelerometer output
    0x19,    0x38,      // Enable Gyro output


};

#if AUTO_GYRO_CALIBRATION
static BOOL Gyro_Calibration(void)
{
    INT32    GyroX,GyroY,GyroZ;
    INT32    i, SampleCount = 100, Retry = 3;
    BOOL     result = FALSE;
    UINT32   gyroInitZeroTolerance = 10;
    INT32    tempX,tempY,tempZ;
    INT32    ZeroOffsetMax, ZeroOffsetMin;

    GyroX =0;
    GyroY =0;
    GyroZ =0;
    ZeroOffsetMax = gyro_getLSB() * gyroInitZeroTolerance;
    ZeroOffsetMin = -ZeroOffsetMax;
    DBG_IND("ZeroOffsetMax=%d, ZeroOffsetMin=%d\r\n",ZeroOffsetMax,ZeroOffsetMin);
    gyro_setCalZeroOffset(0,0,0);
    while (Retry)
    {
        for (i=0;i<SampleCount;i++)
        {
            gyro_getSingleData(&tempX, &tempY, &tempZ);
            if (tempX > ZeroOffsetMax || tempX < ZeroOffsetMin || tempY > ZeroOffsetMax || tempY < ZeroOffsetMin || tempZ > ZeroOffsetMax || tempZ < ZeroOffsetMin)
            {
                DBG_ERR("tempX=%5d, tempY=%5d, tempZ=%5d\r\n",tempX,tempY,tempZ);
                Retry--;
                Delay_DelayMs(100);
                break;
            }
            GyroX+=tempX;
            GyroY+=tempY;
            GyroZ+=tempZ;
            DBG_IND("tempX=%5d, tempY=%5d, tempZ=%5d\r\n",tempX,tempY,tempZ);
            Delay_DelayMs(4);
        }
        if (i >= SampleCount)
        {
            result = TRUE;
            Retry = 0;
        }
    }
    GyroX/=SampleCount;
    GyroY/=SampleCount;
    GyroZ/=SampleCount;
    if (result == TRUE)
    {
        gyro_setCalZeroOffset(-GyroX,-GyroY,-GyroZ);
    }
    DBG_IND("GyroX=%5d, GyroY=%5d, GyroZ=%5d\r\n",GyroX,GyroY,GyroZ);
    return TRUE;
}
#endif

void gyro_cfg(GYRO_CFGINFO *cfgInfo)
{
    g_GyroCfgInfo = *cfgInfo;
    DBG_IND("Axis= %d, %d, %d\r\n",g_GyroCfgInfo.AxisSelec[0],g_GyroCfgInfo.AxisSelec[1],g_GyroCfgInfo.AxisSelec[2]);
    DBG_IND("Dir= %d, %d, %d\r\n",g_GyroCfgInfo.DirSelec[0],g_GyroCfgInfo.DirSelec[1],g_GyroCfgInfo.DirSelec[2]);
}

ER gyro_open(GYRO_OPENOBJ *openObj)
{

    DBG_IND("openObj =0x%x!!\r\n", openObj);
    // parameter check
    if (openObj && openObj->FP_GYRO_CB)
    {
        pGyroCB = openObj->FP_GYRO_CB;
    }
    // state check: only while idle
    if(g_GyroStatus!=GYRO_IDLE)
    {

        //DBG_WRN("already opened , g_GyroStatus=%d!!\r\n", g_GyroStatus);
        return E_OK;
    }
    uiGyroBufOutIdx = 0;
    uiGyroBufInIdx = 0;


    // power on gyro
    // always power-on

    // initializa gyro
    {
        UINT32 uiRecv1 = 0;

        spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_ENG_GYRO_UNIT, FALSE);  // normal mode

        spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_FREQ, g_uiReadFrequency);

        spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_BUSMODE, SPI_MODE_3);
        spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_MSB_LSB, SPI_MSB);
        spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_CS_ACT_LEVEL, SPI_CS_ACT_LEVEL_LOW);
        spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_CS_CK_DLY, 1);

        // Read gyro ID
        spi_open(g_TestSpiID);
        spi_setCSActive(g_TestSpiID, TRUE);
        spi_setTransferLen(g_TestSpiID, SPI_TRANSFER_LEN_2BYTES);
        spi_writeReadSingle(g_TestSpiID, (REG_READ|REG_WHO_AM_I)<<8, &uiRecv1);
        spi_setCSActive(g_TestSpiID, FALSE);
        spi_close(g_TestSpiID);

        if ((uiRecv1&0xFF) != GYRO_ID)
        {
            DBG_ERR("WHO_AM_I (0x%x) != 0x%x\r\n",uiRecv1,GYRO_ID);
            return E_SYS;
        }
    }
    // set initial value
    {
        UINT32 i;
        spi_open(g_TestSpiID);
        spi_setTransferLen(g_TestSpiID, SPI_TRANSFER_LEN_1BYTE);
        for (i=0;i<sizeof(cmd_gyro_init)/sizeof(tGYRO_CMD);i++)
        {
            if (REG_DELAY == cmd_gyro_init[i].uiAddress)
            {
                Delay_DelayMs(cmd_gyro_init[i].uiValue);
                continue;
            }
            spi_setCSActive(g_TestSpiID, TRUE);
            spi_writeSingle(g_TestSpiID, REG_WRITE|(cmd_gyro_init[i].uiAddress&0x7f));
            spi_writeSingle(g_TestSpiID, cmd_gyro_init[i].uiValue);
            spi_setCSActive(g_TestSpiID, FALSE);
        }
        spi_close(g_TestSpiID);
    }

    // state change: ready for gyro
    g_GyroStatus = GYRO_READY;

    // Do gyro calibration
    #if AUTO_GYRO_CALIBRATION
    Gyro_Calibration();
    #endif
    return E_OK;
}


ER gyro_close(void)
{

    DBG_IND("\r\n");
    // state check: only while idle
    if(g_GyroOpMode==GYRO_OP_MODE_FREE_RUN)
    {
        // disable free run
        spi_stopGyro(g_TestSpiID);
        spi_close(g_TestSpiID);

        g_GyroOpMode = GYRO_OP_MODE_SINGLE_ACCESS;
        g_GyroStatus = GYRO_READY;
    }
    #if 0
    if(g_GyroStatus==GYRO_RUN)
    {
        DBG_ERR("GYRO ERR: gyro_close, g_GyroStatus=%d!!\r\n", g_GyroStatus);
        return E_SYS;
    }
    #endif

    // stop/pause gyro
    // not-yet

    // power off gyro
    // always power-on

    // state change: ready for gyro
    g_GyroStatus = GYRO_IDLE;

    return E_OK;
}

ER gyro_shutdown(void)
{
    return E_OK;
}

ER gyro_readReg(UINT32 uiAddr, UINT32 * puiData)
{
    // state check: only while ready
    if(g_GyroStatus!=GYRO_READY)
    {
        DBG_ERR("GYRO ERR: gyro_readReg, g_GyroStatus=%d!!\r\n", g_GyroStatus);
        return E_SYS;
    }

    // state change: ready -> run
    g_GyroStatus = GYRO_RUN;

    // read register
    //normal job
    {
        UINT32 uiRecv1 = 0;//, uiRecv2;
        spi_open(g_TestSpiID);

        spi_setCSActive(g_TestSpiID, TRUE);
        spi_setTransferLen(g_TestSpiID, SPI_TRANSFER_LEN_1BYTE);
        spi_writeSingle(g_TestSpiID, (REG_READ)|(uiAddr&0x7f));
        spi_setTransferLen(g_TestSpiID, SPI_TRANSFER_LEN_2BYTES);
        spi_writeReadSingle(g_TestSpiID, 0x00, &uiRecv1);
        spi_setCSActive(g_TestSpiID, FALSE);

        spi_close(g_TestSpiID);

        *puiData = (uiRecv1&0xFFFF);
    }

    // state change: run -> ready
    g_GyroStatus = GYRO_READY;

    return E_OK;
}

ER gyro_writeReg(UINT32 uiAddr, UINT32 uiData)
{
    // state check: only while ready
    if(g_GyroStatus!=GYRO_READY)
    {
        DBG_ERR("GYRO ERR: gyro_readReg, g_GyroStatus=%d!!\r\n", g_GyroStatus);
        return E_SYS;
    }

    // state change: ready -> run
    g_GyroStatus = GYRO_RUN;

    // write register
    {
    UINT32 uiRecv1;//, uiRecv2;

    spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_FREQ, g_uiWriteFrequency);

    spi_open(g_TestSpiID);

    spi_setCSActive(g_TestSpiID, TRUE);
    spi_setTransferLen(g_TestSpiID, SPI_TRANSFER_LEN_1BYTE);
    spi_writeSingle(g_TestSpiID, (REG_WRITE)|(uiAddr&0x7f));
    spi_setTransferLen(g_TestSpiID, SPI_TRANSFER_LEN_2BYTES);
    spi_writeReadSingle(g_TestSpiID, uiData, &uiRecv1);
    spi_setCSActive(g_TestSpiID, FALSE);

    spi_close(g_TestSpiID);

    spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_FREQ, g_uiReadFrequency);

    }

    // state change: run -> ready
    g_GyroStatus = GYRO_READY;

    return E_OK;
}


ER gyro_setFreeRunParam(GYRO_FREE_RUN_PARAM *frParam)
{
    // state check: only while idle
    //not-yet

    //g_uiGyroFrPeriod = frParam->uiPeriodUs;
    //g_uiGyroFrNumber = frParam->uiDataNum;

    if (!frParam)
    {
        DBG_ERR("frParam is NULL\r\n");
        return E_PAR;
    }
    if (frParam->uiDataNum > GDI2_GB_NUM)
    {
        DBG_ERR("uiDataNum over limit %d\r\n",GDI2_GB_NUM);
        return E_PAR;
    }
    if (frParam->uiTriggerIdx >= frParam->uiDataNum)
    {
        DBG_ERR("uiTriggerIdx %d, uiDataNum  %d\r\n",frParam->uiTriggerIdx,frParam->uiDataNum);
        return E_PAR;
    }
    DBG_IND("uiPeriodUs %d, uiDataNum %d, triggerMode %d, triggerIdx %d\r\n", frParam->uiPeriodUs, frParam->uiDataNum, frParam->triggerMode,frParam->uiTriggerIdx);
    gyroFreeRunParm.uiPeriodUs = frParam->uiPeriodUs;
    gyroFreeRunParm.uiDataNum = frParam->uiDataNum;
    gyroFreeRunParm.uiTriggerIdx = frParam->uiTriggerIdx;
    gyroFreeRunParm.triggerMode = frParam->triggerMode;


    return E_OK;
}

static void gyro_HandleGyroEvent(SPI_GYRO_INT gyroInt)
{
    ER Status;
    UINT32 i, j, k;
    GYRO_BUF_QUEUE gyroData;

    switch (gyroInt)
    {
    case SPI_GYRO_INT_SYNC_END:
        j = uiGyroBufInIdx;
        if (uiGyroBufInIdx == 0)
        {
            uiGyroQueueCount = ((gyroFreeRunParm.uiDataNum - gyroFreeRunParm.uiTriggerIdx) << 1);
        }
        else
        {
            uiGyroQueueCount = (gyroFreeRunParm.uiDataNum << 1);
        }
        for (i=0; i<uiGyroQueueCount; i++)
        {
            Status = spi_getGyroData(g_TestSpiID, &gyroData);
            k = j%GDI2_GB_NUM;
            if (i%2 ==0)
            {
                puiGyroBuf[k][0] = gyroData.uiFrameID;
                puiGyroBuf[k][1] = gyroData.uiDataID;

                //  x =  byte 1 | byte2 (byte 0 is address, we ignore it)
                puiGyroBuf[k][2] = ((gyroData.vRecvWord[0]&0x0000FF00) >> 8)  | ((gyroData.vRecvWord[0]&0x00FF0000) >> 8);
                //  y =  byte 3 | byte4
                puiGyroBuf[k][3] = ((gyroData.vRecvWord[0]&0xFF000000) >> 24) | ((gyroData.vRecvWord[1]&0x000000FF) << 8);
                //  z =  byte 5 | byte6
                puiGyroBuf[k][4] = ((gyroData.vRecvWord[1]&0x0000FF00) >> 8)  | ((gyroData.vRecvWord[1]&0x00FF0000) >> 8);
            }
            else
            {
                puiAccelBuf[k][0] = gyroData.uiFrameID;
                puiAccelBuf[k][1] = gyroData.uiDataID;

                //  x =  byte 1 | byte2 (byte 0 is address, we ignore it)
                puiAccelBuf[k][2] = ((gyroData.vRecvWord[0]&0x0000FF00) >> 8)  | ((gyroData.vRecvWord[0]&0x00FF0000) >> 8);
                //  y =  byte 3 | byte4
                puiAccelBuf[k][3] = ((gyroData.vRecvWord[0]&0xFF000000) >> 24) | ((gyroData.vRecvWord[1]&0x000000FF) << 8);
                //  z =  byte 5 | byte6
                puiAccelBuf[k][4] = ((gyroData.vRecvWord[1]&0x0000FF00) >> 8)  | ((gyroData.vRecvWord[1]&0x00FF0000) >> 8);

                //DBG_IND("k = %d, gsX = %d, gsY = %d, gsZ = %d\r\n",k,puiAccelBuf[k][2],puiAccelBuf[k][2],puiAccelBuf[k][4]);
                j++;
            }
        }
        uiGyroBufInIdx += gyroFreeRunParm.uiDataNum;
        DBG_IND("Sync End = %d ms, uiGyroQueueCount = %d\r\n",clock()/1000,uiGyroQueueCount);
        break;
    case SPI_GYRO_INT_OVERRUN:
        DBG_ERR("SPI_GYRO_INT_OVERRUN!!\r\n");
        break;
    case SPI_GYRO_INT_SEQ_ERR:

        DBG_ERR("SPI_GYRO_INT_SEQ_ERR!! %d ms, count=%d\r\n",clock()/1000,spi_getGyroQueueCount(g_TestSpiID));
        break;

    case SPI_GYRO_INT_QUEUE_THRESHOLD:
        DBG_ERR("SPI_GYRO_INT_QUEUE_THRESHOLD!!\r\n");
        break;
    case SPI_GYRO_INT_QUEUE_OVERRUN:
        DBG_ERR("SPI_GYRO_INT_QUEUE_OVERRUN!!\r\n");
        break;
    default:
        DBG_ERR("%d\r\n",gyroInt);
        break;
    }

    if(pGyroCB!=NULL)
    {
        pGyroCB();
    }
}

static ER gyro_setFreeRun(void)
{
    UINT32 i;
    SPI_GYRO_INFO gyroInfo= {0};

    //
    // 1. Prepare data buffer to store polled gyroscope data
    //
    //useless in new driver//uiDumpCount = 0;
    //useless in new driver//pNextBuf = (UINT32*)uiDumpBufAddr;

    //
    // 2. Start SPI gyro polling
    //
    //gyroInfo.gyroMode = SPI_GYRO_MODE_SIE_SYNC;

    if (gyroFreeRunParm.triggerMode == GYRO_FREERUN_SIE_SYNC)
        gyroInfo.gyroMode = SPI_GYRO_MODE_SIE_SYNC;
    else
        gyroInfo.gyroMode = SPI_GYRO_MODE_FREE_RUN;

    gyroInfo.uiTransferLen = 2;
    gyroInfo.uiTransferCount = gyroFreeRunParm.uiDataNum;
    gyroInfo.uiOpInterval       = 1;
     // reserve 200 us for last transfer
    gyroInfo.uiTransferInterval = (gyroFreeRunParm.uiPeriodUs-200)/gyroFreeRunParm.uiDataNum;
    gyroInfo.uiOp0Length = 7;
    DBG_IND("TLen = %d TCount= %d  OpInt =%d, TInt= %d Op0Len=%d\r\n",gyroInfo.uiTransferLen,gyroInfo.uiTransferCount,gyroInfo.uiOpInterval,gyroInfo.uiTransferInterval,gyroInfo.uiOp0Length);
    gyroInfo.vOp0OutData[0] = REG_READ|REG_GYRO_XOUT_L;
    for (i=1; i<gyroInfo.uiOp0Length; i++)
    {
        gyroInfo.vOp0OutData[i] = 0;
    }
    gyroInfo.uiOp1Length = 7;
    gyroInfo.vOp1OutData[0] = REG_READ|REG_ACCEL_XOUT_L;
    for (i=1; i<gyroInfo.uiOp1Length; i++)
    {
        gyroInfo.vOp1OutData[i] = 0;
    }

    gyroInfo.pEventHandler = gyro_HandleGyroEvent;

    spi_open(g_TestSpiID);

    spi_setConfig(g_TestSpiID, SPI_CONFIG_ID_GYRO_SYNC_END_OFFSET, gyroFreeRunParm.uiTriggerIdx);

    spi_setTransferLen(g_TestSpiID, SPI_TRANSFER_LEN_1BYTE);

    spi_startGyro(g_TestSpiID, &gyroInfo);

    return E_OK;
}

UINT32 gyro_getLSB(void)
{
    return 245;
}


void gyro_RSCInfoInit(GYRO_RSCINFOINIT *pRSCInfoInit)
{
    // please refer to IPL_Ctrl_RSC_Int.h file for the GDFmt/FDDir definition
    pRSCInfoInit->gdFormat = 0;//_INT32
    pRSCInfoInit->AxisSelec[0] = g_GyroCfgInfo.AxisSelec[0];
    pRSCInfoInit->AxisSelec[1] = g_GyroCfgInfo.AxisSelec[1];
    pRSCInfoInit->AxisSelec[2] = g_GyroCfgInfo.AxisSelec[2];
    pRSCInfoInit->DirSelec[0] = g_GyroCfgInfo.DirSelec[0];
    pRSCInfoInit->DirSelec[1] = g_GyroCfgInfo.DirSelec[1];
    pRSCInfoInit->DirSelec[2] = g_GyroCfgInfo.DirSelec[2];
    pRSCInfoInit->uiDataUnit = (65536/gyro_getLSB());
    pRSCInfoInit->uiCalGain[0] = 1024;
    pRSCInfoInit->uiCalGain[1] = 1024;
    pRSCInfoInit->uiCalGain[2] = 1024;
    pRSCInfoInit->iCalOfs[0] = 0;
    pRSCInfoInit->iCalOfs[1] = 0;
    pRSCInfoInit->iCalOfs[2] = 0;
}


ER gyro_setMode(GYRO_OP_MODE opMode)
{
    DBG_IND("opMode %d\r\n", opMode);

    if(g_GyroStatus==GYRO_IDLE)
    {
        //DBG_WRN("Gyro is not opened\r\n");
        return E_SYS;
    }
    // state check: only while idle
    if((g_GyroStatus!=GYRO_READY) && (g_GyroOpMode!=GYRO_OP_MODE_FREE_RUN))
    {
        DBG_ERR("gyro_setMode, g_GyroOpMode = %d, g_GyroStatus=%d!!\r\n", g_GyroOpMode, g_GyroStatus);
        return E_SYS;
    }

    if(opMode==GYRO_OP_MODE_SINGLE_ACCESS)
    {
        if(g_GyroOpMode==GYRO_OP_MODE_SINGLE_ACCESS)
        {
            //DBG_WRN("gyro_setMode, already single access mode!!\r\n");
            return E_OK;
        }
        else if(g_GyroOpMode==GYRO_OP_MODE_FREE_RUN)
        {
            // disable free run
            spi_stopGyro(g_TestSpiID);
            spi_close(g_TestSpiID);

            g_GyroOpMode = GYRO_OP_MODE_SINGLE_ACCESS;
            g_GyroStatus = GYRO_READY;
            uiGyroBufInIdx = 0;
            uiGyroBufOutIdx = 0;
        }
        else
        {
            DBG_ERR("gyro_setMode, g_GyroOpMode=%d!!\r\n", opMode);
            return E_SYS;
        }
    }
    else if(opMode==GYRO_OP_MODE_FREE_RUN)
    {
        if(g_GyroOpMode==GYRO_OP_MODE_FREE_RUN)
        {
            //DBG_WRN("gyro_setMode, already free run mode!!\r\n");
            return E_OK;
        }
        else if(g_GyroOpMode==GYRO_OP_MODE_SINGLE_ACCESS)
        {
            if(gyroFreeRunParm.uiPeriodUs==0)
            {
                DBG_ERR("gyro_setMode, run mode param is not set!!\r\n");
                return E_SYS;
            }
            // enable free run
            gyro_setFreeRun();

            g_GyroOpMode = GYRO_OP_MODE_FREE_RUN;
            g_GyroStatus = GYRO_RUN;
        }
        else
        {
            DBG_ERR("gyro_setMode, g_GyroOpMode=%d!!\r\n", opMode);
            return E_SYS;
        }
    }
    else
    {
        DBG_ERR("gyro_setMode, opMode=%d!!\r\n", opMode);
        return E_SYS;
    }

    return E_OK;
}

static void gyro_AdjustDataOffset(INT32* gx, INT32* gy, INT32* gz)
{
    INT32  tempX,tempY,tempZ;

    //DBG_IND("gx=%ld, gy=%ld, g_xOffset = %ld, g_yOffset = %ld\r\n",*gx,*gy,g_xOffset,g_yOffset);
    tempX = (INT16)*gx;
    if (tempX + g_xOffset > 32767)
        tempX = 32767;
    else if (tempX + g_xOffset < -32768)
        tempX = -32768;
    else
        tempX +=g_xOffset;

    tempY = (INT16)*gy;
    if (tempY + g_yOffset > 32767)
        tempY = 32767;
    else if (tempY + g_yOffset < -32768)
        tempY = -32768;
    else
        tempY +=g_yOffset;

    tempZ = (INT16)*gz;
    if (tempZ + g_zOffset > 32767)
        tempZ = 32767;
    else if (tempZ + g_zOffset < -32768)
        tempZ = -32768;
    else
        tempZ +=g_zOffset;

    *gx = tempX;
    *gy = tempY;
    *gz = tempZ;

    //DBG_IND("gx=%ld, gy=%ld \r\n",*gx,*gy);
}

ER gyro_getSingleData(INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ)
{
    UINT32  tempData = 0;
    if (!puiDatX)
    {
        DBG_ERR("puiDatX is NULL\r\n");
        return E_PAR;
    }
    if (!puiDatY)
    {
        DBG_ERR("puiDatY is NULL\r\n");
        return E_PAR;
    }
    if (!puiDatZ)
    {
        DBG_ERR("puiDatZ is NULL\r\n");
        return E_PAR;
    }
    gyro_readReg(REG_GYRO_XOUT_L, &tempData);
    *(UINT32*)puiDatX = ((tempData & 0x0000FF00) >> 8) | ((tempData & 0x000000FF) << 8);
    gyro_readReg(REG_GYRO_YOUT_L, &tempData);
    *(UINT32*)puiDatY = ((tempData & 0x0000FF00) >> 8) | ((tempData & 0x000000FF) << 8);
    gyro_readReg(REG_GYRO_ZOUT_L, &tempData);
    *(UINT32*)puiDatZ = ((tempData & 0x0000FF00) >> 8) | ((tempData & 0x000000FF) << 8);
    gyro_AdjustDataOffset(puiDatX, puiDatY, puiDatZ);
    return E_OK;
}




ER gyro_getFreeRunData(UINT32 *puiNum, INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ)
{
    UINT32 i, j;
    UINT32 uiDataNum = gyroFreeRunParm.uiDataNum;
    // state check: only while run
    if(g_GyroStatus!=GYRO_RUN)
    {
        DBG_ERR("gyro_getGyroData, g_GyroStatus=%d!!\r\n", g_GyroStatus);
        return E_SYS;
    }

    // op mode check: only while free run mode
    if(g_GyroOpMode!=GYRO_OP_MODE_FREE_RUN)
    {
        DBG_ERR("gyro_getGyroData, not in free run mode!!\r\n", g_GyroOpMode);
        return E_SYS;
    }
    // normal job
    if((uiGyroBufOutIdx+uiDataNum)==uiGyroBufInIdx)
    {
        j = uiGyroBufOutIdx;
        for (i=0; i<uiDataNum; i++)
        {
            UINT32 k;
            k = j%GDI2_GB_NUM;
            *(puiDatX+i) = puiGyroBuf[k][2];
            *(puiDatY+i) = puiGyroBuf[k][3];
            *(puiDatZ+i) = puiGyroBuf[k][4];
            gyro_AdjustDataOffset(puiDatX+i,puiDatY+i,puiDatZ+i);
            j++;
            if (i==0)
            {
                DBG_IND("Count = %d,gyro x=%5d, y=%5d, z=%5d\r\n",uiGyroQueueCount>>1,*puiDatX,*puiDatY,*puiDatZ);
                DBG_IND("accel x=%5d, y=%5d, z=%5d\r\n",puiAccelBuf[k][2],puiAccelBuf[k][3],puiAccelBuf[k][4]);
            }
        }

        uiGyroBufOutIdx = j;
        *puiNum = (uiGyroQueueCount >> 1);

    }
    else if((uiGyroBufOutIdx+uiDataNum)>uiGyroBufInIdx)
    {
        DBG_ERR("asking for future data!!\r\n");
        memset((void *)puiDatX, 0, uiDataNum*sizeof(UINT32));
        memset((void *)puiDatY, 0, uiDataNum*sizeof(UINT32));
        memset((void *)puiDatZ, 0, uiDataNum*sizeof(UINT32));
        *puiNum = 0;
    }
    else if((uiGyroBufOutIdx+uiDataNum)<uiGyroBufInIdx)
    {
        DBG_ERR("asking for past data!!\r\n");
        uiGyroBufOutIdx =  uiGyroBufInIdx;
        memset((void *)puiDatX, 0, uiDataNum*sizeof(UINT32));
        memset((void *)puiDatY, 0, uiDataNum*sizeof(UINT32));
        memset((void *)puiDatZ, 0, uiDataNum*sizeof(UINT32));
        //*puiNum = uiDataNum;
        *puiNum = (uiGyroQueueCount >> 1);
    }
    // state change: run -> run
    g_GyroStatus = GYRO_RUN;

    return E_OK;
}

void gyro_setCalZeroOffset(INT32 xOffset, INT32 yOffset, INT32 zOffset)
{
    g_xOffset = xOffset;
    g_yOffset = yOffset;
    g_zOffset = zOffset;
    DBG_IND("xOffset = %d, yOffset = %d, zOffset = %d\r\n",xOffset,yOffset,zOffset);
}

static void gyro_AdjustGsData(INT32* gx, INT32* gy, INT32* gz)
{
    INT32  tempX,tempY,tempZ;

    //DBG_IND("gx=%ld, gy=%ld, gz=%ld\r\n",*gx,*gy,*gz);

    tempX = (INT16)*gx;
    tempY = (INT16)*gy;
    tempZ = (INT16)*gz;
    *gx = tempX;
    *gy = tempY;
    *gz = tempZ;

    //DBG_IND("gx=%ld, gy=%ld, gz=%ld\r\n",*gx,*gy,*gz);
}

ER gyro_getGsData(INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ)
{
    if (!puiDatX)
    {
        DBG_ERR("puiDatX is NULL\r\n");
        return E_PAR;
    }
    if (!puiDatY)
    {
        DBG_ERR("puiDatY is NULL\r\n");
        return E_PAR;
    }
    if (!puiDatZ)
    {
        DBG_ERR("puiDatZ is NULL\r\n");
        return E_PAR;
    }
    if(g_GyroOpMode==GYRO_OP_MODE_SINGLE_ACCESS)
    {
        UINT32 tempData = 0;

        gyro_readReg(REG_ACCEL_XOUT_L, &tempData);
        *(UINT32*)puiDatX = ((tempData & 0x0000FF00) >> 8) | ((tempData & 0x000000FF) << 8);
        gyro_readReg(REG_ACCEL_YOUT_L, &tempData);
        *(UINT32*)puiDatY = ((tempData & 0x0000FF00) >> 8) | ((tempData & 0x000000FF) << 8);
        gyro_readReg(REG_ACCEL_ZOUT_L, &tempData);
        *(UINT32*)puiDatZ = ((tempData & 0x0000FF00) >> 8) | ((tempData & 0x000000FF) << 8);
    }
    else
    {
        UINT32 k;
        if (uiGyroBufInIdx > 0)
            k = (uiGyroBufInIdx-1)%GDI2_GB_NUM;
        else
            k = uiGyroBufInIdx%GDI2_GB_NUM;
        *puiDatX = puiAccelBuf[k][2];
        *puiDatY = puiAccelBuf[k][3];
        *puiDatZ = puiAccelBuf[k][4];
    }

    gyro_AdjustGsData(puiDatX,puiDatY,puiDatZ);
    return E_OK;
}

