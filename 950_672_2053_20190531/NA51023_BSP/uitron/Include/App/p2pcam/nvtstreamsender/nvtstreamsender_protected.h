/**
    Structure and API definitions of nvtstreamsender library.

    This file is a protected header file, which is used by NVT internally.
    @file nvtstreamsender_protected.h
    Copyright Novatek Microelectronics Corp. 2014. All rights reserved.
*/

#ifndef _NVTSTREAMSENDER_PROTECTED_H_
#define _NVTSTREAMSENDER_PROTECTED_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__ECOS)
#define __LIB_SSENDER_OS_LINUX	0
#define __LIB_SSENDER_OS_ECOS	1
#define __LIB_SSENDER_OS_UITRON	0
#elif defined(__LINUX)
#define __LIB_SSENDER_OS_LINUX	1
#define __LIB_SSENDER_OS_ECOS	0
#define __LIB_SSENDER_OS_UITRON	0
#else
#define __LIB_SSENDER_OS_LINUX	0
#define __LIB_SSENDER_OS_ECOS	0
#define __LIB_SSENDER_OS_UITRON	1
#endif

#if __LIB_SSENDER_OS_LINUX
#include <stdio.h>
#include <sys/time.h>
#elif __LIB_SSENDER_OS_ECOS
#include <time.h>
#else
struct timeval {
	long	tv_sec;		/* seconds */
	long	tv_usec;	/* and microseconds */
};
#endif

/**
   Error Code.
*/
#define    NVTSTREAMSENDER_RET_OK                       ( 0) ///< No error
#define    NVTSTREAMSENDER_RET_ERR                      (-1) ///< Found error
#define    NVTSTREAMSENDER_RET_EAGAIN                   (-2) ///< try again
#define    NVTSTREAMSENDER_RET_EIOCTL                   (-3) ///< ioctl failure
#define    NVTSTREAMSENDER_RET_EINTR                    (-4) ///< interrupt
#define    NVTSTREAMSENDER_RET_ETIME                    (-5) ///< timeout

/**
   Video Codec Type
*/
#define NVTSTREAMSENDER_VTYPE_NONE		0
#define NVTSTREAMSENDER_VTYPE_MJPG		1 ///< motion jpeg
#define NVTSTREAMSENDER_VTYPE_H264		2 ///< h.264
#define NVTSTREAMSENDER_VTYPE_H265		3 ///< h.265

/**
   Audio Codec Type
*/
#define NVTSTREAMSENDER_ATYPE_NONE		0
#define NVTSTREAMSENDER_ATYPE_PCM		1
#define NVTSTREAMSENDER_ATYPE_AAC		2
#define NVTSTREAMSENDER_ATYPE_PPCM		3
#define NVTSTREAMSENDER_ATYPE_ULAW		4
#define NVTSTREAMSENDER_ATYPE_ALAW		5

typedef struct{
    unsigned long long ItemSN; //the serial no. of each stream data
    unsigned long long MaxSN; //the max serial no.
    unsigned int Addr;
    unsigned int Size;
    unsigned int SVCLayerId;
    unsigned int FrameType; //0:P, 1:B, 2:I, 3:IDR, 4:KP
    unsigned int is_new; // 1 means the first time or just configured
    struct timeval TimeStamp;
}VSTREAM_S;

typedef struct{
    unsigned long long ItemSN; //the serial no. of each stream data
    unsigned long long MaxSN; //the max serial no.
    unsigned int Addr;
    unsigned int Size;
    unsigned int is_new; // 1 means the first time or just configured
    struct timeval TimeStamp;
}ASTREAM_S;

typedef struct{
    unsigned int Addr;
    unsigned int Size;
    unsigned int CodecType;			///< Video Codec Type
}VIDEO_INFO_S;

typedef struct{
    unsigned int CodecType;			///< Audio Codec Type
    unsigned int ChannelCnt;
    unsigned int BitsPerSample;
    unsigned int SampePerSecond;	///< Unit: KHz, e.g. 48000 is 48000KHz
}AUDIO_INFO_S;

/**
    Open NvtStream.

    This API would initialize NvtStream.
    Other APIs should be invoked after calling this API.

    @param[out] hDev    NvtStream handle.

    @return
        - @b return the handle, or NVTSTREAMSENDER_RET_ERR if an error occurs
*/
int NvtStreamSender_Open(void);

/**
    Close NvtStream.

    This API will close NvtStream and release resource.

    @param[in] hDev     NvtStream handle.

    @return
        - @b NVTSTREAMSENDER_RET_OK if succeeds
        - @b Other values if fails
*/
int NvtStreamSender_Close(int hDev);

/**
    Get video streaming data.

    This API will get video streaming data.

    @param[in] hDev         The NvtStream handle.
    @param[in] ChanId       The channel ID. (Start from 0)
    @param[in] Timeout_ms   The timeout value in milliseconds. 0 means non-blocking, -1 means always waiting
    @param[out] pVStream    The output streaming data

    @return
        - @b NVTSTREAMSENDER_RET_OK if succeeds
        - @b Other values if fails
*/
int NvtStreamSender_GetVStream(int hDev, int ChanId, int Timeout_ms, VSTREAM_S *pVStream);

/**
    Get audio streaming data.

    This API will get audio streaming data.

    @param[in] hDev         The NvtStream handle.
    @param[in] ChanId       The channel ID. (Start from 0)
    @param[in] Timeout_ms   The timeout value in milliseconds. 0 means always waiting.
    @param[out] pAStream    The output streaming data

    @return
        - @b NVTSTREAMSENDER_RET_OK if succeeds
        - @b Other values if fails
*/
int NvtStreamSender_GetAStream (int hDev, int ChanId, int Timeout_ms, ASTREAM_S *pAStream);

/**
    Get SPS and PPS.

    This API will get the information of the video stream.

    @param[in] hDev             The NvtStream handle.
    @param[in] ChanId           The channel ID. (Start from 0)
    @param[in] Timeout_ms       The timeout value in milliseconds. 0 means non-blocking, -1 means always waiting
    @param[out] pVideoInfo         SPS/PPS data information

    @return
        - @b NVTSTREAMSENDER_RET_OK if success
        - @b Other values if fails
*/
int NvtStreamSender_GetVideoInfo (int hDev, int ChanId, int Timeout_ms, VIDEO_INFO_S *pVideoInfo);

/**
    Get Audio Info.

    This API will get the information of the audio stream.

    @param[in] hDev             The NvtStream handle.
    @param[in] ChanId           The channel ID. (Start from 0)
    @param[in] Timeout_ms       The timeout value in milliseconds. 0 means non-blocking, -1 means always waiting
    @param[out] pAudioInfo      Audio information

    @return
        - @b NVTSTREAMSENDER_RET_OK if success
        - @b Other values if fails
*/
int NvtStreamSender_GetAudioInfo(int hDev, int ChanId, int Timeout_ms, AUDIO_INFO_S *pAudioInfo);

/**
    Get and lock video streaming data.

    This API will get video streaming data and lock. It should call Release

    @param[in] hDev         The streaming handle.
    @param[in] ChanId       The channel ID. (Start from 0)
    @param[in] Timeout_ms   The timeout value in milliseconds. 0 means non-blocking, -1 means always waiting
    @param[out] pVStream    The output streaming data

	@note
	-# If a streaming is locked, NvtStreamSender_ReleaseVStrm should be called to unlock it.
	-# This API only supports single-client.

    @return
        - @b NVTSTREAMSENDER_RET_OK if succeeds
        - @b Other values if fails
*/
int NvtStreamSender_GetnLockVStrm(int hDev, int ChanId, int Timeout_ms, VSTREAM_S *pVStream);

/**
    Release streaming data.

    This API will release streaming data and unlock. It is only valid when a streaming is locked.

    @param[in] hDev         The streaming handle.
    @param[in] ChanId       The channel ID. (Start from 0)

	@note
	-# This API should be called after NvtStreamSender_GetnLockVStrm.
	-# Once this API is called, the data will be removed from the temp buffer.

    @return
        - @b NVTSTREAMSENDER_RET_OK if succeeds
        - @b Other values if fails
*/
int NvtStreamSender_ReleaseVStrm(int hDev, int ChanId);

/**
    Get and lock audio streaming data.

    This API will get audio streaming data and lock. It should call Release

    @param[in] hDev         The streaming handle.
    @param[in] ChanId       The channel ID. (Start from 0)
    @param[in] Timeout_ms   The timeout value in milliseconds. 0 means non-blocking, -1 means always waiting
    @param[out] pAStream    The output streaming data

	@note
	-# If a streaming is locked, NvtStreamSender_ReleaseAStrm should be called to unlock it.
	-# This API only supports single-client.

    @return
        - @b NVTSTREAMSENDER_RET_OK if succeeds
        - @b Other values if fails
*/
int NvtStreamSender_GetnLockAStrm(int hDev, int ChanId, int Timeout_ms, ASTREAM_S *pAStream);

/**
    Release streaming data.

    This API will release streaming data and unlock. It is only valid when a streaming is locked.

    @param[in] hDev         The streaming handle.
    @param[in] ChanId       The channel ID. (Start from 0)

	@note
	-# This API should be called after NvtStreamSender_GetnLockAStrm.
	-# Once this API is called, the data will be removed from the temp buffer.

    @return
        - @b NVTSTREAMSENDER_RET_OK if succeeds
        - @b Other values if fails
*/
int NvtStreamSender_ReleaseAStrm(int hDev, int ChanId);

#if (__LIB_SSENDER_OS_LINUX == 0) && defined(__UITRON)
//Fake IPC: uITRON to CPU2
#include "Type.h"
#include "NvtIpcAPI.h"
extern NVTIPC_KEY __SSender_NvtIPC_Ftok(const char *path);
extern NVTIPC_I32 __SSender_NvtIPC_MsgGet(NVTIPC_KEY key);
extern NVTIPC_ER __SSender_NvtIPC_MsgRel(NVTIPC_U32 msqid);
extern NVTIPC_I32 __SSender_NvtIPC_MsgSnd(NVTIPC_U32 msqid, NVTIPC_SENDTO sendTo, void *pMsg, NVTIPC_U32 msgsz);
extern NVTIPC_I32 __SSender_NvtIPC_MsgRcv(NVTIPC_U32 msqid, void *pMsg, NVTIPC_U32 msgsz);
extern NVTIPC_U32 __SSender_NvtIPC_ShmGet(NVTIPC_KEY key, NVTIPC_U32 size);
extern void __SSender_NvtIPC_ShmFlush(NVTIPC_U32 shm_addr, NVTIPC_U32 shm_size);
extern NVTIPC_ER __SSender_NvtIPC_ShmRel(NVTIPC_U32 shm_addr);

extern int ssender_fakeio_module_init(void);
extern int ssender_fakeio_module_exit(void);
#endif

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif //_NVTSTREAMSENDER_PROTECTED_H_