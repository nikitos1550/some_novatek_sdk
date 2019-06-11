/**
    Structure and API definitions of nvtastream library.

    @file nvtastream.h
    Copyright Novatek Microelectronics Corp. 2014. All rights reserved.
*/
#ifndef _NVTSTREAMRECEIVER_H
#define _NVTSTREAMRECEIVER_H

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

#if 0
#define NVTSTREAMRECEIVER_CMD_SIZE                       256  ///<maximum command length
#endif
#define NVTSTREAMRECEIVER_CMD_SIZE                       12  ///<maximum command length

/**
   Error Code.

   Any error code occured will return.
*/
#define    NVTSTREAMRECEIVER_ER_OK                       ( 0) ///< command OK
#define    NVTSTREAMRECEIVER_ER_ERR                      (-1) ///< command fail
#define    NVTSTREAMRECEIVER_ER_NO_FUNC                  (-2) ///< no operation
#define    NVTSTREAMRECEIVER_ER_PAR_ERR                  (-3) ///< parameter error
#define    NVTSTREAMRECEIVER_ER_IPC_ERR                  (-4) ///< IPC error

/**
    Open NvtStreamReceiver.

    This API would initialize NvtStreamReceiver.
    Need to open NvtStreamReceiver before access NvtStreamReceiver.

    @param[out] handle      NvtStreamReceiver handle.

    @return
        - @b NVTSTREAMRECEIVER_ER_OK if success
        - @b NVTSTREAMRECEIVER_ER_ERR if fail
*/
int NvtStreamReceiver_Open(void);

/**
    Close NvtStreamReceiver.

    This API will close NvtStreamReceiver,release resource.

    @param[in] hDev     NvtStreamReceiver handle.

    @return
        - @b NVTSTREAMRECEIVER_ER_OK if success
        - @b NVTSTREAMRECEIVER_ER_ERR if fail
*/
int NvtStreamReceiver_Close(int hDev);

/**
    Send audio data.

    This API will send audio data to uItron.

    @param[in] hDev     	NvtStreamReceiver handle.
    @param[in] bufAddr      a buffer addr.
    @param[in] bufSize      pointer to buffer size.

    @note cmd and retSize maximun is NVTSTREAMRECEIVER_CMD_SIZE

    @return
        - @b NVTSTREAMRECEIVER_ER_OK if success
        - @b NVTSTREAMRECEIVER_ER_ERR if fail
        - @b NVTSTREAMRECEIVER_ER_NO_FUNC if no uItron command table
        - @b NVTSTREAMRECEIVER_ER_PAR_ERR if API parameter fail
        - @b NVTSTREAMRECEIVER_ER_IPC_ERR if ipc fail

*/
int NvtStreamReceiver_PutAStream(int hDev,char *bufAddr,int* bufSize);

/* ----------------------------------------------------------------- */
#endif /* _NVTSTREAMRECEIVER_H  */
