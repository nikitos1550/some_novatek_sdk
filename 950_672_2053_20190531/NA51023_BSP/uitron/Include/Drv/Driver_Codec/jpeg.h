/*
    Header file for JPEG module

    This file is the header file that define the API for JPEG module.

    @file       jpeg.h
    @ingroup    mIDrvCodec_JPEG
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _JPEG_H
#define _JPEG_H

#include "Type.h"

/**
    @addtogroup mIDrvCodec_JPEG
*/
//@{

/*
    JPEG block format

    @note For JPG_HW_YUV_FORMAT
*/
typedef enum {
	JPEG_BLK_Y_2X    = 0x0800,      ///< Y's block width is 16
	JPEG_BLK_Y_1X    = 0x0400,      ///< Y's block width is 8
	JPEG_BLK_Y_2Y    = 0x0200,      ///< Y's block height is 16
	JPEG_BLK_Y_1Y    = 0x0100,      ///< Y's block height is 8
	JPEG_BLK_U_2X    = 0x0080,      ///< U's block width is 16
	JPEG_BLK_U_1X    = 0x0040,      ///< U's block width is 8
	JPEG_BLK_U_2Y    = 0x0020,      ///< U's block height is 16
	JPEG_BLK_U_1Y    = 0x0010,      ///< U's block height is 8
	JPEG_BLK_V_2X    = 0x0008,      ///< V's block width is 16
	JPEG_BLK_V_1X    = 0x0004,      ///< V's block width is 8
	JPEG_BLK_V_2Y    = 0x0002,      ///< V's block height is 16
	JPEG_BLK_V_1Y    = 0x0001,      ///< V's block height is 8
	ENUM_DUMMY4WORD(JPEG_BLK_FORMAT)
} JPEG_BLK_FORMAT;

/**
    JPEG YUV format

    @note For jpeg_setFileFormat()
*/
typedef enum {
	JPEG_YUV_FORMAT_100     = (JPEG_BLK_Y_1X | JPEG_BLK_Y_1Y),                                                                  ///< YUV 100 (Y plane only)
	JPEG_YUV_FORMAT_111     = (JPEG_BLK_Y_1X | JPEG_BLK_Y_1Y | JPEG_BLK_U_1X | JPEG_BLK_U_1Y | JPEG_BLK_V_1X | JPEG_BLK_V_1Y),  ///< YUV 111
	JPEG_YUV_FORMAT_211     = (JPEG_BLK_Y_2X | JPEG_BLK_Y_1Y | JPEG_BLK_U_1X | JPEG_BLK_U_1Y | JPEG_BLK_V_1X | JPEG_BLK_V_1Y),  ///< YUV 2h11 (4:2:2 in Exif)
	JPEG_YUV_FORMAT_211V    = (JPEG_BLK_Y_1X | JPEG_BLK_Y_2Y | JPEG_BLK_U_1X | JPEG_BLK_U_1Y | JPEG_BLK_V_1X | JPEG_BLK_V_1Y),  ///< YUV 2v11
	JPEG_YUV_FORMAT_222     = (JPEG_BLK_Y_2X | JPEG_BLK_Y_1Y | JPEG_BLK_U_2X | JPEG_BLK_U_1Y | JPEG_BLK_V_2X | JPEG_BLK_V_1Y),  ///< YUV 222h
	JPEG_YUV_FORMAT_222V    = (JPEG_BLK_Y_1X | JPEG_BLK_Y_2Y | JPEG_BLK_U_1X | JPEG_BLK_U_2Y | JPEG_BLK_V_1X | JPEG_BLK_V_2Y),  ///< YUV 111v
	JPEG_YUV_FORMAT_411     = (JPEG_BLK_Y_2X | JPEG_BLK_Y_2Y | JPEG_BLK_U_1X | JPEG_BLK_U_1Y | JPEG_BLK_V_1X | JPEG_BLK_V_1Y),  ///< YUV 411  (4:2:0 in Exif)
	JPEG_YUV_FORMAT_422     = (JPEG_BLK_Y_2X | JPEG_BLK_Y_2Y | JPEG_BLK_U_2X | JPEG_BLK_U_1Y | JPEG_BLK_V_2X | JPEG_BLK_V_1Y),  ///< YUV 422h
	JPEG_YUV_FORMAT_422V    = (JPEG_BLK_Y_2X | JPEG_BLK_Y_2Y | JPEG_BLK_U_1X | JPEG_BLK_U_2Y | JPEG_BLK_V_1X | JPEG_BLK_V_2Y),  ///< YUV 422v
	ENUM_DUMMY4WORD(JPEG_YUV_FORMAT)
} JPEG_YUV_FORMAT;

/**
    JPEG interrupt status

    @note For jpeg_getStatus(), jpeg_setEnableInt(), jpeg_setDisableInt() and jpeg_waitDone().
*/
typedef enum {
	JPEG_INT_FRAMEEND   = 0x01,         ///< Frame end. Encoding/decoding a frame is done.
	///< @note  When bit-stream buffer end and frame end are both happened,
	///<        only bit-stream buffer end interrupt will be issued.
	///<        You have to set the bit-stream buffer again to issue the frame end interrupt.
	JPEG_INT_SLICEDONE  = 0x02,         ///< Slice Done. (Not support in NT96650)
	JPEG_INT_DECERR     = 0x04,         ///< Decode error. Encounter error in JPEG decode.
	JPEG_INT_BUFEND     = 0x08,         ///< Bit-stream buffer end. Bit-stream length exceed specified length.
	JPEG_INT_ALL        = 0x0F,         ///< All interrupts of above

	ENUM_DUMMY4WORD(JPEG_INT)
} JPEG_INT;

/**
    JPEG Decode Scale Ratio

    @note For jpeg_setScaleRatio().
*/
typedef enum {
	JPEG_DECODE_RATIO_WIDTH_1_2,        ///< 1/2 of Width Only (Not supported in NT96650)
	JPEG_DECODE_RATIO_BOTH_1_2,         ///< 1/2 of Width/Height
	JPEG_DECODE_RATIO_BOTH_1_4,         ///< 1/4 of Width/Height
	JPEG_DECODE_RATIO_BOTH_1_8,         ///< 1/8 of Width/Height

	ENUM_DUMMY4WORD(JPEG_DECODE_RATIO)
} JPEG_DECODE_RATIO;

/**
    JPEG DC Output Scaling ratio

    JPEG DC Output scaling ratio.
*/
typedef enum {
	JPEG_DC_OUT_RATIO_1_2 = 1,          ///< DC output scaled to 1/2
	JPEG_DC_OUT_RATIO_1_4,              ///< DC output scaled to 1/4
	JPEG_DC_OUT_RATIO_1_8,              ///< DC output scaled to 1/8

	ENUM_DUMMY4WORD(JPEG_DC_OUT_RATIO)
} JPEG_DC_OUT_RATIO;

/**
    JPEG DC Output Configuration structure

    @note For jpeg_setDCOut().
*/
typedef struct {
	UINT32              DCEnable;       ///< DC output enable
	///< - @b TRUE  : Enable DC output function
	///< - @b FALSE : Disable DC output function
	JPEG_DC_OUT_RATIO   DCXRatio;       ///< DC output width (horizontal) ratio
	JPEG_DC_OUT_RATIO   DCYRatio;       ///< DC output height (vertical) ratio
	UINT32              DCYAddr;        ///< DC output Y DRAM address
	UINT32              DCUAddr;        ///< DC output UV packed DRAM address
	UINT32              DCVAddr;        ///< Obsoleted in NT96650
	UINT32              DCYLineOffset;  ///< DC output Y line offset
	UINT32              DCULineOffset;  ///< DC output UV packed line offset
	UINT32              DCVLineOffset;  ///< Obsoleted in NT96650
} JPEG_DC_OUT_CFG, *PJPEG_DC_OUT_CFG;

/**
    JPEG Bit Rate Control Information

    @note For jpeg_getBRCInfo().
*/
typedef struct {
	UINT32 uiBRCInfo1;                  ///< Rho Q/8
	UINT32 uiBRCInfo2;                  ///< Rho Q/4
	UINT32 uiBRCInfo3;                  ///< Rho Q/2
	UINT32 uiBRCInfo4;                  ///< Rho Q
	UINT32 uiBRCInfo5;                  ///< Rho 2Q
	UINT32 uiBRCInfo6;                  ///< Rho 4Q
	UINT32 uiBRCInfo7;                  ///< Rho 8Q
} JPEG_BRC_INFO, *PJPEG_BRC_INFO;

/**
    JPEG configuration identifier

    @note For jpeg_setConfig()
*/
typedef enum {
	JPEG_CONFIG_ID_CHECK_DEC_ERR,           ///< Enable / Disable checking decode error
	///< (Default is DISABLED)
	///< Context can be:
	///< - @b TRUE  : JPEG driver will check decode error
	///< - @b FALSE : JPEG driver will NOT check decode error

	JPEG_CONFIG_ID_FREQ,                    ///< JPEG module clock (Unit: MHz), will be active on succeeding jpeg_open().
	///< Context can be one of the following:
	///< - @b 80    : 80 MHz (Default value)
	///< - @b 120   : 120 MHz
	///< - @b 160   : 160 MHz
	///< - @b 192   : 192 MHz

	ENUM_DUMMY4WORD(JPEG_CONFIG_ID)
} JPEG_CONFIG_ID;


/**
    JPEG configuration identifier

    @note For jpeg_setConfig()
*/
typedef enum {
	JPEG_ID_0,                              ///< JPEG controller ID 0

	JPEG_ID_1,                              ///< JPEG controller ID 1

	ENUM_DUMMY4WORD(JPEG_ID)
} JPEG_ID;

// -------------------------------------------------
// The API for the JPEG codec driver
// -------------------------------------------------
extern ER       jpeg_open(void);
extern ER       jpeg_close(void);
extern BOOL     jpeg_isOpened(void);
extern ID       jpeg_getLockStatus(void);

extern ER       jpeg_setConfig(JPEG_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32   jpeg_getConfig(JPEG_CONFIG_ID ConfigID);

extern UINT32   jpeg_getStatus(void);
extern UINT32   jpeg_getActiveStatus(void);
extern UINT32   jpeg_getComSize(void);
extern void     jpeg_setSWReset(void);

extern void     jpeg_setStartEncode(void);
extern void     jpeg_setStartDecode(void);

extern void     jpeg_setEndEncode(void);
extern void     jpeg_setEndDecode(void);

extern UINT32   jpeg_waitDone(void);
extern BOOL     jpeg_waitDonePolling(void);

extern ER       jpeg_setEnableInt(UINT32 uiInt);
extern void     jpeg_setDisableInt(UINT32 uiInt);

extern void     jpeg_setImgStartAddr(UINT32 ImgYAddr, UINT32 ImgUAddr, UINT32 ImgVAddr);
extern void     jpeg_getImgStartAddr(UINT32 *ImgYAddr, UINT32 *ImgUAddr, UINT32 *ImgVAddr);

extern ER       jpeg_setImgLineOffset(UINT32 YLOFS, UINT32 ULOFS, UINT32 VLOFS);
extern UINT32   jpeg_getImgLineOffsetY(void);
extern UINT32   jpeg_getImgLineOffsetU(void);
extern UINT32   jpeg_getImgLineOffsetV(void);

extern ER       jpeg_setFileFormat(UINT32 ImgWidth, UINT32 ImgHeight, JPEG_YUV_FORMAT FileFormat);

// Bit-stream control
extern ER       jpeg_setCmpStartAddr(UINT32 BSAddr, UINT32 BufSize);
extern UINT32   jpeg_getBSStartAddr(void);
extern UINT32   jpeg_getBSCurAddr(void);
extern void     jpeg_setBSOutput(BOOL Enable);

// Restart marker
extern ER       jpeg_setRestartInterval(UINT32 MCUNum);
extern ER       jpeg_setRestartEnable(BOOL Enable);
extern UINT32   jpeg_setRestartEOF(void);

// Encode format transform (420 -> 422)
extern void     jpeg_setFmtTranEnable(void);
extern void     jpeg_setFmtTranDisable(void);

// Encode DC output
extern ER       jpeg_setDCOut(PJPEG_DC_OUT_CFG pDCOutCfg);

// Decode crop
extern ER       jpeg_setCrop(UINT32 StartX, UINT32 StartY, UINT32 Width, UINT32 Height);
extern void     jpeg_setCropDisable(void);
extern void     jpeg_setCropEnable(void);

// Decode scaling
extern void     jpeg_setScaleEnable(void);
extern void     jpeg_setScaleDisable(void);
extern ER       jpeg_setScaleRatio(JPEG_DECODE_RATIO ScaleRatio);

// Table access
extern ER       jpeg_setQTabHW(UINT8 *pucQTabY, UINT8 *pucQTabUV);
extern void     jpeg_setEncodeHuffTabHW(UINT16 *puiHuffTabLAC, UINT16 *puiHuffTabLDC, UINT16 *puiHuffTabCAC, UINT16 *puiHuffTabCDC);
extern void     jpeg_setDecodeHuffTabHW(UINT8 *pHuffDC0th, UINT8 *pHuffDC1th, UINT8 *pHuffAC0th, UINT8 *pHuffAC1th);

// Profiling
extern UINT32   jpeg_getCycleCnt(void);

// BRC control
extern void     jpeg_getBRCInfo(PJPEG_BRC_INFO pBRCInfo);

//@}

// -------------------------------------------------
// The API for the JPEG codec driver
// -------------------------------------------------
extern ER       jpeg_composite_open(JPEG_ID id);
extern ER       jpeg_composite_close(JPEG_ID id);
extern BOOL     jpeg_composite_isOpened(JPEG_ID id);
extern ID       jpeg_composite_getLockStatus(JPEG_ID id);

extern ER       jpeg_composite_setConfig(JPEG_ID id, JPEG_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32   jpeg_composite_getConfig(JPEG_ID id, JPEG_CONFIG_ID ConfigID);

extern UINT32   jpeg_composite_getStatus(JPEG_ID id);
extern UINT32   jpeg_composite_getActiveStatus(JPEG_ID id);
extern UINT32   jpeg_composite_getComSize(JPEG_ID id);
extern void     jpeg_composite_setSWReset(JPEG_ID id);

extern void     jpeg_composite_setStartEncode(JPEG_ID id);
extern void     jpeg_composite_setStartDecode(JPEG_ID id);

extern void     jpeg_composite_setEndEncode(JPEG_ID id);
extern void     jpeg_composite_setEndDecode(JPEG_ID id);

extern UINT32   jpeg_composite_waitDone(JPEG_ID id);
extern BOOL     jpeg_composite_waitDonePolling(JPEG_ID id);

extern ER       jpeg_composite_setEnableInt(JPEG_ID id, UINT32 uiInt);
extern void     jpeg_composite_setDisableInt(JPEG_ID id, UINT32 uiInt);

extern void     jpeg_composite_setImgStartAddr(JPEG_ID id, UINT32 ImgYAddr, UINT32 ImgUAddr, UINT32 ImgVAddr);
extern void     jpeg_composite_getImgStartAddr(JPEG_ID id, UINT32 *ImgYAddr, UINT32 *ImgUAddr, UINT32 *ImgVAddr);

extern ER       jpeg_composite_setImgLineOffset(JPEG_ID id, UINT32 YLOFS, UINT32 ULOFS, UINT32 VLOFS);
extern UINT32   jpeg_composite_getImgLineOffsetY(JPEG_ID id);
extern UINT32   jpeg_composite_getImgLineOffsetU(JPEG_ID id);
extern UINT32   jpeg_composite_getImgLineOffsetV(JPEG_ID id);

extern ER       jpeg_composite_setFileFormat(JPEG_ID id, UINT32 ImgWidth, UINT32 ImgHeight, JPEG_YUV_FORMAT FileFormat);

// Bit-stream control
extern ER       jpeg_composite_setCmpStartAddr(JPEG_ID id, UINT32 BSAddr, UINT32 BufSize);
extern UINT32   jpeg_composite_getBSStartAddr(JPEG_ID id);
extern UINT32   jpeg_composite_getBSCurAddr(JPEG_ID id);
extern void     jpeg_composite_setBSOutput(JPEG_ID id, BOOL Enable);

// Restart marker
extern ER       jpeg_composite_setRestartInterval(JPEG_ID id, UINT32 MCUNum);
extern ER       jpeg_composite_setRestartEnable(JPEG_ID id, BOOL Enable);
extern UINT32   jpeg_composite_setRestartEOF(JPEG_ID id);

// Encode format transform (420 -> 422)
extern void     jpeg_composite_setFmtTranEnable(JPEG_ID id);
extern void     jpeg_composite_setFmtTranDisable(JPEG_ID id);

// Encode DC output
extern ER       jpeg_composite_setDCOut(JPEG_ID id, PJPEG_DC_OUT_CFG pDCOutCfg);

// Decode crop
extern ER       jpeg_composite_setCrop(JPEG_ID id, UINT32 StartX, UINT32 StartY, UINT32 Width, UINT32 Height);
extern void     jpeg_composite_setCropDisable(JPEG_ID id);
extern void     jpeg_composite_setCropEnable(JPEG_ID id);

// Decode scaling
extern void     jpeg_composite_setScaleEnable(JPEG_ID id);
extern void     jpeg_composite_setScaleDisable(JPEG_ID id);
extern ER       jpeg_composite_setScaleRatio(JPEG_ID id, JPEG_DECODE_RATIO ScaleRatio);

// Table access
extern ER       jpeg_composite_setQTabHW(JPEG_ID id, UINT8 *pucQTabY, UINT8 *pucQTabUV);
extern void     jpeg_composite_setEncodeHuffTabHW(JPEG_ID id, UINT16 *puiHuffTabLAC, UINT16 *puiHuffTabLDC, UINT16 *puiHuffTabCAC, UINT16 *puiHuffTabCDC);
extern void     jpeg_composite_setDecodeHuffTabHW(JPEG_ID id, UINT8 *pHuffDC0th, UINT8 *pHuffDC1th, UINT8 *pHuffAC0th, UINT8 *pHuffAC1th);

// Profiling
extern UINT32   jpeg_composite_getCycleCnt(JPEG_ID id);

// BRC control
extern void     jpeg_composite_getBRCInfo(JPEG_ID id, PJPEG_BRC_INFO pBRCInfo);

//--------------------------------------------------------------------------
// Backward Compatible
//--------------------------------------------------------------------------
#define jpeg_open()                               jpeg_composite_open(JPEG_ID_0)
#define jpeg_close()                              jpeg_composite_close(JPEG_ID_0)
#define jpeg_isOpened()                           jpeg_composite_isOpened(JPEG_ID_0)
#define jpeg_getLockStatus()                      jpeg_composite_getLockStatus(JPEG_ID_0)
#define jpeg_setConfig(ConfigID, uiConfig)        jpeg_composite_setConfig(JPEG_ID_0, ConfigID, uiConfig)
#define jpeg_getConfig(ConfigID)                  jpeg_composite_getConfig(JPEG_ID_0, ConfigID)
#define jpeg_getStatus()                          jpeg_composite_getStatus(JPEG_ID_0)
#define jpeg_getActiveStatus()                    jpeg_composite_getActiveStatus(JPEG_ID_0)
#define jpeg_getComSize()                         jpeg_composite_getComSize(JPEG_ID_0)
#define jpeg_setSWReset()                         jpeg_composite_setSWReset(JPEG_ID_0)
#define jpeg_setStartEncode()                     jpeg_composite_setStartEncode(JPEG_ID_0)
#define jpeg_setStartDecode()                     jpeg_composite_setStartDecode(JPEG_ID_0)
#define jpeg_setEndEncode()                       jpeg_composite_setEndEncode(JPEG_ID_0)
#define jpeg_setEndDecode()                       jpeg_composite_setEndDecode(JPEG_ID_0)
#define jpeg_waitDone()                           jpeg_composite_waitDone(JPEG_ID_0)
#define jpeg_waitDonePolling()                    jpeg_composite_waitDonePolling(JPEG_ID_0)
#define jpeg_setEnableInt(uiInt)                  jpeg_composite_setEnableInt(JPEG_ID_0, uiInt)
#define jpeg_setDisableInt(uiInt)                 jpeg_composite_setDisableInt(JPEG_ID_0, uiInt)
#define jpeg_setImgStartAddr(ImgYAddr, ImgUAddr, ImgVAddr) jpeg_composite_setImgStartAddr(JPEG_ID_0, ImgYAddr, ImgUAddr, ImgVAddr)
#define jpeg_getImgStartAddr(ImgYAddr, ImgUAddr, ImgVAddr) jpeg_composite_getImgStartAddr(JPEG_ID_0, ImgYAddr, ImgUAddr, ImgVAddr)
#define jpeg_setImgLineOffset(YLOFS, ULOFS, VLOFS) jpeg_composite_setImgLineOffset(JPEG_ID_0, YLOFS, ULOFS, VLOFS)
#define jpeg_getImgLineOffsetY()                  jpeg_composite_getImgLineOffsetY(JPEG_ID_0)
#define jpeg_getImgLineOffsetU()                  jpeg_composite_getImgLineOffsetU(JPEG_ID_0)
#define jpeg_getImgLineOffsetV()                  jpeg_composite_getImgLineOffsetV(JPEG_ID_0)
#define jpeg_setFileFormat(ImgWidth, ImgHeight, FileFormat) jpeg_composite_setFileFormat(JPEG_ID_0, ImgWidth, ImgHeight, FileFormat)
#define jpeg_setCmpStartAddr(BSAddr, BufSize)     jpeg_composite_setCmpStartAddr(JPEG_ID_0, BSAddr, BufSize)
#define jpeg_getBSStartAddr()                     jpeg_composite_getBSStartAddr(JPEG_ID_0)
#define jpeg_getBSCurAddr()                       jpeg_composite_getBSCurAddr(JPEG_ID_0)
#define jpeg_setBSOutput(Enable)                  jpeg_composite_setBSOutput(JPEG_ID_0, Enable)
#define jpeg_setRestartInterval(MCUNum)           jpeg_composite_setRestartInterval(JPEG_ID_0, MCUNum)
#define jpeg_setRestartEnable(Enable)             jpeg_composite_setRestartEnable(JPEG_ID_0, Enable)
#define jpeg_setRestartEOF()                      jpeg_composite_setRestartEOF(JPEG_ID_0)
#define jpeg_setFmtTranEnable()                   jpeg_composite_setFmtTranEnable(JPEG_ID_0)
#define jpeg_setFmtTranDisable()                  jpeg_composite_setFmtTranDisable(JPEG_ID_0)
#define jpeg_setDCOut(pDCOutCfg)                  jpeg_composite_setDCOut(JPEG_ID_0, pDCOutCfg)
#define jpeg_setCrop(StartX, StartY, Width, Height) jpeg_composite_setCrop(JPEG_ID_0, StartX, StartY, Width, Height)
#define jpeg_setCropDisable()                     jpeg_composite_setCropDisable(JPEG_ID_0)
#define jpeg_setCropEnable()                      jpeg_composite_setCropEnable(JPEG_ID_0)
#define jpeg_setScaleEnable()                     jpeg_composite_setScaleEnable(JPEG_ID_0)
#define jpeg_setScaleDisable()                    jpeg_composite_setScaleDisable(JPEG_ID_0)
#define jpeg_setScaleRatio(ScaleRatio)            jpeg_composite_setScaleRatio(JPEG_ID_0, ScaleRatio)
#define jpeg_setQTabHW(pucQTabY, pucQTabUV)     jpeg_composite_setQTabHW(JPEG_ID_0, pucQTabY, pucQTabUV)
#define jpeg_setEncodeHuffTabHW(puiHuffTabLAC, puiHuffTabLDC, puiHuffTabCAC, puiHuffTabCDC) jpeg_composite_setEncodeHuffTabHW(JPEG_ID_0, puiHuffTabLAC, puiHuffTabLDC, puiHuffTabCAC, puiHuffTabCDC)
#define jpeg_setDecodeHuffTabHW(pHuffDC0th, pHuffDC1th, pHuffAC0th, pHuffAC1th) jpeg_composite_setDecodeHuffTabHW(JPEG_ID_0, pHuffDC0th, pHuffDC1th, pHuffAC0th, pHuffAC1th)
#define jpeg_getCycleCnt()                        jpeg_composite_getCycleCnt(JPEG_ID_0)
#define jpeg_getBRCInfo(pBRCInfo)                 jpeg_composite_getBRCInfo(JPEG_ID_0, pBRCInfo)

#endif
