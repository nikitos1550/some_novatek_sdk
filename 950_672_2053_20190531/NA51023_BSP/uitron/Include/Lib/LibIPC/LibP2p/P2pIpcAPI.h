/*
    Copyright (c) 2016  Novatek Microelectronics Corporation

    @file P2pIpc.h

    @version

    @date
*/

#ifndef _P2PIPCAPI_H
#define _P2PIPCAPI_H

#include "Type.h"
#include "nvtp2p.h"

#define NVTP2P_RET_OK                       0
#define NVTP2P_RET_ERR                      (-1)

typedef INT32 (*NVTP2P_CB)(NVTP2P_DEVICE_MSG *msg);

typedef struct {
UINT32 sharedMemAddr;
UINT32 sharedMemSize;
NVTP2P_CB RequestCB;
NVTP2P_CB NotifyCB;
}P2PIPC_OPEN;

extern void P2pIpc_InstallID(void) _SECTION(".kercfg_text"); 

extern ER P2pIpc_Open(P2PIPC_OPEN *pOpen);

extern ER P2pIpc_Close(void);

extern ER P2pIpc_Server_Start(VOID *pdata, INT32 length);

extern ER P2pIpc_Server_Stop(VOID *pdata, INT32 length);

extern ER P2pIpc_Request_App(INT32 reqid, VOID *pdata, INT32 length);

extern ER P2pIpc_Notify_App(INT32 ntfid, INT32 arg);

extern ER P2pIpc_Stream_App(VOID *pdata, INT32 length);

#endif /* _FTYPEIPCAPI_H  */
