/**
    Encode JPEG file.

    @file       JpgEnc.h
    @ingroup    mIUserJPEG
    @brief      JPEG encoder

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _JPGENC_H
#define _JPGENC_H

#include "SysKer.h"
#include "JpgHeader.h"
#include "JpegBRC.h"

/**
    @addtogroup mIUserJPEG
*/
//@{

/**
     @name The size of DQT(define quantization table)
*/
//@{
#define DQT_SIZE    64
//@}

/**
    JPEG header form.
*/
typedef enum _JPG_HEADER_FORM {
	JPG_HEADER_STD,                     ///< standard JPEG header
	JPG_HEADER_MOV,                     ///< MOV JPEG header
	JPG_HEADER_QV5,                     ///< QV5 JPEG header
	JPG_HEADER_QV5_NO_HUFFTABLE,        ///< QV5 JPEG header without huffman table
	ENUM_DUMMY4WORD(JPG_HEADER_FORM)
} JPG_HEADER_FORM;

/**
    JPEG encoding configuration.
*/
typedef enum _JPG_ENC_DATA_SET {
	JPG_BRC_STD_QTABLE_QUALITY,         ///< The quality of BRC table base. Default value is 50.
	ENUM_DUMMY4WORD(JPG_ENC_DATA_SET)
} JPG_ENC_DATA_SET;

/**
    DC output mode configuration structure.
*/
typedef struct _JPG_DC_OUT_CFG {
	UINT32  DCEnable;             ///< [in]DC out enable, TRUE for enable, FALSE for disable
	JPG_YUV_FORMAT  YUVFormat;    ///< [in]YUV format, refer to JPG_YUV_FORMAT
	JPG_PXL_LAYOUT  PxlLayout;    ///< [in]raw data layout, refer to JPG_PXL_LAYOUT
	UINT32  SourceWidth;          ///< [in]source width
	UINT32  SourceHeight;         ///< [in]source height
	UINT32  TargetWidth;          ///< [in]target width
	UINT32  TargetHeight;         ///< [in]target height
	UINT32  DCYAddr;              ///< [in]raw data address Y
	UINT32  DCUAddr;              ///< [in]raw data address UV
	UINT32  DCVAddr;              ///< reserved
	UINT32  DCWidth;              ///< [out]real DC out Width by HW condition
	UINT32  DCHeight;             ///< [out]real DC out Height by HW condition
	UINT32  DCYLineOffset;        ///< [out]real DC out LineOffset Y by HW condition
	UINT32  DCULineOffset;        ///< [out]real DC out LineOffset UV by HW condition
	UINT32  DCVLineOffset;        ///< reserved
	UINT32  DCXRatio;             ///< [out]DC output X coordinate (horizontal) ratio
	UINT32  DCYRatio;             ///< [out]DC output Y coordinate (vertical) ratio
} JPG_DC_OUT_CFG, *PJPG_DC_OUT_CFG;

/**
    JPEG Encode configuration structure.
*/
typedef struct _JPG_ENC_CFG {
	UINT32    RawImageAddr[3];                ///< Raw Image address. Ex:RawImageAddr[0] for Y, RawImageAddr[1] for UV
	UINT32    JPGBSAddr;                      ///< Compressed data ouput location.
	UINT32    *JPGBSBufSize;                  ///< [in]available buffer size for output bitstream.[out]real output bitstream size.
	UINT32    Width;                          ///< Raw Image Width, This value should be 16-multiplier.
	UINT32    Height;                         ///< Raw Image Height, This value should be 8 or 16 multiplier, depends on output bs format.
	UINT32    LineOffset[3];                  ///< Raw Image line offset. Ex:LineOffset[0] for Y, LineOffset[1] for UV
	JPG_YUV_FORMAT  YUVFormat;                ///< YUV format, refer to JPG_YUV_FORMAT
	JPG_PXL_LAYOUT  PxlLayout;                ///< Raw data layout, refer to JPG_PXL_LAYOUT
	BOOL    BSOutDis;                         ///< If BSOutDis is TRUE, JPEG engine wiil NOT output bitstream data.
	BOOL    BrcEn;                            ///< Bit Rate Control enable, TRUE for enable and FALSE for disable.
	JPG_DC_OUT_CFG  DCOutCfg;                 ///< DC out configuration, refer to JPG_DC_OUT_CFG.
	JPG_HEADER_FORM  HeaderForm;              ///< Header form, refer to JPG_HEADER_FORM.
} JPG_ENC_CFG, *PJPG_ENC_CFG;

/**
    Bit Rate Control configuration structure.
*/
typedef struct _JPG_BRC_CFG {
	UINT32          uiInitialQF;        ///< initial QF value in bit rate control
	UINT32          uiTargetSize;       ///< target compressed size when bit rate control is enabled
	UINT32          uiLowBoundSize;     ///< bit stream lower boundary size(error tolerance)
	UINT32          uiUpBoundSize;      ///< bit stream upper boundary size(error tolerance)
	UINT32          uiMaxReTryCnt;      ///< maxmum re-try count
	UINT32          uiWidth;            ///< image width
	UINT32          uiHeight;           ///< image height
} JPG_BRC_CFG, *PJPG_BRC_CFG;


/**
    Encode one image into JPEG bitstream.

    @note It's a synchronous API.
    @param[in] ID User JPG handle ID.
    @param[in] JpgParam Encoding configuration: #_JPG_ENC_CFG.
    @param uiQuality This parameter is only valid in Non-BRC mode and the value should between 0 to 100.\n
                     100 is the best quality and 1 is the worst one. 0 means default quality.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_OBJ:  Reach BRC limitation or retry count, the bitstream might be correct in BRC mode.
        - @b ER_SYS:  Failed.
        - @b E_NOMEM: Bitstream Buffer is too small.
*/
extern ER Jpg_EncOneImgEx(UJPG_ID JpgID, PJPG_ENC_CFG JpgParam, UINT32 uiQuality);

/**
    Open JPEG engine in asynchronous encoding mode.

    @note It's an asynchronous API.
    @param[in] ID User JPG handle ID.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER Jpg_ASyncEncOpenEx(UJPG_ID JpgID);

/**
    Set JPEG header in asynchronous encoding mode.

    @note It's an asynchronous API.
    @param[in] ID User JPG handle ID.
    @param[in] JpgParam Encoding configuration: #_JPG_ENC_CFG.
    @param[in] pQTable_Y Specific Q Table Y which could be made by Jpg_AsyncSetQTable() or Jpg_AsyncSetBRCQTable().
    @param[in] pQTable_UV Specific Q Table UV which could be made by Jpg_AsyncSetQTable() or Jpg_AsyncSetBRCQTable().
    @return JPEG header size.
*/
extern UINT32 Jpg_ASyncEncSetHeaderEx(UJPG_ID JpgID, PJPG_ENC_CFG JpgParam, UINT8 *pQTable_Y, UINT8 *pQTable_UV);

/**
    Start to encode in asynchronous encoding mode.

    @note It's an asynchronous API.
    @param[in] ID User JPG handle ID.
    @param[in] BSBufAddr The address of output bitstream (NOT including header).
    @param[in] BSBufSize The available size for output bitstream (NOT including header).
    @param[in] ImgYAddr Raw image address Y.
    @param[in] ImgUVAddr Raw image address UV.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER Jpg_ASyncEncStartEx(UJPG_ID JpgID, UINT32 BSBufAddr, UINT32 BSBufSize, UINT32 ImgYAddr, UINT32 ImgUVAddr);

/**
    Wait for JPEG to finish in asynchronous encoding mode.

    @note It's an asynchronous API.
    @param[in] ID User JPG handle ID.
    @param[out] pJPGFileSize JPG file size including JFIF header and bitsteam.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER Jpg_ASyncEncWaitDoneEx(UJPG_ID JpgID, UINT32 *pJPGFileSize);

/**
    Close JPEG engine in asynchronous encoding mode.

    @note It's an asynchronous API.
    @param[in] ID User JPG handle ID.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER Jpg_ASyncEncCloseEx(UJPG_ID JpgID);

/**
    Set Q table in asynchronous BRC encoding mode.

    @note It's an asynchronous API.
    @param[in] ID User JPG handle ID.
    @param[in] uiQF Quality value in BRC mode. 1 is the best quality and 512 is the worst one.
    @param[out] pQTable_Y The corresponding Q table Y.
    @param[out] pQTable_UV The corresponding Q table UV.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern void Jpg_AsyncSetBRCQTableEx(UJPG_ID JpgID, UINT32 uiQF, UINT8 *pQTable_Y, UINT8 *pQTable_UV);

/**
    Set Q table in asynchronous Non-BRC encoding mode.

    @note It's an asynchronous API.
    @param[in] ID User JPG handle ID.
    @param[in] uiQuality Quality value in Non-BRC mode. 100 is the best quality and 1 is the worst one.
    @param[out] pQTable_Y The corresponding Q table Y.
    @param[out] pQTable_UV The corresponding Q table UV.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern void Jpg_AsyncSetQTableEx(UJPG_ID JpgID, UINT32 uiQuality, UINT8 *pQTable_Y, UINT8 *pQTable_UV);

/**
    Get DC out setting

    @param[in,out] JpgDCOut DC out configuration: #_JPG_DC_OUT_CFG.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER Jpg_GetDCOutConfig(PJPG_DC_OUT_CFG JpgDCOut);


/**
    Set bit rate control

    This API is used to setup bit rate control.

	@param[in] ID User JPG handle ID.
    @param[in] pBrcCfg Bit rate control configuration: #_JPG_BRC_CFG
    @return
        - @b E_OK: Succeed.
        - @b E_PAR: Parameter error.
*/
extern ER Jpg_SetBitRateCtrlEx(UJPG_ID JpgID, PJPG_BRC_CFG pBrcCfg);

/**
    Get BRC QF value.

    After encoding finished, use this API to get predicted QF value to encode again.

	@param[in] ID User JPG handle ID.
    @param[in] InputRaw JPEG BRC YUV type: #JPGBRC_INPUT_RAW.
    @param[in,out] pQF QF value, [in]original QF, [out]predicted QF.
    @param[in] BSSize Bitstream size.

*/
extern void Jpg_GetBRCQFEx(UJPG_ID JpgID, JPGBRC_INPUT_RAW InputRaw, UINT32 *pQF, UINT32 BSSize);

/**
    Set encoding data.

    In order to change some default setting, such as the quality of BRC table base.

	@param[in] ID User JPG handle ID.
    @param[in] attribute Encoding configuration: #_JPG_ENC_DATA_SET.
    @param[in] Value Value of the attribute.

*/
extern void Jpg_EncSetDataEx(UJPG_ID JpgID, JPG_ENC_DATA_SET attribute, UINT32 Value);


//--------------------------------------------------------------------------
// Backward Compatible
//--------------------------------------------------------------------------
extern ER Jpg_ReAssignBSBuf(UINT32 *pBuff, UINT32 *pSize);
#define	Jpg_EncOneImg(pBuff, pSize)	Jpg_EncOneImgEx(UJPG_ID_0, pBuff, pSize)
#define Jpg_ASyncEncOpen()			Jpg_ASyncEncOpenEx(UJPG_ID_0)
#define Jpg_ASyncEncSetHeader(JpgParam, pQTable_Y, pQTable_UV) Jpg_ASyncEncSetHeaderEx(UJPG_ID_0, JpgParam, pQTable_Y, pQTable_UV)
#define Jpg_ASyncEncStart(BSBufAddr, BSBufSize, ImgYAddr, ImgUVAddr)	Jpg_ASyncEncStartEx(UJPG_ID_0, BSBufAddr, BSBufSize, ImgYAddr, ImgUVAddr)
#define Jpg_ASyncEncWaitDone(pJPGFileSize)		Jpg_ASyncEncWaitDoneEx(UJPG_ID_0, pJPGFileSize)
#define Jpg_ASyncEncClose()			Jpg_ASyncEncCloseEx(UJPG_ID_0)
#define Jpg_AsyncSetBRCQTable(uiQF, pQTable_Y, pQTable_UV)		Jpg_AsyncSetBRCQTableEx(UJPG_ID_0, uiQF, pQTable_Y, pQTable_UV)
#define Jpg_AsyncSetQTable(uiQuality, pQTable_Y, pQTable_UV)	Jpg_AsyncSetQTableEx(UJPG_ID_0, uiQuality, pQTable_Y, pQTable_UV)
#define Jpg_SetBitRateCtrl(pBrcCfg)		Jpg_SetBitRateCtrlEx(UJPG_ID_0, pBrcCfg)
#define Jpg_GetBRCQF(InputRaw, pQF, BSSize)				Jpg_GetBRCQFEx(UJPG_ID_0, InputRaw, pQF, BSSize)
#define Jpg_EncSetData(attribute, Value)				Jpg_EncSetDataEx(UJPG_ID_0, attribute, Value)
//@}
#endif // _JPGENC_H
