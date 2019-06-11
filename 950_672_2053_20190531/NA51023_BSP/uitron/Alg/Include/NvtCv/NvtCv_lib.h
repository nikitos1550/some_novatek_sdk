/**
    NVT CV process functions.

    Definitions of NVT CV process functions.

    @file       NvtCv_lib.h
    @ingroup    NvtCv
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef _NVTCV_LIB_H
#define _NVTCV_LIB_H

#include "Type.h"
#include "NvtCvFuns.h"


/**
    Return status.
*/
typedef enum
{
    NVTCV_STA_OK                = 0,    ///< the status is OK
    NVTCV_STA_ERROR             = -1,   ///< some error occurred
    NVTCV_STA_INVALID_FUN       = -2,   ///< invalid function enum

    ENUM_DUMMY4WORD(NVTCV_STA)
} NVTCV_STA;

/**
    Information of function call.
*/
typedef struct _NVTCV_FUN_INFO
{
    NVTCV_FUN iFun;                         ///< function enum
    UINT32 uiAddrPrms;                      ///< address of function parameters
} NVTCV_FUN_INFO;

/**
    Application preprocess callback

    @param[in] pInfo: pointer of function info

    @return the result of preprocessing
*/
typedef NVTCV_STA (*NVTCV_PREPROCESS_CB)(NVTCV_FUN_INFO *pInfo);

/**
    Application postprocess callback

    @param[in] pInfo: pointer of function info

    @return the result of postprocessing
*/
typedef NVTCV_STA (*NVTCV_POSTPROCESS_CB)(NVTCV_FUN_INFO *pInfo);

/**
    Information of callback function.
*/
typedef struct _NVTCV_CB_INFO
{
    NVTCV_FUN iFun;                         ///< function enum
    NVTCV_PREPROCESS_CB  pfPreprocessCb;    ///< preprocess callback
    NVTCV_POSTPROCESS_CB pfPostprocessCb;   ///< postprocess callback
} NVTCV_CB_INFO;


/**
    Install required system resource.
*/
extern VOID NvtCv_InstallID(VOID) _SECTION(".kercfg_text");

/**
    Query the required working memory size.

    @return the required working memory size
*/
extern UINT32 NvtCv_GetWrkMemSize(VOID);

/**
    Open NvtCv.

    @param[in] uiAddrWorkMem: working memory address. The required memory size can be get by NvtCv_GetWrkMemSize().
*/
extern VOID NvtCv_Open(UINT32 uiAddrWorkMem);

/**
    Set preprocess/postprocess callback, which is called in NvtCv_Preprocess/NvtCv_Postprocess

    @param[in] pInfo: pointer of callback info

    @return the result of setting callback
*/
extern NVTCV_STA NvtCv_SetProcessCb(NVTCV_CB_INFO *pInfo);

/**
    Application preprocess.

    @param[in] nvtCvFun: function enum
    @param[in] uiAddrPrms: address of function parameters
*/
extern VOID NvtCv_Preprocess(NVTCV_FUN nvtCvFun, UINT32 uiAddrPrms);

/**
    NvtCv function execution.

    Note
    1. Both the input image(s) and the structure of function parameter(s) MUST be written back if they are in cache.
    2. Nested NvtCv function execution is not allowed.
    3. NvtCv is non-blocking execution, thus user can do other things after a NvtCv function is issued.

    @param[in] nvtCvFun: function enum
    @param[in] uiAddrPrms: address of function parameters
*/
extern VOID NvtCv_Execute(NVTCV_FUN nvtCvFun, UINT32 uiAddrPrms);

/**
    Wait NvtCv execution done.
*/
extern VOID NvtCv_WaitExecuteDone(VOID);

/**
    Application postprocess.

    @param[in] nvtCvFun: function enum
    @param[in] uiAddrPrms: address of function parameters
*/
extern VOID NvtCv_Postprocess(NVTCV_FUN nvtCvFun, UINT32 uiAddrPrms);

/**
    Close NvtCv.
*/
extern VOID NvtCv_Close(VOID);


#endif  //_NVTCV_LIB_H