#ifndef _NVTCODEC_COMMON_H
#define _NVTCODEC_COMMON_H
/*
    Copyright (c) 2015~  Novatek Microelectronics Corporation

    @file nvtcodec_common.h

    @version

    @date
*/
#include <stdio.h>
#include <sys/time.h>
#include "nvt_type.h"
#include "nvtuctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define DEBUG

#define PRINT_OUTFILE stderr

#define CHKPNT                   fprintf(PRINT_OUTFILE, "\033[37mCHK: %d, %s\033[0m\r\n",__LINE__,__func__) ///< Show a color sting of line count and function name in your insert codes
#define DBG_WRN(fmtstr, args...) fprintf(PRINT_OUTFILE, "\033[33m%s(): \033[0m" fmtstr,__func__, ##args)
#define DBG_ERR(fmtstr, args...) fprintf(PRINT_OUTFILE, "\033[31m%s(): \033[0m" fmtstr,__func__, ##args)


#ifdef DEBUG
#define DBG_IND(fmtstr, args...) fprintf(PRINT_OUTFILE, "%s(): " fmtstr, \
    __func__, ##args)
#else
#define DBG_IND(fmtstr, args...)
#endif

#define DBG_DUMP(fmtstr, args...) fprintf(PRINT_OUTFILE, fmtstr, ##args)

#define CODEC_VIDEO_SHM_KEY 3872
#define CODEC_AUDIO_SHM_KEY 3873

typedef enum {
    CONFIG_OPERATION_GET,
    CONFIG_OPERATION_SET,
    CONFIG_OPERATION_MAX,
} ConfigOperation;

typedef struct
{
    UINT32 bufLength;
    UINT8 *bufPtr;
} PayloadBuf;

inline UINT8 NvtCodec_CheckLockTime(struct timespec currTime, struct timespec lockTime);
INT64 time_diff_ms(struct timeval *startTV, struct timeval *endTV);
NVTRET NvtCodec_Uctrl_Command(char *cmd, void *configData, int dataSize, ConfigOperation configOP, char *retString, int retBufSize);
void *NvtCodec_SHM_Attach(key_t shmKey, size_t shmSize, UINT8 *createFlag);
void NvtCodec_SHM_Deatttach(void *shmAddr);

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTCODEC_COMMON_H  */
