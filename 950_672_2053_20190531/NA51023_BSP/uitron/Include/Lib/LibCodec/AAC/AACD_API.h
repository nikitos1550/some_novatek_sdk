/**
    @file       AACD_API.h
    @ingroup    mIAVAAC

    @brief      Header file of AAC Decoder API Library.

                This file is the Header file of AAC decoder Library.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

/**
    @addtogroup mIAVAAC
*/
//@{

#ifndef _AACD_API_H
#define _AACD_API_H

/**
    Error Code definition

    @note Returned error code by AAC Decoder API.
*/
typedef enum _EN_AUDIO_ERROR_CODE {
	EN_AUDIO_ERROR_NONE          = 0x0000,    ///< No error
	EN_AUDIO_ERROR_CONFIGUREFAIL = 0x0001,    ///< Configure failed
	EN_AUDIO_ERROR_RUNFAIL       = 0x0002,    ///< Run fail
	EN_AUDIO_ERROR_STREAM_EMPTY  = 0x0003,    ///< No input stream
	EN_AUDIO_ERROR_NOINITIAL     = 0x0004,    ///< Not initial
	EN_AUDIO_ERROR_UNKNOWN       = 0x0005,    ///< Unknown error
	EN_AUDIO_ERROR_TOTAL
} EN_AUDIO_ERROR_CODE;

/**
    Audio sampling rate definition

    @note Audio sampling rate definition.
*/
typedef enum _EN_AUDIO_SAMPLING_RATE {
	EN_AUDIO_SAMPLING_RATE_NONE  = 0x0000,     ///< None
	EN_AUDIO_SAMPLING_RATE_8000  = 0x0001,     ///< 8K Hz
	EN_AUDIO_SAMPLING_RATE_11025 = 0x0002,     ///< 11.025K Hz
	EN_AUDIO_SAMPLING_RATE_12000 = 0x0003,     ///< 12K Hz
	EN_AUDIO_SAMPLING_RATE_16000 = 0x0004,     ///< 16K Hz
	EN_AUDIO_SAMPLING_RATE_22050 = 0x0005,     ///< 22.05K Hz
	EN_AUDIO_SAMPLING_RATE_24000 = 0x0006,     ///< 24K Hz
	EN_AUDIO_SAMPLING_RATE_32000 = 0x0007,     ///< 32K Hz
	EN_AUDIO_SAMPLING_RATE_44100 = 0x0008,     ///< 44.1K Hz
	EN_AUDIO_SAMPLING_RATE_48000 = 0x0009,     ///< 48K Hz
	EN_AUDIO_SAMPLING_RATE_TOTAL
} EN_AUDIO_SAMPLING_RATE;


/**
    AAC decoder coding type definition

    @note AAC decoder coding type definition.
*/
typedef enum _EN_AUDIO_CODING_TYPE {
	EN_AUDIO_CODING_TYPE_DISABLE = 0,     ///< Skip AAC header decode or not encode AAC header.
	EN_AUDIO_CODING_TYPE_ENABLE  = 1,     ///< Enable AAC header decode or enocde AAC header.
	EN_AUDIO_CODING_TYPE_TOTAL
} EN_AUDIO_CODING_TYPE;


/**
   AAC Decode Buffer Information

   @note Used by AACD_DecodeOneFrame().
*/
typedef struct _ST_AUDIO_AACD_BUFINFO {
	unsigned int    u32AvailBytes;          ///<[in] Avaliable bytes, it must greater than one frame size.
	unsigned int   *pu32InBufferAddr;       ///<[in] Stream buffer address.
	unsigned int   *pu32OutBufferAddr;      ///<[out]Decode buffer address.
} ST_AUDIO_AACD_BUFINFO, *pST_AUDIO_AACD_BUFINFO;


/**
   AAC Decode Return Information

   @note Used by AACD_DecodeOneFrame().
*/
typedef struct _ST_AUDIO_AACD_RTNINFO {
	unsigned int u32DecodeOutSamples;       ///<[out] Output samples per channel
	unsigned int u32OneFrameConsumeBytes;   ///<[out] Consumed bytes after decoding one frame
	unsigned int u32nChans;                 ///<[out] Audio channels
	unsigned int u32bitRate;                ///<[out] Bit rate
	unsigned int u32SampleFreq;             ///<[out] Sampling frequency
	unsigned int u32bitsPerSample;          ///<[out] Bits per sample
} ST_AUDIO_AACD_RTNINFO, *pST_AUDIO_AACD_RTNINFO;


/**
   AAC Decoder Configuration Information

   @note Used by AACD_Init().
*/
typedef struct _ST_AUDIO_AACDEC_CFG {
	EN_AUDIO_SAMPLING_RATE    enSampleRate;        ///< [in] Audio sampling rate.
	unsigned int              u32nChannels;        ///< [in] Audio channels.
	EN_AUDIO_CODING_TYPE      enCodingType;        ///< [in] Audio Coding type.
} ST_AUDIO_AACDEC_CFG, *pST_AUDIO_AACDEC_CFG;

/**
    @name   AAC Decoder function API
*/
//@{

/**
    AAC Decoder Initialization

    Initialize a new AAC decoder.

    @param[in] pstInputCfg       AAC decoder configuration.

    @return EN_AUDIO_ERROR_CODE
*/
extern EN_AUDIO_ERROR_CODE AACD_Init(pST_AUDIO_AACDEC_CFG pstInputCfg);

/**
    AAC Decode One Frame

    Process an AAC decode frame data

    @param[in] pstBufInfo     AAC decode buffer information.
    @param[in] pRtnINfo       AAC decode return information.

    @return EN_AUDIO_ERROR_CODE
*/
extern EN_AUDIO_ERROR_CODE AACD_DecodeOneFrame(pST_AUDIO_AACD_BUFINFO pstBufInfo, pST_AUDIO_AACD_RTNINFO pRtnINfo);

//@}

//@}

/*---------------------------------------------------------------------------
functionname:AUD_AACD_GetVersion
description:  Get AACD code version.
returns:     AACD code version
---------------------------------------------------------------------------*/
//extern short int AUD_AACD_GetVersion(void);

#endif
