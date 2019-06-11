#ifndef _AUDCODEC_H
#define _AUDCODEC_H
/*
    Copyright (c) 2014~  Novatek Microelectronics Corporation

    @file audcodec.h

    @version

    @date
*/

#include <cyg/nvtipc/NvtIpcAPI.h>

/**
    Error Code definition

    @note Returned error code by AAC Encoder API.
*/
typedef enum _EN_AUDIO_ERROR_CODE {
	EN_AUDIO_ERROR_NONE          = 0x0000,   ///< No error
	EN_AUDIO_ERROR_CONFIGUREFAIL = 0x0001,   ///< Fail to configure the encoder
	EN_AUDIO_ERROR_RUNFAIL       = 0x0002,   ///< Fail to run the encoder
	EN_AUDIO_ERROR_STREAM_EMPTY  = 0x0003,   ///< No input stream
	EN_AUDIO_ERROR_NOINITIAL     = 0x0004,   ///< AAC Encoder not initial
	EN_AUDIO_ERROR_UNKNOWN       = 0x0005,   ///< Unknown error
	EN_AUDIO_ERROR_TOTAL
} EN_AUDIO_ERROR_CODE;

/**
    Audio sampling rate definition

    @note Audio sampling rate definition.
*/
typedef enum _EN_AUDIO_SAMPLING_RATE {
	EN_AUDIO_SAMPLING_RATE_NONE  = 0x0000,   ///< None
	EN_AUDIO_SAMPLING_RATE_8000  = 0x0001,   ///< 8K Hz
	EN_AUDIO_SAMPLING_RATE_11025 = 0x0002,   ///< 11.025K Hz
	EN_AUDIO_SAMPLING_RATE_12000 = 0x0003,   ///< 12K Hz
	EN_AUDIO_SAMPLING_RATE_16000 = 0x0004,   ///< 16K Hz
	EN_AUDIO_SAMPLING_RATE_22050 = 0x0005,   ///< 22.5K Hz
	EN_AUDIO_SAMPLING_RATE_24000 = 0x0006,   ///< 24K Hz
	EN_AUDIO_SAMPLING_RATE_32000 = 0x0007,   ///< 32K Hz
	EN_AUDIO_SAMPLING_RATE_44100 = 0x0008,   ///< 44.1K Hz
	EN_AUDIO_SAMPLING_RATE_48000 = 0x0009,   ///< 48K Hz
	EN_AUDIO_SAMPLING_RATE_TOTAL
} EN_AUDIO_SAMPLING_RATE;

/**
    Audio support bit rate definition

    @note Audio support bit rate definition.
*/
typedef enum _EN_AUDIO_BIT_RATE {
	EN_AUDIO_BIT_RATE_NONE = 0x0000,         ///< None
	EN_AUDIO_BIT_RATE_16k  = 0x0001,         ///< 16K
	EN_AUDIO_BIT_RATE_32k  = 0x0002,         ///< 32K
	EN_AUDIO_BIT_RATE_48k  = 0x0003,         ///< 48K
	EN_AUDIO_BIT_RATE_64k  = 0x0004,         ///< 64K
	EN_AUDIO_BIT_RATE_80k  = 0x0005,         ///< 80K
	EN_AUDIO_BIT_RATE_96k  = 0x0006,         ///< 96K
	EN_AUDIO_BIT_RATE_112k = 0x0007,         ///< 112K
	EN_AUDIO_BIT_RATE_128k = 0x0008,         ///< 128K
	EN_AUDIO_BIT_RATE_144k = 0x0009,         ///< 144K
	EN_AUDIO_BIT_RATE_160k = 0x000A,         ///< 160K
	EN_AUDIO_BIT_RATE_192k = 0x000B,         ///< 192K
	EN_AUDIO_BIT_RATE_TOTAL
} EN_AUDIO_BIT_RATE;

/**
    Audio Support Enocde Stop Frequency

    @note Audio support encode stop frequency definition.
*/
typedef enum _EN_AUDIO_ENC_STOP_FREQ {
	EN_AUDIO_ENC_STOP_FREQ_08K = 0x0000,    ///< 8KHz
	EN_AUDIO_ENC_STOP_FREQ_10K = 0x0001,    ///< 10KHz
	EN_AUDIO_ENC_STOP_FREQ_11K = 0x0002,    ///< 11KHz
	EN_AUDIO_ENC_STOP_FREQ_12K = 0x0003,    ///< 12KHz
	EN_AUDIO_ENC_STOP_FREQ_14K = 0x0004,    ///< 14KHz
	EN_AUDIO_ENC_STOP_FREQ_16K = 0x0005,    ///< 16KHz
	EN_AUDIO_ENC_STOP_FREQ_18K = 0x0006,    ///< 18KHz
	EN_AUDIO_ENC_STOP_FREQ_20K = 0x0007,    ///< 20KHz
	EN_AUDIO_ENC_STOP_FREQ_24K = 0x0008,    ///< 24KHz
	EN_AUDIO_ENC_STOP_FREQ_TOTAL
} EN_AUDIO_ENC_STOP_FREQ;

/**
    AAC Encoder Coding Type Definition

    @note AAC encoder coding type definition.
*/
typedef enum _EN_AUDIO_CODING_TYPE {
	EN_AUDIO_CODING_TYPE_DISABLE = 0,     ///< Skip AAC header encode AAC header.
	EN_AUDIO_CODING_TYPE_ENABLE  = 1,     ///< Enable AAC header encode AAC header.
	EN_AUDIO_CODING_TYPE_TOTAL
} EN_AUDIO_CODING_TYPE;

/*
Support Bitrate/sampling rate/channel are as below.
Sampling rate(kHz)  8       11.025      12        16      22.05       24        32       44.1       48
Channel Number    1    2    1    2    1    2    1    2    1    2    1    2    1    2    1    2    1    2
Bit rate(kbps)
            16    O    O    O    O    O    O    O    O    O    O    O    O    O    O    O    O    O    O
            32    O    O    O    O    O    O    O    O    O    O    O    O    O    O    O    O    O    O
            48         O    O    O    O    O    O    O    O    O    O    O    O    O    O    O    O    O
            64              O    O    O    O    O    O    O    O    O    O    O    O    O    O    O    O
            80                   O         O    O    O    O    O    O    O    O    O    O    O    O    O
            96                                       O    O    O    O    O    O    O    O    O    O    O
            112                                      O    O    O    O    O    O    O    O    O    O    O
            128                                           O    O    O    O    O    O    O    O    O    O
            144                                                O         O    O    O    O    O    O    O
            160                                                O         O    O    O    O    O    O    O
            192                                                                    O    O    O    O    O
*/
/**
   AAC Eecoder Configuration Information

   @note Used by AACE_InitConfig().
*/
typedef struct _ST_AUDIO_AACENC_CFG {
	EN_AUDIO_SAMPLING_RATE    enSampleRate;        ///< [in] AAC encoder output sampling rate.
	unsigned int              u32nChannels;        ///< [in] Audio channels.
	EN_AUDIO_BIT_RATE         enBitRate;           ///< [in] Bit rate.
	EN_AUDIO_CODING_TYPE      enCodingType;        ///< [in] Coding type.
	EN_AUDIO_ENC_STOP_FREQ    enEncStopFreq;        ///< [in] Stop frequency.
} ST_AUDIO_AACENC_CFG, *pST_AUDIO_AACENC_CFG;


/**
   AAC Encode Buffer Information

   @note Used by AACE_EncodeOneFrame().
*/
typedef struct _ST_AUDIO_AACENC_BUFINFO {
	unsigned int    u32nSamples;            ///< [in] Total samples, it must >=1024*u32nChannels.
	unsigned int   *pu32InBufferAddr;       ///< [in] Stream buffer address.
	unsigned int   *pu32OutBufferAddr;      ///< [out] Encode buffer address.
} ST_AUDIO_AACENC_BUFINFO, *pST_AUDIO_AACENC_BUFINFO;

/**
   AAC Encode Return Information

   @note Used by AACE_EncodeOneFrame().
*/
typedef struct _ST_AUDIO_AACENC_RTNINFO {
	unsigned int u32EncodeOutBytes; ///< [out] Encode output bytes.
} ST_AUDIO_AACENC_RTNINFO, *pST_AUDIO_AACENC_RTNINFO;

typedef enum {
	AACE_CMD_OPEN = 0,
	AACE_CMD_ENCODEONE,
	ENUM_DUMMY4WORD(AACE_CMD)
} AACE_CMD;

typedef struct {
	pST_AUDIO_AACENC_CFG pstInputCfg;
} AACE_OPEN_PARAM, *pAACE_OPEN_PARAM;

typedef struct {
	ST_AUDIO_AACENC_BUFINFO stBufInfo;
	unsigned char *ancBytes;
	short numAncBytes;
	int numOutBytes;
} AACE_ENCODEONE_PARAM, *pAACE_ENCODEONE_PARAM;

typedef struct {
	NVTIPC_U32 cmd;
	NVTIPC_U32 addr;
	NVTIPC_U32 size;
	NVTIPC_U32 ret;
} AACE_IPC_DATA;

__externC void AudCodec_CmdLine(char *szCmd);

#endif /* _AUDCODEC_H  */

