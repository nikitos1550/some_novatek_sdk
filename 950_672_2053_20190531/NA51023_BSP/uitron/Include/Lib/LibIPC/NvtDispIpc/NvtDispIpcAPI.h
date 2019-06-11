/////////////////////////////////////////////////////////////////
/*
    Copyright (c) 2014~  Novatek Microelectronics Corporation

    @file NvtDispIpcAPI.h

    @version

    @date

*//////////////////////////////////////////////////////////////////
#ifndef _NVTDISPIPCAPI_H
#define _NVTDISPIPCAPI_H

/**
   Warning Code.

   Any warning code occured will display on uart or return.
*/
typedef enum _NVTDISPIPC_WR {
	NVTDISPIPC_WR_OK                =   0, ///< no warning
	NVTDISPIPC_WR_ALREADY_OPEN      =  -1, ///< NvtDispIpc_Open called twice
	NVTDISPIPC_WR_NOT_OPEN          =  -2, ///< NvtDispIpc_Open not to call before.
	ENUM_DUMMY4WORD(NVTDISPIPC_WR)
} NVTDISPIPC_WR;



/**
    Init parameters for NvtDispIpc module.

    This structure is used in NvtDispIpc_Open() to specify the NvtDispIpc_RcvTsk task needed information
    for normal working.
    Memory is for drawing OSD.
*/
typedef struct {
	UINT32                  sharedMemAddr;                           ///<  shared memory address
	UINT32                  sharedMemSize;                           ///<  shared memory size
} NVTDISPIPC_OPEN;
/*-----------------------------------
    functions declare
-----------------------------------*/

/**
     Install NvtDispIpc flag and semaphore id.

*/
extern void       NvtDispIpc_InstallID(void) _SECTION(".kercfg_text");

/**
    Open NvtDispIpc task.

    This is the NvtDispIpc task initial function.

    @param[in] pOpen:  Init info for NvtDispIpc.

    @return
        - @b E_OK: initial success.
        - @b E_PAR: the initial parameter has some error.

    Example:
    @code

    @endcode

*/
extern ER     NvtDispIpc_Open(NVTDISPIPC_OPEN *pOpen);

/**
    Close NvtDispIpc task.

    @return
        - @b E_OK: close success.

*/
extern ER     NvtDispIpc_Close(void);


#endif  // _NVTDISPIPCAPI_H

