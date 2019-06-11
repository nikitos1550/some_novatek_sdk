/*
    Gyro Driver InvenSense IDG2020.

    @file       GyroDrv_IDG2020.c
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#include <stdio.h>
#include <string.h>
#include "Debug.h"
#include "Type.h"
#include "ErrorNo.h"

#include "Debug.h"
#include "cache.h"
#include "dma.h"
#include "GyroDrv.h"
#include "spi.h"
#include "SysKer.h"
#include "Delay.h"

#define GD_ERR_MSG(...)               debug_msg("^R\033[1m"__VA_ARGS__)
#define GD_WRN_MSG(...)               debug_msg("^Y\033[1m"__VA_ARGS__)
#define GD_MSG_MSG(...)               debug_msg("^M\033[1m"__VA_ARGS__)
#define GD_TST_MSG(...)               //debug_msg("^M\033[1m"__VA_ARGS__)

void gyro_cfg(GYRO_CFGINFO *cfgInfo)
{
}

ER gyro_open(GYRO_OPENOBJ *openObj)
{
    return E_SYS;
}


ER gyro_close(void)
{
    return E_OK;
}

ER gyro_shutdown(void)
{
    return E_OK;
}

ER gyro_readReg(UINT32 uiAddr, UINT32 * puiData)
{
    return E_OK;
}

ER gyro_writeReg(UINT32 uiAddr, UINT32 uiData)
{
    return E_OK;
}


ER gyro_setFreeRunParam(GYRO_FREE_RUN_PARAM *frParam)
{
    return E_OK;
}


ER gyro_setMode(GYRO_OP_MODE opMode)
{
    return E_OK;
}


ER gyro_getFreeRunData(UINT32 *puiNum, INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ)
{
    return E_OK;
}

void gyro_setCalZeroOffset(INT32 xOffset, INT32 yOffset, INT32 zOffset)
{
}

ER gyro_getSingleData(INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ)
{
    return E_OK;
}

UINT32 gyro_getLSB(void)
{
    return 50;
}

void gyro_RSCInfoInit(GYRO_RSCINFOINIT *pRSCInfoInfo)
{

}

ER gyro_getGsData(INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ)
{
    return E_OK;
}


