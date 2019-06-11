#include "SysCommon.h"
#include "GxImage.h"
#include "UIInfo.h"
#include "UIPhotoFuncInfo.h"
#include "UIAppMovie.h"
#include "alg_MovieExe.h"
#include "alg_id.h"
#include "ImageApp_MovieMulti.h"

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h"
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)

// Common header files for pull-mode video buffer
#include "ImageUnit_UserProc.h"

#if MOVIE_FD_FUNC_
#include "fd_lib.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "UIInfo.h"
#include "ImageUnit_ImagePipe.h"
#define MOVIE_FD_FACESCALE	16
#endif
#if _TSR_FUNC_
#include "UIAppPhoto.h"
#include "UIControlWnd.h"
#include "tsr_lib.h"
#include "TSD_DSP_lib.h"
#include "UIAppMovie.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "UIInfo.h"
#include "ImageUnit_ImagePipe.h"
#include "dsp.h"
#if TSR_READ_TSD_MODEL_FROM_SDCARD
#define TSR_TSD_MODEL_NAME "tsd_model.bin"
#endif
#define TSR_OCR_MODEL_NAME "ocr_model.bin"
#endif
#if TSR_MODEL_IN_PSTORE
#include "PStore.h"
#define PS_TSR_MODEL1_DATA            "TSRMDL1_DAT"
#define PS_TSR_MODEL1_SIZE            "TSRMDL1_SIZ"
#define PS_TSR_MODEL2_DATA            "TSRMDL2_DAT"
#define PS_TSR_MODEL2_SIZE            "TSRMDL2_SIZ"
#endif

// Global variable
extern URECT gMovieOsdDispCord;

#if (_ADAS_FUNC_ == ENABLE)
#include "adas_Apps.h"
#endif  // #if (_ADAS_FUNC_ == ENABLE)

//#NT#2016/09/29#KCHong -begin
//#NT#The GPS related variables should not depend on ADAS.
#if (GPS_FUNCTION == ENABLE)
extern FLOAT g_CurSpeed;
extern BOOL g_GPSLinked;
extern BOOL g_GPSStatus;
#endif
//#NT#2016/09/29#KCHong -end

// #NT#2015/09/17#JS Liu - begin
#if _CVHOST_EX_
#include "CvHost_lib.h"
#include "alg_CvHostEx.h"

static BOOL g_CvHostExDone = FALSE;
#endif
// #NT#2015/09/17#JS Liu - end
//#NT#2016/05/31#Omega Yu -begin
#if _NVTCV_EX_
#include "NvtCv_lib.h"
#include "UIAppMovie_NvtCvEx.h"

static BOOL g_NvtCvExDone = FALSE;
#endif
//#NT#2016/05/31#Omega Yu -end
//#NT#2016/03/02#Lincy Lin -begin
//#NT#Support object tracking function
#if MOVIE_OT_FUNC
#include "NvtOt.h"
#include "ImageUnit_ImagePipe.h"
#include "UIMovieMapping.h"
#include "UIAppMovie.h"
#endif
//#NT#2016/03/02#Lincy Lin -end

//#NT#2016/05/20#Yang Jin -begin
//#NT#Support Dis dsp function
#if MOVIE_DISDSP_FUNC
#include "dis_alg_dsp.h"
#include "ImageUnit_ImagePipe.h"
#endif
//#NT#2016/05/20#Yang Jin -end

//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if MOVIE_DDD_FUNC
#include "DDD_lib.h"
#include "UIAppMovie.h"
//#NT#2016/05/11#Isiah Chang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#include "crypto.h"

#if 0
#define DDD_MODEL_NAME "ciphertext.bin"
#else
#define DDD_MODEL_NAME "sdm_model_fixed.data"
#endif

#define DDD_MAX_DB_FILE_SIZE    (12 * 1024 * 1024)
//#NT#2016/05/11#Isiah Chang -end
#endif
//#NT#2016/04/22#Nestor Yang -end

//#NT#2016/05/23#David Tsai -begin
//#NT# Add tampering detection and background calculation function
//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection & trip-zone detection function
#if MOVIE_BC_FUNC || MOVIE_TD_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC || MOVIE_SCD_FUNC
#include "BC_lib.h"
#include "UIMovieMapping.h"
#include "UIAppMovie.h"
#include "ImageUnit_ImagePipe.h"
#endif

#if MOVIE_TD_FUNC
#include "TD_lib.h"
#endif

#if MOVIE_TWD_FUNC
#include "TWD_lib.h"
#endif
#if MOVIE_TZD_FUNC
#include "TZD_lib.h"
#endif
//#NT#2016/10/14#Yuzhe Bian -end
//#NT#2016/05/23#David Tsai -end

//#NT#2016/10/26#Omega Yu -begin
//#NT# Support scene change detection function
#if MOVIE_SCD_FUNC
#include "SCD_lib.h"
#endif
//#NT#2016/10/26#Omega Yu -end

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
#include "ftg_lib.h"
#include "UIMovieMapping.h"
#include "UIAppMovie.h"
#include "ImageUnit_ImagePipe.h"
#include "UIPhotoMapping.h"
#include "ftg_fd_lib.h"
#endif
//#NT#2016/10/17#Bin Xiao -end

//#NT#2017/04/28#KCHong -begin
//#NT# CNN Object detection
#if (_OD_FUNC_ == ENABLE)
#include "NvtCNN.h"
#include "UIAppMovie.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "UIInfo.h"
#include "ImageUnit_ImagePipe.h"
#include "GxImage.h"
#include "displyr.h"
#include "display.h"

#include "Dx.h"
#include "DxApi.h"
#include "DxDisplay.h"
static DX_HANDLE gDevHDMIObj = 0;
static UINT32 g_hdmi_mode = 0;
#endif
//#NT#2017/04/28#KCHong -end

#include "DxSensor.h"
#if DDD_MODEL_IN_PSTORE
#include "PStore.h"
#define PS_DDD_MODEL1_DATA            "DDDMDL1_DAT"
#define PS_DDD_MODEL1_SIZE             "DDDMDL1_SIZ"
#endif

#include "ImageUnit_UserProc.h"


#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppMovie_PhotoFuncInfo
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#if MOVIE_BC_FUNC || MOVIE_TD_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC
static BOOL gBCLock = TRUE;
static UINT32 gBCCounter = 0, gBCProcessRate = 4; // 3;
#if MOVIE_BC_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC
static UINT32 gBCUpdateCounter = 0;
#endif
#endif
#if 0// MOVIE_FTG_FUNC
static BOOL gFTGLock = TRUE;
#endif
extern UINT32 System_GetEnableSensor(void);

#if (MOVIE_TSR_FUNC_ && _TSR_FUNC_)

static BOOL gInited = FALSE;

void Movie_TSRInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	TSR_Init(buf, cachebuf, 1);
}

void Movie_TSRProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    ISF_DATA         isf_data;
    ISF_UNIT         *pUnit;
    ISF_PORT         *pSrc;
    IMG_BUF*         IMG_TEMP;
	BOOL			 buf_got = FALSE;

	if (ImageApp_MovieMulti_IsStreamRunning(_CFG_REC_ID_1) == FALSE) { // not in recording
		return;
	}

#if 0
    {
        pUnit = &ISF_UserProc;
		pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
        if(pSrc)
        {
            if (!ImageUnit_IsAllowPull(pSrc))
            {
                ImageUnit_Begin(pUnit, 0);
                ImageUnit_SetParam(ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3), USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
                ImageUnit_End();
            }
            ImageUnit_PullData(pSrc, (ISF_DATA*)&isf_data, 0);
            IMG_TEMP = (IMG_BUF *)&isf_data.Desc[0];
            debug_msg("ISF_UserProc ISF_OUT%d has output, size is %d, %d\r\n",
                      (ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3) + 1), IMG_TEMP->Width, IMG_TEMP->Height);
            ImageUnit_ReleaseData((ISF_DATA*)&isf_data);
	//return;

        }
        else
            debug_msg("ISF_UserProc ISF_OUT%d failed!\r\n", (ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3) + 1));
	}
#endif

	if(TSR_ValidForProcess() == FALSE)
		return;

    pUnit = &ISF_UserProc;
#if 0
    ImageUnit_Begin(pUnit, 0);
    ImageUnit_SetParam(ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3), USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
    ImageUnit_End();
#endif
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
	wai_sem(AlgPullIsfDataSemiID[MOVIE_ALG_IPL1_3]);
    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, &isf_data, 0) == ISF_OK) {
        IMG_TEMP = (IMG_BUF *)&isf_data.Desc[0];
        TSR_PreProcess(IMG_TEMP, TSR_IMAGERATIO_16_9, buf->Addr, cachebuf->Addr);
		ImageUnit_ReleaseData(&isf_data);
		buf_got = TRUE;
    }
	sig_sem(AlgPullIsfDataSemiID[MOVIE_ALG_IPL1_3]);
	if(buf_got)	{
		TSR_Process();
	}
}

void Movie_TSRProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvtsr = {
	{
		PHOTO_PRIORITY_SH,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_TSRProcess",
		Movie_TSRInit,          ///< init tsr, only execute at first process
		Movie_TSRProcess,       ///< process tsr
		Movie_TSRProcessEnd,     ///< process end tsr
		NULL,                      ///< exit fp, only execute at photo close
		TSR_CalcBuffSize,       ///< get working buffer tsr
		TSR_CalcCacheBuffSize,  ///< get working cache buffer tsr
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

BOOL Movie_TSRDSPReadModel(CHAR *filePath, UINT32 mdlAddr, UINT32 mdlSize)
{
    FST_FILE filehdl;
    filehdl = FileSys_OpenFile(filePath, FST_OPEN_READ);
    if(!filehdl)
    {
        DBG_ERR("Error: can not open file %s\r\n", filePath);
        return FALSE;
    }

    FileSys_ReadFile(filehdl, (UINT8*)mdlAddr, &mdlSize, 0, NULL);
    FileSys_CloseFile( filehdl );
    return TRUE;
}

#if TSR_MODEL_IN_PSTORE
UINT32 Movie_TSRCheckPSFormat(void)
{
#define SETTING_SIZE 30
#define FORMAT_CHK_STRING  "format="
    UINT32      uiReadSize = SETTING_SIZE;
    UINT32      uiUpdateChk;
    CHAR        cDataBuf[SETTING_SIZE]={0}, *pCh;
    FST_FILE    pFile;
    pFile = FileSys_OpenFile("A:\\tsr_psformat", FST_OPEN_READ);
    if (pFile)
    {
        FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
        FileSys_CloseFile(pFile);
        if (uiReadSize)
        {
            pCh = strstr(cDataBuf, FORMAT_CHK_STRING);
            sscanf(pCh + sizeof(FORMAT_CHK_STRING) - 1, "%d", &uiUpdateChk);
            DBG_DUMP("Movie_TSRCheckPSFormat FormatChk = %d, size %d\r\n", uiUpdateChk, sizeof(FORMAT_CHK_STRING));
            return uiUpdateChk;
        }
    }
    else
    {
    }
    return 0;
}
UINT32 Movie_TSRCheckDBUpdate(void)
{
#define SETTING_SIZE 30
#define UPDATE_CHK_STRING  "update="
    UINT32      uiReadSize = SETTING_SIZE;
    UINT32      uiUpdateChk;
    CHAR        cDataBuf[SETTING_SIZE]={0}, *pCh;
    FST_FILE    pFile;
    pFile = FileSys_OpenFile("A:\\tsr_update", FST_OPEN_READ);
    if (pFile)
    {
        FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
        FileSys_CloseFile(pFile);
        if (uiReadSize)
        {
            pCh = strstr(cDataBuf, UPDATE_CHK_STRING);
            sscanf(pCh + sizeof(UPDATE_CHK_STRING) - 1, "%d", &uiUpdateChk);
            DBG_DUMP("Movie_TSRCheckDBUpdate UpdateChk = %d, size %d\r\n", uiUpdateChk, sizeof(UPDATE_CHK_STRING));
            return uiUpdateChk;
        }
    }
    else
    {
    }
    return 0;
}
#endif

#if TSR_READ_TSD_MODEL_FROM_SDCARD
UINT32 Movie_TSRGetDBFileSizeFrCard_TSD(void)
{
    FST_FILE filehdl = NULL;
    UINT32 ui32FileSize = 0;
    CHAR* pBinaryFileName = "B:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
        DBG_DUMP("TSR load tsd model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }
    if(filehdl)
    {
        FileSys_SeekFile(filehdl, 0, FST_SEEK_END);
        ui32FileSize = (UINT32)FileSys_TellFile(filehdl);
        FileSys_SeekFile(filehdl, 0, FST_SEEK_SET);
        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("TSR_Init Failed: load tsd model failed!\r\n");
        return 0;
    }
    return ui32FileSize;
}
UINT32 Movie_TSRGetDBFileSize_TSD(void)
{
    UINT32 ui32FileSize = 0;
#if TSR_MODEL_IN_PSTORE
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32  result;
    if ((pSection = PStore_OpenSection(PS_TSR_MODEL1_SIZE, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER
                    || Movie_TSRCheckDBUpdate())
    {
        result = PStore_ReadSection((UINT8 *)&ui32FileSize, 0, sizeof(UINT32), pSection);
        DBG_IND("result=%d, ui32FileSize=0x%x\r\n", result, ui32FileSize);
        if (ui32FileSize == 0)
        {
            DBG_WRN("PStore Save default TSD Model Size\r\n");
            ui32FileSize = Movie_TSRGetDBFileSizeFrCard_TSD();
            DBG_WRN("ui32FileSize=%d\r\n", ui32FileSize);
            PStore_WriteSection((UINT8 *)&ui32FileSize, 0, sizeof(UINT32), pSection);
            PStore_CloseSection(pSection);
        }
        else
        {
            DBG_DUMP("PStore Load TSD Model Size OK\r\n");
            PStore_CloseSection(pSection);
        }
    }
    else
    {
        DBG_ERR("PStore Load TSD Model Size Fail!\r\n");
        return 0;
    }
#else
    ui32FileSize=Movie_TSRGetDBFileSizeFrCard_TSD();
#endif
    return ui32FileSize;
}
void Movie_TSRGetDBFile_TSD(UINT8* pBinaryMap, UINT32* uiBuflen)
{
    FST_FILE filehdl = NULL;
    CHAR* pBinaryFileName = "B:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
        DBG_DUMP("TSR load tsd model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }
    if(filehdl)
    {
        FileSys_ReadFile(filehdl, (UINT8*)pBinaryMap,
                         uiBuflen, 0, NULL);
        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("TSR_Init Failed: load tsd model failed!\r\n");
        return;
    }
}
#endif

UINT32 Movie_TSRGetDBFileSizeFrCard_OCR(void)
{
    FST_FILE filehdl = NULL;
    UINT32 ui32FileSize = 0;
    CHAR* pBinaryFileName = "B:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
        DBG_DUMP("TSR load ocr model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }
    if(filehdl)
    {
        FileSys_SeekFile(filehdl, 0, FST_SEEK_END);
        ui32FileSize = (UINT32)FileSys_TellFile(filehdl);
        FileSys_SeekFile(filehdl, 0, FST_SEEK_SET);
        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("TSR_Init Failed: load ocr model failed!\r\n");
        return 0;
    }
    return ui32FileSize;
}
UINT32 Movie_TSRGetDBFileSize_OCR(void)
{
    UINT32 ui32FileSize = 0;
#if TSR_MODEL_IN_PSTORE
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32  result;
    if ((pSection = PStore_OpenSection(PS_TSR_MODEL2_SIZE, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER
                    || Movie_TSRCheckDBUpdate())
    {
        result = PStore_ReadSection((UINT8 *)&ui32FileSize, 0, sizeof(UINT32), pSection);
        DBG_IND("result=%d, ui32FileSize=0x%x\r\n",result,ui32FileSize);
        if (ui32FileSize==0)
        {
            DBG_WRN("PStore Save default OCR Model Size\r\n");
            ui32FileSize=Movie_TSRGetDBFileSizeFrCard_OCR();
            DBG_WRN("ui32FileSize=%d\r\n",ui32FileSize);
            PStore_WriteSection((UINT8 *)&ui32FileSize,0,sizeof(UINT32),pSection);
            PStore_CloseSection(pSection);
        }
        else
        {
            DBG_DUMP("PStore Load OCR Model Size OK\r\n");
            PStore_CloseSection(pSection);
        }
    }
    else
    {
        DBG_ERR("PStore Load OCR Model Size Fail!\r\n");
        return 0;
    }
#else
    ui32FileSize=Movie_TSRGetDBFileSizeFrCard_OCR();
#endif
    return ui32FileSize;
}
void Movie_TSRGetDBFile_OCR(UINT8* pBinaryMap, UINT32* uiBuflen)
{
    FST_FILE filehdl = NULL;
    CHAR* pBinaryFileName = "B:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
        DBG_DUMP("TSR load ocr model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }
    if(filehdl)
    {
        FileSys_ReadFile(filehdl, (UINT8*)pBinaryMap,
                         uiBuflen, 0, NULL);
        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("TSR_Init Failed: load ocr model failed!\r\n");
        return;
    }
}

INT32 Movie_TSRGetDBFileAndInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if(!gInited)
	{
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
		UINT32 uiDBFileSize_tsd;
		UINT32 uiBuflen_tsd;
		UINT32 tsdModelAddr;
	#endif
    	UINT32 uiDBFileSize_ocr;
    	UINT32 uiBuflen_ocr;
    	UINT32 ocrModelAddr;
    	UINT32 tsdDspBufAddr;

		if(FileSys_WaitFinish() == FST_STA_ERROR)
		{
			DBG_WRN("TSR_Init Failed: file system is not ready!\r\n");
			return 0;
		}

		tsdDspBufAddr = buf->Addr;

	#if TSR_READ_TSD_MODEL_FROM_SDCARD
	 	uiDBFileSize_tsd = Movie_TSRGetDBFileSize_TSD();
		uiDBFileSize_ocr = Movie_TSRGetDBFileSize_OCR();
		uiBuflen_tsd = ALIGN_CEIL_16(uiDBFileSize_tsd);
    	uiBuflen_ocr = ALIGN_CEIL_16(uiDBFileSize_ocr);
		tsdModelAddr = ALIGN_CEIL_4(tsdDspBufAddr + NvtCv_TSDDSP_CalcBuffSize());
    	ocrModelAddr = ALIGN_CEIL_4(tsdModelAddr + uiBuflen_tsd);
    #else
		uiDBFileSize_ocr = Movie_TSRGetDBFileSize_OCR();
		uiBuflen_ocr = ALIGN_CEIL_16(uiDBFileSize_ocr);
		ocrModelAddr = ALIGN_CEIL_4(tsdDspBufAddr + NvtCv_TSDDSP_CalcBuffSize());
	#endif

	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    	if(uiDBFileSize_tsd > NvtCv_TSDDSP_TSDMaxModelSize())
    	{
        	DBG_ERR("TSD DB file size is too large!\r\n");
        	return 0;
    	}
	#endif

    	if(uiDBFileSize_ocr > NvtCv_TSDDSP_OCRMaxModelSize())
    	{
        	DBG_ERR("OCR DB file size is too large!\r\n");
        	return 0;
    	}

	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    	if(buf->Size < NvtCv_TSDDSP_CalcBuffSize() + uiBuflen_tsd + uiBuflen_ocr + 4)
    	{
        	DBG_ERR("memory buf is (%d) not enough!\r\n", buf->Size);
			return 0;
    	}
	#else
	    if(buf->Size < NvtCv_TSDDSP_CalcBuffSize() + uiBuflen_ocr + 4)
    	{
        	DBG_ERR("memory buf is (%d) not enough!\r\n", buf->Size);
			return 0;
    	}
    #endif

#if (TSR_MODEL_IN_PSTORE==DISABLE)
    	{//load Tsr Model
        	FST_FILE filehdl = NULL;

		#if TSR_READ_TSD_MODEL_FROM_SDCARD
        	CHAR* pBinaryFileName_tsd = "B:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
        	filehdl = FileSys_OpenFile(pBinaryFileName_tsd, FST_OPEN_READ);
        	if(!filehdl)
        	{
            	CHAR* pBinaryFileName2_tsd = "A:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
            	DBG_DUMP("TSR load tsd model from SD-card!\r\n");
            	filehdl = FileSys_OpenFile(pBinaryFileName2_tsd, FST_OPEN_READ);
        	}
        	if(filehdl)
        	{
            	FileSys_ReadFile(filehdl, (UINT8*)tsdModelAddr, &uiBuflen_tsd, 0, NULL);
            	FileSys_CloseFile(filehdl);
        	}
        	else
        	{
            	DBG_ERR("TSR_Init Failed: load tsd model failed!\r\n");
            	return 0;
        	}
		#endif

        	CHAR* pBinaryFileName_ocr = "B:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
        	filehdl = FileSys_OpenFile(pBinaryFileName_ocr, FST_OPEN_READ);
        	if(!filehdl)
        	{
            	CHAR* pBinaryFileName2_ocr = "A:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
            	DBG_DUMP("TSR load ocr model from SD-card!\r\n");
            	filehdl = FileSys_OpenFile(pBinaryFileName2_ocr, FST_OPEN_READ);
        	}

        	if(filehdl)
        	{
            	FileSys_ReadFile(filehdl, (UINT8*)ocrModelAddr, &uiBuflen_ocr, 0, NULL);
            	FileSys_CloseFile(filehdl);
        	}
        	else
        	{
            	DBG_ERR("TSR_Init Failed: load ocr model failed!\r\n");
            	return 0;
        	}
    	}
#else
    	PPSTORE_SECTION_HANDLE  pSection;
    	UINT32  result;
    	UINT32  uiUpdateDBChk = Movie_TSRCheckDBUpdate();

    	if(Movie_TSRCheckPSFormat() == 1)
    	{
        	DBG_WRN("PStore format!\r\n");
        	System_PS_Format();
		#if TSR_READ_TSD_MODEL_FROM_SDCARD
        	uiDBFileSize_tsd = Movie_TSRGetDBFileSize_TSD();
		#endif
        	uiDBFileSize_ocr = Movie_TSRGetDBFileSize_OCR();
    	}
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    	uiBuflen_tsd = uiDBFileSize_tsd;
	#endif
    	uiBuflen_ocr = uiDBFileSize_ocr;
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    	if ((pSection = PStore_OpenSection(PS_TSR_MODEL1_DATA, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    	{
        	result = PStore_ReadSection((UINT8 *)tsdModelAddr, 0, uiBuflen_tsd, pSection);
        	if (result != E_PS_OK || uiUpdateDBChk)
        	{
            	Movie_TSRGetDBFile_TSD((UINT8 *)tsdModelAddr, &uiBuflen_tsd);
            	DBG_WRN("Save TSD Model Data in PStore [size:%d]\r\n", uiBuflen_tsd);
            	PStore_WriteSection((UINT8 *)tsdModelAddr, 0, uiBuflen_tsd, pSection);
            	PStore_CloseSection(pSection);
        	}
        	else
        	{
            	DBG_DUMP("PStore Load TSD Model Data OK\r\n");
            	PStore_CloseSection(pSection);
        	}
    	}
    	else
    	{
        	DBG_ERR("TSR_Init Failed: load tsd model failed!\r\n");
        	return 0;
    	}
	#endif

    	if((pSection = PStore_OpenSection(PS_TSR_MODEL2_DATA, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    	{
        	result = PStore_ReadSection((UINT8 *)ocrModelAddr, 0, uiBuflen_ocr, pSection);
        	if (result != E_PS_OK || uiUpdateDBChk)
        	{
            	Movie_TSRGetDBFile_OCR((UINT8 *)ocrModelAddr, &uiBuflen_ocr);
            	DBG_WRN("Save OCR Model Data in PStore [size=%d]\r\n", uiBuflen_ocr);
            	PStore_WriteSection((UINT8 *)ocrModelAddr, 0, uiBuflen_ocr, pSection);
            	PStore_CloseSection(pSection);
        	}
        	else
        	{
            	DBG_DUMP("PStore Load OCR Model Data OK\r\n");
            	PStore_CloseSection(pSection);
        	}
    	}
    	else
    	{
        	DBG_ERR("TSR_Init Failed: load ocr model failed!\r\n");
        	return 0;
    	}
#endif
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    	NvtCv_TSDDSP_Init(tsdDspBufAddr, cachebuf->Addr, tsdModelAddr, ocrModelAddr);
    #else
    	NvtCv_TSDDSP_Init(tsdDspBufAddr, cachebuf->Addr, ocrModelAddr);
    #endif
		gInited = TRUE;
	}
	return 1;
}

void Movie_TSRDSPInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	gInited = FALSE;
    if(!dsp_isOpened(DSP_ID_1))
    {
        DBG_WRN("DSP is not start, app open DSP automatic!\r\n");
        SxCmd_DoCommand("dsp start 1");
    }
}

void Movie_TSRDSPProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if (ImageApp_MovieMulti_IsStreamRunning(_CFG_REC_ID_1) == FALSE) { // not in recording
		return;
	}

	if(!Movie_TSRGetDBFileAndInit(buf, cachebuf))
	{
		return;
	}
	NvtCv_TSDDSP_Process();
}

void Movie_TSRDSPProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if (ImageApp_MovieMulti_IsStreamRunning(_CFG_REC_ID_1) == FALSE) { // not in recording
		return;
	}

#if 1
#if 0
    VControl *pCurrnetWnd;
    Ux_GetFocusedWindow(&pCurrnetWnd);
    Ux_SendEvent(pCurrnetWnd, NVTEVT_UPDATE_INFO, 1, UIAPPPHOTO_CB_ALGEND);
#else
	//#NT#2018/06/06#B_B Jiang -start
	if(!TSR_GetLockStatus())
	//#NT#2018/06/06#B_B Jiang -end
		Ux_PostEvent(NVTEVT_UPDATE_INFO, 1, UIAPPPHOTO_CB_ALGEND);

#endif
#else
    ISF_DATA		 isf_data;
    ISF_UNIT		 *pUnit;
    ISF_PORT		 *pSrc;
    pUnit = &ISF_UserProc;
    ImageUnit_Begin(pUnit, 0);
    ImageUnit_SetParam(ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3), USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
    ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA*)&isf_data, 0) == ISF_OK)
    {
        if(((IMG_BUF *)&isf_data.Desc[0])->PxlAddr[0]==0)
        { return; }
	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);

        //	logSize = TSR_GetLog(&logBuf);
        //	  if(logSize!=0)
        //	  { mp4log_AddLog(DbgLogTsrID, (char*)logBuf, logSize); }
        ImageUnit_ReleaseData((ISF_DATA*)&isf_data);
        DBG_DUMP("tsd dsp end 4\r\n");
	}
#endif
}

UINT32 Movie_TSRDSP_CalcBufWithModel(VOID)
{
#if POWERON_WAIT_FS_READY
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    UINT32 uiDBFileSize_tsd = Movie_TSRGetDBFileSize_TSD();
	#endif
    UINT32 uiDBFileSize_ocr = Movie_TSRGetDBFileSize_OCR();
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    if(uiDBFileSize_tsd==0)
        uiDBFileSize_tsd=Movie_TSRGetDBFileSizeFrCard_TSD();
    DBG_DUMP("^Ytsd filesize:%d\r\n", uiDBFileSize_tsd);
	#endif
    if(uiDBFileSize_ocr==0)
        uiDBFileSize_ocr=Movie_TSRGetDBFileSizeFrCard_OCR();
    DBG_DUMP("^Yocr filesize:%d\r\n", uiDBFileSize_ocr);
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    if(uiDBFileSize_tsd > NvtCv_TSDDSP_TSDMaxModelSize())
    {
        DBG_ERR("TSD DB file size is too large!\r\n");
        return 0;
    }
	#endif
    if(uiDBFileSize_ocr > NvtCv_TSDDSP_OCRMaxModelSize())
    {
        DBG_ERR("OCR DB file size is too large!\r\n");
        return 0;
    }
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
    return NvtCv_TSDDSP_CalcBuffSize() + ALIGN_CEIL_16(uiDBFileSize_tsd) + ALIGN_CEIL_16(uiDBFileSize_ocr) + 4;
	#else
    return NvtCv_TSDDSP_CalcBuffSize() + ALIGN_CEIL_16(uiDBFileSize_ocr) + 4;
	#endif
#else
	#if TSR_READ_TSD_MODEL_FROM_SDCARD
	return NvtCv_TSDDSP_CalcBuffSize() + ALIGN_CEIL_16(NvtCv_TSDDSP_TSDMaxModelSize()) + ALIGN_CEIL_16(NvtCv_TSDDSP_OCRMaxModelSize()) + 4;
	#else
    return NvtCv_TSDDSP_CalcBuffSize() + ALIGN_CEIL_16(NvtCv_TSDDSP_OCRMaxModelSize()) + 4;
	#endif
#endif
}
PHOTO_FUNC_INFO PhotoFuncInfo_mvtsrdsp = {
	{
		PHOTO_PRIORITY_SH,              ///< function hook process Id.
		IPL_PATH_1,                     ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
		"Movie_TSRDSPProcess",
		Movie_TSRDSPInit,               ///< init tsr, only execute at first process
		Movie_TSRDSPProcess,            ///< process tsr
		Movie_TSRDSPProcessEnd,         ///< process end tsr
		NULL,                      ///< exit fp, only execute at photo close
		Movie_TSRDSP_CalcBufWithModel,  ///< get working buffer tsr
		NvtCv_TSDDSP_CalcCacheBuffSize, ///< get working cache buffer tsr
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif

//#NT#2017/04/28#KCHong -begin
//#NT# CNN Object detection
#define DEBUG_DUMP_IMG           DISABLE
#if (_OD_FUNC_ == ENABLE)
void Movie_ODInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	OD_Init(buf, cachebuf, 1);
}



void Movie_ODProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
#if (DEBUG_DUMP_IMG == ENABLE)
	static UINT32    frame_cnt = 0;
	char             filename[50];
#endif

	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH4));

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
        OD_Process((IMG_BUF *)&isf_data.Desc[0]);
#if (DEBUG_DUMP_IMG == ENABLE)
		frame_cnt++;
		if (!(frame_cnt % 100)) {
			snprintf(filename, 50, "A:\\test%05d.yuv", frame_cnt);
			DBG_DUMP("Save yuv file %s\r\n", filename);
			GxImg_DumpBuf(filename, (IMG_BUF *)&isf_data.Desc[0], GX_IMAGE_DUMP_ALL);
		}
#endif
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}
}

#define OD_DRAW_RESULT    ENABLE

void Movie_ODProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	UINT32 ODNumber = 0;
	OD_Result od_result[MAX_DETECTED_OBJECT] = {0};
    #if 0  /// For CNN demo!
	URECT DispCord = {0, 0, 960, 240};
    #else
    URECT DispCord = {0, 0, 1920, 1080};
    #endif
    DISPLAYER_PARAM   DispLyr = {0} ;

    if (!g_hdmi_mode) {
            gDevHDMIObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_HDMIOUT);
            if (gDevHDMIObj) {
                g_hdmi_mode = Dx_Getcaps(gDevHDMIObj, DISPLAY_CAPS_VIDMODE, 0);
            }
    }

    switch(g_hdmi_mode)
    {
        case HDMI_640X480P60:
            DispCord.w = 640;
            DispCord.h = 480;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = 7;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = 7;
            break;
        case HDMI_3840X2160P30:
            DispCord.w = 960;
            DispCord.h = 240;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = 4;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = 4;
            break;
        case HDMI_1920X1080I60:
            DispCord.w = 1920;
            DispCord.h = 1080;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = 10;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = 10;
            break;
        default:
            debug_msg("xExamOD_odgetresult Error: Unknown HDMI mode (%d)\r\n", g_hdmi_mode);
            break;
    }

	ODNumber = OD_GetResult(od_result, &DispCord);

#if (OD_DRAW_RESULT == ENABLE)
	UINT32            i = 0;
	PDISP_OBJ         pDispObj = NULL;

	idec_setFDAllEn(0, 1);
	pDispObj = disp_getDisplayObject(DISP_1);
	if (ODNumber > 16) {
		ODNumber = 16;
	}
	for (i = 0; i < 16; i++) {
		DispLyr.SEL.SET_FDEN.FD_NUM = DISPFD_NUM0 << i;
		DispLyr.SEL.SET_FDEN.bEn = FALSE;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);
	}

	for (i = 0; i < ODNumber; i++) {
		//DBG_DUMP("Object %d = [x=%d,y=%d,w=%d,h=%d]\r\n",i,od_result[i].x,od_result[i].y,od_result[i].w,od_result[i].h);
		DispLyr.SEL.SET_FDSIZE.FD_NUM = DISPFD_NUM0 << i;
		DispLyr.SEL.SET_FDSIZE.uiFDX = od_result[i].x;
		DispLyr.SEL.SET_FDSIZE.uiFDY = od_result[i].y;
		DispLyr.SEL.SET_FDSIZE.uiFDW = od_result[i].w;
		DispLyr.SEL.SET_FDSIZE.uiFDH = od_result[i].h;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDSIZE, &DispLyr);

		DispLyr.SEL.SET_FDCOLOR.FD_NUM = DISPFD_NUM0 << i;

		// Blue   : 0x1d/0xff/0x6b
		// White  : 0xff/0x80/0x80
		// Red    : 0x4c/0x54/0xff
		// Yellow : 0xff/0x00/0x94
		switch (od_result[i].class) {
		case 0:     // Bus (blue)
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0x1d;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0xff;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0x6b;
			break;
		case 1:     // Car (white)
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0xff;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0x80;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0x80;
			break;
		case 2:     // Human (red)
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0x4c;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0x54;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0xff;
			break;
		default:    // Others (yellow)
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0xff;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0x00;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0x94;
			break;
		}
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDCOLOR, &DispLyr);

		DispLyr.SEL.SET_FDEN.FD_NUM = DISPFD_NUM0 << i;
		DispLyr.SEL.SET_FDEN.bEn = TRUE;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);
	}
	pDispObj->load(TRUE);
#endif
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvod = {
	{
		PHOTO_PRIORITY_M,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_ODProcess",
		Movie_ODInit,                       ///< init OD, only execute at first process
		Movie_ODProcess,                    ///< process OD
		Movie_ODProcessEnd,                 ///< process end OD
		NULL,                      ///< exit fp, only execute at photo close
		OD_CalcBuffSize,                    ///< get working buffer OD
		OD_CalcCacheBuffSize,               ///< get working cache buffer OD
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2017/04/28#KCHong -end


#if MOVIE_FD_FUNC_
void Movie_FDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    FD_Config(FD_CFG_MAX_FACE, MAX_FDNUM);
	FD_Config(FD_CFG_FACE_SCALE, MOVIE_FD_FACESCALE);
	FD_Config(FD_CFG_PROCESS_RATE, 3);
	FD_Init(buf, cachebuf);
}

void Movie_FDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	//UINT32           ImageRatioIdx;

	//ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
	//ImageRatioIdx = IMAGERATIO_16_9;
	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH4));

	if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		#if 0
		{
			static UINT32 isdump = 0;
			PIMG_BUF pImgBuf = (IMG_BUF *) &isf_data.Desc[0];
			if (!isdump)
			{
				DBG_DUMP("[dump]pImgBuf w = %lu, h = %lu, fmt = %lu\r\n", pImgBuf->Width, pImgBuf->Height, pImgBuf->PxlFmt);
				GxImg_DumpBuf("A:\\fd.yuv", pImgBuf, GX_IMAGE_DUMP_ALL);
				isdump = 1;
			}
		}
		#endif
		//FD_Process((IMG_BUF *)&isf_data.Desc[0], Get_FDImageRatioValue(ImageRatioIdx));
		FD_Process((IMG_BUF *)&isf_data.Desc[0]);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}
}

UINT32 Movie_FDCalcBuffSize(void)
{
	FD_Config(FD_CFG_FACE_MAX_SCALE, MOVIE_FD_FACESCALE);
	return FD_CalcBuffSize();
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvfd = {
	{
		PHOTO_PRIORITY_M,       ///< function hook process Id.
		IPL_PATH_1,             ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_FDProcess",
		Movie_FDInit,           ///< init fp, only execute at first process
		Movie_FDProcess,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		Movie_FDCalcBuffSize,   ///< get working buffer fp
		FD_CalcCacheBuffSize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif



// #NT#2015/09/17#JS Liu - begin
#if _CVHOST_EX_
void Movie_CvHostExProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if (g_CvHostExDone != TRUE) {
		debug_msg("\n\r************************\n");
		debug_msg("\r*  Run CvHost Example  *\n");
		debug_msg("\r************************\n");
#if _CVHOST_EX_ENDSP_
		SxCmd_DoCommand("dsp open");    // ASSUME DSP is opened already!!
#endif

		CvHostEx_Opening(buf);
		CvHostEx_Sobel3x3(buf);
		CvHostEx_Gaussian3x3(buf);

		g_CvHostExDone = TRUE;

#if _CVHOST_EX_ENDSP_
		SxCmd_DoCommand("dsp close");
#endif
		debug_msg("\r** CvHost Example End **\n");
	}
}

UINT32 Movie_CvHostExCalcBufSize(void)
{
	UINT32 ImgRowsMax = 1080;
	UINT32 ImgColsMax = 1920;
	UINT32 ImgSizeMax = ImgRowsMax * ImgColsMax;
	UINT32 CvHostBufSize = CvHost_GetWorkMemSize();

	return ImgSizeMax * 2 + CvHostBufSize;  // input gray, output gray and CvHost working buffer
}

PHOTO_FUNC_INFO MovieFuncInfo_CvHostEx = {
	{
		PHOTO_PRIORITY_M,           ///< function hook process Id.
		IPL_PATH_1,                 ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,      ///< isr tigger event
		"Movie_CvHostExProcess",    ///< process name
		NULL,                       ///< init fp, only execute at first process
		Movie_CvHostExProcess,      ///< process fp
		NULL,                       ///< process end fp
		NULL,                       ///< exit fp, only execute at photo close
		Movie_CvHostExCalcBufSize,  ///< get working buffer fp
		NULL,                       ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
// #NT#2015/09/17#JS Liu - end


//#NT#2016/05/31#Omega Yu -begin
#if _NVTCV_EX_
void Movie_NvtCvExProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if (g_NvtCvExDone != TRUE) {
		debug_msg("\n\r************************\n");
		debug_msg("\r*  Run NvtCv Example   *\n");
		debug_msg("\r************************\n");
#if _NVTCV_EX_ENDSP_
		SxCmd_DoCommand("dsp open");    // ASSUME DSP is opened already!!
#endif

		UIAppMovie_NvtCvEx_test(buf);

		g_NvtCvExDone = TRUE;

#if _NVTCV_EX_ENDSP_
		SxCmd_DoCommand("dsp close");
#endif
		debug_msg("\r** NvtCv Example End **\n");
	}
}

UINT32 Movie_NvtCvExCalcBufSize(void)
{
	UINT32 ImgRowsMax = 1080;
	UINT32 ImgColsMax = 1920;
	UINT32 ImgSizeMax = ImgRowsMax * ImgColsMax;
	UINT32 NvtCvBufSize = NvtCv_GetWrkMemSize();

	return ImgSizeMax * 3 + NvtCvBufSize;  // input gray, output gray and NvtCv working buffer
}

PHOTO_FUNC_INFO MovieFuncInfo_NvtCvEx = {
	{
		PHOTO_PRIORITY_M,           ///< function hook process Id.
		IPL_PATH_1,                 ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,      ///< isr tigger event
		"Movie_NvtCvExProcess",     ///< process name
		NULL,                       ///< init fp, only execute at first process
		Movie_NvtCvExProcess,       ///< process fp
		NULL,                       ///< process end fp
		NULL,                       ///< exit fp, only execute at photo close
		Movie_NvtCvExCalcBufSize,   ///< get working buffer fp
		NULL,                       ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif // _NVTCV_EX_
//#NT#2016/05/31#Omega Yu -end

#if (_ADAS_FUNC_ == ENABLE)
void Photo_ADASInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ADAS_MidPrioAppsInit(buf, cachebuf, 1, 0);
}

void Photo_ADASProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA isf_data;
	ISF_UNIT *pUnit;
	ISF_PORT *pSrc;
	BOOL     buf_got = FALSE;

	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH4));

	wai_sem(AlgPullIsfDataSemiID[MOVIE_ALG_IPL1_4]);
    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		#if 0   // For dump file test
		static UINT32 frame_cnt = 0;
		char filename[50];
		FST_FILE pFile = NULL;
		UINT32 addr = 0, size = 0;
		IMG_BUF *pImgBuf = (IMG_BUF *)&isf_data.Desc[0];
		frame_cnt++;
		if (!(frame_cnt % 100)) {
			snprintf(filename, 50, "A:\\test%05d.yuv", frame_cnt);
			DBG_DUMP("Save yuv file %s\r\n", filename);
			DBG_DUMP("pImgBuf w = %lu, h = %lu, fmt = %lu\r\n", pImgBuf->Width, pImgBuf->Height, pImgBuf->PxlFmt);
			DBG_DUMP("pImgBuf PxlAddr = 0x%08X 0x%08X 0x%08X, LineOffset = %lu %lu %lu, metadata addr = 0x%08X, size = 0x%08X\r\n",
			pImgBuf->PxlAddr[0], pImgBuf->PxlAddr[1], pImgBuf->PxlAddr[2], pImgBuf->LineOffset[0], pImgBuf->LineOffset[1], pImgBuf->LineOffset[2], pImgBuf->Metadata.Addr, pImgBuf->Metadata.Size);

			pFile = FileSys_OpenFile(filename, FST_OPEN_WRITE|FST_OPEN_ALWAYS);
			if (pFile) {
				addr = pImgBuf->PxlAddr[0];
				size = pImgBuf->Width * pImgBuf->Height * 2;
				FileSys_WriteFile(pFile, (UINT8 *)addr, &size, 0, NULL);
				FileSys_CloseFile(pFile);
				DBG_DUMP("Save file %s\r\n", filename);
			}
		}
		#endif  // #if 0   // For dump file test

		// Copy data first
		if (ADAS_GetAppsStatus()) {
			ADAS_MidPrioAppsCopySrcDataInt((IMG_BUF *)&isf_data.Desc[0]);
		}

		// Relese data buffer
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		buf_got = TRUE;
	}
	sig_sem(AlgPullIsfDataSemiID[MOVIE_ALG_IPL1_4]);
	if (buf_got) {
		// Process data
		ADAS_MidPrioAppsProcess();
	}
}

void Photo_ADASProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	URECT DispCord = {0, 0, OSD_W, OSD_H};
	ADAS_APPS_RESULT_INFO *pAdasRlt = MovieExe_GetAdasRltOSD();

	if (ADAS_GetAppsStatus() == FALSE) {
		return;
	}

#if (_AUTOVP_FUNC_ == ENABLE)
	URECT ADASCropWin = {0};
	// Get crop window setting
	if (ADAS_GetNewCropWindow(&ADASCropWin) == TRUE) {
		Ux_PostEvent(NVTEVT_CB_ADAS_SETCROPWIN, 2, ((ADASCropWin.x << 16) | ADASCropWin.y), ((ADASCropWin.w << 16) | ADASCropWin.h));
	}
#endif  // #if (_AUTOVP_FUNC_ == ENABLE)

	// Get FC/LD Result
	DispCord = gMovieOsdDispCord;
	if (ADAS_GetLdFcRslt(pAdasRlt, &DispCord) == ADAS_STA_OK) {
#if (GPS_FUNCTION == ENABLE)
		if (((g_GPSLinked == FALSE) || (g_GPSStatus == FALSE) || (g_CurSpeed > 50)) && (ADAS_GetStopDectRslt() == STOPDECT_STOP_FALSE))
#else   // #if (GPS_FUNCTION == ENABLE)
		if (ADAS_GetStopDectRslt() == STOPDECT_STOP_FALSE)
#endif  // #if (GPS_FUNCTION == ENABLE)
		{
			if ((pAdasRlt->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE) &&
				((pAdasRlt->LdwsRsltInfo.DepartureDirSound == LDWS_DEPARTURE_LEFT) || (pAdasRlt->LdwsRsltInfo.DepartureDirSound == LDWS_DEPARTURE_RIGHT))) {
				if (UI_GetData(FL_MOVIE_LDWS) == MOVIE_LDWS_ON) {
					Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM, 1, ADAS_ALARM_LD);
				}
			} else if ((pAdasRlt->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE) && (pAdasRlt->FcwsRsltInfo.FCSoundWarning == TRUE)) {
				if (UI_GetData(FL_MOVIE_FCW) == MOVIE_FCW_ON) {
					Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM, 1, ADAS_ALARM_FC);
				}
			}
		}
	}
#if (_DRAW_LDWSINFO_ON_VIDEO_ == ENABLE)
	MOVIE_RECODE_INFO *pRecInfo = (MOVIE_RECODE_INFO *)ImageApp_MovieMulti_GetConfig(_CFG_REC_ID_1);
	if (pRecInfo) {
		DispCord.x = 0;
		DispCord.y = 0;
		DispCord.w = pRecInfo->size.w;
		DispCord.h = pRecInfo->size.h;
		pAdasRlt = MovieExe_GetAdasRltVideo();
		ADAS_GetLdFcRslt(pAdasRlt, &DispCord);
	}
#endif

	// Get Log Data
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	UINT32 LogSize = 0;
	UINT32 LogBuf = 0;
	LogSize = ADAS_GetLdFcLog(&LogBuf);
	if (LogSize) {
		mp4log_AddLog(DbgLogFCLDID, (char *)LogBuf, LogSize);
	}
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)

#if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
	if (1)
#else   // #if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
	if (ADAS_IsAdasDebug())
#endif  // #if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
	{
		Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
	}
}

PHOTO_FUNC_INFO PhotoFuncInfo_adas = {
	{
		PHOTO_PRIORITY_M,                           ///< function hook process Id.
		IPL_PATH_1,                                 ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,                      ///< isr tigger event
		"Photo_ADASProcess",
		Photo_ADASInit,                             ///< init fp, only execute at first process
		Photo_ADASProcess,                          ///< process fp
		Photo_ADASProcessEnd,                       ///< process end fp
		NULL,                                       ///< exit fp, only execute at photo close
		ADAS_CalcMidPrioAppsBuffSize,               ///< get working buffer fp
		NULL,                                       ///< get working cache buffer fp
	},
	NULL                                            ///< point next Function Obj, last Function must be set NULL
};

#if (_AUTOVP_FUNC_ == ENABLE)
void Photo_AutoVPInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ADAS_LowPrioAppsInit(buf, cachebuf, 1, 1 * 1000); // 1s
}

void Photo_AutoVPProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA isf_data;
	ISF_UNIT *pUnit;
	ISF_PORT *pSrc;
	BOOL     buf_got = FALSE;

	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));

	wai_sem(AlgPullIsfDataSemiID[MOVIE_ALG_IPL1_3]);
    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		#if 0   // For dump file test
		static UINT32    frame_cnt = 0;
		char             filename[50];
		frame_cnt++;
		if (!(frame_cnt % 100)) {
			snprintf(filename, 50, "A:\\test%05d.yuv", frame_cnt);
			DBG_DUMP("Save yuv file %s\r\n", filename);
			GxImg_DumpBuf(filename, (IMG_BUF *)&isf_data.Desc[0], GX_IMAGE_DUMP_ALL);
		}
		#endif  // #if 0   // For dump file test

        //ADAS_AppsSetAutoVPEanble(TRUE);

		// Copy data first
		if (ADAS_GetAppsStatus()) {
			ADAS_LowPrioAppsCopySrcDataInt((IMG_BUF *)&isf_data.Desc[0]);
		}

		// Relese data buffer
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		buf_got = TRUE;
	}
	sig_sem(AlgPullIsfDataSemiID[MOVIE_ALG_IPL1_3]);
	if (buf_got) {
		// Process data
		ADAS_LowPrioAppsProcess();
	}
}

void Photo_AutoVPProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	if (ADAS_GetAppsStatus() == FALSE) {
		return;
	}
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	UINT32 LogSize = 0;
	UINT32 LogBuf = 0;
	LogSize = ADAS_GetAutoVPLog(&LogBuf);
	if (LogSize) {
		mp4log_AddLog(DbgLogAutoVPID, (char *)LogBuf, LogSize);
	}
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
}

PHOTO_FUNC_INFO PhotoFuncInfo_autoVP = {
	{
		PHOTO_PRIORITY_L,                           ///< function hook process Id.
		IPL_PATH_1,                                 ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,                      ///< isr tigger event
		"Photo_AutoVPProcess",
		Photo_AutoVPInit,                           ///< init fp, only execute at first process
		Photo_AutoVPProcess,                        ///< process fp
		Photo_AutoVPProcessEnd,                     ///< process end fp
		NULL,                                       ///< exit fp, only execute at photo close
		ADAS_CalcLowPrioAppsBuffSize,               ///< get working buffer fp
		ADAS_CalcLowPrioAppsCacheBuffSize,          ///< get working cache buffer fp
	},
	NULL                                            ///< point next Function Obj, last Function must be set NULL
};
#endif  // #if (_AUTOVP_FUNC_ == ENABLE)

#if _TODO   // [ADAS_TODO] (_SNG_FUNC_ == ENABLE)
void Photo_SnGInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ADAS_HighPrioAppsInit(buf, cachebuf, 1, 66); // 66ms
}

void Photo_SnGProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ADAS_HighPrioAppsProcess();
}

void Photo_SnGProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	URECT DispCord = {0, 0, OSD_W, OSD_H};
	ADAS_APPS_RESULT_INFO *pAdasRlt = MovieExe_GetAdasRltOSD();

	if (ADAS_GetAppsStatus() == FALSE) {
		return;
	}

	// Get SnG Result
	DispCord = gMovieOsdDispCord;
	if (ADAS_GetSnGRslt(pAdasRlt, &DispCord) == ADAS_STA_OK) {

#if (GPS_FUNCTION == ENABLE)
		if ((g_GPSLinked == FALSE) || (g_GPSStatus == FALSE) || (g_CurSpeed <= 50)) {
#endif  // #if (GPS_FUNCTION == ENABLE)
			if (pAdasRlt->SnGInfo.uiGoStatus == 1) {
				Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM, 1, ADAS_ALARM_GO);
			}
			if (pAdasRlt->SnGInfo.uiStopStatus == 3) {
				Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM, 1, ADAS_ALARM_STOP);
			}
#if (GPS_FUNCTION == ENABLE)
		}
#endif  // #if (GPS_FUNCTION == ENABLE)
	}

#if _TODO   // [ADAS_TODO] (_DRAW_SNG_ROI_ == ENABLE)
	ADAS_SNG_ROI_INFO *pSnGROI = NULL;

	pSnGROI = MovieExe_GetSnGROIOSD();
	ADAS_GetSnGRoi(pSnGROI, &DispCord);

#if _TODO   // [ADAS_TODO] (_DRAW_LDWSINFO_ON_VIDEO_ == ENABLE)
	IMG_BUF InputImg = {0};
	ImageUnit_PullData(ImageUnit_Out(ISF_IPL(0), ISF_OUT1), (ISF_DATA *)&InputImg, 0);
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = InputImg.Width;
	DispCord.h = (InputImg.Height == 1088) ? 1080 : (InputImg.Height);

	pAdasRlt = MovieExe_GetAdasRltVideo();
	ADAS_GetSnGRslt(pAdasRlt, &DispCord);

	pSnGROI = MovieExe_GetSnGROIVideo();
	ADAS_GetSnGRoi(pSnGROI, &DispCord);
#endif
#endif

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	UINT32 LogSize = 0;
	UINT32 LogBuf = 0;
	LogSize = ADAS_GetSnGLog(&LogBuf);
	if (LogSize) {
		mp4log_AddLog(DbgLogSnGID, (char *)LogBuf, LogSize);
	}
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
}

PHOTO_FUNC_INFO PhotoFuncInfo_SnG = {
	{
		PHOTO_PRIORITY_H,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Photo_SnGProcess",
		Photo_SnGInit,           ///< init fp, only execute at first process
		Photo_SnGProcess,        ///< process fp
		Photo_SnGProcessEnd,     ///< process end fp
		NULL,                    ///< exit fp, only execute at photo close
		ADAS_CalcHighPrioAppsBuffSize,        ///< get working buffer fp
		NULL,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif  // #if (_SNG_FUNC_ == ENABLE)
#endif  // #if (_ADAS_FUNC_ == ENABLE)

//#if MOVIE_DIS || MOVIE_FTG_FUNC || MOVIE_SCD_FUNC   // [ADAS_TODO] || _ADAS_FUNC_
#if MOVIE_DIS || MOVIE_SCD_FUNC   // [ADAS_TODO] || _ADAS_FUNC_
#include "af_api.h"
#include "ipl_header_infor_raw.h"
#include "dis_alg.h"
#include "ipl_cmd.h"
#include "dis_api.h"


void Photo_DIS_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	dis_initialize(buf, cachebuf);

	dis_setDisViewRatio(900);

    DIS_InstallCmd();

#if _TODO   // [ADAS_TODO] (_ADAS_FUNC_ == ENABLE)
	//Disable FW/ROI dis to decrease CPU usage.
	DISALG_CFGINFO info;
	info.iCfgVal = FALSE;
	dis_SetConfig(DISALG_CFG_GLOBALMOTEN, &info);
	dis_SetConfig(DISALG_CFG_ROIMOTEN, &info);
#endif

	//AF_VA_Setup(0);
}

//#NT#2016/05/25#Harry_Tsai -begin
void Photo_DIS(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


#if (IPCAM_DIS_COMP_FUNC)
	dis_compensation(buf,cachebuf,FALSE);
#else
    dis_compensation(buf,cachebuf,TRUE);
#endif

#if (MOVIE_FTG_FUNC)
    if (!gFTGLock)
        ftg_savedisrslt();
#endif
}

//#NT#2016/05/25#Harry_Tsai -end

//
PHOTO_FUNC_INFO PhotoFuncInfo_dis = {
	{
		PHOTO_PRIORITY_H,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
//#NT#2016/05/04#Harry_Tsai -begin
//#NT# Change to VD
		PHOTO_ISR_IPE_FRM_END,       ///< isr tigger event
		"Photo_DIS",
//#NT#2016/05/04#Harry_Tsai -end
		Photo_DIS_Init,                   ///< init fp, only execute at first process
		Photo_DIS,           ///< process fp
		NULL,                   ///< process end fp
		NULL,                      ///< exit fp, only execute at photo close
		dis_GetPrvMaxBuffer,        ///< get working buffer fp
		NULL,                   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#else
PHOTO_FUNC_INFO PhotoFuncInfo_dis = {0};
#endif

//#NT#2016/03/02#Lincy Lin -begin
//#NT#Support object tracking function
#if MOVIE_OT_FUNC
void Movie_OTInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
//#NT#2017/06/16#MB Wang -begin
//#NT#Set Alg Version and Feature Type for OT
    OT_SetConfig(OT_CFG_ALG_VERSION,1);  //0: single scale, 1: multiscale.
    OT_SetConfig(OT_CFG_FEATURE_TYPE,0);  // feature type can be set 0 or 1.
//#NT#2017/06/16#MB Wang -end
	NvtOt_Init(buf);
}

void Movie_OTProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA         isf_data;
    ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	UINT32           ImageRatioIdx;
	OT_UIMGSIZE      InputImgSize;
	IMG_BUF*         IMG_TEMP;
	BOOL             buf_got = FALSE;

	if (ImageApp_MovieMulti_IsStreamRunning(_CFG_REC_ID_1) == FALSE) { // not in recording
		return;
	}

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));

	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));

	wai_sem(AlgPullIsfDataSemiID[MOVIE_ALG_IPL1_3]);
	if (ImageUnit_IsAllowPull(pSrc) && (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK)) {

		IMG_TEMP = (IMG_BUF *)&isf_data.Desc[0];

		InputImgSize.width = IMG_TEMP->Width;
		InputImgSize.height = IMG_TEMP->Height;
		InputImgSize.lineoffset = IMG_TEMP->LineOffset[0];
		InputImgSize.addr = IMG_TEMP->PxlAddr[0];
		InputImgSize.imgRatio = ImageRatioIdx;

		OT_SetImgSize( &InputImgSize);	//set should be done for every frame,addr maybe differ.
		ImageUnit_ReleaseData((ISF_DATA*)&isf_data);
		buf_got = TRUE;
	}
	sig_sem(AlgPullIsfDataSemiID[MOVIE_ALG_IPL1_3]);
	if (buf_got) {
		if(!OT_PROCESS_STA()){
			NvtCv_ot_process();
		}
		//#NT#2016/06/08#Lincy Lin -begin
		//#NT#Implement generic OSD and video drawing mechanism for ALG function
		Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
		//#NT#2016/06/08#Lincy Lin -end
	}
}
PHOTO_FUNC_INFO PhotoFuncInfo_mvot = {
	{
		PHOTO_PRIORITY_SH,          ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_OTProcess",
		Movie_OTInit,           ///< init fp, only execute at first process
		Movie_OTProcess,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		OT_CalcBuffSize,        ///< get working buffer fp
		NULL,//OT_CalcCacheBuffSize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif
//#NT#2016/03/02#Lincy Lin -end

//#NT#2016/05/20#Yang Jin -begin
//#NT#Support Dis dsp function
#if MOVIE_DISDSP_FUNC
void Movie_DISDSPInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	DisDsp_Init(buf);
}

void Movie_DISDSPProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	IMG_BUF   InputImg = {0};
	ISF_PORT *pSrc = ImageUnit_Out(ISF_IPL(0), ISF_OUT3);
	DIS_DSP_ROIGMV_OUT roiOut;
	UIMGSIZE InputImgSize = {0};

	if (pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE)) {
		ImageUnit_PullData(ImageUnit_Out(ISF_IPL(0), ISF_OUT3), (ISF_DATA *)&InputImg, 0);

		InputImgSize.width = InputImg.Width;
		InputImgSize.height = InputImg.Height;
		InputImgSize.lineoffset = InputImg.LineOffset[0];
		InputImgSize.addr = InputImg.PxlAddr[0];

		DisDsp_ImgSize_SetInputInfo(&InputImgSize);
		NvtCv_disdsp_process();

		roiOut = DisDsp_Roi_GetResult();
		DBG_ERR("x = %d\t y = %d\t valid = %d\n\r", roiOut.vector.iX, roiOut.vector.iY, roiOut.bValidVec);
	}
}
PHOTO_FUNC_INFO PhotoFuncInfo_mvdisdsp = {
	{
		PHOTO_PRIORITY_SH,          ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_DISDSPProcess",
		Movie_DISDSPInit,       ///< init fp, only execute at first process
		Movie_DISDSPProcess,    ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		DisDsp_CalcBuffSize,    ///< get working buffer fp
		NULL,                   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif
//#NT#2016/05/20#Yang Jin -end



//#NT#2016/11/16#ML Cui -begin
//#NT# The FD for Driver Drowsiness Detection(DDDFD)
#if MOVIE_DDD_FD_FUNC
#include "DDD_fd.h"
#include "ImageUnit_ImagePipe.h"

UINT32 g_pDDDFdInputImgBuf = 0;

void Movie_DDDFdInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	//GET_EXTERN_STU(FD_MODEL_NAME).pfInit(buf, cachebuf, 3);
	debug_msg("DDDFD Init\r\n");
	FD_FUNC(DDDFd)->pfInit(buf, cachebuf);
	FD_FUNC(DDDFd)->pfSetParam(FD_PARAM_USE_HISTORY, TRUE);
    FD_FUNC(DDDFd)->pfSetParam(FD_PARAM_MAX_RSLT, 2);
    FD_FUNC(DDDFd)->pfSetParam(FD_PARAM_ADJ_CONTRAST, FALSE);

	g_pDDDFdInputImgBuf = buf->Addr;
    buf->Size -= DDD_INPUTIMG_BUF_SIZE;
    buf->Addr += DDD_INPUTIMG_BUF_SIZE;
}

void Movie_DDDFdProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF InputImg = {0};
#if 0
	UINT32 myDualCam;
#endif
	ISF_PORT *pSrc;
	ISF_UNIT *pUnit;
	ISF_DATA isf_data;

	static UINT32 index = 0;

	if (index++ % 3 != 0)
		return;

#if 0
#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == SENSOR_1)
		myDualCam = DUALCAM_FRONT;
	else
		myDualCam = DUALCAM_BEHIND;
#elif (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == SENSOR_1 || System_GetEnableSensor() == SENSOR_2)
	{//only enable one sensor
		if (System_GetEnableSensor() == SENSOR_1)
			myDualCam = DUALCAM_FRONT;
		else
			myDualCam = DUALCAM_BEHIND;
	}
	else
	{//enable dual sensors
		myDualCam = DUALCAM_BEHIND;
	}
#else
#error "Undefined case"
#endif
#endif
#if 0
	if (myDualCam == DUALCAM_FRONT)
		pUnit = ISF_IPL(0);
	else
		pUnit = ISF_IPL(1);

#else
    pUnit = &ISF_UserProc;
    ImageUnit_Begin(pUnit, 0);
    ImageUnit_SetParam(ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3), USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
    ImageUnit_End();
#endif

	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));

    InputImg.Width = DDD_MAX_INPUTIMG_WIDTH;
    InputImg.Height = DDD_MAX_INPUTIMG_HEIGHT;
    InputImg.LineOffset[0] = ALIGN_CEIL_4(DDD_MAX_INPUTIMG_WIDTH);
    InputImg.PxlAddr[0] = g_pDDDFdInputImgBuf;

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3)), &isf_data, 0) == ISF_OK)
	{
        DDD_resizeInputImg((IMG_BUF *)&isf_data.Desc[0], &InputImg);
        ImageUnit_ReleaseData(&isf_data);
        FD_FUNC(DDDFd)->pfProcess(&InputImg);
    }
	else
	    DBG_ERR("get image data failed from ISF_OUT%d\r\n", (ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3) + 1));

}

static UINT32 Movie_DDDFdGetBuf(VOID)
{
	return FD_FUNC(DDDFd)->pfCalcBuffSize();
}

static UINT32 Movie_DDDFdGetCache(VOID)
{
	return FD_FUNC(DDDFd)->pfCalcCacheBuffSize();
}

static VOID Movie_DDDFdEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvdddfd = {
	{
		PHOTO_PRIORITY_M,                 ///< function hook process Id.
		IPL_PATH_1,                   ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,      ///< isr tigger event
		"Movie_DDDFdProcess",
		Movie_DDDFdInit,           ///< init tsr, only execute at first process
		Movie_DDDFdProcess,        ///< process tsr
		Movie_DDDFdEnd,            ///< process end tsr
		NULL,                      ///< exit fp, only execute at photo close
		Movie_DDDFdGetBuf,       ///< get working buffer tsr
		Movie_DDDFdGetCache,  ///< get working cache buffer tsr
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/11/16#ML Cui -end


//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if MOVIE_DDD_FUNC
#define DDD_USING_FD_OR_NOT DISABLE
#if DDD_USING_FD_OR_NOT
#include "NvtFd.h"
#include "fde_lib.h"
static UINT32 g_pFdHandleBuf = 0;
#endif
#if !(MOVIE_FD_FUNC_ || MOVIE_DDD_FD_FUNC)
#error "DDD need enable MOVIE_FD_FUNC_ or MOVIE_DDD_FD_FUNC"
#endif

#include "DDD_fd.h"

static UINT32 g_pDDDInputImgBuf = 0;
static BOOL g_bDDDNeedInit = TRUE;

static INT32 Movie_DDDGetFdRslt(const IMG_BUF *pImgBuf, RECT *pFdRect)
{
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
    INT32 iFaceNum;
#if DDD_USING_FD_OR_NOT
    if(pFdRect->width != 0 && g_pFdHandleBuf)
    {
        FACE face = {0};

        face.uiStx = pFdRect->x;
        face.uiSty = pFdRect->y;
        face.uiWidth = pFdRect->width;
        face.uiHeight = pFdRect->height;
        face.uiTrgIdx = FDE_R000_P000;

        UINT32 t1 = Perf_GetCurrent();
        iFaceNum = NvtFD_DetectFaceOrNot(pImgBuf, &face, 1, g_pFdHandleBuf);

        if(iFaceNum)
        {
//            debug_msg("Front time : %d\r\n", Perf_GetCurrent() - t1);
            return iFaceNum;
        }
        else
        {
            INT32 iShake = DDD_GetPoseAngleShake();
            //The FD's Left and Right is opposite to DDD's
            if(iShake > 200)
                face.uiTrgIdx = FDE_R000_PR90;
            else if(iShake < -200)
                face.uiTrgIdx = FDE_R000_PL90;
            iFaceNum = NvtFD_DetectFaceOrNot(pImgBuf, &face, 1, g_pFdHandleBuf);
            if(iFaceNum)
            {

				debug_msg("Profile %d time : %d\r\n", face.uiTrgIdx, Perf_GetCurrent() - t1);
				return iFaceNum;
			}

		}
		debug_msg("recall FD wast time : %d \r\n");
	}

#endif

#if MOVIE_DDD_FD_FUNC
    FACE faceRslt;
    iFaceNum = FD_FUNC(DDDFd)->pfGetFDResult(&faceRslt, 1, FD_SORT_BY_SIZE);

    if(iFaceNum > 0)
    {
        pFdRect->x = (INT32)faceRslt.uiStx;
        pFdRect->y = (INT32)faceRslt.uiSty;
        pFdRect->width = (INT32)faceRslt.uiWidth;
        pFdRect->height = (INT32)faceRslt.uiHeight;
    }
#else
    iFaceNum = FD_GetRsltFaceNum();
    if(iFaceNum >= 0)
    {
        static FD_FACE faceRslt[FD_NUM_MAX] = {0};
#if 1
        URECT DispCord;
        DispCord.x = 0;
        DispCord.y = 0;
        DispCord.w = pImgBuf->Width;
        DispCord.h = pImgBuf->Height;
        FD_GetRsltFace(faceRslt, &DispCord);
        pFdRect->height = pFdRect->width = MAX(faceRslt[0].w, faceRslt[0].h);
        pFdRect->x = faceRslt[0].x + faceRslt[0].w / 2 - pFdRect->width / 2;
        pFdRect->y = faceRslt[0].y + faceRslt[0].h / 2 - pFdRect->height / 2;
#else

        FD_GetRsltFace(faceRslt, 0);
        pFdRect->x = faceRslt[0].x * pImgBuf->Width / 640;
        pFdRect->y = faceRslt[0].y * pImgBuf->Height / 360;
        pFdRect->width = MAX(faceRslt[0].w * pImgBuf->Width / 640,
                             faceRslt[0].h * pImgBuf->Height / 360);
        pFdRect->height = pFdRect->width;
#endif
    }
#endif



    return iFaceNum;
}
//#NT#2016/11/10#Brain Yen -begin
//#NT#Add for save DDD MODEL in pstore
#if DDD_MODEL_IN_PSTORE
UINT32 Movie_DDDCheckPSFormat(void)
{
    #define SETTING_SIZE 30
    #define FORMAT_CHK_STRING  "format="

    UINT32      uiReadSize = SETTING_SIZE;
    UINT32      uiUpdateChk;
    CHAR        cDataBuf[SETTING_SIZE]={0}, *pCh;
    FST_FILE    pFile;

    pFile = FileSys_OpenFile("A:\\ddd_psformat", FST_OPEN_READ);
    if (pFile)
    {
        FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
        FileSys_CloseFile(pFile);
        //DBG_DUMP("Read size %d\r\n", uiReadSize);
        if (uiReadSize)
        {
            pCh = strstr(cDataBuf, FORMAT_CHK_STRING);
            sscanf(pCh + sizeof(FORMAT_CHK_STRING) - 1, "%d", &uiUpdateChk);
            DBG_DUMP("Movie_DDDCheckPSFormat FormatChk = %d, size %d\r\n", uiUpdateChk, sizeof(FORMAT_CHK_STRING));
            return uiUpdateChk;
        }
    }
    else
    {
        //DBG_ERR("Open ddd_psformat check file error!\r\n");
    }
    return 0;

}
UINT32 Movie_DDDCheckDBUpdate(void)
{
    #define SETTING_SIZE 30
    #define UPDATE_CHK_STRING  "update="

    UINT32      uiReadSize = SETTING_SIZE;
    UINT32      uiUpdateChk;
    CHAR        cDataBuf[SETTING_SIZE]={0}, *pCh;
    FST_FILE    pFile;

    pFile = FileSys_OpenFile("A:\\ddd_update", FST_OPEN_READ);
    if (pFile)
    {
        FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
        FileSys_CloseFile(pFile);
        //DBG_DUMP("Read size %d\r\n", uiReadSize);
        if (uiReadSize)
        {
            pCh = strstr(cDataBuf, UPDATE_CHK_STRING);
            sscanf(pCh + sizeof(UPDATE_CHK_STRING) - 1, "%d", &uiUpdateChk);
            DBG_DUMP("Movie_DDDCheckDBUpdate UpdateChk = %d, size %d\r\n", uiUpdateChk, sizeof(UPDATE_CHK_STRING));
            return uiUpdateChk;
        }
    }
    else
    {
        //DBG_ERR("Open ddd_update check file error!\r\n");
    }
    return 0;

}
#endif
//#NT#2016/11/10#Brain Yen -end
UINT32 Movie_DDDGetDBFileSizeFrCard(void)
{
    FST_FILE filehdl = NULL;
    UINT32 ui32FileSize = 0;

    CHAR* pBinaryFileName = "B:\\DDD_MODEL\\"DDD_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\DDD_MODEL\\"DDD_MODEL_NAME;
        DBG_DUMP("DDD load model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }

    if(filehdl)
    {
        // Get file size info
        FileSys_SeekFile(filehdl, 0, FST_SEEK_END);
        ui32FileSize = (UINT32)FileSys_TellFile(filehdl);
        //DBG_DUMP("ui32FileSize=0x%x\r\n",ui32FileSize);
        FileSys_SeekFile(filehdl, 0, FST_SEEK_SET);

        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("DDD_Init Failed: load model failed!\r\n");
        return 0;
    }
    return ui32FileSize;
}
//#NT#2016/05/11#Isiah Chang -begin
//#NT# Decrypt DDD database file and then feed it into DDD_Init().
UINT32 Movie_DDDGetDBFileSize(void)
{
    UINT32 ui32FileSize = 0;
#if DDD_MODEL_IN_PSTORE
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32  result;
    if ((pSection = PStore_OpenSection(PS_DDD_MODEL1_SIZE, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        result = PStore_ReadSection((UINT8 *)&ui32FileSize, 0, sizeof(UINT32), pSection);
        DBG_IND("result=%d, ui32FileSize=0x%x\r\n",result,ui32FileSize);
        if (ui32FileSize==0)
        {
            DBG_ERR("PStore Save default DDD Model Size\r\n");
            ui32FileSize=Movie_DDDGetDBFileSizeFrCard();
            DBG_ERR("ui32FileSize=%d\r\n",ui32FileSize);
            PStore_WriteSection((UINT8 *)&ui32FileSize,0,sizeof(UINT32),pSection);
            PStore_CloseSection(pSection);
        }
        else
        {
            DBG_DUMP("PStore Load DDD Model Size OK\r\n");
            PStore_CloseSection(pSection);
        }
    }
    else
    {
        DBG_ERR("PStore Load DDD Model Size Fail!\r\n");
        return 0;
    }
#else
    ui32FileSize=Movie_DDDGetDBFileSizeFrCard();
#endif
    return ui32FileSize;
}
//#NT#2016/05/11#Isiah Chang -end
void Movie_DDDGetDBFile(UINT8* pBinaryMap, UINT32* uiBuflen)
{
    //load Landmark Model
    FST_FILE filehdl = NULL;

    CHAR* pBinaryFileName = "B:\\DDD_MODEL\\"DDD_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\DDD_MODEL\\"DDD_MODEL_NAME;
        DBG_DUMP("DDD load model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }

    if(filehdl)
    {
        FileSys_ReadFile(filehdl, (UINT8*)pBinaryMap,
                     uiBuflen, 0, NULL);
        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("DDD_Init Failed: load model failed!\r\n");
        return;
    }
}
void Movie_DDDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
#if DDD_USING_FD_OR_NOT
	g_pFdHandleBuf = buf->Addr;
	buf->Size -= NvtFD_GetBuffSize();
	buf->Addr += NvtFD_GetBuffSize();
#endif

    g_pDDDInputImgBuf = buf->Addr;
    buf->Size -= DDD_INPUTIMG_BUF_SIZE;
    buf->Addr += DDD_INPUTIMG_BUF_SIZE;
}

void Movie_DDDInitWithDBFile(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
#if (DDD_MODEL_IN_PSTORE)
    if(Movie_DDDCheckPSFormat()==1)
    {
        DBG_ERR("PStore format!\r\n");
        System_PS_Format();
    }
#endif
#if 0
    ER bRetval;
#endif
    UINT32 pBinaryMap = buf->Addr;
    UINT32 uiDBFileSize = Movie_DDDGetDBFileSize();
    UINT32 uiBuflen = ALIGN_CEIL_16(uiDBFileSize);
#if (DDD_MODEL_IN_PSTORE)
    UINT32 uiUpdateDBChk=Movie_DDDCheckDBUpdate();
#endif

    if(uiDBFileSize > DDD_MAX_DB_FILE_SIZE)
    {
        DBG_ERR("DDD DB file size is too large!\r\n");
        return;
    }

    pBinaryMap = ALIGN_CEIL_4(pBinaryMap);//Input address must algin words
    if(buf->Size < uiBuflen + 4)
    {
        DBG_ERR("memory buf is (%d) not enough!\r\n", buf->Size);
    }

#if (DDD_MODEL_IN_PSTORE==DISABLE)
    {
        FST_FILE filehdl = NULL;

        CHAR* pBinaryFileName = "B:\\DDD_MODEL\\"DDD_MODEL_NAME;
        filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
        if(!filehdl)
        {
            CHAR* pBinaryFileName2 = "A:\\DDD_MODEL\\"DDD_MODEL_NAME;
            DBG_DUMP("DDD load model from SD-card!\r\n");
            filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
        }

        if(filehdl)
        {
            FileSys_ReadFile(filehdl, (UINT8*)pBinaryMap,
                         &uiBuflen, 0, NULL);
            FileSys_CloseFile(filehdl);
        }
        else
        {
            DBG_ERR("DDD_Init Failed: load model failed!\r\n");
            return;
        }
    }
#else
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32  result;

    uiBuflen=ALIGN_CEIL_16(uiDBFileSize);
    if ((pSection = PStore_OpenSection(PS_DDD_MODEL1_DATA, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        result = PStore_ReadSection((UINT8 *)pBinaryMap, 0, uiBuflen, pSection);
        if (result != E_PS_OK || uiUpdateDBChk)
        {
            DBG_ERR("Save DDD Model Data in PStore\r\n");
            Movie_DDDGetDBFile((UINT8 *)pBinaryMap, &uiBuflen);
            PStore_WriteSection((UINT8 *)pBinaryMap,0,uiBuflen,pSection);
            PStore_CloseSection(pSection);
        }
        else
        {
            DBG_DUMP("PStore Load DDD Model Data OK\r\n");
            PStore_CloseSection(pSection);
        }
    }
    else
    {
        DBG_ERR("DDD_Init Failed: load model failed!\r\n");
        return;
    }
#endif

	buf->Size -= uiBuflen + 4;
	buf->Addr += uiBuflen + 4;

#if 0
	crypto_open();
	bRetval = crypto_decryption(pBinaryMap, pBinaryMap, uiBuflen);
	crypto_close();

	if (bRetval != E_OK)
	{
		DBG_ERR("DDD decrypt failed!\r\n");
		return;
	}
#endif

	if (DDD_Init(buf, cachebuf, pBinaryMap,
				 Movie_DDDGetFdRslt) == DDD_STA_OK)
	{
		DBG_DUMP("DDD init success\r\n");
		if (DDD_LoadParam("A:\\DDD_config.txt"))
			DBG_DUMP("DDD load param from SD card\r\n");
		else
			DBG_DUMP("DDD using default param\r\n");

		if (DDD_LoadUserParam("A:\\ddd_user_config.txt"))
			DBG_DUMP("DDD load user param from SD card\r\n");
		else
			DBG_DUMP("DDD using default user param\r\n");
	}
	else
		DBG_ERR("DDD init failed!\r\n");
}

void Movie_DDDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF InputImg = {0};
#if 0
	UINT32 myDualCam;
#endif
	ISF_PORT *pSrc;
	ISF_UNIT *pUnit;
	ISF_DATA isf_data;
	static BOOL bNeedClean = FALSE;

	if(g_bDDDNeedInit)
	{
		Movie_DDDInitWithDBFile(buf, cachebuf);
		g_bDDDNeedInit = FALSE;
	}
#if 0
#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == SENSOR_1)
		myDualCam = DUALCAM_FRONT;
	else
		myDualCam = DUALCAM_BEHIND;
#elif (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == SENSOR_1 || System_GetEnableSensor() == SENSOR_2)
	{
		//only enable one sensor
		if (System_GetEnableSensor() == SENSOR_1)
			myDualCam = DUALCAM_FRONT;
		else
			myDualCam = DUALCAM_BEHIND;
	}
	else
	{
		//enable dual sensors
		myDualCam = DUALCAM_BEHIND;
	}
#else
#error "Undefined case"
#endif
#endif
#if 0
	if (myDualCam == DUALCAM_FRONT)
		pUnit = ISF_IPL(0);
	else
		pUnit = ISF_IPL(1);
#else
    pUnit = &ISF_UserProc;
    ImageUnit_Begin(pUnit, 0);
    ImageUnit_SetParam(ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3), USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
    ImageUnit_End();
#endif
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));

    InputImg.Width = DDD_MAX_INPUTIMG_WIDTH;
    InputImg.Height = DDD_MAX_INPUTIMG_HEIGHT;
    InputImg.LineOffset[0] = ALIGN_CEIL_4(DDD_MAX_INPUTIMG_WIDTH);
    InputImg.PxlAddr[0] = g_pDDDInputImgBuf;

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3)), &isf_data, 0) == ISF_OK)
	{
        DDD_resizeInputImg((IMG_BUF *)&isf_data.Desc[0], &InputImg);
        ImageUnit_ReleaseData(&isf_data);
		DDD_Process(&InputImg);
		if (!bNeedClean)
			bNeedClean = TRUE;
	}
	else
	{
		//we should clean the DDD result here to prevent OSD keep redrawing the landmark
		if (bNeedClean)
		{
			DDD_ClearRsltMsg();
			bNeedClean = FALSE;
		}
		DBG_ERR("get image data failed from ISF_OUT%d\r\n", (ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3) + 1));
	}
}



//Warning frequency will be more and more frequently then bState is always true
#define DDD_SET_WRN_STATE(bFlag, bState, maxFreq, minFreq, rate) \
	static UINT32 ui##bFlag##Frame = 0;   \
	static UINT32 ui##bFlag##Freq = minFreq;  \
	static BOOL   bFlag = FALSE;    \
	Movie_DDDSetWarnForStatistic(bState, &ui##bFlag##Frame, &ui##bFlag##Freq, \
								 &bFlag, maxFreq, minFreq, rate)

static void Movie_DDDSetWarnForStatistic(BOOL bState,
		UINT32 *uiWarnFrame, UINT32 *uiWarnFreq, BOOL *bWarn,
		UINT32 uiMaxFreq, UINT32 uiMinFreq, FLOAT rate)
{

	//debug_msg("uiWarnFrame:%d\r\n", *uiWarnFrame);
	if (bState)
	{

		(*uiWarnFrame)++;
		if (*uiWarnFrame == 1)
		{
			*bWarn = TRUE;
		}
		else if ((*uiWarnFrame) % (*uiWarnFreq) == 0)
		{
			*bWarn = TRUE;
			*uiWarnFreq /= rate;
			if (*uiWarnFreq < uiMinFreq)
				*uiWarnFreq = uiMinFreq;
		}
	}
	else
	{
		*uiWarnFrame = 0;
		*uiWarnFreq = uiMaxFreq;
		*bWarn = FALSE;
	}

}

//#NT#2016/07/20#Brain Yen -begin
//#NT#Add for DDD ararm
void Movie_DDDProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	RECT trackRect;
	DDD_WARN dowsyState;
	UINT32 WarnEvent = 0;
	static UINT32 PreWarnEvent = DDD_UWRN_NORM;
	static UINT16 EventCnt = 0;
	if (DDD_GetTrackingRect(&trackRect))
	{

		dowsyState = DDD_GetUserWarning();

		DDD_SET_WRN_STATE(bYawnWarn, (dowsyState & (DDD_UWRN_YAWN | DDD_UWRN_YAWN_DURA)), 500, 50, 2.0);
		DDD_SET_WRN_STATE(bPerWarn, (dowsyState & (DDD_UWRN_BLINK | DDD_UWRN_EYE)), 150, 30, 1.2);
		DDD_SET_WRN_STATE(bNodeWarn, (dowsyState & (DDD_UWRN_NOD | DDD_UWRN_NOD_DURA)),  500, 50, 2.0);
		DDD_SET_WRN_STATE(bDisWarn, (dowsyState & (DDD_UWRN_DIS | DDD_UWRN_DIS_DURA)),  500, 50, 2.0);


		if (dowsyState)
		{
			if (dowsyState & DDD_UWRN_NOFACE)
			{
				EventCnt = 0;
				PreWarnEvent = DDD_UWRN_NOFACE;
			}
			else
			{
				if (bDisWarn)
				{
					DBG_IND("Serious Warning! Driver distraction!\r\n");
					WarnEvent = DDD_ALARM_DIS;
					bPerWarn = FALSE;
				}
				else if((dowsyState & DDD_UWRN_EYE_DURA))
				{
					DBG_IND("Serious Warning! Driver's eye closed too long!\r\n");
					WarnEvent = DDD_ALARM_EYE;
				}
				else if(bPerWarn)
				{
					DBG_IND("Warning! Driver drossy!!\r\n");
					WarnEvent = DDD_ALARM_PERCLOS;
					bPerWarn = FALSE;
				}
				else if(bYawnWarn)
				{
					DBG_IND("Remind~ driver yawn too frequently\r\n");
					WarnEvent = DDD_ALARM_YAWN;
					bYawnWarn = FALSE;
				}
				else if(bNodeWarn)
				{
					DBG_IND("Annotation! Driver is a little tired!\r\n");
					WarnEvent = DDD_ALARM_NODE;
					bNodeWarn = FALSE;
				}
				if (PreWarnEvent == WarnEvent)
					EventCnt++;
				else
					EventCnt = 0;
				if ((EventCnt % 20) == 0)
				{
					Ux_PostEvent(NVTEVT_CB_DDD_SHOWALARM, 1, WarnEvent);
				}
				PreWarnEvent = WarnEvent;
			}
		}
		else
		{
			EventCnt = 0;
			PreWarnEvent = DDD_UWRN_NORM;
		}
	}

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#if (MOVIE_DDD_SAVE_LOG == ENABLE)

	{
		UINT32 uiLogSize;
		UINT32 uiLogAddr;
		uiLogSize = DDD_GetLog(&uiLogAddr);
//        debug_msg("uiLogSize: %d\r\n", uiLogSize);
		if (uiLogSize)
		{
			mp4log_AddLog(DbgLogDDDID, (CHAR *)uiLogAddr, uiLogSize);
		}
	}
#endif
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
}
#undef DDD_SET_WRN_STATE
//#NT#2016/07/20#Brain Yen -end
//#NT#2016/05/11#Isiah Chang -begin
//#NT# Decrypt DDD database file and then feed it into DDD_Init().
UINT32 Movie_DDDCalcBufWithBinaryMap(VOID)
{
	UINT32 uiDBFileSize;
#if POWERON_WAIT_FS_READY
	uiDBFileSize = Movie_DDDGetDBFileSize();
	if (uiDBFileSize == 0)
		uiDBFileSize = Movie_DDDGetDBFileSizeFrCard();
	DBG_DUMP("^Yddd filesize:%d\r\n", uiDBFileSize);
	if (uiDBFileSize > DDD_MAX_DB_FILE_SIZE)
	{
		DBG_ERR("DDD DB file size is too large!\r\n");
		return 0;
	}
#else
	uiDBFileSize = DDD_MAX_DB_FILE_SIZE;
#endif
#if DDD_USING_FD_OR_NOT
	return NvtFD_GetBuffSize() + DDD_CalcBuffSize() + DDD_INPUTIMG_BUF_SIZE + uiDBFileSize + 4;
#else
	return DDD_CalcBuffSize() + uiDBFileSize + DDD_INPUTIMG_BUF_SIZE + 4;
#endif
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvddd = {
	{
		PHOTO_PRIORITY_SH,          ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_DDDProcess",
		Movie_DDDInit,          ///< init fp, only execute at first process
		Movie_DDDProcess,       ///< process fp
		Movie_DDDProcessEnd,    ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		Movie_DDDCalcBufWithBinaryMap,///< get working buffer fp
		DDD_CalcCacheBuffSize,  ///< get working cache buffer fp
	},
	NULL                    ///< point next Function Obj, last Function must be set NULL
};
//#NT#2016/05/11#Isiah Chang -end
#endif
//#NT#2016/04/22#Nestor Yang -end

//#NT#2016/05/27#David Tsai -begin
//#NT#Support tampering detection and background capture function
//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection & trip-zone detection function
#if MOVIE_BC_FUNC || MOVIE_TD_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC
//#NT#2016/10/14#Yuzhe Bian -end
void Movie_BCLock(BOOL bLock)
{
	gBCLock = bLock;
}

void Movie_BCProcessRateSet(UINT32 ProcessRate)
{
	gBCProcessRate = ProcessRate;
}

UINT32 Movie_BCProcessRateGet(void)
{
	return gBCProcessRate;
}

void Movie_BCInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	BC_Init(buf->Addr, cachebuf->Addr);
}

void Movie_BCProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF          InputImg;
	UINT32           ImageRatioIdx, myDualCam;
	ISF_UNIT        *pUnit;

	if (gBCLock == TRUE) {
		return;
	}

	gBCCounter++;
	if (gBCCounter >= gBCProcessRate) {
		gBCCounter = 0;
		gBCUpdateCounter++;
	} else {
		return;
	}

#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == (SENSOR_1)) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2)) {
		if (System_GetEnableSensor() == (SENSOR_1)) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		myDualCam = UI_GetData(FL_DUAL_CAM);
	}
#endif

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
	if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
		pUnit = ISF_IPL(0);
	} else {
		pUnit = ISF_IPL(1);
	}
	ImageUnit_PullData(ImageUnit_Out(pUnit, ISF_OUT2), (ISF_DATA *)&InputImg, 0);
	//GxImg_DumpBuf("A:\\test.raw",&InputImg,GX_IMAGE_DUMP_ALL);

	BC_Process(&InputImg);

}

PHOTO_FUNC_INFO PhotoFuncInfo_mvbc = {
	{
		PHOTO_PRIORITY_M,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_BCProcess",
		Movie_BCInit,           ///< init fp, only execute at first process
		Movie_BCProcess,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		BC_CalcBuffSize,        ///< get working buffer fp
		BC_CalcCacheBuffSize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif


#if MOVIE_TD_FUNC
void Movie_TDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	TDInit(buf->Addr, cachebuf->Addr);
}

void Movie_TDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF          InputImg;
	UINT32           ImageRatioIdx, myDualCam;
	ISF_UNIT        *pUnit;
	static UINT32    TDUpdateCounter = 0;
	if (gBCLock == TRUE) {
		return;
	}
	if (gBCUpdateCounter != TDUpdateCounter) {
		TDUpdateCounter = gBCUpdateCounter;
	} else {
		return;
	}

#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == (SENSOR_1)) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2)) {
		if (System_GetEnableSensor() == (SENSOR_1)) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		myDualCam = UI_GetData(FL_DUAL_CAM);
	}
#endif

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
	if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
		pUnit = ISF_IPL(0);
	} else {
		pUnit = ISF_IPL(1);
	}
	ImageUnit_PullData(ImageUnit_Out(pUnit, ISF_OUT2), (ISF_DATA *)&InputImg, 0);
	//GxImg_DumpBuf("A:\\test.raw",&InputImg,GX_IMAGE_DUMP_ALL);

	TDProcess(&InputImg);
	//#NT#2016/06/08#Lincy Lin -begin
	//#NT#Implement generic OSD and video drawing mechanism for ALG function
	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
	//#NT#2016/06/08#Lincy Lin -end

	//#NT#2016/10/27#Charlie Chang -begin
	//#NT#notify to ipc if td status change

	BOOL td_status = FALSE;
	static BOOL old_status = FALSE;
	td_status = TDGetAlarm();
	if (td_status != old_status) {
		if (td_status == TRUE) {
			NvtUctrl_Notify(TAMPER1_TRIGGER);
		} else {
			NvtUctrl_Notify(TAMPER1_NOTRIGGER);
		}
		old_status = td_status;
	}

	//#NT#2016/10/27#Charlie Chang -end
}

//#NT#2016/08/06#XQ Liu -begin
//#NT#Support saving log for TD
void Movie_TDProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#if (MOVIE_TD_SAVE_LOG == ENABLE)
	UINT32 logSize = 0;
	UINT32 logBuf = 0;
	logSize = TD_GetLog(&logBuf);
	if (logSize) {
		mp4log_AddLog(DbgLogTDID, (char *)logBuf, logSize);
	}
#endif
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
}
//#NT#2016/08/06#XQ Liu -end

PHOTO_FUNC_INFO PhotoFuncInfo_mvtd = {
	{
		PHOTO_PRIORITY_M,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_TDProcess",
		Movie_TDInit,           ///< init fp, only execute at first process
		Movie_TDProcess,        ///< process fp
		Movie_TDProcessEnd,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		TDCalcBuffSize,        ///< get working buffer fp
		TDCalcCacheBuffSize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/05/27#David Tsai -end

//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection function
#if MOVIE_TWD_FUNC
void Movie_TWDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	nvt_twd_init(buf->Addr, cachebuf->Addr);
}
void Movie_TWDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF          InputImg;
	UINT32           ImageRatioIdx, myDualCam;
	ISF_UNIT        *pUnit;
	static UINT32    TWDUpdateCounter = 0;
	if (gBCLock == TRUE) {
		return;
	}
	if (gBCUpdateCounter != TWDUpdateCounter) {
		TWDUpdateCounter = gBCUpdateCounter;
	} else {
		return;
	}
#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == (SENSOR_1)) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2)) {
		if (System_GetEnableSensor() == (SENSOR_1)) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		myDualCam = UI_GetData(FL_DUAL_CAM);
	}
#endif

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
	if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
		pUnit = ISF_IPL(0);
	} else {
		pUnit = ISF_IPL(1);
	}

	ImageUnit_PullData(ImageUnit_Out(pUnit, ISF_OUT2), (ISF_DATA *)&InputImg, 0);

	nvt_twd_process(&InputImg);

	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);

}
PHOTO_FUNC_INFO PhotoFuncInfo_mvtwd = {
	{
		PHOTO_PRIORITY_M,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_TWDProcess",
		Movie_TWDInit,           ///< init fp, only execute at first process
		Movie_TWDProcess,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		nvt_twd_calcbuffsize,        ///< get working buffer fp
		nvt_twd_calccachebuffsize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-zone detection function
#if MOVIE_TZD_FUNC
void Movie_TZDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	nvt_tzd_init(buf->Addr, cachebuf->Addr);
}
void Movie_TZDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF          InputImg;
	UINT32           ImageRatioIdx, myDualCam;
	ISF_UNIT        *pUnit;
	static UINT32    TZDUpdateCounter = 0;
	if (gBCLock == TRUE) {
		return;
	}
	if (gBCUpdateCounter != TZDUpdateCounter) {
		TZDUpdateCounter = gBCUpdateCounter;
	} else {
		return;
	}
#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == (SENSOR_1)) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2)) {
		if (System_GetEnableSensor() == (SENSOR_1)) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		myDualCam = UI_GetData(FL_DUAL_CAM);
	}
#endif

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
	if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
		pUnit = ISF_IPL(0);
	} else {
		pUnit = ISF_IPL(1);
	}

	ImageUnit_PullData(ImageUnit_Out(pUnit, ISF_OUT2), (ISF_DATA *)&InputImg, 0);

	nvt_tzd_process(&InputImg);

	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);

}
PHOTO_FUNC_INFO PhotoFuncInfo_mvtzd = {
	{
		PHOTO_PRIORITY_M,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_TZDProcess",
		Movie_TZDInit,           ///< init fp, only execute at first process
		Movie_TZDProcess,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		nvt_tzd_calcbuffsize,        ///< get working buffer fp
		nvt_tzd_calccachebuffsize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/10/26#Omega Yu -begin
//#NT# Support scene change detection function
#if MOVIE_SCD_FUNC
void Movie_SCDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	SCD_Init();
}

void Movie_SCDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	HEADER_RAW_DATA rawHeaderInfo = {0};
	DISALG_VECTOR disOffset = {0};
	SCD_PROC_PRMS prms = {0};

	if (IPL_HeaderRawGetCurInfo_Dly1VD(IPL_PATH_1, &rawHeaderInfo) != E_OK) {
		DBG_ERR("DIS: Get header error!!!\r\n");
	}

	dis_getFrameCorrVecPxl(&disOffset, rawHeaderInfo.frame_cnt);
	//DBG_DUMP("dis: %d %d %d %d\n", disOffset.vector.iX, disOffset.vector.iY, disOffset.frame_cnt, disOffset.score);

	if (1 == disOffset.score) {
		prms.iX = disOffset.vector.iX;
		prms.iY = disOffset.vector.iY;
		SCD_Process(&prms);
	}
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvscd = {
	{
		PHOTO_PRIORITY_M,             ///< function hook process Id.
		IPL_PATH_1,             ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Movie_SCDProcess",
		Movie_SCDInit,          ///< init fp, only execute at first process
		Movie_SCDProcess,       ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		NULL,                   ///< get working buffer fp
		NULL,                   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif // MOVIE_SCD_FUNC
//#NT#2016/10/26#Omega Yu -end

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#if 0//(MOVIE_FTG_FUNC == ENABLE) //OLDFTG

void Movie_FTGLock(BOOL bLock)
{
	gFTGLock = bLock;
	ftg_lock(bLock);
}

BOOL Movie_FTGGetLockSts(void)
{
	return gFTGLock;
}


void Movie_FTGInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	//URECT roi_rect = {300,300,400,300};
	//ftg_setfdroi(TRUE, &roi_rect);

	ftg_init(buf, cachebuf, 5, 15, 3);
}

void Movie_FTGProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF          InputImg;
	UINT32           ImageRatioIdx, myDualCam;
	ISF_UNIT        *pUnit;
	ISF_PORT        *pSrc;


	if (gFTGLock == TRUE) {
		return;
	}

#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == (SENSOR_1)) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2)) {
		if (System_GetEnableSensor() == (SENSOR_1)) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		myDualCam = UI_GetData(FL_DUAL_CAM);
	}
#endif

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
	if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
		pUnit = ISF_IPL(0);
	} else {
		pUnit = ISF_IPL(1);
	}

	pSrc = ImageUnit_Out(pUnit, ISF_OUT1);
	if (pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE)) {
		ImageUnit_PullData(ImageUnit_Out(ISF_IPL(0), ISF_OUT1), (ISF_DATA *)&InputImg, 0);
	} else {

		ImageUnit_PullData(ImageUnit_Out(ISF_IPL(0), ISF_OUT2), (ISF_DATA *)&InputImg, 0);
	}

#if 0
	static UINT32 ftg_num2 = 0;
	static UINT32 ftg_time2 = 0;
	static UINT32 ftg_max2 = 0;
	UINT32 t1 = Perf_GetCurrent();
	ftg_process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
	UINT32 t2 = Perf_GetCurrent();
	if (ftg_num2 < 1500 && ftg_num2 >= 1000) {
		if (ftg_max2 < t2 - t1) {
			ftg_max2 = t2 - t1;
		}
		ftg_time2 += t2 - t1;
		ftg_num2++;
	} else if (ftg_num2 >= 1000) {
		DBG_ERR("500 FTG uses %d us...max %d us\n\r", ftg_time2, ftg_max2);
	} else {
		ftg_num2++;
	}
#else
	ftg_process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
#endif

	Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvftg = {
	{
		PHOTO_PRIORITY_L,              ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,   ///< isr tigger event
		"Movie_FTGProcess",
		Movie_FTGInit,           ///< init fp, only execute at first process
		Movie_FTGProcess,        ///< process fp
		NULL,                    ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		ftg_calcbuffsize,        ///< get working buffer fp
		ftg_calccachebuffsize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/10/17#Bin Xiao -end
//#NT# Support Face Tracking Grading(FTG)

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support FD used by Face Tracking Grading(FTG)//OLDFTG
#if 0//MOVIE_FTG_FUNC
void Movie_FTGFDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ftg_config(ftg_fd_cfg_scale_eng, ftg_fd_scale_eng2);
	ftg_config(ftg_fd_cfg_sensitivity, 1);

	// Init FD buffer, set max face number to 10, and process rate as 3
	ftg_fd_init(buf, cachebuf, MAX_FDNUM, 15, 5);
}

void Movie_FTGFDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IMG_BUF          InputImg;
	UINT32           ImageRatioIdx, myDualCam;
	ISF_UNIT        *pUnit;
	ISF_PORT        *pSrc;

#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == (SENSOR_1)) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2)) {
		if (System_GetEnableSensor() == (SENSOR_1)) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		myDualCam = UI_GetData(FL_DUAL_CAM);
	}
#endif

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));

	if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
		pUnit = ISF_IPL(0);
	} else {
		pUnit = ISF_IPL(1);
	}

	pSrc = ImageUnit_Out(pUnit, ISF_OUT1);
	if (pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE)) {
		ImageUnit_PullData(ImageUnit_Out(ISF_IPL(0), ISF_OUT1), (ISF_DATA *)&InputImg, 0);
	} else {

		ImageUnit_PullData(ImageUnit_Out(ISF_IPL(0), ISF_OUT2), (ISF_DATA *)&InputImg, 0);
	}

#if 0
	static UINT32 ftg_num3 = 0;
	static UINT32 ftg_time3 = 0;
	static UINT32 ftg_max3 = 0;
	UINT32 t1 = Perf_GetCurrent();
	ftg_fd_process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
	UINT32 t2 = Perf_GetCurrent();
	if (ftg_num3 < 1500 && ftg_num3 >= 1000) {
		if (ftg_max3 < t2 - t1) {
			ftg_max3 = t2 - t1;
		}
		ftg_time3 += t2 - t1;
		ftg_num3++;
	} else if (ftg_num3 >= 1000) {
		DBG_ERR("500 FTG_FD uses %d us...max %d us\n\r", ftg_time3, ftg_max3);
	} else {
		ftg_num3++;
	}
#else
	ftg_fd_process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
#endif

	//Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvftgfd = {
	{
		PHOTO_PRIORITY_M,              ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,   ///< isr tigger event
		"Movie_FTGFDProcess",
		Movie_FTGFDInit,           ///< init fp, only execute at first process
		Movie_FTGFDProcess,        ///< process fp
		NULL,                    ///< process end fp
		NULL,                    ///< exit fp, only execute at photo close
		ftg_fd_calcbuffsize,        ///< get working buffer fp
		ftg_fd_calccachebuffsize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/10/17#Bin Xiao -end


//#NT#2017/08/29#Charlie Chang -begin
//#NT# sample for getting YUV and than send to DSP

void Movie_DSPFunc(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	////init


	return;
}

void Movie_DSPProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;

#if (DEBUG_DUMP_IMG == ENABLE)
	static UINT32    frame_cnt = 0;
	char             filename[50];
#endif

	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		//IMG_BUF 		*pISF_buf = NULL;
		//pISF_buf = (IMG_BUF *)&isf_data.Desc[0];
		//DBG_IND("send DSP YUV w=%d h=%d\r\n",pISF_buf->Width, pISF_buf->Height);

#if (DEBUG_DUMP_IMG == ENABLE)
		frame_cnt++;
		if (!(frame_cnt % 100)) {
			snprintf(filename, 50, "A:\\test%05d.yuv", frame_cnt);
			DBG_DUMP("Save yuv file %s\r\n", filename);
			GxImg_DumpBuf(filename, (IMG_BUF *)&isf_data.Desc[0], GX_IMAGE_DUMP_ALL);
		}
#endif
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvToDSPFunc = {
	{
		PHOTO_PRIORITY_M,              ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,   ///< isr tigger event
		"Movie_SendYUVToDSP",
		Movie_DSPFunc,           ///< init fp, only execute at first process
		Movie_DSPProcess,        ///< process fp
		NULL,                    ///< process end fp
		NULL,        ///< get working buffer fp
		NULL,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};


#define  USE_SW_MD_FUNC     ENABLE          /// Use SW MD to reduce memory usage

#if USE_SW_MD_FUNC
#include "md_api_660.h"
#else
#include "md_lib.h"
#endif

static UINT32 prev_proc_time, curr_proc_time;
static UINT32 proc_period;
static UINT32 md_result = FALSE;

#if !USE_SW_MD_FUNC
static MOVIE_MDParam g_MdParam = {0};

void init_user_param(MOVIE_MDParam *md_info)
{
	MD_USR_PARAM_INFO *default_param = &md_info->param_info;
	MD_USR_DISTRICT *dist_region = md_info->dist_region;
	unsigned int i;
	md_info->fps = 2;
	md_info->md_mode = 0;
	memset(default_param, 0, sizeof(MD_USR_PARAM_INFO));
	default_param->GLOBAL_INFO.u_height = 480;
	default_param->GLOBAL_INFO.u_width = 848;
	default_param->GLOBAL_INFO.tbg = 29490;
	default_param->GLOBAL_INFO.time_period = 10;
	default_param->GLOBAL_INFO.scene_change_th = 64;
	default_param->GLOBAL_INFO.alarm_th = 10;
	default_param->GLOBAL_INFO.event_postprocessing = 1;
	default_param->GLOBAL_INFO.u_mb_height = 16;
	default_param->GLOBAL_INFO.u_mb_width = 16;
	default_param->GLOBAL_INFO.mb_w_num = default_param->GLOBAL_INFO.u_width / default_param->GLOBAL_INFO.u_mb_width;
	default_param->GLOBAL_INFO.mb_h_num = default_param->GLOBAL_INFO.u_height / default_param->GLOBAL_INFO.u_mb_height;
	default_param->GLOBAL_INFO.object_size = 30;
	default_param->GLOBAL_INFO.single_people_size = 60;
	default_param->GLOBAL_INFO.single_w = 1;
	default_param->GLOBAL_INFO.single_h = 2;

	default_param->GLOBAL_INFO.sub_region_num = 4;
	default_param->GLOBAL_INFO.sub_region[0].is_enabled = 1;
	default_param->GLOBAL_INFO.sub_region[0].start_block_x = 0;
	default_param->GLOBAL_INFO.sub_region[0].start_block_y = 0;
	default_param->GLOBAL_INFO.sub_region[0].end_block_x = 3;
	default_param->GLOBAL_INFO.sub_region[0].end_block_y = 3;
	default_param->GLOBAL_INFO.sub_region[0].alarm_th = 10;
	default_param->GLOBAL_INFO.sub_region[0].alarm = 0;
	default_param->GLOBAL_INFO.sub_region[1].is_enabled = 0;
	default_param->GLOBAL_INFO.sub_region[1].start_block_x = 0;
	default_param->GLOBAL_INFO.sub_region[1].start_block_y = 0;
	default_param->GLOBAL_INFO.sub_region[1].end_block_x = 0;
	default_param->GLOBAL_INFO.sub_region[1].end_block_y = 0;
	default_param->GLOBAL_INFO.sub_region[1].alarm_th = 10;
	default_param->GLOBAL_INFO.sub_region[1].alarm = 0;
	default_param->GLOBAL_INFO.sub_region[2].is_enabled = 1;
	default_param->GLOBAL_INFO.sub_region[2].start_block_x = 4;
	default_param->GLOBAL_INFO.sub_region[2].start_block_y = 4;
	default_param->GLOBAL_INFO.sub_region[2].end_block_x = 7;
	default_param->GLOBAL_INFO.sub_region[2].end_block_y = 7;
	default_param->GLOBAL_INFO.sub_region[2].alarm_th = 10;
	default_param->GLOBAL_INFO.sub_region[2].alarm = 0;
	default_param->GLOBAL_INFO.sub_region[3].is_enabled = 0;
	default_param->GLOBAL_INFO.sub_region[3].start_block_x = 0;
	default_param->GLOBAL_INFO.sub_region[3].start_block_y = 0;
	default_param->GLOBAL_INFO.sub_region[3].end_block_x = 0;
	default_param->GLOBAL_INFO.sub_region[3].end_block_y = 0;
	default_param->GLOBAL_INFO.sub_region[3].alarm_th = 10;
	default_param->GLOBAL_INFO.sub_region[3].alarm = 0;

	default_param->LV_INFO[0].alpha = 32;
	default_param->LV_INFO[0].tb = 25;
	default_param->LV_INFO[0].sigma = 25;
	default_param->LV_INFO[0].init_val = 0;
	default_param->LV_INFO[0].tg = 9;
	default_param->LV_INFO[0].lum_diff_th = 0;
	default_param->LV_INFO[0].tex_diff_th = 0;
	default_param->LV_INFO[0].tex_th = 0;
	default_param->LV_INFO[0].tex_ratio_th = 127;
	default_param->LV_INFO[0].lvl_gm_md2_th = 10;
	default_param->LV_INFO[1].alpha = 32;
	default_param->LV_INFO[1].tb = 25;
	default_param->LV_INFO[1].sigma = 25;
	default_param->LV_INFO[1].init_val = 0;
	default_param->LV_INFO[1].tg = 9;
	default_param->LV_INFO[1].lum_diff_th = 150;
	default_param->LV_INFO[1].tex_diff_th = 100;
	default_param->LV_INFO[1].tex_th = 50;
	default_param->LV_INFO[1].tex_ratio_th = 100;
	default_param->LV_INFO[1].lvl_gm_md2_th = 10;
	default_param->LV_INFO[2].alpha = 32;
	default_param->LV_INFO[2].tb = 25;
	default_param->LV_INFO[2].sigma = 25;
	default_param->LV_INFO[2].init_val = 0;
	default_param->LV_INFO[2].tg = 9;
	default_param->LV_INFO[2].lum_diff_th = 150;
	default_param->LV_INFO[2].tex_diff_th = 100;
	default_param->LV_INFO[2].tex_th = 50;
	default_param->LV_INFO[2].tex_ratio_th = 100;
	default_param->LV_INFO[2].lvl_gm_md2_th = 10;
	default_param->LV_INFO[3].alpha = 32;
	default_param->LV_INFO[3].tb = 25;
	default_param->LV_INFO[3].sigma = 25;
	default_param->LV_INFO[3].init_val = 0;
	default_param->LV_INFO[3].tg = 9;
	default_param->LV_INFO[3].lum_diff_th = 150;
	default_param->LV_INFO[3].tex_diff_th = 100;
	default_param->LV_INFO[3].tex_th = 50;
	default_param->LV_INFO[3].tex_ratio_th = 100;
	default_param->LV_INFO[3].lvl_gm_md2_th = 10;

	md_info->dist_region_nu = 1;
	memset(dist_region, 0, 4 * sizeof(MD_USR_DISTRICT));
	for (i = 0; i < md_info->dist_region_nu; i++) {
		dist_region[i].x = 0;
		dist_region[i].y = 0;
		dist_region[i].width = 848;
		dist_region[i].height = 480;
		dist_region[i].level = 0;
	}
}
#endif

UINT32 Movie_md_GetResult(void)
{
	return md_result;
}

UINT32 Movie_md_CalBufSize(void)
{
#if USE_SW_MD_FUNC
	return 0;
#else
	return mdl_calc_buf_size();
#endif
}

UINT32 Movie_md_CalCacheBufSize(void)
{
	return 0;
}

void Movie_md_init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
#if USE_SW_MD_FUNC
    UINT8 md_wnd[32][32];

	prev_proc_time = 0;
	curr_proc_time = 0;
	proc_period = 1000000 / 10;      /// 10 fps to reduce converge time
	MD_SetLevel(0, 12);
	memset(&md_wnd[0][0], 1, 32*32);
	MD_SetDetWin(0, &md_wnd[0][0]);
#else
	if (buf->Addr) {
		DBG_DUMP("--Do mdl_init\r\n");
		mdl_init(buf->Addr);
		init_user_param(&g_MdParam);
		mdl_chg_param((MD_USR_PARAM_INFO *)&(g_MdParam.param_info));
		//mdl_set_user_lv_info((MD_USR_DISTRICT *)&g_MdParam.dist_region, g_MdParam.dist_region_nu);
	} else {
		DBG_ERR("Get md buf fail\n", buf->Addr);
	}

	prev_proc_time = 0;
	curr_proc_time = 0;
	proc_period = (g_MdParam.fps) ? (1000000 / g_MdParam.fps) : 0;
#endif
}

void Movie_md_process(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
#if USE_SW_MD_FUNC
	ISF_DATA isf_data;
	ISF_UNIT *pUnit;
	ISF_PORT *pSrc;
	IMG_BUF *pBuf = NULL;

	if (UI_GetData(FL_MOVIE_MOTION_DET)) {
		curr_proc_time = clock();
		if ((curr_proc_time - prev_proc_time) >= proc_period) {
			pUnit = &ISF_UserProc;
			pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
			if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
				pBuf = (IMG_BUF *)&isf_data.Desc[0];
				md_result = MD_Process(0, pBuf);
				prev_proc_time = curr_proc_time;
				ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
			}
		}
	}
#else
	ISF_DATA isf_data;
	ISF_UNIT *pUnit;
	ISF_PORT *pSrc;
	IMG_BUF *pBuf = NULL;
	HEADER_YUV_MD_INFO *md_info = NULL;

	if (UI_GetData(FL_MOVIE_MOTION_DET) && g_MdParam.fps) {
		curr_proc_time = clock();
		if ((curr_proc_time - prev_proc_time) >= proc_period) {
			pUnit = &ISF_UserProc;
			pSrc = ImageUnit_Out(pUnit, ImageApp_MovieMulti_GetAlgDataPort(_CFG_REC_ID_1, _CFG_ALG_PATH3));
			if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
				pBuf = (IMG_BUF *)&isf_data.Desc[0];
				md_info = (HEADER_YUV_MD_INFO *)(pBuf->Metadata.Addr);
				if (md_info) {
					mdl_set_win_info(md_info);
					mdl_set_evt_addr(md_info->evt_addr);
					mdl_process(0);
					md_result = mdl_get_global_alarm();
					prev_proc_time = curr_proc_time;
				}
				ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
			}
		}
	}
#endif
}

void Movie_md_uninit(void)
{
#if !USE_SW_MD_FUNC
	mdl_uninit();
#endif
}

PHOTO_FUNC_INFO PhotoFuncInfo_movie_motion = {
	{
		PHOTO_PRIORITY_M,             ///< function hook process Id.
		IPL_PATH_1,                   ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,        ///< isr tigger event
		"MOVIE_MD",
		Movie_md_init,                /// < init fp, only execute at first process
		Movie_md_process,             ///< process fp
		NULL,                         ///< process end fp
		Movie_md_uninit,              ///< exit fp, only execute at photo close
		Movie_md_CalBufSize,          ///< get working buffer fp
		Movie_md_CalCacheBufSize,     ///< get working cache buffer fp
	},
	NULL                              ///< point next Function Obj, last Function must be set NULL
};

#if (MOVIE_STRM_PATH_USERPROC == ENABLE)

void Movie_StrmInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	////init
	return;
}


void Movie_StrmProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ISF_OUT13);

	if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		//IMG_BUF 		*pISF_buf = NULL;
		//pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do waht you want~~~

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
	}
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvToStrmFunc = {
	{
		PHOTO_PRIORITY_M,              ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,   ///< isr tigger event
		"Movie_SendYUVToStrm",
		Movie_StrmInit,           ///< init fp, only execute at first process
		Movie_StrmProcess,        ///< process fp
		NULL,                    ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		NULL,        ///< get working buffer fp
		NULL,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif


void alg_Movie_InitPhotoFunc(PHOTO_FUNC_INFO *phead)
{
#if (_ADAS_FUNC_ == ENABLE)
	//#NT#2016/03/25#KCHong -begin
	//#NT#New ADAS
	// --------------------------------------------------------------------------------------------------
	//    *** ADAS parameter setting ***
	//    User can set ADAS parameters (FSize/LD sensitivity/FC warning distance/ LD debounce time/FC debounce time)
	//    If not set, ADAS lib will use default settings.
	//    Important notice:
	//        ADAS_SetLdFcFsizeValueByPixel(), ADAS_SetLdwsLeftSensitivity(), ADAS_SetLdwsRightSensitivity() MUST be set before ADAS_Init()
	//        Only ADAS_SetFcwsUIWarningDist() / ADAS_SetFcwsDebounceTime / ADAS_SetLdwsDebounceTime can be changed during ADAS detection.
	//        Ex. Different car speed has different FC warning distance.
	//
	//        FSize          : Please refer to APNote to measure FSize
	//        LD Sensitivity : 0~50, default is 15. Bigger value means more sentive.
	//        FC distance    : 1~40, default is 15. The unit is meter.
	//   --------------------------------------------------------------------------------------------------
	// Set fsize value. This value CANNOT be changed on-the-fly.
	ADAS_SetLdFcFsizeValueByPixel(188);

	// Set fcws warning distance (unit: m). This value can be changed on-the-fly.
	ADAS_SetFcwsUIWarningDist(15);
	// Set fcws debounce time (unit: ms). This value can be changed on-the-fly.
	ADAS_SetFcwsDebounceTime(10 * 1000);
	// Set fcws output format
	ADAS_SetFcwsCarRsltFmt(ADAS_FCWS_RSLT_LANE_WIDTH);

	// Set ldws left side sensitivity. This value CANNOT be changed on-the-fly.
	ADAS_SetLdwsLeftSensitivity(15);
	// Set ldws right side sensitivity. This value CANNOT be changed on-the-fly.
	ADAS_SetLdwsRightSensitivity(15);
	// Set ldws debounce time (Unit: ms). This value can be changed on-the-fly.
	ADAS_SetLdwsDebounceTime(10 * 1000);
	// Set fc/ld IPL source. This value CANNOT be changed on-the-fly.
	ADAS_selMidPrioAppsDataSrc(IPL_PATH_1);
	// Set autoVP IPL source. This value CANNOT be changed on-the-fly.
	ADAS_selLowPrioAppsDataSrc(IPL_PATH_1);

	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_adas;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;   // find last func
		}
		pcurr->pNext = pfunc;       // append this func
		pfunc->pNext = NULL;        // mark end
	}

	#if 0
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_SnG;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
	#endif

#if (_AUTOVP_FUNC_ == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_autoVP;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;   //find last func
		}
		pcurr->pNext = pfunc;       //append this func
		pfunc->pNext = NULL;        //mark end
	}
#endif  // #if (_AUTOVP_FUNC_ == ENABLE)
#endif  // #if (_ADAS_FUNC_ == ENABLE)

#if MOVIE_TSR_FUNC_
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvtsr;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvtsr.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvtsr.Info.IPLId = IPL_PATH_2;
		}

		DBG_IND("myDualCam = %d, TSR IPLID = %d\r\n", myDualCam, PhotoFuncInfo_mvtsr.Info.IPLId);

		TSR_Lock(FALSE);
	}
#endif

#if MOVIE_TSR_FUNC_
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvtsrdsp;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvtsrdsp.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvtsrdsp.Info.IPLId = IPL_PATH_2;
		}

		DBG_IND("myDualCam = %d, TSR IPLID = %d\r\n", myDualCam, PhotoFuncInfo_mvtsrdsp.Info.IPLId);
	}
#endif

	//#NT#2017/04/28#KCHong -begin
	//#NT# CNN Object detection
#if (_OD_FUNC_ == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvod;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    /// find last func
		}
		pcurr->pNext = pfunc;                                           /// append this func
		pfunc->pNext = NULL;                                            /// mark end
		PhotoFuncInfo_mvod.Info.IPLId = IPL_PATH_1;
	}
#endif
	//#NT#2017/04/28#KCHong -end

#if _CVHOST_EX_
	{
		PHOTO_FUNC_INFO *pfunc = &MovieFuncInfo_CvHostEx;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

#if _NVTCV_EX_
	{
		PHOTO_FUNC_INFO *pfunc = &MovieFuncInfo_NvtCvEx;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

#if MOVIE_DIS
	{
		// DIS function

		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_dis;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif

#if MOVIE_DSP_FUNC
	{
		// Send YUV to DSP function
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvToDSPFunc;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}

#endif


#if MOVIE_FD_FUNC_
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvfd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvfd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvfd.Info.IPLId = IPL_PATH_2;
		}

		DBG_IND("myDualCam = %d, FD IPLID = %d\r\n", myDualCam, PhotoFuncInfo_mvfd.Info.IPLId);
	}
#endif

	//Support object tracking function
#if MOVIE_OT_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvot;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvot.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvot.Info.IPLId = IPL_PATH_2;
		}
	}
#endif
	//Support Dis dsp function

#if MOVIE_DISDSP_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvdisdsp;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvdisdsp.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvdisdsp.Info.IPLId = IPL_PATH_2;
		}
	}
#endif

	//#NT#2016/05/20#Yang Jin -end


	//Support DDD's Face Detection(DDDFD)
#if MOVIE_DDD_FD_FUNC
	{
		UINT32      myDualCam;

		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvdddfd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvdddfd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvdddfd.Info.IPLId = IPL_PATH_2;
		}

	}
#endif
	//#NT#2016/11/15#ML Cui -end

	//Support Driver Drowsiness Detection(DDD)
#if MOVIE_DDD_FUNC
	{
		UINT32      myDualCam;

		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvddd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvddd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvddd.Info.IPLId = IPL_PATH_2;
		}
	}

	DDD_Lock(FALSE);
#endif

#if MOVIE_BC_FUNC || MOVIE_TD_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvbc;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvbc.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvbc.Info.IPLId = IPL_PATH_2;
		}
	}
#endif

#if MOVIE_TD_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvtd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvtd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvtd.Info.IPLId = IPL_PATH_2;
		}
	}
#endif

	// Support trip-wire detection & trip-zone detection function
#if MOVIE_TWD_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvtwd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvtwd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvtwd.Info.IPLId = IPL_PATH_2;
		}
	}
#endif

#if MOVIE_TZD_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvtzd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvtzd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvtzd.Info.IPLId = IPL_PATH_2;
		}
	}
#endif
	//#NT#2016/10/14#Yuzhe Bian -end

	//#NT#2016/10/27#Omega Yu -begin
	//#NT# Support scene change detection function
#if MOVIE_SCD_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvscd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvscd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvscd.Info.IPLId = IPL_PATH_2;
		}
	}
#endif
	//#NT#2016/10/27#Omega Yu -end

	//#NT#2016/10/17#Bin Xiao -begin
	//#NT# Support Face Tracking Grading(FTG)
#if 0//MOVIE_FTG_FUNC//OLDFTG
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvftg;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvftg.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvftg.Info.IPLId = IPL_PATH_2;
		}

		DBG_IND("myDualCam = %d, FTG IPLID = %d\r\n", myDualCam, PhotoFuncInfo_mvftg.Info.IPLId);
	}
#endif

#if 0//MOVIE_FTG_FUNC
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvftgfd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvftgfd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvftgfd.Info.IPLId = IPL_PATH_2;
		}

		DBG_IND("myDualCam = %d, FTGFD IPLID = %d\r\n", myDualCam, PhotoFuncInfo_mvftgfd.Info.IPLId);
	}
#endif

	{
		// MD function
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_movie_motion;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
}


