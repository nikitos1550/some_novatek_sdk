
//#NT#2010/07/29#Lily Kao -begin

/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitUSB.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for USB mode
                Get memory and set parameters for USB mode

    @note       Nothing.

    @date       2010/07/29
*/

/** \addtogroup mIPRJAPCfg */
//@{

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "GxDisplay.h"
#include "UIAppUsbDisk.h"
#include "AppLib.h"
#include "UIInfo.h"

#include "PlaybackTsk.h"
#include "DxType.h"
#include "DxStorage.h"
#include "MsdcNvtCb.h"
#if (defined(_BSP_NA51023_) && _TODO)
#include "usb3dev.h"
#endif
#include "interrupt.h"
#if (LINUX_MSDC == ENABLE)
#include "usbipc_api.h"
#include "usb.h"
#endif
#include "DxApi.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppUsbDiskExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (SDHOTPLUG_FUNCTION == ENABLE)
#include "UIStorageCheck.h"
static BOOL m_bIsMsdcOpened = FALSE;
#endif
//#NT#2009/07/01#Brad Chen -begin
//#NT#Add to support MSDC CDROM function
static DX_HANDLE pLastStrgDev = 0;
//#NT#2009/07/01#Brad Chen -end

#if (LINUX_MSDC == ENABLE)
static USBIPC_MSDC_INFO gmsdc_info = {
		.luns             = 1,
		.id_vendor        = USB_VID,
		.id_product   	  = USB_PID_MSDC,
		.manufacturer 	  = USB_VENDER_STRING, 0x00,
		.product          = USB_PRODUCT_STRING, 0x00,
		.serial           = USB_SERIAL_NUM_STRING, 0x00,
};
#endif

//#NT#2010/10/21#Lily Kao -begin
extern void USBMakerInit_SerialNumber(void);

extern BOOL DrvCARD_DetStrgCard(void);
extern BOOL DrvCARD_DetStrgCardWP(void);

_ALIGNED(4) const static UINT8 gMSDCManuStrDesc[] = {
	USB_VENDER_DESC_STRING_LEN * 2 + 2, // size of String Descriptor = 6 bytes
	0x03,                       // 03: String Descriptor type
	USB_VENDER_DESC_STRING
};

_ALIGNED(4) const static UINT8 gMSDCProdStrDesc[] = {
	USB_PRODUCT_DESC_STRING_LEN * 2 + 2, // size of String Descriptor = 6 bytes
	0x03,                       // 03: String Descriptor type
	USB_PRODUCT_DESC_STRING
};
_ALIGNED(16) UINT8 gMSDCInquiryData[36] = {
	0x00, 0x80, 0x05, 0x02, 0x20, 0x00, 0x00, 0x00,
	//Vendor identification,
	USB_VENDER_STRING,/*'\0',*/
	USB_PRODUCT_STRING, '\0',
	USB_PRODUCT_REVISION
};
//#NT#2010/10/21#Lily Kao -end

void USBMakerInit_UMSD(USB_MSDC_INFO *pUSBMSDCInfo)
{
	pUSBMSDCInfo->pManuStringDesc = (USB_STRING_DESC *)gMSDCManuStrDesc;
	pUSBMSDCInfo->pProdStringDesc = (USB_STRING_DESC *)gMSDCProdStrDesc;

	//USBMakerInit_SerialNumber();
	pUSBMSDCInfo->pSerialStringDesc = NULL;

	pUSBMSDCInfo->VID = USB_VID;
	pUSBMSDCInfo->PID = USB_PID_MSDC;

	pUSBMSDCInfo->pInquiryData = (UINT8 *)&gMSDCInquiryData[0];
}

/**
  Initialize application for USB MSDC mode

  Initialize application for USB MSDC mode.

  @param void
  @return void
*/
#if (LINUX_MSDC == ENABLE)
ER AppInit_ModeUSBMSDC(void)
{
	ER retV = E_OK;
	UINT32 lun = 0;
	USBIPC_MSDC_INFO *msdc_info = &gmsdc_info;

#if (SDHOTPLUG_FUNCTION == ENABLE)
	if (TRUE == m_bIsMsdcOpened || UIStorageCheck(STORAGE_CHECK_ERROR, NULL) == TRUE) {
		return NVTEVT_CONSUME;
	}
	m_bIsMsdcOpened = TRUE;
#endif

#if (FS_MULTI_STRG_FUNC == ENABLE)
	lun = 2;
#else
	lun = 1;
#endif

	int_setConfig(INT_CONFIG_ID_USB3_GIC_DESTINATION, CC_CORE_CA53_CORE2);

	//send ipc to trigger Linux open MSDC here
	msdc_info->luns = lun;
	usbipc_setconfig(USBIPC_CONFIG_MSDC_INFO, (UINT32)msdc_info, 0);
	usbipc_send_cmd(USBIPC_CMDID_MSDC, 1, 0);

	return retV;
}
#else
//#NT#2010/12/13#Lily Kao -begin
ER AppInit_ModeUSBMSDC(void)
//#NT#2010/12/13#Lily Kao -end
{
	char *pDxName = NULL;
	USB_MSDC_INFO       MSDCInfo = {0};
	UINT32              uiPoolAddr;
	DX_HANDLE pStrgDev = 0;
	ER retV = E_OK;
#if (SDHOTPLUG_FUNCTION == ENABLE)
	if (TRUE == m_bIsMsdcOpened || UIStorageCheck(STORAGE_CHECK_ERROR, NULL) == TRUE) {
		return NVTEVT_CONSUME;
	}
	m_bIsMsdcOpened = TRUE;
#endif

	// Get buffer memory for MSDC task, MSDC share the same buffer memory with SIDC,
	// make sure the buffer memory size is enough for both
	uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);

	MSDCInfo.uiMsdcBufAddr = uiPoolAddr;
	MSDCInfo.uiMsdcBufSize = MSDC_MIN_BUFFER_SIZE;
	USBMakerInit_UMSD(&MSDCInfo);
	//The callback functions for the MSDC Vendor command.
	//If project doesn't need the MSDC Vendor command, set this callback function as NULL.
	MSDCInfo.msdc_check_cb = NULL;
	MSDCInfo.msdc_vendor_cb = NULL;
	//#NT#2016/12/20#Niven Cho -begin
	//#NT#MULTI_DRIVE
#if (FS_MULTI_STRG_FUNC == ENABLE)
	char *pDxName2 = NULL;
	DX_HANDLE pStrgDev2 = 0;
#endif

	//#NT#2016/06/07#Niven Cho[102874] -begin
	//#NT# linux must be closed after filesys closing
	//System_CPU2_Stop();
	//#NT#2016/06/07#Niven Cho[102874] -end
	pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT | FS_DX_TYPE_DRIVE_A);

#if (FS_SWITCH_STRG_FUNC == ENABLE)
	UINT32 uiDxState=0;
	if (Dx_GetState((DX_HANDLE)pStrgDev, STORAGE_STATE_INSERT,&uiDxState)!=DX_OK || uiDxState==FALSE) {
		pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_B);
	}
#endif

#if (FS_MULTI_STRG_FUNC == ENABLE)
	pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT | FS_DX_TYPE_DRIVE_B);
#endif

	MSDCInfo.pStrgHandle[0] = pStrgDev;

	if (Dx_GetInfo(pStrgDev, DX_INFO_NAME, &pDxName) != DX_OK) {
		pDxName = NULL;
	}

#if (FS_MULTI_STRG_FUNC == ENABLE)
	MSDCInfo.pStrgHandle[1] = pStrgDev2;

	if (Dx_GetInfo(pStrgDev2, DX_INFO_NAME, &pDxName2) != DX_OK) {
		pDxName2 = NULL;
	}
#endif

#if (FS_MULTI_STRG_FUNC == ENABLE)
	MSDCInfo.pStrgHandle[1] = pStrgDev2;

	if (Dx_GetInfo(pStrgDev2, DX_INFO_NAME, &pDxName2) != DX_OK) {
		pDxName2 = NULL;
	}
#endif

#if (FS_MULTI_STRG_FUNC == ENABLE)
	MSDCInfo.msdc_storage_detCB[0] = (MSDC_StorageDet_CB)DrvCARD_DetStrgCard;
	MSDCInfo.msdc_strgLock_detCB[0] = (MSDC_StrgLockDet_CB)DrvCARD_DetStrgCardWP;
	MSDCInfo.msdc_storage_detCB[1] = NULL;
	MSDCInfo.msdc_strgLock_detCB[1] = NULL;
	MSDCInfo.msdc_type[0] = MSDC_STRG;
	MSDCInfo.msdc_type[1] = MSDC_STRG;
	MSDCInfo.LUNs = 2;
#else
	if (pDxName == NULL || strcmp(pDxName, "Storage_EmbMem5") == 0) {
		MSDCInfo.msdc_storage_detCB[0] = NULL;
		MSDCInfo.msdc_strgLock_detCB[0] = NULL;
	} else if (strcmp(pDxName, "Storage_Card1") == 0) {
		MSDCInfo.msdc_storage_detCB[0] = (MSDC_StorageDet_CB)DrvCARD_DetStrgCard;
		MSDCInfo.msdc_strgLock_detCB[0] = (MSDC_StrgLockDet_CB)DrvCARD_DetStrgCardWP;
	}
	MSDCInfo.msdc_type[0] = MSDC_STRG;
	MSDCInfo.LUNs = 1;
#endif

	//#NT#2009/08/29#Ben Wang -Begin
	// Close File System task before running USB MSDC
	//GxFile_Exit();
	FileSys_CloseEx('A', FST_TIME_INFINITE);
	//#NT#2009/08/29#Ben Wang -end
#if (FS_MULTI_STRG_FUNC == ENABLE)
	FileSys_CloseEx('B', FST_TIME_INFINITE);
#endif
	//#NT#2016/12/20#Niven Cho -end
#if defined(_CPU2_LINUX_)
	//#NT#2016/06/07#Niven Cho[102874] -begin
	//#NT# linux must be closed after filesys closing
	System_CPU2_Stop();
	//#NT#2016/06/07#Niven Cho[102874] -end
#endif
#if (defined(_BSP_NA51023_) && _TODO)
	//set SDIO interrupt direction to CPU1
	int_setConfig(INT_CONFIG_ID_SDIO_GIC_DESTINATION, CC_CORE_CA53_CORE1);
#endif

	Msdc_SetConfig(USBMSDC_CONFIG_ID_SELECT_POWER, USBMSDC_POW_BUSPOWER);
	Msdc_SetConfig(USBMSDC_CONFIG_ID_RC_DISABLESTALL, FALSE);
	// auto detect spped
	//Msdc_SetConfig(USBMSDC_CONFIG_ID_SPEED, U3DEV_SPEED_HS);

#if (defined(_BSP_NA51023_) && _TODO)
	int_setConfig(INT_CONFIG_ID_USB3_GIC_DESTINATION, CC_CORE_CA53_CORE1);
#endif
#if(MSDCVENDOR_NVT==ENABLE)
	MsdcNvtCb_Attach(&MSDCInfo);
#endif

	retV = Msdc_Open(&MSDCInfo);
	if (retV != E_OK) {
		debug_err(("Error open USB MSDC task:%d\r\n", retV));
	}
	return retV;
}
#endif


INT32 MSDCExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_OPEN, paramNum, paramArray);
//#NT#2009/07/01#Brad Chen -begin
//#NT#Add to support MSDC CDROM function
	pLastStrgDev = GxStrg_GetDevice(0);
//#NT#2009/07/01#Brad Chen -end
	if (pLastStrgDev == 0) {
		//PowerOn_CB() POWERON_CB_SXOPEN 沒有提早啟動File System
		debug_err(("MSDC: Wait for FS start...\r\n"));
		FileSys_WaitFinish();

		//update
		pLastStrgDev = GxStrg_GetDevice(0);
	}

	System_SetState(SYS_STATE_FS, FS_NOT_INIT);

	return (AppInit_ModeUSBMSDC());
}

INT32 MSDCExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (SDHOTPLUG_FUNCTION == ENABLE)
	if (FALSE == m_bIsMsdcOpened) {
		return NVTEVT_CONSUME;
	}
	m_bIsMsdcOpened = FALSE;
#endif

#if (LINUX_MSDC == ENABLE)
	usbipc_send_cmd(USBIPC_CMDID_MSDC, 0, 0);
#else
	Msdc_Close();
#endif


	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_CLOSE, paramNum, paramArray);
	return NVTEVT_CONSUME;
}

EVENT_ENTRY CustomMSDCObjCmdMap[] = {
	{NVTEVT_EXE_OPEN,               MSDCExe_OnOpen},
	{NVTEVT_EXE_CLOSE,              MSDCExe_OnClose},
	{NVTEVT_NULL,                   0},  //End of Command Map
};

CREATE_APP(CustomMSDCObj, APP_SETUP)

//#NT#2010/07/29#Lily Kao -end

