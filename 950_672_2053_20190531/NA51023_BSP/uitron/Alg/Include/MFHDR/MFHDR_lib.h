#ifndef MFHDR_LIB_H
#define MFHDR_LIB_H

#include <Type.h>

//---------------------------------------------------------------------------------------------

/**
    HDR library data for external usage
*/
typedef struct _HDR_DATA_INFO
{
    UINT32 uiInputDataAddr[6];              ///> start address of UV packed input, use cache address
    UINT32 uiWorkBufStartAddr;              ///> Start address of working buffer, use cache address
    UINT32 uiWorkBufSize;                   ///> working buffer size
    UINT32 uiWidth;                         ///> input image width
    UINT32 uiHeight;                        ///> input image height
    UINT32 uiEvGap;                         ///> EvGap if input images, 1 & 2 are available
} HDR_DATA_INFO;

/**
    Status of all HDR process
*/
typedef enum
{
    MFHDR_SUCCESS=0,                          ///> HDR process success
    MFHDR_FAIL=1                              ///> HDR process fail
} HDR_STATUS;

//---------------------------------------------------------------------------------------------

/**
    HDR library API to do all HDR process.
    Library will proceed accord to the parameters provided
    in the HDR_DATA_INFO object.
*/
extern UINT32 MFHDR_DoAllProcess(HDR_DATA_INFO *pHdrParam);

/**
    HDR library API to get allocated output data address.
    Please define a UINT32*3 array to save address for 1
    YCC420 output images. The order of index coorespond to
    the plane of output data is:
    0~2 -> Y0 Cb0 Cr0
*/
extern VOID MFHDR_GetOutputDataAddr(UINT32 *puiOutputDataAddr);

#endif
