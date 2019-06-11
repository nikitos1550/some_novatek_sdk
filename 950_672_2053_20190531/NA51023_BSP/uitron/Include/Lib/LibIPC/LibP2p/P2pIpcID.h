/**
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    @file       P2pIpcID.h
    @ingroup    mISYSP2pIPC

    @brief

    @note       Nothing.

    @date       2016/03/24
*/

/** \addtogroup mISYSP2pIPC */
//@{

#ifndef _P2PIPCID_H
#define _P2PIPCID_H

#include "SysKer.h"

#define P2PIPC_FLGBIT_TSK_READY      FLGPTN_BIT(0)
#define P2PIPC_FLGBIT_SYSREQ_GOTACK  FLGPTN_BIT(1)
#define P2PIPC_FLGBIT_RECV_CMD       FLGPTN_BIT(2)
#define P2PIPC_FLGBIT_CMD_FINISH     FLGPTN_BIT(3)
#define P2PIPC_FLGBIT_TSK_CLOSE      FLGPTN_BIT(4)

extern UINT32 _SECTION(".kercfg_data") P2PIPC_FLG_ID;
extern UINT32 _SECTION(".kercfg_data") P2PIPC_DO_TSK_SEM_ID;
extern UINT32 _SECTION(".kercfg_data") P2PIPC_REQ_APP_SEM_ID;
extern UINT32 _SECTION(".kercfg_data") P2PIPC_DO_TSK_ID;
#endif //_P2PIPCID_H