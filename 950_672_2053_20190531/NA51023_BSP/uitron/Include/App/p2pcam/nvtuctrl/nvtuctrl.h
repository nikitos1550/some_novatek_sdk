/**
    Structure and API definitions of nvtuctrl library.

    @file nvtuctrl.h
    Copyright Novatek Microelectronics Corp. 2014. All rights reserved.
*/
#ifndef _NVTUCTRL_NOIPC_H
#define _NVTUCTRL_NOIPC_H

#include <stdio.h>
#include "Type.h"

extern void nvtuctrl_InstallID(void) _SECTION(".kercfg_text");

#ifdef __cplusplus
extern "C" {
#endif

#define NVTUCTRL_CMD_SIZE                       256  ///<maximum command length

/**
   Error Code.

   Any error code occured will return.
*/
#define    NVTUCTRL_ER_OK                       ( 0) ///< command OK
#define    NVTUCTRL_ER_ERR                      (-1) ///< command fail
#define    NVTUCTRL_ER_NO_FUNC                  (-2) ///< no operation
#define    NVTUCTRL_ER_PAR_ERR                  (-3) ///< parameter error
#define    NVTUCTRL_ER_IPC_ERR                  (-4) ///< IPC error

/**
    Open nvtuctrl.

    This API would initialize nvtuctrl.
    Need to open nvtuctrl before access nvtuctrl.

    @param[out] handle      nvtuctrl handle.

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
*/
int nvtuctrl_open(void);

/**
    Close nvtuctrl.

    This API will close nvtuctrl,release resource.

    @param[in] hdl      nvtuctrl handle.

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
*/
int nvtuctrl_close(int hdl);

/**
    Send command.

    This API will send command string to uItron.

    @param[in] hdl          nvtuctrl handle.
    @param[in] cmd          The pointer of command string.
    @param[in/out] retAddr  [in]a buffer addr.[out]return string after perform command
    @param[in/out] retSize  [in]buffer size.[out]return result string size.

    @note cmd and retSize maximun is NVTUCTRL_CMD_SIZE

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
        - @b NVTUCTRL_ER_NO_FUNC if no uItron command table
        - @b NVTUCTRL_ER_PAR_ERR if API parameter fail
        - @b NVTUCTRL_ER_IPC_ERR if ipc fail

*/
int nvtuctrl_send_cmd(int hdl,char *cmd,char *retAddr,int retSize);

/**
    Receive command.

    This API will receive command string from uItron.

    @param[in] hdl          nvtuctrl handle.
    @param[in/out] cmd      [in]a buffer addr for receive command.[out]receive string.
    @param[in/out] cmdSize  [in]the buffer size[out]string size while received command.

    @note cmd maximun is NVTUCTRL_CMD_SIZE

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
        - @b NVTUCTRL_ER_IPC_ERR if ipc fail
*/
int nvtuctrl_get_cmd(int hdl,char *cmd,int cmdSize);

/**
    Receive raw data command.

    This API will receive command raw data from uItron.

    @param[in] hdl          nvtuctrl handle.
    @param[in/out] cmd      [in]a buffer addr for receive command.[out]receive string.
    @param[in/out] cmdSize  [in]the buffer size[out]data size while received command.

    @note cmd maximun is NVTUCTRL_CMD_SIZE

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
        - @b NVTUCTRL_ER_IPC_ERR if ipc fail
*/
int nvtuctrl_get_rawdata(int hdl,void *buf,int *bufSize);

/**
    Send command and get config.

    This API will send command to uItron to get config.

    @param[in] hdl          nvtuctrl handle.
    @param[in] confCmd      The pointer of command config.
    @param[in/out] retAddr  [in]a buffer addr.[out]return string after perform command
    @param[in/out] retSize  [in]buffer size.[out]return result string size.

    @note cmd and retSize maximun is NVTUCTRL_CMD_SIZE

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
        - @b NVTUCTRL_ER_NO_FUNC if no uItron command table
        - @b NVTUCTRL_ER_PAR_ERR if API parameter fail
        - @b NVTUCTRL_ER_IPC_ERR if ipc fail

*/
int nvtuctrl_get_config(int hdl,char *cmd,void *configData,int dataSize,char *retAddr,int retSize);

/**
    Send command with config.

    This API will send command with config to uItron.

    @param[in] hdl          nvtuctrl handle.
    @param[in] confCmd      The pointer of command config.
    @param[in/out] retAddr  [in]a buffer addr.[out]return string after perform command
    @param[in/out] retSize  [in]buffer size.[out]return result string size.

    @note cmd and retSize maximun is NVTUCTRL_CMD_SIZE

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
        - @b NVTUCTRL_ER_NO_FUNC if no uItron command table
        - @b NVTUCTRL_ER_PAR_ERR if API parameter fail
        - @b NVTUCTRL_ER_IPC_ERR if ipc fail

*/
int nvtuctrl_set_config(int hdl,char *cmd,void *configData,int dataSize,char *retAddr,int retSize);

extern int nvtuctrl_setnotify(UINT32 CmdId,void *data, int dataSize,int *pOutRet);

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTUCTRL_NOIPC_H  */
