#ifndef _NVTCODEC_H
#define _NVTCODEC_H
/*
    Copyright (c) 2015~  Novatek Microelectronics Corporation

    @file nvtcodec.h

    @version

    @date
*/
#include <stdio.h>
#include <nvt_type.h>
#include <sys/time.h>
#include "Type.h"
#include "nvtevent_.h"
#include "fd_lib.h"
#include "nvtcodec_event.h"
#include "../../../../Include/App/videosprite/videosprite.h"
#include <nvtvideosprite_protected.h>
#ifdef __cplusplus
extern "C" {
#endif

#define VIDEO_ENCODER_CHANNEL_MAX   16
#define COMMON_BUF_MAX   16
#define VIDEO_OUTPUT_CHANNEL_MAX    1
#define AUDIO_INPUT_CHANNEL_MAX     1
#define AUDIO_OUTPUT_CHANNEL_MAX    1
#define VENC_STREAM_PACK_MAX        4

#define VPS_LENGTH_MAX 256
#define SPS_LENGTH_MAX 256
#define PPS_LENGTH_MAX 256
#define MD_USR_MAX_REGION 4
typedef enum {
	H264_NALU_TYPE_SLICE = 1,       ///< Coded slice (P-Frame)
	H264_NALU_TYPE_DPA,             ///< Coded data partition A
	H264_NALU_TYPE_DPB,             ///< Coded data partition B
	H264_NALU_TYPE_DPC,             ///< Coded data partition C
	H264_NALU_TYPE_IDR = 5,         ///< Instantaneous decoder refresh (I-Frame)
	H264_NALU_TYPE_SEI,             ///< Supplement Enhancement Information
	H264_NALU_TYPE_SPS = 7,         ///< Sequence parameter sets
	H264_NALU_TYPE_PPS = 8,         ///< Picture parameter sets
	H264_NALU_TYPE_AUD,             ///< AUD
	H264_NALU_TYPE_EOSEQ,           ///< End sequence
	H264_NALU_TYPE_EOSTREAM,        ///< End stream
	H264_NALU_TYPE_FILL,            ///< Filler data
	H264_NALU_TYPE_SPSEXT,          ///< SPS extension
	H264_NALU_TYPE_AUXILIARY,       ///< Auxiliary slice
	H264_NALU_TYPE_MAX,
	ENUM_DUMMY4WORD(H264_NALU_TYPE)
} H264_NALU_TYPE;

typedef enum {
	H265_NALU_TYPE_SLICE = 1,
	H265_NALU_TYPE_IDR = 19,
	H265_NALU_TYPE_VPS = 32,
	H265_NALU_TYPE_SPS = 33,
	H265_NALU_TYPE_PPS = 34,
	H265_NALU_TYPE_AUD = 35,
	H265_NALU_TYPE_EOS_NUT = 36,
	H265_NALU_TYPE_EOB_NUT = 37,
	H265_NALU_TYPE_SEI_PREFIX = 39,
	H265_NALU_TYPE_SEI_SUFFIX = 40,
	ENUM_DUMMY4WORD(H265_NALU_TYPE)
} H265_NALU_TYPE;

typedef enum {
	JPEG_PACK_TYPE_PIC,             ///< JPEG picture data
	JPEG_PACK_TYPE_MAX,
	ENUM_DUMMY4WORD(JPEG_PACK_TYPE)
} JPEG_PACK_TYPE;

typedef union {
	H264_NALU_TYPE eH264Type;           ///< H.264 pack type
	JPEG_PACK_TYPE eJPEGType;           ///< JPEG pack type
	H265_NALU_TYPE eH265Type;           ///< H.265 pack type
} CodecVencPackType;

typedef enum {
	CODEC_VENC_INPUT_SOURCE_SENSOR1,
	CODEC_VENC_INPUT_SOURCE_SENSOR2,
	CODEC_VENC_INPUT_SOURCE_SENSOR3,
	CODEC_VENC_INPUT_SOURCE_SENSOR4,
	CODEC_VENC_INPUT_SOURCE_UVC,
	CODEC_VENC_INPUT_SOURCE_MAX,
	ENUM_DUMMY4WORD(CodecVencInputSource)
} CodecVencInputSource;

typedef enum {
	CODEC_VENC_FORMAT_MJPEG,
	CODEC_VENC_FORMAT_H264,
	CODEC_VENC_FORMAT_H265,
        CODEC_VENC_FORMAT_YUV,
	CODEC_VENC_FORMAT_MAX,
	ENUM_DUMMY4WORD(CodecVencFormat)
} CodecVencFormat;

typedef enum {
	CODEC_VENC_PROFILE_BASELINE,
	CODEC_VENC_PROFILE_MAIN,
	CODEC_VENC_PROFILE_HIGH,
	CODEC_VENC_PROFILE_MAX,
	ENUM_DUMMY4WORD(CodecVencProfile)
} CodecVencProfile;

typedef struct {

	UINT8 bROIEnable;            ///< ROI enable or disable
	UINT32 uiX;                 ///< X coordinate of top-left point of ROI window
	UINT32 uiY;                 ///< Y coordinate of top-left point of ROI window
	UINT32 uiWidth;             ///< Width of ROI window
	UINT32 uiHeight;            ///< Height of ROI window
	INT32 iQP;             ///< ROI QP,uiDeltaQp=0,  Range: 1~51.   uiDeltaQp=1, Range: -32~31
} CodecVencROI;

typedef struct {
	UINT32 	uiDeltaQp;		// 0 = all fixed qp, 1 = all delta qp
	CodecVencROI sROIConfig[10];    ///< support 10 index to contrl ROI

} CodecVencROISetting;

#if 0
typedef struct {
	UINT8 bCropEnable;           ///< Cropping enable or disable. NVT_TRUE means video encoder will crop area with target width and height from video source.
	UINT32 uiCoordinateX;       ///< X coordinate of top-left point of cropping window
	UINT32 uiCoordinateY;       ///< Y coordinate of top-left point of cropping window
} CodecVencCrop;
#endif
typedef enum {
	CODEC_VENC_SVCT_DISABLE,    ///< SVC-T disable
	CODEC_VENC_SVCT_LAYER_1,    ///< SVC-T enable with 1 layers
	CODEC_VENC_SVCT_LAYER_2,    ///< SVC-T enable with 2 layers
	CODEC_VENC_SVCT_MAX,
	ENUM_DUMMY4WORD(CodecVencSVCT)
} CodecVencSVCT;

typedef struct {
	CodecVencSVCT eSVCTemporalLayer; ///< SVC temporal layer control
} CodecVencSVCSetting;

typedef struct {
	UINT32 uiEnable;            ///<
	UINT32 uiStaticTime;        ///< Range: 0~20
	UINT32 uiByteRate;          ///< Target byte rate
	UINT32 uiFrameRate;         ///<
	UINT32 uiGOP;               ///< Group of pictures for H.26x. Range: 1~4096 (Unit: frame)
	UINT32 uiInitIQp;            ///< Initial I-frame QP value.Range: 0~51
	UINT32 uiMinIQp;            ///< Minimum I-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMaxIQp;            ///< Maximum I-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiInitPQp;            ///< Initial P-frame QP value.Range: 0~51
	UINT32 uiMinPQp;            ///< Minimum P-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMaxPQp;            ///< Maximum P-frame QP value. Range: 0~51, 0 is best quality
	INT32  iIPWeight;           ///< Range: -100 ~ 100, Set to 0 by default
	UINT32 uiChangePos;         ///< Range: 0~100
	UINT32 uiRowRcEnalbe;       ///< 0 :disable RowRc,  1:enable RowRc
	UINT32 uiRowRcQpRange;      ///<
	UINT32 uiRowRcQpStep;       ///<
} CodecVencVBR;

typedef struct {
	UINT32 uiEnable;            ///<
	UINT32 uiStaticTime;        ///< Range: 0~20
	UINT32 uiByteRate;          ///< Target byte rate
	UINT32 uiFrameRate;         ///<
	UINT32 uiGOP;               ///< Group of pictures for H.264/265. Range: 1~4096 (Unit: frame)
	UINT32 uiKeyPPeriod;        ///< base on frame rate   Range: 0~4096
	UINT32 uiInitIQp;           ///< Initial I-frame QP value, Range: 0~51, 0 is best quality
	UINT32 uiMinIQp;            ///< Minimum I-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMaxIQp;            ///< Maximum I-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiInitPQp;           ///< Initial P-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMinPQp;            ///< Minimum P-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMaxPQp;            ///< Maximum P-frame QP value. Range: 0~51, 0 is best quality
	INT32  iIPWeight;           ///< Range: -100~100
	INT32  iKeyPWeight;         ///< Range: -100~100
	INT32  iMotionAQStrength;   ///< Range: -20~20
	UINT32 uiRowRcEnalbe;       ///< 0 :disable RowRc,  1:enable RowRc
	UINT32 uiRowRcQpRange;      ///<
	UINT32 uiRowRcQpStep;       ///<
	UINT32 uiStillFrameCnd;     ///< Range: 1~Unlimited
	UINT32 uiMotionRatioThd;    ///< Range: 1~100
	UINT32 uiIPsnrCnd;          ///< Range: 0~10000
	UINT32 uiPPsnrCnd;          ///< Range: 0~10000
	UINT32 uiKeyPPsnrCnd;       ///< Range: 0~10000
} CodecVencEVBR;


typedef struct {
	UINT32 uiEnable;            ///<
	UINT32 uiStaticTime;        ///< Range: 0~20
	UINT32 uiByteRate;          ///< Target byte rate
	UINT32 uiFrameRate;         ///<
	UINT32 uiGOP;               ///< Group of pictures for H.26x. Range: 1~4096 (Unit: frame)
	UINT32 uiInitIQp;            ///< Initial I-frame QP value.Range: 0~51
	UINT32 uiMinIQp;            ///< Minimum I-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMaxIQp;            ///< Maximum I-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiInitPQp;            ///< Initial P-frame QP value.Range: 0~51
	UINT32 uiMinPQp;            ///< Minimum P-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMaxPQp;            ///< Maximum P-frame QP value. Range: 0~51, 0 is best quality
	INT32  iIPWeight;           ///< Range: -100 ~ 100
	UINT32 uiRowRcEnalbe;       ///< 0 :disable RowRc,  1:enable RowRc
	UINT32 uiRowRcQpRange;      ///<
	UINT32 uiRowRcQpStep;       ///<
} CodecVencCBR;

typedef struct {
	UINT32    uiFrameRate;      ///<
	UINT32    uiGOP;            ///< Group of pictures for H.26x. Range: 1~4096 (Unit: frame)
	UINT32    uiByteRate;       ///< Target byte rate
} CodecVencBRCNoneInfo;

typedef enum {
	CODEC_VENC_BRC_NONE,
	CODEC_VENC_BRC_CBR,
	CODEC_VENC_BRC_VBR,
	CODEC_VENC_BRC_FIXQP,
        CODEC_VENC_BRC_EVBR,
	CODEC_VENC_BRC_MAX,
	ENUM_DUMMY4WORD(CODEC_VENC_BRC_TYPE)
} CODEC_VENC_BRC_TYPE;

typedef enum {
	CODEC_VENC_RT_NONE,         ///< no rotate
	CODEC_VENC_RT_BUILDIN_90,   ///< for codec rotate 90 Clockwise
	CODEC_VENC_RT_BUILDIN_270,  ///< for codec rotate 270 Clockwise
	CODEC_VENC_RT_EXTEND_90,    ///< for extend unit rotate 90 Clockwise
	CODEC_VENC_RT_EXTEND_180,   ///< for extend rotate 180 Clockwise
	CODEC_VENC_RT_EXTEND_270,   ///< for extend rotate 270 Clockwise
	CODEC_VENC_RT_EXTEND_MAX,
	ENUM_DUMMY4WORD(CODEC_VENC_RT_TYPE)
} CodecVencRT;

typedef struct {
	UINT32 uiByteRate;  ///< Target byte rate
	UINT32 uiFrameRate; ///< frame rate about fixed QP type
	UINT32 uiGOP;       ///< Group of pictures for H.26x. Range: 1~4096 (Unit: frame)
	UINT32 uiEnable;     ///< fixedQP is enable or not
	UINT32 uiIFixQP;        ///< fix I frame QP value
	UINT32 uiPFixQP;        ///< fix P frame QP value
} CodecVencFIXQP;



typedef struct {

	INT32  aq_enable;         ///< 0 = aq off, 1 = aq on
	UINT32 aq_str;            ///< aq strength: 1~8 (680);
	INT32  sraq_init_aslog2;  ///< initial i_ASlog2 for sraq (680): -1: disable sraq @1st Frame; 0~63 (default: 36) #0
	INT32  max_sraq;          ///< max ssim aq: 0~ 15 (default:  8)
	INT32  min_sraq;          ///< min ssim aq: 0~-15 (default: -8)
        INT32  sraq_const_aslog2; // 0:update aslog2 by each frame, 1:use sraq_init_aslog2 for each frame, default: 0
} CodecVencAQ;

typedef struct {
	UINT32 uiLTRInterval;   ///< long-term reference frame interval. range : 0 ~ 4095
	UINT32 uiLTRPreRef;     ///< long-term reference setting. 0 : all long-term reference to IDR frame, 1: reference latest long-term reference frame.
} CodecVencLTR;


typedef struct {

    UINT8 bEnable;               ///< enalbe/disable codec 3DNR function
} CodecVencNRSetting;

/**
    CodecVencSetting
    video lineoffset need alignment 64
*/
typedef struct {
	CodecVencInputSource eVencInputSource;  ///< Input video source
	CodecVencFormat eVencFormat;            ///< Video encoded format
	CodecVencProfile eVencProfile;          ///< H.264 profile
	UINT32 uiTargetWidth;                   ///< Target width of encoded data Range: 144 ~ (width of input source)
	UINT32 uiTargetHeight;                  ///< Target height of encoded data Range: 128 ~ (height of input source)
	CODEC_VENC_BRC_TYPE eBRCType;          ///< Bit rate control type)
	CodecVencVBR sVencVBRConfig;            ///< VBR configuration (for H26x)
	CodecVencCBR sVencCBRConfig;            ///< CBR configuration  (for H26x)
	CodecVencFIXQP sVencFixQPConfig;        ///< Fix QP configuration (for H26x)
        CodecVencEVBR  sVencEVBRConfig;         ///< EVBR configuration (for H26x)
	CodecVencBRCNoneInfo sVencBRCNoneInfo;  ///< No-BRC configuration for mjpg)
	//CodecVencCrop sVencCrop;                ///< Cropping area
	CodecVencSVCSetting sSVCSetting;        ///< SVC setting
	UINT32 uiBufNum;                        ///< Encoder buffer number (This parameter is not supported.)
        CodecVencNRSetting sNRSetting;          ///< codec NR setting
	CodecVencRT eVencRotateType;            ///< Rotate type (#CodecVencRT)
	CodecVencAQ sAQSetting;                 ///< adaptive quantization setting
        CodecVencLTR sLTRInfo;                  ///< long-term reference
} CodecVencSetting;

typedef struct {
	UINT32 uiDatatLength;                   ///< Data length of snapshot
	UINT32 uiSnapshotData;                  ///< Pointer to snapshot data
} CodecVencSnapshot;

typedef struct {
	UINT32 uiVencChannelID;                 ///< channel ID
	UINT32 uiWidth;                         ///< scaling width
	UINT32 uiHeight;                        ///< scaling height
} CodecVencSnapshotConfig;

typedef struct {
	UINT8 bStart;                            ///< Video encoder status. NVT_TRUE means video encoder already start.
	UINT32 uiMaxWidth;                      ///<
	UINT32 uiMaxHeight;                     ///<
} CodecVencStatus;

typedef struct {
	CodecVencPackType uPackType;            ///< Pack Type
	UINT32 uiDataLength;                    ///< Length of encoded data
	UINT8 *puiStreamPayloadData;            ///< Pointer to encoded data
	UINT8 bLastPack;                         ///< Flag of frame end. NVT_TRUE means the last pack of one frame.
} CodecVencPack;

typedef struct {
	CodecVencFormat eVencFormat;            ///< Encoded format of video frame
	UINT32 uiPackNum;                       ///< Pack number in video frame
	UINT32 uiSeqNum;                        ///< Video frame sequence number
	struct timeval sTimeStamp;              ///< Encoded timestamp
	UINT32 uiFrameType;                    ///<  Video Frame type 0:P frame, 1:B frame, 2:I frame , 3:IDR, 4:key p frame
	UINT32 uiSVCTemporalLayerID;            ///< SVC temporal layer ID
	CodecVencPack sVencPack[VENC_STREAM_PACK_MAX];  ///< Pack array of encoded data
} CodecVencStream;

typedef struct {
	UINT8 VPSLength;                        ///< VPS length
	UINT8 VPS[VPS_LENGTH_MAX];              ///< VPS data
	UINT8 SPSLength;                        ///< SPS length
	UINT8 SPS[SPS_LENGTH_MAX];              ///< SPS data
	UINT8 PPSLength;                        ///< PPS length
	UINT8 PPS[PPS_LENGTH_MAX];              ///< PPS data
} CodecVencParamSet;

typedef enum {
	CODEC_AIN_SOURCE_LINEIN,                ///< Line-in audio source
	CODEC_AIN_SOURCE_MICIN,                 ///< Mic-in audio source
	CODEC_AIN_SOURCE_MAX,
	ENUM_DUMMY4WORD(CodecAinSource)
} CodecAinSource;

typedef enum {
	CODEC_AIO_SAMPLE_RATE_8000 = 8000,
	CODEC_AIO_SAMPLE_RATE_11025 = 11025,
	CODEC_AIO_SAMPLE_RATE_12000 = 12000,
	CODEC_AIO_SAMPLE_RATE_16000 = 16000,
	CODEC_AIO_SAMPLE_RATE_22050 = 22050,
	CODEC_AIO_SAMPLE_RATE_24000 = 24000,
	CODEC_AIO_SAMPLE_RATE_32000 = 32000,
	CODEC_AIO_SAMPLE_RATE_44100 = 44100,
	CODEC_AIO_SAMPLE_RATE_48000 = 48000,
	CODEC_AIO_SAMPLE_RATE_MAX,
	ENUM_DUMMY4WORD(CodecAioSampleRate)
} CodecAioSampleRate;

typedef enum {
	CODEC_AIO_BIT_WIDTH_8BIT,               ///< 8 bits sampling precision
	CODEC_AIO_BIT_WIDTH_16BIT,              ///< 16 bits sampling precision
	CODEC_AIO_BIT_WIDTH_32BIT,              ///< 32 bits sampling precision
	CODEC_AIO_BIT_WIDTH_MAX,
	ENUM_DUMMY4WORD(CodecAioBitWidth)
} CodecAioBitWidth;

typedef enum {
	CODEC_AIO_WORK_MODE_I2S,
	CODEC_AIO_WORK_MODE_AC97,
	CODEC_AIO_WORK_MODE_MAX,
	ENUM_DUMMY4WORD(CodecAioWorkMode)
} CodecAioWorkMode;

typedef enum {
	CODEC_AIO_DUPLEX_MODE_HALF,             ///< Half duplex, audio input will be set to mute when audio output start
	CODEC_AIO_DUPLEX_MODE_FULL,             ///< Full duplex
	CODEC_AIO_DUPLEX_MODE_MAX,
	ENUM_DUMMY4WORD(CodecAioDuplexMode)
} CodecAioDuplexMode;

typedef enum {
	CODEC_AIO_SOUND_MODE_MONO,              ///< Mono mode. This parameter is not supported. (Please select #CODEC_AIO_SOUND_MODE_LEFT or #CODEC_AIO_SOUND_MODE_RIGHT)
	CODEC_AIO_SOUND_MODE_STEREO,            ///< Stereo mode
	CODEC_AIO_SOUND_MODE_LEFT,              ///< Duplicate left channel to right channel
	CODEC_AIO_SOUND_MODE_RIGHT,             ///< Duplicate right channel to left channel
	CODEC_AIO_SOUND_MODE_MAX,
	ENUM_DUMMY4WORD(CodecAioSoundMode)
} CodecAioSoundMode;

typedef enum {
	CODEC_AENC_FORMAT_PCM,                  ///< PCM format
	CODEC_AENC_FORMAT_AAC,                  ///< AAC format
	CODEC_AENC_FORMAT_ULAW,                 ///< G.711 ulaw format.
	CODEC_AENC_FORMAT_ALAW,                 ///< G.711 alaw format.
	CODEC_AENC_FORMAT_MAX,
	ENUM_DUMMY4WORD(CodecAencFormat)
} CodecAencFormat;

typedef enum {
	CODEC_AIN_CODEC_EMBEDDED,               ///< Embedded audio codec
	CODEC_AIN_CODEC_EXTERNAL,               ///< External audio codec
	ENUM_DUMMY4WORD(CodecAinCodec)
} CodecAinCodec;

typedef struct {
	UINT8 bNREnable;                ///< Audio noise reduction disable/enable
	INT32 iNRdB;                   ///< Maximum suppression level of noise
	INT32 iHPFCutOffFreq;          ///< Cut-off frequency of HPF pre-filtering
	INT32 iBiasSensitive;          ///< Bias sensitive
}CodecANRSetting;

typedef struct {
	BOOL bFiltEnable;
}CodecAudFiltSetting;

typedef struct {
	CodecAinSource eAinSource;              ///< Audio input source (This parameter is not supported.)
	UINT32 uiAinVolume;                     ///< Audio input volume. Range: 0~8, 0 means mute
	CodecAioSampleRate eAinSampleRate;      ///< Sample rate of audio input
	CodecAioBitWidth eAinBitWidth;          ///< Sampling precision. This parameter is not supported (Fixed to #CODEC_AIO_BIT_WIDTH_16BIT).
	CodecAioDuplexMode eAinDuplexMode;      ///< Audio input/output duplex mode
	CodecAioSoundMode eAinSoundMode;        ///< Sound channel mode
	CodecAencFormat eAinFormat;             ///< Audio encoded format
	UINT32 uiAinBitRate;                    ///< Encoded bitrate for AAC. Range: 16~192 (Unit: Kbps). This parameter is not supported (Fixed in 192Kbps)
        CodecANRSetting sAinNRSetting;          ///< audio noise reduction settings
    UINT32  uiAinG711Sample;                ///< only for G711 , set sample per byte (default 1024)
    CodecAinCodec uiAinCodec;               ///< Audio input codec
    CodecAudFiltSetting sAinAudFiltSetting; ///< audio filter settings
    UINT32  uiAinAudSetting;                ///< Audio setting
} CodecAinSetting;

typedef struct {
	CodecAencFormat eAinFormat;             ///< Encoded format of audio frame
	UINT32 uiDataLength;                    ///< Audio frame data length
	UINT32 uiSeqNum;                        ///< Audio frame sequence number
	struct timeval sTimeStamp;              ///< Encoded timestamp
	UINT8 *puiStreamPayloadData;            ///< Pointer to payload data of audio frame
} CodecAencStream;

typedef enum {
	CODEC_AOUT_TARGET_LINEOUT,              ///< Audio output to line-out
	CODEC_AOUT_TARGET_SPEAKER,              ///< Audio output to speaker
	CODEC_AOUT_TARGET_MAX,
	ENUM_DUMMY4WORD(CodecAoutTarget)
} CodecAoutTarget;

typedef struct {

	UINT8 bAECEnable;		  ///< Enable/Disable the AEC
	UINT8 bLeakEstEnable;              ///< Enable/Disable the AEC leak estimate.
	INT32 iLeakEstValue;              ///< Initial Condition of the leak estimate. Value range 25 ~ 99.
	INT32 iNoiseCancelLevel;          ///< Defualt is -20dB. Suggest value range -3 ~ -40. Unit in dB.
	INT32 iEchoCancelLevel;           ///< Defualt is -50dB. Suggest value range -30 ~ -60. Unit in dB.
	UINT32 uiFilterLen;               ///< Set AEC internal Filter Length. Set to 0 is using default value. Default is 0.
	UINT32 uiFrameSize;		  ///< Set AEC internal Frame Size. Set to 0 is using default value. Default is 0.
	UINT32 uiNotchRadius;  	          ///< Set the Notch filter Radius. Value range 0 ~ 1000.

}CodecAECSetting;

/**
    CodecAoutSetting

    @note Because AEC is processed by CPU1, the recommended maximum sample rate is 32KHz. \n
    Please setup audio input format as PCM when AEC is enabled.
*/
typedef struct {
	CodecAoutTarget eAoutTarget;            ///< Audio output target
	CodecAioSampleRate eAoutSampleRate;     ///< Audio output sample rate. This parameter is not supported (It is controlled by audio input).
	UINT32 uiAoutVolume;                    ///< Audio output volume. Range: 0~64, 0 means mute
	CodecAioBitWidth eAoutBitWidth;         ///< Sampling precision. This parameter is not supported (Fixed to #CODEC_AIO_BIT_WIDTH_16BIT).
	CodecAioSoundMode eAoutSoundMode;       ///< Sound channel mode
	CodecAencFormat eAoutFormat;            ///< Audio output data encoded format.
        CodecAECSetting sAoutAECSetting;        ///< Audio AEC Setting structure
} CodecAoutSetting;

typedef struct {
	UINT32 uiDataLength;                    ///< Payload data length
	UINT8 *puiStreamPayloadData;            ///< Pointer to audio output data
} CodecAoutStream;

typedef struct {
	UINT8 bStart;                            ///< Audio output to line-out
} CodecAoutStatus;


/**
    CodecVencSetting

    @note HDMI output is only supported in two stream mode(setup in uItron)
*/
typedef enum {
	CODEC_VOUT_INTERFACE_CVBS_NTSC,             ///< CVBS with NTSC
	CODEC_VOUT_INTERFACE_CVBS_PAL,              ///< CVBS with PAL
	CODEC_VOUT_INTERFACE_LCD,                   ///<
	//CODEC_VOUT_INTERFACE_HDMI_720x480P60,     ///< HDMI with 720x480p60
	CODEC_VOUT_INTERFACE_HDMI_1280x720P30,      ///< HDMI with 1280x720P30
	CODEC_VOUT_INTERFACE_HDMI_1920x1080P30,     ///< HDMI with 1920x1080P30
	CODEC_VOUT_INTERFACE_HDMI_1920x1080I60,     ///< HDMI with 1920x1080i60
	CODEC_VOUT_INTERFACE_HDMI_3840x2160P30,     ///< HDMI with 3840x2160P30
	CODEC_VOUT_INTERFACE_HDMI_3840x2160P25,     ///< HDMI with 3840x2160P25
	CODEC_VOUT_INTERFACE_HDMI_3840x2160P24,     ///< HDMI with 3840x2160P24
	CODEC_VOUT_INTERFACE_HDMI_4096x2160P30,     ///< HDMI with 4096x2160P30
	CODEC_VOUT_INTERFACE_HDMI_4096x2160P25,     ///< HDMI with 4096x2160P25
	CODEC_VOUT_INTERFACE_HDMI_4096x2160P24,     ///< HDMI with 4096x2160P24
	CODEC_VOUT_INTERFACE_AUTO,             ///< monitor auto detection
	CODEC_VOUT_INTERFACE_MAX,
	ENUM_DUMMY4WORD(CodecVoutInterface)
} CodecVoutInterface;
/*
typedef enum {
    CODEC_VOUT_INTERFACE_CVBS_NTSC,
    CODEC_VOUT_INTERFACE_CVBS_PAL,
    CODEC_VOUT_INTERFACE_HDMI_720x480P60,
    CODEC_VOUT_INTERFACE_HDMI_1920x1080I60,
    CODEC_VOUT_INTERFACE_HDMI_AUTO,
    CODEC_VOUT_INTERFACE_MAX,
    ENUM_DUMMY4WORD(CodecVoutInterface)
} CodecVoutInterface;
*/
typedef struct {
	CodecVoutInterface eVoutInterface;      ///< Video output enable
	INT32 iChannel;                         ///< Video output interface
//	UINT8  bEnable;                         ///< Video output Enable/ Disable
} CodecVoutSetting;

typedef enum {
	Codec_OSD_TEXT_POSITION_UPPER_LEFT,
	Codec_OSD_TEXT_POSITION_UPPER_RIGHT,
	Codec_OSD_TEXT_POSITION_LOWER_LEFT,
	Codec_OSD_TEXT_POSITION_LOWER_RIGHT,
	Codec_OSD_TEXT_POSITION_MAX,
	ENUM_DUMMY4WORD(CodecOSDTextPosition)
} CodecOSDTextPosition;

typedef struct {
	UINT32 uiSrcVideoID;    ///< The source video channel, the blending channel. Range: 0~2
	UINT32 uiDesVideoID;    ///< The destination video channel, copy data to the blending channel
	UINT8 bEnable;           ///< Enable / disable blending function. NVT_TRUE: enable, NVT_FALSE: disable
	UINT32 uiAlpha;         ///< A: is the source image. B: is alpha value. C: is the destination image. The copy blending weight is (B/256)*A + ((256-B)/256)*C. Range: 0~256
} CodecBlendSetting;


typedef enum {
	NVTMPP_DDR_1                    =   0, ///< DDR1
	NVTMPP_DDR_2                    =   1, ///< DDR2
	NVTMPP_DDR_MAX = NVTMPP_DDR_2,
	ENUM_DUMMY4WORD(NVTMPP_DDR)
} NVTMPP_DDR;

#if 0
//already declaration in \na51023_bsp\uitron\Include\Common
typedef struct {
	INT32  w;       ///< Rectangle width
	INT32  h;       ///< Rectangle height
} ISIZE, *PISIZE;
#endif

typedef struct{

	BOOL  bEanble;         ///< Enable Common pool
	ISIZE sResolution;     ///< set Common Pool resolution , normal pool size be width*height*1.5
	UINT32 uiCount;        ///< the number of common pool for this Pool index
	NVTMPP_DDR ddr;        ///< common pool is from DDR1 or DDR2

}ComBufInfo;

/**
*MD struct define
*
**/
#define MD_MAX_SUB_REGION_NUM 4
#define MD_USR_MAX_LV_NUM 4
#define MD_USR_MAX_OBJ_NUM 128

typedef struct {
	INT32  iX;
	INT32  iY;
	INT32  iWidth;
	INT32  iHeight;
	INT32  iLevel;
} USR_DISTRICT;

typedef struct {
	INT32  iIsEnabled;
	INT32  iStartBlockX;
	INT32  iStartBlockY;
	INT32  iEndBlockX;
	INT32  iEndBlockY;
	INT32  iAlarmTh;
	INT32  iAlarm;
} USR_RECT;

typedef struct {
	UINT32  uiAlpha;
	UINT32  uiTb;
	UINT32  uiSigma;
	UINT32  uiInitVal;
	UINT32  uiTg;
	UINT32  uiLumDiffTh;
	UINT32  uiTexDiffTh;
	UINT32  uiTexTh;
	UINT32  uiTexRatioTh;
	UINT32  uiLvlGmMd2Th;
	UINT32  reserved[6];
} USR_LV_INFO;

typedef struct {
	USR_RECT SubRegion[MD_USR_MAX_REGION];
	UINT32  uiSubRegionNum;
	UINT32  uiTbg;
	UINT32  uiTimePeriod;
	UINT32  uiSceneChangeTh;
	//UINT32  stripe_num;
	//====below may be deleted...
	UINT32  uiHeight;           /*  user define height
                                        suggest not change default value    */
	UINT32  uiWidth;            /*  user define width
                                        suggest not change default value    */
	UINT32  uiMbHeight;        /*  user define mb height (suggest value = k_mb_widthh * 2)
                                        too large would affect the accuracy of motion detection */
	UINT32  uiMbWidth;         /*  user define mb width (suggest value = k_mb_widthh * 2)
                                        too large would affect the accuracy of motion detection */
	UINT32  uiFrameCount;
	UINT32  uiTrainingTime;      /*  Needed training frames number for motion detection
                                        application  */
	UINT32  uiMdType;
	UINT32  uiRotate;
	UINT32  uiFlip;
	UINT32  uiEnableType;
	UINT32  uiMbWNum;
	UINT32  uiMbHNum;
	INT32   iAlarmTh;

	UINT32  uiObjectSize;        /*  object size threshold   */
	UINT32  uiCrossType;         /*  none:0 cross_Entrance:1 cross_region:2  */
	UINT32  uiOverlapTh;         /*  object overlapping threshold  */
	UINT32  uiBlk1X;
	UINT32  uiBlk1Y;
	UINT32  uiBlk2X;
	UINT32  uiBlk2Y;
	UINT32  uiBlk3X;
	UINT32  uiBlk3Y;
	UINT32  uiBlk4X;
	UINT32  uiBlk4Y;

	UINT32  uiSinglePeopleSize;
	UINT32  uiSingleW;
	UINT32  uiSingleH;
	UINT32  uiEvtPostprocessing;
	UINT32  reserved[5];
} USR_GLOBAL_INFO;

typedef struct {
	USR_GLOBAL_INFO  sGlobalInfo;
	USR_LV_INFO      sLvInfo[MD_USR_MAX_LV_NUM];
	UINT32  		 reserved[6];
} USR_PARAM_INFO;

typedef struct {
	UINT32 uiFps; //fps=0 means turn off
	UINT32 uiMdMode;
	UINT32 uiDistRegionNu;
	USR_DISTRICT sDistRegion[MD_MAX_SUB_REGION_NUM];
	USR_PARAM_INFO sParamInfo;
	UINT32  reserved[6];
} MD_PARAM_INFO;

typedef struct {
	UINT32 uiBcWidth;
	UINT32 uiBcHeight;
	UINT8  uBcReset;
	UINT8  uBcRsltNew;
	UINT8  uBcUpdateProb;
	UINT8  uBcUpdateNeighborEn;
	UINT8  uBcDeghostEn;
	UINT32 uiBcMaxFgFrmNum;
	UINT32 uiBcMinObjAreaPct;
	UINT32 uiBcThCrossFrmNum;
	UINT8  uBcThFeature;
	UINT8  uBcSensi;
	UINT32 uiReserved[6];
} BC_USR_PARAM;

typedef struct {
	UINT32 uiFps; //fps=0 means turn off
	UINT32 uiBcMode;
	BC_USR_PARAM sParamInfo;
	UINT32  reserved[6];
} BC_PARAM_INFO;

typedef struct {
	UINT32  tbg;
	UINT32  time_period;    //====below may be deleted...
	UINT32  u_height;
	UINT32  u_width;
	UINT32  u_mb_height;
	UINT32  u_mb_width;
	UINT32  md_type;
	UINT32  mb_w_num;
	UINT32  mb_h_num;
	UINT32  event_postprocessing;
	UINT32  reserved[5];
} ROI_MD_INFO;

typedef struct {
	ROI_MD_INFO  global_info;
	USR_LV_INFO      lv_info[MD_USR_MAX_LV_NUM];
	UINT32           reserved[6];
} ROI_USR_PARAM_INFO;

typedef struct {
	UINT32 uiEnable;
	UINT32 uiDistRegionNu;
	USR_DISTRICT sDistRegion[MD_MAX_SUB_REGION_NUM];
	ROI_USR_PARAM_INFO sParamInfo;
	UINT32  reserved[6];
} ROI_MD_PARAM_INFO;

typedef struct {
	unsigned int en_auto_adj_param;
	unsigned int en_ref_md;
	unsigned int en_too_light;
	unsigned int en_too_dark;
	unsigned int en_occlusion;
	unsigned int en_blur;
	unsigned int en_sc_chg;
	unsigned int en_md_clr;
} VQAEnable;

typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned int mb_x_size;
	unsigned int mb_y_size;
	unsigned int auto_adj_period;
	unsigned int update_frame_num;
	unsigned int g_alarm_frame_num;
} VQAGlobal;

typedef struct {
	unsigned int too_light_strength_th;
	unsigned int too_light_cover_th;
	unsigned int too_light_alarm_times;
	unsigned int too_dark_strength_th;
	unsigned int too_dark_cover_th;
	unsigned int too_dark_alarm_times;
	unsigned int occlusion_strength_th;
	unsigned int occlusion_diff_th;
	unsigned int occlusion_cover_th;
	unsigned int occlusion_alarm_times;
} VQALight;

typedef struct {
	unsigned int blur_strength;
	unsigned int blur_cover_th;
	unsigned int blur_alarm_times;
} VQAContrast;

typedef struct {
	unsigned int sc_cover_th;
	unsigned int sc_alarm_times;
} VQASceneChange;

typedef struct {
	unsigned int fps; //fps=0 means turn off
	VQAEnable enable_param;
	VQAGlobal global_param;
	VQALight light_param;
	VQAContrast contrast_param;
	VQASceneChange sc_param;
} IVSVQAParam;

typedef struct {
	unsigned int fps; //fps=0 means turn off
	unsigned int reserved[9];
} IVSPDParam;



typedef struct {
        UINT32  x;      ///< X coordinate of the top-left point of the rectangle
        UINT32  y;      ///< Y coordinate of the top-left point of the rectangle
        UINT32  width;      ///< Rectangle width
        UINT32  height;      ///< Rectangle height
} PDURECT;

#define NVT_PD_MAX_NUM	32
typedef struct {
	unsigned int timestamp;
	unsigned int people_counts;
	PDURECT people[NVT_PD_MAX_NUM];
} NVT_PD_RESULT_T;


typedef struct {
	UINT32 uiMaxFaceNum;		///< Maximum number of detected faces
	UINT32 uiFaceScale; 		///< the value defining the minimal face to detect releative to the maximum of image width and weight.
	UINT32 uiProcessRate;		///< the process rate of FD_Process(), if set value 3, means calling the function FD_Process() 3 times will run the face detection once.
	UINT32 uiDrawFace;			///< Draw face rectangle on the streaming or not, You have to enable IPCAM_FD_DRAW_VIDEO/STAMP_FUNC first.
} CodecFDSetting;

typedef struct {
        UINT32  x;      ///< X coordinate of the top-left point of the rectangle
        UINT32  y;      ///< Y coordinate of the top-left point of the rectangle
        UINT32  w;      ///< Rectangle width
        UINT32  h;      ///< Rectangle height
} FDURECT;

typedef struct {
        char Event;
        UINT32 uiFaceNumber;
        FDURECT info[FD_NUM_MAX];
} CodecFDData;

typedef struct {
        UINT32 uiFtgMaxFaceNum;     ///< Maximum number of tracking faces. Range: 1~36
        UINT32 uiFtgFaceScale;      ///< The ratio of minimum face size to maximum image size. If the value is 15, face size is 80*80. Range: 0~15
        UINT32 uiFtgAdaBoostType;   ///< not support now, default value is 3
        UINT32 uiFtgProcessRate ;   ///< config od the process rate of frame, set 3 means 3 frames detect OD 1 time.
        UINT32 uiFtgScoreThreshold; ///< landmark score threshold range [-1024 1024]
        UINT32 uiFtgOTLife;         ///< config max obj life OT
        UINT32 uiFtgODLife;         ///< config max obj life OD
        UINT32 uiFtgOutObjScale;    ///< config output obj context range, can be set to 0~100, e.g. set 15 means output obj box with 1.5 times the original size and context range.
        UINT32 uiFtgLandmarkFrq;    ///< How many frame to get landmark info
} CodecFTGSetting;

typedef struct {
        UINT16 uiCoordinateX;       ///< X coordinate of the top-left point of the detected face
        UINT16 uiCoordinateY;       ///< Y coordinate of the top-left point of the detected face
        UINT16 uiWidth;             ///< Width of the detected face
        UINT16 uiHeight;            ///< Height of the detected face
        UINT32 id;                  ///< The face id which be detected
        UINT16 type;                ///< result type: 1--first appearance, other--not first.
        UINT16 clarity;             ///< result clarity
        UINT16 luma;                ///< result luma
        INT32  angle_horizontal;    ///< result face horizontal angle
        INT32  angle_vertical;      ///< result face vertical angle
        INT32  angle_roll;          ///< result face roll angle
        INT32  score;               ///< result face quality reference score
} CodecFTGObjData;

typedef struct {
        UINT32 uiFaceNum;               ///< Number of detected face
        CodecFTGObjData *psFTGObjData;    ///< Pointer to object data of tracking face
        UINT32 uiTimeStamp;             ///< Time stamp of tracking face (This parameter is not supported.)
} CodecFTGData;

// follow with uITRON
#define FTG_OVERRALL_MAX_BUF_SIZE      250*1024
#define FTG_FACE_MAX_BUF_SIZE      100*1024
#define FTG_MAX_FACE_NUM      3

typedef struct {
        UINT32 score;                   ///< The face score in this picture
        char overrall_pic[FTG_OVERRALL_MAX_BUF_SIZE];   ///< The actually overall picture memory address
        UINT32 overrall_size;           ///< The overall picture size
        char face_pic[FTG_FACE_MAX_BUF_SIZE];           ///< The actually face picture memory address
        UINT32 face_size;               ///< The face picture size
} CodecFTG_PIC_DATA;

typedef struct {
        UINT32 id;                      ///< The face id
        UINT32 pic_num;                 ///< The number of pictures which are saved. The max number is 3
        CodecFTG_PIC_DATA pic_list[FTG_MAX_FACE_NUM]; ///< Picture list. Include max 3 overall pictures and 3 face pictures
} CodecFTG_SNAPSHOT;

typedef struct {
        UINT16 uiCoordinateX;
        UINT16 uiCoordinateY;
        UINT16 uiWidth;
        UINT16 uiHeight;
} CodecIAObjData;

typedef struct {
	UINT32	EventFlag;
	UINT32	NotifyFlag;
	UINT32	SerialNumber;
	char EventData[EVENT_BUF_SIZE];
} CodecEventInfo;

//MD
#define NVT_MD_MAX_SUB_REGION_NUM 4
#define NVT_MD_USR_MAX_OBJ_NUM 128
#define NVT_MD_USR_MAX_EVT_SIZE 8192

typedef struct {
	int start_x;
	int start_y;
	int end_x;
	int end_y;
	int label;
    unsigned int reserved[6];
} nvt_md_usr_obj_t;

typedef struct {
	unsigned int motion_alarm;        ///< alarm
	unsigned int in_num;
	unsigned int out_num;
	unsigned int obj_num;
	unsigned char status;
	unsigned int sub_motion_alarm[NVT_MD_MAX_SUB_REGION_NUM]; ///< sub alarm
	nvt_md_usr_obj_t obj[NVT_MD_USR_MAX_OBJ_NUM]; ///< obj info
	unsigned char evt_map_addr[NVT_MD_USR_MAX_EVT_SIZE];        ///< evt map
    unsigned int reserved[6];
} nvt_md_usr_rst_t;

typedef struct {
	char md_event;
	unsigned int timestamp;
	nvt_md_usr_rst_t md_reslt;
    unsigned int reserved[5];
} nvt_md_data_t;

/******/
// video encoder
/**
    Get current video encoder setting

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[out] psVencSetting   Pointer to the data structure of video encoder setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetVencSetting(UINT32 uiVencChannelID, CodecVencSetting *psVencSetting);

/**
    Set new video encoder setting

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[in] psVencSetting    Pointer to the data structure of video encoder setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetVencSetting(UINT32 uiVencChannelID, CodecVencSetting *psVencSetting);

/**
    Get current video encoder ROI setting

    @param[in] uiVencChannelID      Video encoder channel ID
    @param[out] psVencROISetting    Pointer to the data structure of video encoder ROI setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetVencROISetting(UINT32 uiVencChannelID, CodecVencROISetting *psVencROISetting);

/**
    Set new video encoder ROI setting

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[in] psVencROISetting Pointer to the data structure of video encoder ROI setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetVencROISetting(UINT32 uiVencChannelID, CodecVencROISetting *psVencROISetting);

/**
    Start video encoder

    @param[in] uiVencChannelID  Video encoder channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_StartVenc(UINT32 uiVencChannelID);

/**
    Stop video encoder

    @param[in] uiVencChannelID  Video encoder channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_StopVenc(UINT32 uiVencChannelID);

/**
    Request next frame encoded as I-frame

    @param[in] uiVencChannelID  Video encoder channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_RequestVencIFrame(UINT32 uiVencChannelID);
/**
    Get video encoded data

    @note
    -# This function must be used after video encoder start, and not request I frame in first time
    -# In order to keep encoder flow non-blocking, encoded buffer not be locked even if this buffer is being gotten by user. \n
    The encoded buffer always is refreshed in its turn. If need to use encoded buffer more than 5 seconds, \n
    it's recommended to set bCopyBuf as NVT_TRUE and get stream from indirect memory buffer. \n
    This way has additional memory copy effort, but can ensure that payload data not be refreshed.

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[out] psVencStream    Pointer to the data structure of video encoded data
    @param[in] uiTimeOutMs      Function time out, 0 means block mode(Unit is millisecond)
    @param[in] bCopyBuf         Payload data handle method, NVT_TRUE means copy data into additional memory buffer *

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetVencStream_Without_RequestIFrame(UINT32 uiVencChannelID, CodecVencStream *psVencStream, UINT32 uiTimeOutMs, UINT8 bCopyBuf);

/**
    Get video encoded data

    @note
    -# This function must be used after video encoder start
    -# In order to keep encoder flow non-blocking, encoded buffer not be locked even if this buffer is being gotten by user. \n
    The encoded buffer always is refreshed in its turn. If need to use encoded buffer more than 5 seconds, \n
    it's recommended to set bCopyBuf as NVT_TRUE and get stream from indirect memory buffer. \n
    This way has additional memory copy effort, but can ensure that payload data not be refreshed.

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[out] psVencStream    Pointer to the data structure of video encoded data
    @param[in] uiTimeOutMs      Function time out, 0 means block mode(Unit is millisecond)
    @param[in] bCopyBuf         Payload data handle method, NVT_TRUE means copy data into additional memory buffer *

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetVencStream(UINT32 uiVencChannelID, CodecVencStream *psVencStream, UINT32 uiTimeOutMs, UINT8 bCopyBuf);

/**
    Release video encoded data (This function must be used after video encoder start.)

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[in] psVencStream     Pointer to the data structure of video encoded data

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_ReleaseVencStream(UINT32 uiVencChannelID, CodecVencStream *psVencStream);

/**
    Get parameter set for H.264

    @note
    -# This function must be used after video encoder start

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[out] psVencParamSet  Pointer to the data structure of parameter set

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_RequestVencParamSet(UINT32 uiVencChannelID, CodecVencParamSet *psVencParamSet);

/**
    Get one snapshot that is encoded to JPEG

    @note
    -# This function must be used after video encoder start

    @param[in] psVencSnapshotConfig Pointer to the data structure of JPEG snapshot configuration
    @param[out] psVencSnapshot      Pointer to the data structure of JPEG snapshot

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_RequestVencJPEG(CodecVencSnapshotConfig *psVencSnapshotConfig, CodecVencSnapshot *psVencSnapshot);

/**
    Release the JPEG snapshot resource

    @param[in] void

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_ReleaseVencJPEG(void);

/**
    Get video encoder status

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[out] psVencStatus    Pointer to the data structure of encoder status

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetVencStatus(UINT32 uiVencChannelID, CodecVencStatus *psVencStatus);



/**
    Get video encoder stream handle

    @param[in] uiVencChannelID  Video encoder channel ID

    @return video stream handle
*/
extern INT32 NvtCodec_GetVencStreamHandle(UINT32 uiVencChannelID);


// video output
/**
    Get current video output setting

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[out] psVoutSetting   Pointer to the data structure of video output setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetVoutSetting(UINT32 uiVoutChannelID, CodecVoutSetting *psVoutSetting);

/**
    Set new video output setting

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[in] psVoutSetting    Pointer to the data structure of video output setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetVoutSetting(UINT32 uiVoutChannelID, CodecVoutSetting *psVoutSetting);

/**
    set display Enable / Disable

    @param[in] bEnable  enable/disable disaplay

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetVoutEnable(UINT8 bEnable);

// audio input
/**
    Get current audio input setting

    @param[in] uiAinChannelID   Audio input channel ID
    @param[out] psAinSetting    Pointer to the data structure of audio input setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/

extern NVTRET NvtCodec_GetAinSetting(UINT32 uiAinChannelID, CodecAinSetting *psAinSetting);

/**
    Set new audio input setting

    @param[in] uiAinChannelID   Audio input channel ID
    @param[in] psAinSetting     Pointer to the data structure of audio input setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetAinSetting(UINT32 uiAinChannelID, CodecAinSetting *psAinSetting);

/**
    Start audio input

    @param[in] uiAinChannelID   Audio input channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_StartAin(UINT32 uiAinChannelID);

/**
    Stop audio input

    @param[in] uiAinChannelID   Audio input channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_StopAin(UINT32 uiAinChannelID);

/**
    Get audio encoded data

    @note
    -# This function must be used after audio input start

    @param[in] uiAinChannelID   Audio input channel ID
    @param[out] psAencStream    Pointer to the data structure of audio encoded data
    @param[in] uiTimeOutMs      Function time out, 0 means block mode(Unit is millisecond)
    @param[in] bCopyBuf         Payload data handle method, NVT_TRUE means copy data into additional memory buffer *

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetAinStream(UINT32 uiAinChannelID, CodecAencStream *psAencStream, UINT32 uiTimeOutMs, UINT8 bCopyBuf);

/**
    Release audio encoded data

    @note
    -# This function must be used after audio input start

    @param[in] uiAinChannelID   Audio input channel ID
    @param[in] psAencStream     Pointer to the data structure of audio encoded data

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_ReleaseAinStream(UINT32 uiAinChannelID, CodecAencStream *psAencStream);


/**
    Get audio encoder stream handle

    @param[in] uiAinChannelID   Audio input channel ID

    @return audio stream handle
*/
extern INT32 NvtCodec_GetAinStreamHandle(UINT32 uiAinChannelID);


// audio output
/**
    Get current audio output setting.

    @param[in] uiAoutChannelID  Audio output channel ID
    @param[out] psAoutSetting   Pointer to the data structure of audio output setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetAoutSetting(UINT32 uiAoutChannelID, CodecAoutSetting *psAoutSetting);

/**
    Set new audio output setting.

    @param[in] uiAoutChannelID  Audio output channel ID
    @param[in] psAoutSetting    Pointer to the data structure of audio output setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetAoutSetting(UINT32 uiAoutChannelID, CodecAoutSetting *psAoutSetting);

/**
    Start audio output.

    @param[in] uiAoutChannelID  Audio output channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_StartAout(UINT32 uiAoutChannelID);

/**
    Stop audio output.

    @param[in] uiAoutChannelID  Audio output channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_StopAout(UINT32 uiAoutChannelID);

/**
    Play audio data

    @note
    -# This function must be used after audio output start.

    @param[in] uiAoutChannelID      Audio output channel ID
    @param[in] psAoutStream         Pointer to the data structure of audio output data
    @param[out] puiSendStreamSize   The size of audio data that is send to play buffer

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SendAoutStream(UINT32 uiAoutChannelID, CodecAoutStream *psAoutStream, UINT32 *puiSendStreamSize);

/**
    Get audio output status

    @param[in] uiAoutChannelID  Audio output channel ID
    @param[out] psAoutStatus    Pointer to the data structure of audio output status

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetAoutStatus(UINT32 uiAoutChannelID, CodecAoutStatus *psAoutStatus);

/**
    Get audio decode stream handle

    @param[in] uiAoutChannelID   Audio output channel ID

    @return audio stream handle
*/
extern INT32 NvtCodec_GetAoutStreamHandle(UINT32 uiAoutChannelID);

/**
    Set audio input volumeg

    @param[in] uiAinChannelID   Audio input channel ID
    @param[in] uiAinVolume      Audio input volume ( 0 ~ 8, 0 = mute )

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetAinVolume(UINT32 uiAinChannelID, UINT32 uiAinVolume );

/**
    Set new audio out volume

    @param[in] uiAoutChannelID   Audio output channel ID
    @param[in] uiAoutVolume      Audio output volume

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetAoutVolume(UINT32 uiAoutChannelID, UINT32 uiAoutVolume);
// overlay
/**
    application-wide initialization

    @param[in] uiMaxSize  max osd size : 2 * max width * max height

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_InitVds(unsigned int uiMaxSize);

/**
    application-wide un-initialization

    @param[in] void

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_UninitVds(void);

/**
    application-wide ping-pong buffer configuration

    @param[in] phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in] id     serial number
    @param[in] width  max osd width for image #id
    @param[in] height max osd height for image #id

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AllocOSDBuf(VDS_PHASE phase, int id, unsigned int width, unsigned int height);

/**
    add/update an mask

    @param[in] image mask configuration

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AddVdsMask(CodecOSDIMAGE *image);

/**
    add/update an osd

    @param[in] image osd configuration

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AddVdsOSD(CodecOSDIMAGE *image);

/**
    draw ext mask for the specified stream

    @param[in] vid stream number

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SwapVdsExtMask(int vid);

/**
    draw ps mask for the specified ipl

    @param[in] vid stream number

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SwapVdsPsMask(int vid);

/**
    draw btn ext osd for the specified stream

    @param[in] vid stream number

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SwapVdsBtnExtOSD(int vid);

/**
    draw ps osd for the specified stream

    @param[in] vid stream number

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SwapVdsPsOSD(int vid);

/**
    clear all osd and mask

    @param[in] void

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_ClearVds(void);

/**
    free ping pong buffer

    @param[in] if actually return ping pong buffer to system

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_FreeBuffer(int release);

/**
    defrag ping pong buffer

    @param[in] void

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_Defragment(void);

/**
    delete an osd

    @param[in] phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in] id     serial number

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_DelVdsOSD(VDS_PHASE phase, int id);

/**
    Get information of images stored in videosprite

    @param[in]      phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in]      mask   requery mask(1) or osd(0)
    @param[out]     image  information of images stored in videosprite
    @param[in,out]  num    input:number of input image ; output:number of images stored in videosprite

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetImages(VDS_PHASE phase, int mask, CodecOSDIMAGE *image, int *num);

/**
    Get free ping pong buffer of an osd

    @param[in]      phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in]      id     id of osd
    @param[out]     addr   address of the ping pong buffer
    @param[out]     size   size of the ping pong buffer

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetLockPingPong(VDS_PHASE phase, int id, unsigned int *addr, unsigned int *size);

/**
    Release resuorce locked by NvtCodec_GetLockPingPong()

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_UnlockPingPong(void);

/**
    application-wide ping-pong buffer configuration

    @param[in] phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in] id     serial number
    @param[in] width  max osd width for image #id
    @param[in] height max osd height for image #id

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AllocPingPongBuf(VDS_PHASE phase, int id, unsigned int width, unsigned int height);

/**
    application-wide scale buffer configuration

    @param[in] phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in] id     serial number
    @param[in] PreW   max width of input osd for image #id
    @param[in] PreH   max height of intpu osd for image #id
    @param[in] PostW  max width of output osd for image #id after scaling
    @param[in] PostH  max height of output osd for image #id after scaling

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AllocScaleBuf(VDS_PHASE phase, int id, unsigned int PreW, unsigned int PreH, unsigned int PostW, unsigned int PostH);

/**
    application-wide single buffer configuration

    @param[in] phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in] id     serial number
    @param[in] width  max osd width for image #id
    @param[in] height max osd height for image #id

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AllocSingleBuf(VDS_PHASE phase, int id, unsigned int width, unsigned int height);

/**
    add/update an osd of scale buffer with a specified scale ratio

    @param[in] image osd configuration
    @param[in] ratio scale up ratio : 1 2 4 8 16

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AddVdsScaleOSD(CodecOSDIMAGE *image, int ratio);

/**
    allocate mosaic's buffer from uitron for VDS_PHASE_EXT

    @param[in] uiMaxSize how many bytes to allocate

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_InitMosaicBuf(unsigned int uiMaxSize);

/**
    application-wide mosaic buffer configuration

    @param[in] phase  only VDS_PHASE_EXT is valid because VDS_PHASE_PS allocates mosaic buffer in hardware driver
    @param[in] vid    streaming number
    @param[in] width  streaming width
    @param[in] height streaming height

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AllocMosaicBuf(VDS_PHASE phase, int vid, unsigned int width, unsigned int height);

/**
    free mosaic buffer

    @param[in] release if actually return mosaic buffer to system

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_FreeMosaicBuffer(int release);

/**
    set mosaic block size

    @param[in] phase  VDS_PHASE_EXT VDS_PHASE_PS
    @param[in] vid    for VDS_PHASE_EXT it's vid ; for VDS_PHASE_PS it's ipl_id
    @param[in] blksz  8/16/32/64

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SetVdsMosaicBlksz(VDS_PHASE phase, int vid, unsigned int blksz);
/**
    set motion info for alg

    @param[in] Background Construction param

    @return NvtErrorCode (Refer to nvt_type.h)
*/
ER NvtCodec_SetBcCfg(BC_PARAM_INFO *pBcInfo);


/**
    Enable/Disable BC information include in YUV data

    @param[in] bEnable   0:Disable YUV include BC info, 1: ENable YUV include BC info

    Please make sure YUV path is Enable!!!!

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetBcYUV(UINT32 uiEnable);


/**
    set motion info for alg

    @param[in] motion lib param

    @return NvtErrorCode (Refer to nvt_type.h)
*/
ER NvtCodec_SetMotionCfg(MD_PARAM_INFO *pMdInfo);

//blending
/**
    Get the current blending information

    @param[in] uiBlendChannelID     Blending channel ID (Reserved now)
    @param[out] psBlendSetting      Pointer to the data structure of Blending configuration

    @return NvtErrorCode (Refer to nvt_type.h)
*/
//extern NVTRET NvtCodec_GetBlendSetting(UINT32 uiBlendChannelID, CodecBlendSetting *psBlendSetting);

/**
    Set the blending information

    @param[in] uiBlendChannelID     Blending channel ID (Reserved now)
    @param[in] psBlendSetting       Pointer to the data structure of Blending configuration

    @return NvtErrorCode (Refer to nvt_type.h)
*/
//extern NVTRET NvtCodec_SetBlendSetting(UINT32 uiBlendChannelID, CodecBlendSetting *psBlendSetting);

//extern NVTRET NvtCodec_SetBlendSetting(UINT32 uiBlendChannelID, CodecBlendSetting *psBlendSetting);

/**
    start event handler by using nvtevent

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_EventHandler_Init(void);

/**
    stop event handler by using nvtevent

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_EventHandler_Exit(void);

/**
    set VQA parameters for alg

    @param[in] vqa param

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SetVqaParam(IVSVQAParam *sIVSVQAParam);

/**
    get VQA result

    @param[in] pointer of vqa result
    @param[in] timeout in millisec

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetVqaResult(IPCAM_VQA_RESULT *pVqaRes, int iTimeOutMillisec);

/*
	Exit wait VQA event
*/
extern ER NvtCodec_ExitVqa(void);

/**
    set PD parameters for alg

    @param[in] pd param

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SetPdParam(IVSPDParam *sIVSPDParam);

/**
    get PD result

    @param[in] pointer of pd result
    @param[in] timeout in millisec

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetPdResult(IPCAM_PD_RESULT *pPdRes, int iTimeOutMillisec);

/*
	Exit wait PD event
*/
extern ER NvtCodec_ExitPd(void);
/*
    Wait MD event and get MD result
*/
extern ER NvtCodec_GetMdData(IPCAM_MD_RESULT *md_data, int iTimeOutMillisec);
/*
	Exit wait MD event
*/
extern ER NvtCodec_ExitMd(void);
/**
    Start face detection

    @param[in] uiFDIdx      Index of face detection window (Only supports one detection window)

    @return NvtErrorCode (Refer to nvt_type.h)
*/
/*
    Wait BC event and get BC result
*/
extern ER NvtCodec_GetBcData(IPCAM_BC_RESULT *bc_data, int iTimeOutMillisec);
/*
	Exit wait BC event
*/
extern ER NvtCodec_ExitBc(void);

extern ER NvtCodec_StartFD(UINT32 uiFDIdx);
/**
    Stop face detection

    @param[in] uiFDIdx      Index of face detection window (Only supports one detection window)

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_StopFD(UINT32 uiFDIdx);
/**
    Get current face detection setting

    @param[in] uiFDIdx      Index of face detection window (Only supports one detection window)
    @param[in] psFDSetting  Pointer to the data structure of face detection setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetFDSetting(UINT32 uiFDIdx, CodecFDSetting *psFDSetting);
/**
    Set new face detection setting

    @param[in] uiFDIdx      Index of face detection window (Only supports one detection window)
    @param[in] psFDSetting  Pointer to the data structure of face detection setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SetFDSetting(UINT32 uiFDIdx, CodecFDSetting *psFDSetting);
/**
    Release face detection result

    @note
    -# This function must be used after face detection start

    @param[in] uiFDIdx          Index of face detection window (Only supports one detection window)
    @param[in] fd_data         Pointer to the data structure of face detection result

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetFDData(IPCAM_FD_INFO *fd_data, int iTimeOutMillisec);
/*
	Exit wait FD event
*/
extern ER NvtCodec_ExitFd(void);

// FTG (Face Tracking Grading)
/**
    Get current face tracking setting

    @param[in] uiFTGIdx         Index of face tracking frame.
    @param[in] psFTGSetting     Pointer to the data structure of face tracking setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetFTGSetting(UINT32 uiFTGIdx, CodecFTGSetting *psFTGSetting);

/**
    Set new face tracking setting

    @param[in] uiFTGIdx         Index of face tracking frame.
    @param[in] psFTGSetting     Pointer to the data structure of face tracking setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SetFTGSetting(UINT32 uiFTGIdx, CodecFTGSetting *psFTGSetting);

/**
    Start face tracking and grading

    @param[in] uiFTGIdx         Index of face tracking frame.

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_StartFTG(void);

/**
    Stop face tracking and grading

    @param[in] uiFTGIdx         Index of face tracking frame.

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_StopFTG(UINT32 uiFTGIdx);

/**
    Get face tracking grading result

    @note
    -# This function must be used after face tracking grading start

    @param[in] uiFTGIdx         Index of face tracking frame.
    @param[in] psFTGData        Pointer to the data structure of face tracking result
    @param[in] psTargetCoord    Mapping coordinate of face tracking result
    @param[in] bUpdated         Data update flag. NVT_TRUE means this data is new.

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetFTGData(IPCAM_FTG_RESULT *FTG_data, int iTimeOutMillisec);

/*
	Exit wait FD event
*/
extern ER NvtCodec_ExitFTG(void);

/*
    Get uitron common pool setting

    @param[in] uiBufIndex,  select common pool info which by uiBufIndex
    @param[out] psComBuf,   the pointer to the data structure of common buf info

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetComBufSetting(UINT32 uiBufIndex, ComBufInfo *psComBuf);


/*
    Set uitron common pool setting

    @param[in] uiBufIndex,  select common pool info which by uiBufIndex
    @param[in] psComBuf,   the pointer to the data structure of common buf info which want to set

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetComBufSetting(UINT32 uiBufIndex, ComBufInfo *psComBuf);


extern void NvtCodec_Video_Init(void);
extern void NvtCodec_Audio_Init(void);
#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTCODEC_H  */
