/*
    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.

    @file       ImageApp_MovieMulti.h
    @ingroup    mIImageApp

    @note       This header file is updated by ImageApp_MovieMulti 0.00.000

    @date       2018/05/15
*/

#ifndef IMAGEAPP_MOVIEMULTI_H
#define IMAGEAPP_MOVIEMULTI_H


#include "ImageStream.h"
#include "ImageApp_MovieCommon.h"
#include "ipl_alg_infor.h"
#include "ImageUnit_BsMux.h"

/**
    @addtogroup mIImageApp
*/
//@{

enum {
	MOVIEMULTI_PARAM_START = 0x0000ED000,
	// ipl group
	MOVIEMULTI_PARAM_IPL_GROUP_BEGIN = MOVIEMULTI_PARAM_START,
	MOVIEMULTI_PARAM_IPL_INFO = MOVIEMULTI_PARAM_IPL_GROUP_BEGIN,                   /// imagepipe
	MOVIEMULTI_PARAM_IPL_SCALE_QUALITY_RATIO,                                       /// imagepipe
	MOVIEMULTI_PARAM_IPL_SCALE_MATHOD_SMALL,                                        /// imagepipe
	MOVIEMULTI_PARAM_IPL_SCALE_MATHOD_LARGE,                                        /// imagepipe
	MOVIEMULTI_PARAM_IPL_FORCED_IMG_SIZE,                                           /// var + imagepipe
	MOVIEMULTI_PARAM_IPL_GROUP_END,
	// image group
	MOVIEMULTI_PARAM_IMG_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0100,
	MOVIEMULTI_PARAM_SIZE = MOVIEMULTI_PARAM_IMG_GROUP_BEGIN,                       /// vdoenc + bsmux
	MOVIEMULTI_PARAM_FRAMERATE,                                                     /// vdoenc + bsmux
	MOVIEMULTI_PARAM_DAR,                                                           /// vdoenc + bsmux
	MOVIEMULTI_PARAM_IMG_GROUP_END,
	// video codec group
	MOVIEMULTI_PARAM_CODEC_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0200,
	MOVIEMULTI_PARAM_VDO_ENCODER_OBJ = MOVIEMULTI_PARAM_CODEC_GROUP_BEGIN,          /// vdoenc
	MOVIEMULTI_PARAM_CODEC,                                                         /// vdoenc + bsmux
	MOVIEMULTI_PARAM_PROFILE,                                                       /// vdoenc
	MOVIEMULTI_PARAM_TARGETRATE,                                                    /// vdoenc + bsmux
	MOVIEMULTI_PARAM_INITQP,                                                        /// vdoenc
	MOVIEMULTI_PARAM_MINQP,                                                         /// vdoenc
	MOVIEMULTI_PARAM_MAXQP,                                                         /// vdoenc
	MOVIEMULTI_PARAM_AQ_INFO,                                                       /// vdoenc
	MOVIEMULTI_PARAM_CBR_INFO,                                                      /// vdoenc
	MOVIEMULTI_PARAM_ENCBUF_MS,                                                     /// vdoenc
	MOVIEMULTI_PARAM_ENCBUF_RESERVED_MS,                                            /// vdoenc
	MOVIEMULTI_PARAM_TIMELAPSE_TIME,                                                /// vdoenc
	MOVIEMULTI_PARAM_SKIP_FRAME,                                                    /// vdoenc
	MOVIEMULTI_PARAM_3DNR_CB,                                                       /// vdoenc
	MOVIEMULTI_PARAM_MIN_I_RATIO,                                                   /// vdoenc
	MOVIEMULTI_PARAM_MIN_P_RATIO,                                                   /// vdoenc
	MOVIEMULTI_PARAM_LEVEL_IDC,                                                     /// vdoenc
	MOVIEMULTI_PARAM_CODEC_GROUP_END,
	// imgcap group
	MOVIEMULTI_PARAM_IMGCAP_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0300,
	MOVIEMULTI_PARAM_IMGCAP_MAXSIZE = MOVIEMULTI_PARAM_IMGCAP_GROUP_BEGIN,          /// var
	MOVIEMULTI_PARAM_IMGCAP_SIZE,                                                   /// var
	MOVIEMULTI_PARAM_IMGCAP_EXIF_EN,                                                /// vdoenc
	MOVIEMULTI_PARAM_IMGCAP_ALLOC_SNAPSHOT_BUF,                                     /// vdoenc
	MOVIEMULTI_PARAM_IMGCAP_GROUP_END,
	// audio codec group
	MOVIEMULTI_PARAM_AUD_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0400,
	MOVIEMULTI_PARAM_AUD_ENCODER_OBJ = MOVIEMULTI_PARAM_AUD_GROUP_BEGIN,            /// audenc
	MOVIEMULTI_PARAM_AUD_SAMPLERATE,                                                /// audenc
	MOVIEMULTI_PARAM_AUD_SAMPLERATE_FILEINFO,                                       /// bsmux
	MOVIEMULTI_PARAM_AUD_CHS,                                                       /// audin
	MOVIEMULTI_PARAM_AUD_CHS_FILEINFO,                                              /// bsmux
	MOVIEMULTI_PARAM_AUD_CODEC,                                                     /// audenc
	MOVIEMULTI_PARAM_AUD_CODEC_FILEINFO,                                            /// bsmux
	MOVIEMULTI_PARAM_AUD_ADTS_HEADER,                                               /// audenc
	MOVIEMULTI_PARAM_AUD_FILETYPE,                                                  /// audenc
	MOVIEMULTI_PARAM_AUD_ENCBUF_MS,                                                 /// audenc
	MOVIEMULTI_PARAM_AUD_CAP_FUNC_EN,                                               /// var
	MOVIEMULTI_PARAM_AUD_CAP_REG_CB,                                                /// userproc
	MOVIEMULTI_PARAM_AUD_MUTE_ENC_FUNC_EN,                                          /// audenc
	MOVIEMULTI_PARAM_AUD_MUTE_ENC,                                                  /// audenc
	MOVIEMULTI_PARAM_AUD_CHS_NUM,                                                   /// audenc
	MOVIEMULTI_PARAM_AUD_GROUP_END,
	// file group
	MOVIEMULTI_PARAM_FILE_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0500,
	MOVIEMULTI_PARAM_FILE_FORMAT = MOVIEMULTI_PARAM_FILE_GROUP_BEGIN,               /// audenc + vdoenc + bsmux
	MOVIEMULTI_PARAM_REC_FORMAT,                                                    /// vdoenc + bsmux
	MOVIEMULTI_PARAM_FILE_STRGID,                                                   /// bsmux
	MOVIEMULTI_PARAM_FILE_SEAMLESSSEC,                                              /// vdoenc + bsmux
	MOVIEMULTI_PARAM_FILE_ROLLBACKSEC,                                              /// bsmux
	MOVIEMULTI_PARAM_FILE_KEEPSEC,                                                  /// bsmux
	MOVIEMULTI_PARAM_FILE_ENDTYPE,                                                  /// bsmux
	MOVIEMULTI_PARAM_FILE_OVERLAP,                                                  /// bsmux
	MOVIEMULTI_PARAM_MAX_FILE,                                                      /// bsmux
	MOVIEMULTI_PARAM_FILE_EMRON,                                                    /// bsmux
	MOVIEMULTI_PARAM_FILE_PLAYFRAMERATE,                                            /// bsmux
	MOVIEMULTI_PARAM_FILE_BUFRESSEC,                                                /// bsmux
	MOVIEMULTI_PARAM_PRECALC_BUFFER,                                                /// bsmux
	MOVIEMULTI_PARAM_GPS_DATA,                                                      /// bsmux
	MOVIEMULTI_PARAM_FILE_TRIGEMR,                                                  /// bsmux
	MOVIEMULTI_PARAM_RO_DEL_TYPE,                                                   /// var
	MOVIEMULTI_PARAM_RO_DEL_PERCENT,                                                /// var
	MOVIEMULTI_PARAM_RO_DEL_NUM,                                                    /// var
	MOVIEMULTI_PARAM_USER_DATA,                                                     /// bsmux
	MOVIEMULTI_PARAM_FILE_GOP_SIZE,                                                 /// bsmux
	MOVIEMULTI_PARAM_FILE_GROUP_END,
	// display group
	MOVIEMULTI_PARAM_DISP_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0600,
	MOVIEMULTI_PARAM_DISP_REG_CB = MOVIEMULTI_PARAM_DISP_GROUP_BEGIN,
	MOVIEMULTI_PARAM_DISP_DIR,
	MOVIEMULTI_PARAM_DISP_DUAL_EN,
	MOVIEMULTI_PARAM_DISP_GROUP_END,
	// wifi group
	MOVIEMULTI_PARAM_WIFI_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0700,
	MOVIEMULTI_PARAM_WIFI_REG_CB = MOVIEMULTI_PARAM_WIFI_GROUP_BEGIN,
	MOVIEMULTI_PARAM_WIFI_GROUP_END,
	// misc group
	MOVIEMULTI_PARAM_MISC_GROUP_BEGIN = MOVIEMULTI_PARAM_START + 0x0900,
	MOVIEMULTI_PARAM_USER_CB = MOVIEMULTI_PARAM_MISC_GROUP_BEGIN,                   /// var
	MOVIEMULTI_PARAM_D2D_MODE,
	MOVIEMULTI_PARAM_YUV_COMPRESS,
	MOVIEMULTI_PARAM_TV_RANGE,                                                      /// imagepipe(510) + vdoenc
	MOVIEMULTI_PARAM_RAW_BUF_NUM,  // D2D mode : set one/two raw buf num, default is 3

	MOVIEMULTI_PARAM_MISC_GROUP_END,
/*
	MOVIE_PARAM_3DNRLEVEL,
	MOVIE_PARAM_IMM_CB,
	MOVIE_PARAM_DUAL_DISP_EN,
	MOVIE_PARAM_DUAL_DISP_IPL_SIZE,
	MOVIE_PARAM_DUAL_DISP_AR,
	MOVIE_PARAM_MULTI_TEMPORARY_LAYER,
	MOVIE_PARAM_IMGCAP_JPG_BUFNUM,
*/
};

typedef enum {
	MOVIE_IPL_SIZE_AUTO = 0,
	MOVIE_IPL_SIZE_MAIN,
	MOVIE_IPL_SIZE_CLONE,
	MOVIE_IPL_SIZE_WIFI,
	MOVIE_IPL_SIZE_ALG,
	MOVIE_IPL_SIZE_DISP,
	ENUM_DUMMY4WORD(MOVIE_IPL_SIZE)
} MOVIE_IPL_SIZE;

typedef enum {
	RO_DEL_TYPE_PERCENT = 0,
	RO_DEL_TYPE_NUM,
	ENUM_DUMMY4WORD(MOVIE_RO_DEL_TYPE)
} MOVIE_RO_DEL_TYPE;

typedef struct {
	IPL_SEL_ITEM   item;
	UINT32  value;
} MOVIEMULTI_RECODE_IPL_INFO;

typedef struct {
	UINT32 aud_rate;
	UINT32 aud_ch;
	UINT32 aud_ch_num;
} MOVIEMULTI_AUDIO_INFO;

typedef void (*MOVIE_DISP_PIP_PROCESS_CB)(void);
typedef void (*MOVIE_WIFI_PIP_PROCESS_CB)(void);

//typedef NMEDIAREC_CALCSEC_SETTING2 MOVIEMULTI_CALCSEC_SETTING2;
//CID 129717
typedef struct {
	UINT64    givenSpace;  //in: free space
	NMEDIAREC_CALCSEC_SETTING info[8];
} MOVIEMULTI_CALCSEC_SETTING2;

typedef struct {
	MOVIE_CFG_REC_ID        rec_id;
	CHAR*   movie;
	CHAR*   emr;
	CHAR*   snapshot;
	BOOL   file_naming;
} MOVIEMULTI_RECODE_FOLDER_NAMING;

typedef struct {
	UINT32    rotate_dir;
	UINT32  pip_view;
} MOVIEMULTI_CFG_DISP_INFO;

extern void ImageApp_MovieMulti_InstallID(void);

extern ISF_RV ImageApp_MovieMulti_Open(void);
extern ISF_RV ImageApp_MovieMulti_Close(void);

extern ISF_RV ImageApp_MovieMulti_Config(UINT32 config_id, UINT32 value);

extern ISF_RV ImageApp_MovieMulti_FileOption(MOVIE_RECODE_FILE_OPTION *prec_option);
extern ISF_RV ImageApp_MovieMulti_ImgLinkForDisp(MOVIE_CFG_REC_ID id, UINT32 action, BOOL allow_pull);
extern ISF_RV ImageApp_MovieMulti_ImgLinkForDisp2(MOVIE_CFG_REC_ID id, UINT32 action, BOOL allow_pull);
extern ISF_RV ImageApp_MovieMulti_ImgLinkForStreaming(MOVIE_CFG_REC_ID id, UINT32 action, BOOL allow_pull);
extern ISF_RV ImageApp_MovieMulti_ImgLinkForAlg(MOVIE_CFG_REC_ID id, MOVIE_CFG_ALG_PATH path, UINT32 action, BOOL allow_pull);

extern ISF_RV ImageApp_MovieMulti_RecStart(MOVIE_CFG_REC_ID id);
extern ISF_RV ImageApp_MovieMulti_RecStop(MOVIE_CFG_REC_ID id);

extern ISF_RV ImageApp_MovieMulti_TriggerSnapshot(MOVIE_CFG_REC_ID id);

extern ISF_RV ImageApp_MovieMulti_SetParam(MOVIE_CFG_REC_ID id, UINT32 param, UINT32 value);
extern ISF_RV ImageApp_MovieMulti_GetParam(MOVIE_CFG_REC_ID id, UINT32 param, UINT32* value);

extern ISF_RV ImageApp_MovieMulti_StreamingStart(MOVIE_CFG_REC_ID id);
extern ISF_RV ImageApp_MovieMulti_StreamingStop(MOVIE_CFG_REC_ID id);

extern ISF_RV ImageApp_MovieMulti_DispStart(MOVIE_CFG_REC_ID id);
extern ISF_RV ImageApp_MovieMulti_DispStop(MOVIE_CFG_REC_ID id);

extern ISF_RV ImageApp_MovieMulti_UvacStart(MOVIE_CFG_REC_ID id);
extern ISF_RV ImageApp_MovieMulti_UvacStop(MOVIE_CFG_REC_ID id);

extern ISF_RV ImageApp_MovieMulti_AudCapStart(MOVIE_CFG_REC_ID id);
extern ISF_RV ImageApp_MovieMulti_AudCapStop(MOVIE_CFG_REC_ID id);

extern UINT32 ImageApp_MovieMulti_GetDispDataPort(MOVIE_CFG_REC_ID id);
extern UINT32 ImageApp_MovieMulti_GetDispDataPort2(MOVIE_CFG_REC_ID id);
extern UINT32 ImageApp_MovieMulti_GetStreamingDataPort(MOVIE_CFG_REC_ID id);
extern UINT32 ImageApp_MovieMulti_GetAlgDataPort(MOVIE_CFG_REC_ID id, MOVIE_CFG_ALG_PATH path);
extern UINT32 ImageApp_MovieMulti_GetVdoEncPort(MOVIE_CFG_REC_ID id);
extern UINT32 ImageApp_MovieMulti_GetDispImgTransPort(MOVIE_CFG_REC_ID id);

extern UINT32 ImageApp_MovieMulti_Recid2ImgLink(MOVIE_CFG_REC_ID id);
extern UINT32 ImageApp_MovieMulti_Recid2BsPort(MOVIE_CFG_REC_ID id);
extern UINT32 ImageApp_MovieMulti_Recid2Table(MOVIE_CFG_REC_ID id);
extern UINT32 ImageApp_MovieMulti_BsPort2Table(UINT32 port);
extern UINT32 ImageApp_MovieMulti_BsPort2Imglink(UINT32 port);
extern UINT32 ImageApp_MovieMulti_BsPort2Recid(UINT32 port);
extern UINT32 ImageApp_MovieMulti_VePort2Imglink(UINT32 port);

extern UINT32 ImageApp_MovieMulti_IsStreamRunning(MOVIE_CFG_REC_ID id);
extern UINT32 ImageApp_MovieMulti_GetFreeRec(void);
extern UINT32 ImageApp_MovieMulti_GetFreeRec2(MOVIEMULTI_CALCSEC_SETTING2 *pSetting);
extern UINT32 ImageApp_MovieMulti_GetRemainSize(CHAR drive, UINT64 *p_size);
extern UINT32 ImageApp_MovieMulti_SetReservedSize(CHAR drive, UINT64 size, UINT32 is_loop);

extern INT32 ImageApp_MovieMulti_FMOpen(CHAR drive);
extern INT32 ImageApp_MovieMulti_FMClose(CHAR drive);
extern INT32 ImageApp_MovieMulti_FMConfig(MOVIE_CFG_FDB_INFO *p_cfg);
extern void ImageApp_MovieMulti_FMSetSortBySN(CHAR *pDelimStr, UINT32 nDelimCount, UINT32 nNumOfSn);

/**
    set crash the assigned rec id to backup files

    set crash the assigned rec id to backup files

    @param[in] MOVIE_CFG_REC_ID id

    @return ISF_RV
*/
extern ISF_RV ImageApp_MovieMulti_SetCrash(MOVIE_CFG_REC_ID rec_id, UINT32 is_set);
extern ISF_RV ImageApp_MovieMulti_TrigEMR(MOVIE_CFG_REC_ID rec_id);
extern ISF_RV ImageApp_MovieMulti_SetCutNow(MOVIE_CFG_REC_ID rec_id);

extern void ImageApp_MovieMulti_Root_Path(CHAR * prootpath, UINT32 rec_id);
extern ISF_RV ImageApp_MovieMulti_Folder_Naming(MOVIEMULTI_RECODE_FOLDER_NAMING *pfolder_naming);
extern BOOL ImageApp_MovieMulti_isFileNaming(MOVIE_CFG_REC_ID rec_id);
extern ISF_RV ImageApp_MovieMulti_RegUserCB(MovieUserEventCb *fncb);

//@}
#endif//IMAGEAPP_MOVIEMULTI_H

