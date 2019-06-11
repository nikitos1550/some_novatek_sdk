/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanSD.c
    @ingroup    mIPRJAPKeyIO

    @brief      Detect SD card status
                Detect SD card insert/remove, lock/unlock

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "Type.h"
#include "DxCfg.h"
#include "IOCfg.h"

#include "DxStorage.h"
#include "DxCommon.h"
#include "DxApi.h"

#include "sdio.h"
#include "SdmmcDesc.h"
#include "StrgDef.h"
#include "Debug.h"
#include "Utility.h"
#include "pll.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

void DrvCARD_EnableCardPower(BOOL bEn);
BOOL DrvCARD_DetStrgCard(void);
BOOL DrvCARD_DetStrgCard_TRUE(void);//for eMMC
BOOL DrvCARD_DetStrgCardWP(void);
BOOL DrvCARD_DetStrgCardWP_FALSE(void);//for eMMC

//public func
UINT32 DrvCARDGetcaps(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvCARDGetcaps3(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvCARDInit(void *pInitParam); // Set Init Parameters
UINT32 DrvCARDInit3(void *pInitParam); // Set Init Parameters
UINT32 DrvCARDOpen(void); // Common Constructor
UINT32 DrvCARDClose(void); // Common Destructor
UINT32 DrvCARDState(UINT32 StateID, UINT32 Value); // General Properties
UINT32 DrvCARDControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
UINT32 DrvCARDCommand(CHAR *pcCmdStr); //General Command Console

#if !defined(_CARD1_NONE_)
//dx object
DX_OBJECT gDevCARD1 = {
	DXFLAG_SIGN,
	DX_CLASS_STORAGE_EXT | DX_TYPE_CARD1,
	STORAGE_VER,
	"Storage_Card1",
	0, 0, 0, 0,
	DrvCARDGetcaps,
	0,
	DrvCARDInit,
	DrvCARDOpen,
	DrvCARDClose,
	DrvCARDState,
	DrvCARDControl,
	DrvCARDCommand,
	0,
};
#endif

#if !defined(_CARD3_NONE_)
//for eMMC test on FPGA
DX_OBJECT gDevCARD3 = {
	DXFLAG_SIGN,
	DX_CLASS_STORAGE_EXT | DX_TYPE_CARD3,
	STORAGE_VER,
	"Storage_Card3",
	0, 0, 0, 0,
	DrvCARDGetcaps3,
	0,
	DrvCARDInit3,
	DrvCARDOpen,
	DrvCARDClose,
	DrvCARDState,
	DrvCARDControl,
	DrvCARDCommand,
	0,
};
#endif


#if !defined(_CARD1_NONE_)
UINT32 DrvCARDGetcaps(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
	UINT32 v = 0;
	switch (CapID) {
	case STORAGE_CAPS_BASE:
		DBG_IND("get card plug caps\r\n");
		v = STORAGE_BF_DETPLUG | STORAGE_BF_DETWP;
		break;
	case STORAGE_CAPS_HANDLE:
		v = (UINT32)sdio_getStorageObject(STRG_OBJ_FAT1);
		break;
	default:
		break;
	}
	return v;
}
#endif

#if !defined(_CARD3_NONE_)
UINT32 DrvCARDGetcaps3(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
	UINT32 v = 0;
	switch (CapID) {
	case STORAGE_CAPS_BASE:
		DBG_IND("get card plug caps\r\n");
		v = STORAGE_BF_DETPLUG | STORAGE_BF_DETWP;
		break;
	case STORAGE_CAPS_HANDLE:
		CHKPNT;
		v = (UINT32)sdio3_getStorageObject(STRG_OBJ_FAT1);
		break;
	default:
		break;
	}
	return v;
}
#endif

#if !defined(_CARD1_NONE_)
UINT32 DrvCARDInit(void *pInitParam) // Set Init Parameters
{
	PSTORAGE_OBJ   pStrg;
	DXSTRG_INIT *g_pInit = (DXSTRG_INIT *)pInitParam; //fat and pstore not complete

	DBG_FUNC_BEGIN("\r\n");

	pStrg = sdio_getStorageObject(STRG_OBJ_FAT1);
	sdio_setCallBack(SDIO_CALLBACK_CARD_DETECT, (SDIO_CALLBACK_HDL)DrvCARD_DetStrgCard);
	sdio_setCallBack(SDIO_CALLBACK_WRITE_PROTECT, (SDIO_CALLBACK_HDL)DrvCARD_DetStrgCardWP);

#if (_FPGA_EMULATION_ == ENABLE)
	// Support default speed only
	sdio_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,           SDIO_BUS_SPEED_MODE_DS);
	// Source clock 96 MHz can run faster than default value
	sdio_setConfig(SDIO_CONFIG_ID_SRCLK,                    SDIO_SRC_CLK_96MHZ);
#else

	/*
	// Enable High Speed mode
	//sdio_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE, (UINT32)(SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS|SDIO_BUS_SPEED_MODE_SDR12|SDIO_BUS_SPEED_MODE_SDR25|SDIO_BUS_SPEED_MODE_SDR50));
	sdio_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE, (UINT32)(SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS|SDIO_BUS_SPEED_MODE_SDR12|SDIO_BUS_SPEED_MODE_SDR25|SDIO_BUS_SPEED_MODE_DDR50));
	*/

	//#NT#2017/10/24#Nestor Yang -begin
	//#NT# Adjust SDIO driving, requested by SA1
	/*
	// Enable setting driving sink of clock pin
	sdio_setConfig(SDIO_CONFIG_ID_DRIVING_SINK_EN, (UINT32)TRUE);

	// Default speed driving -> 5.0 mA (Shirley, 2017/09/22)
	sdio_setConfig(SDIO_CONFIG_ID_DS_DRIVING, 150);
	//sdio_setConfig(SDIO_CONFIG_ID_DS_MAX_CLK, 48000000); //default for DS

	// High speed driving    -> 10.0 mA (Shirley, 2017/09/22)
	sdio_setConfig(SDIO_CONFIG_ID_HS_DRIVING, 150);
	*/
	sdio_setConfig(SDIO_CONFIG_ID_DRIVING_SINK_EN,          TRUE);  // for clock.
	sdio_setConfig(SDIO_CONFIG_ID_DRIVING_SINK_CMDDATA_EN,  TRUE);  // for cmd and data.

	// Default speed driving
	sdio_setConfig(SDIO_CONFIG_ID_DS_DRIVING, 200);
	sdio_setConfig(SDIO_CONFIG_ID_DS_CMD_DRIVING, 150);
	sdio_setConfig(SDIO_CONFIG_ID_DS_DATA_DRIVING, 150);

	// High speed driving
	sdio_setConfig(SDIO_CONFIG_ID_HS_DRIVING, 200);
	sdio_setConfig(SDIO_CONFIG_ID_HS_CMD_DRIVING, 150);
	sdio_setConfig(SDIO_CONFIG_ID_HS_DATA_DRIVING, 150);
	//#NT#2017/10/24#Nestor Yang -end

	//sdio_setConfig(SDIO_CONFIG_ID_HS_MAX_CLK, 48000000); //default for HS
	/*
	// Set clock source
	sdio_setConfig(SDIO_CONFIG_ID_SRCLK, PLL_CLKSEL_SDIO_48);
	// Set maximum bus width to 8 bits
	sdio_setConfig(SDIO_CONFIG_ID_BUS_WIDTH, SD_HOST_BUS_8_BITS);
	*/
	//sdio_setCallBack(SDIO_CALLBACK_POWER_CONTROL, (SDIO_CALLBACK_HDL)DrvCARD_EnableCardPower); //(to improve card capability)
#endif
	//if(STRG_OBJ_FAT1)
	{
		pStrg->SetParam(STRG_SET_MEMORY_REGION, g_pInit->buf.Addr, g_pInit->buf.Size);
	}

	return DX_OK;
}
#endif

#if !defined(_CARD3_NONE_)
UINT32 DrvCARDInit3(void *pInitParam) // Set Init Parameters
{
	PSTORAGE_OBJ   pStrg;
	DXSTRG_INIT *g_pInit = (DXSTRG_INIT *)pInitParam; //fat and pstore not complete

	DBG_FUNC_BEGIN("\r\n");
	CHKPNT;
	pStrg = sdio3_getStorageObject(STRG_OBJ_FAT1);
	if (pStrg == NULL) {
		DBG_ERR("Get SDIO3 storage object failed\r\n");
		return DX_NULL_POINTER;
	}

	pStrg->SetParam(STRG_SET_MEMORY_REGION, g_pInit->buf.Addr, g_pInit->buf.Size);

	sdio3_setCallBack(SDIO_CALLBACK_CARD_DETECT, (SDIO_CALLBACK_HDL)DrvCARD_DetStrgCard_TRUE);
	sdio3_setCallBack(SDIO_CALLBACK_WRITE_PROTECT, (SDIO_CALLBACK_HDL)DrvCARD_DetStrgCardWP_FALSE);

#if (_FPGA_EMULATION_ == ENABLE)
	// Support default speed only
	sdio3_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,          SDIO_BUS_SPEED_MODE_DS);
	// Source clock 96 MHz can run faster than default value
	sdio3_setConfig(SDIO_CONFIG_ID_SRCLK,                   SDIO_SRC_CLK_96MHZ);
#else
	// Support default speed + high speed
	sdio3_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,          SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS);

	// Default speed driving -> 5.0 mA
	sdio3_setConfig(SDIO_CONFIG_ID_DS_DRIVING,              50);
	// High speed driving    -> 10.0 mA
	sdio3_setConfig(SDIO_CONFIG_ID_HS_DRIVING,              100);

	// Adjust driving/sink according to operating mode
	sdio3_setConfig(SDIO_CONFIG_ID_DRIVING_SINK_EN,         TRUE);
#endif

	sdio3_setConfig(SDIO_CONFIG_ID_BUS_WIDTH,               SD_HOST_BUS_4_BITS);

	sdio3_setConfig(SDIO_CONFIG_ID_FORMAD_OEMID, (UINT32)"NOVATEK");
	sdio3_setConfig(SDIO_CONFIG_ID_FORMAD_VOL_LABLE, (UINT32)"SD_VOL");
	sdio3_setConfig(SDIO_CONFIG_ID_FORMAD_RAND_VOL_ID_EN,   TRUE);

	return DX_OK;
}
#endif

#if !(defined(_CARD1_NONE_) && defined(_CARD3_NONE_))
UINT32 DrvCARDOpen(void) // Common Constructor
{
	DrvCARD_EnableCardPower(TRUE);

	return DX_OK;
}

UINT32 DrvCARDClose(void) // Common Destructor
{
	DrvCARD_EnableCardPower(FALSE);

	return DX_OK;
}

UINT32 DrvCARDState(UINT32 StateID, UINT32 Value) // General Properties
{
	DBG_FUNC_BEGIN("\r\n");

	if (StateID & DXGET) {
		UINT32 rvalue =  0;
		StateID &= ~DXGET;
		DBG_IND("get %08x\r\n", StateID);
		switch (StateID) {
		case STORAGE_STATE_INSERT:
			rvalue = DrvCARD_DetStrgCard();
			break;
		case STORAGE_STATE_LOCK: {
				BOOL bLock = FALSE;
				if (DrvCARD_DetStrgCard()) {
					bLock = DrvCARD_DetStrgCardWP();
				}
				rvalue = bLock;
			}
			break;
			break;
		default:
			DBG_ERR("state=0x%02X not support!\r\n", StateID);
			break;
		}
		return rvalue;
	} else if (StateID & DXSET) {
		StateID &= ~DXSET;
		DBG_IND("set %08x\r\n", StateID);
		switch (StateID) {
		default:
			DBG_ERR("state=0x%02X not support!\r\n", StateID);
			break;
		}
	}
	return DX_OK;
}

UINT32 DrvCARDControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
	DBG_FUNC_BEGIN("\r\n");
	DBG_IND("ctrl %08x\r\n", CtrlID);

	switch (CtrlID) {
	case STORAGE_CTRL_POWER:
		DrvCARD_EnableCardPower(Param1);
		break;
	default:
		DBG_ERR("ctrlid=0x%02X not support!\r\n", CtrlID);
		break;
	}
	return DX_OK;
}

UINT32 DrvCARDCommand(CHAR *pcCmdStr) //General Command Console
{
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// GPIO related

#define KEYSCAN_CD_GPIO_INT     GPIO_INT_00
#define KEYSCAN_CARDDET_INT     DISABLE
#define KEYSCAN_SDWRPTDET_GPIO  DISABLE

void DrvCARD_EnableCardPower(BOOL bEn)
{
	gpio_setDir(GPIO_CARD_POWER, GPIO_DIR_OUTPUT);
	if (bEn) {
		DBG_IND("[SD] - card power enable\r\n");
		gpio_clearPin(GPIO_CARD_POWER);
		SwTimer_DelayMs(20);
	} else {

		DBG_IND("[SD] - card power disable\r\n");
		gpio_setPin(GPIO_CARD_POWER);
		SwTimer_DelayMs(350);
	}
}

#if (KEYSCAN_CARDDET_INT == ENABLE)
static volatile UINT32  uiStrgCardIntCnt  = 0;

/**
  ISR of SD card detection

  ISR of SD card detection

  @param void
  @return void
*/
static void KeyScan_DetStrgCardIsr(UINT32 uiEvent)
{
	uiStrgCardIntCnt++;

	// Debounce
	if (uiStrgCardIntCnt > 1) {
		uiStrgCardIntCnt = 0;
	}
}
#endif

void DrvCARD_SetInsert(UINT32 status)
{
#if (KEYSCAN_CARDDET_INT == ENABLE)
	if (status) {
		gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
	} else {
		gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_NEGLOW);
	}
#endif
}

void DrvCARD_DetRestart(void)
{
#if (KEYSCAN_CARDDET_INT == ENABLE)
	gpio_setIntIsr(KEYSCAN_CD_GPIO_INT, KeyScan_DetStrgCardIsr);
	gpio_enableInt(KEYSCAN_CD_GPIO_INT);
#endif
}

/**
  Detect Storage card is inserted or not

  Detect Storage card is inserted or not.
  Return TRUE if storage card is inserted, FALSE if storage card is removed.

  @param void
  @return BOOL: TRUE -> Storage card inserted, FALSE -> Storage card removed
*/
BOOL DrvCARD_DetStrgCard(void)
{
#if 1
	return (gpio_getPin(GPIO_CARD_DETECT) == 0 ? TRUE : FALSE);
#else
	return TRUE;
#endif
}

/**
  Detect Storage card is write protected or not

  Detect Storage card is write protected or not.
  Return TRUE if storage card is write protected, FALSE if storage card is not write protected.

  @param void
  @return BOOL: TRUE -> Storage card is write protected, FALSE -> Storage card is not write protected
*/
BOOL DrvCARD_DetStrgCardWP(void)
{
#if 0
	return (gpio_getPin(GPIO_CARD_WP) == 1 ? TRUE : FALSE);
#else
	return FALSE;
#endif
}

/**
  Always return card inserted (TRUE) for eMMC

  @param void
  @return BOOL: TRUE -> Storage card inserted, FALSE -> Storage card removed
*/
BOOL DrvCARD_DetStrgCard_TRUE(void)
{
	return TRUE;
}

/**
  Always return no write protected (FALSE) for eMMC

  @param void
  @return BOOL: TRUE -> Storage card is write protected, FALSE -> Storage card is not write protected
*/
BOOL DrvCARD_DetStrgCardWP_FALSE(void)
{
	return FALSE;
}

#endif
//@}
