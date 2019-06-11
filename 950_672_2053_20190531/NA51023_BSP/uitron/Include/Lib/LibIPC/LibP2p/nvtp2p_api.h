#ifndef _NVTP2P_API_H_
#define _NVTP2P_API_H_

#include <nvtp2p.h>

#ifndef __externC
#ifdef __cplusplus
# define __externC extern "C"
#else
# define __externC extern
#endif
#endif

__externC int          NvtP2P_SysReqAck  (int AckValue);

//==========exposed functions to coorperated company's adaptor============
//==============================start=====================================
__externC int          NvtP2P_Request_Dev(int reqid, void *pdata, int length);
__externC int          NvtP2P_Notify_Dev (int ntfid, int arg);
__externC int          NvtP2P_Stream_Dev (void *pdata, int length);

//==========exposed functions to coorperated company's adaptor============
//==============================end=======================================

//===============adaptor implemented by coorperated company===============
//==============================start=====================================
typedef struct _NVTP2P_CMDID_IMPL
{
        int               (*P2P_CmdId_Server_Start)(NVTP2P_APP_MSG *pRcvMsg);
        int               (*P2P_CmdId_Server_Stop) (NVTP2P_APP_MSG *pRcvMsg);
        int               (*P2P_CmdId_Request_App) (NVTP2P_APP_MSG *pRcvMsg);
        int               (*P2P_CmdId_Notify_App)  (NVTP2P_APP_MSG *pRcvMsg);
        int               (*P2P_CmdId_Stream_App)  (NVTP2P_APP_MSG *pRcvMsg);
}NVTP2P_CMDID_IMPL;

__externC NVTP2P_CMDID_IMPL gP2PCmdImplFp;

//===============adaptor implemented by coorperated company===============
//==============================end=======================================

//======================exposed functions to uitron=======================
//==============================start=====================================
int NvtP2P_CmdId_Server_Start    (NVTP2P_APP_MSG *RcvMsg);
int NvtP2P_CmdId_Server_Stop     (NVTP2P_APP_MSG *RcvMsg);
int NvtP2P_CmdId_Request_App     (NVTP2P_APP_MSG *RcvMsg);
int NvtP2P_CmdId_Notify_App      (NVTP2P_APP_MSG *RcvMsg);
int NvtP2P_CmdId_Stream_App      (NVTP2P_APP_MSG *RcvMsg);
int NvtP2P_CmdId_Sysreq_Ack_App  (NVTP2P_APP_MSG *RcvMsg);
int NvtP2P_CmdId_Uninit          (NVTP2P_APP_MSG *RcvMsg);

#if defined(__ECOS)
#if defined(__IPC)
__externC void NvtP2PECOS_CmdLine(char *szCmd);
#else
__externC int NvtP2PECOS_CmdLine(char *szCmd);
#endif
#else
#if defined(__LINUX_660)
__externC int NvtP2PECOS_CmdLine(char *szCmd, char *szBuf1Addr, char *szBuf1Size, char *szBuf2Addr, char *szBuf2Size);
#else
__externC int NvtP2PECOS_CmdLine(char *szCmd);
#endif
#endif

//======================exposed functions to uitron=======================
//==============================end=======================================

#endif //_NVTP2P_API_H_

