/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       PrjCfg.h
    @ingroup

    @brief

    @note       Nothing.
*/

#ifndef _PRJCFG_H_
#define _PRJCFG_H_

#include "Type.h"
#include "Debug.h"


#define DEBUG_LEVEL_NONE                    0   ///< there is no debug message will show beside using debug_msg
#define DEBUG_LEVEL_ERROR                   1   ///< only debug_err() message will show
#define DEBUG_LEVEL_WARNING                 2   ///< only debug_wrn() and debug_err() message will show
#define DEBUG_LEVEL_INDICATION              3   ///< debug_ind(), debug_wrn() and debug_err() message will show

#define DEBUG_LEVEL                         DEBUG_LEVEL_ERROR   ///< debug level there are DEBUG_LEVEL_NONE/DEBUG_LEVEL_ERROR/DEBUG_LEVEL_WARNING/DEBUG_LEVEL_INDICATION can be select

#if (DEBUG_LEVEL >= DEBUG_LEVEL_ERROR)
#define debug_err(msg)                      do{debug_msg("ERR: ");debug_msg msg ;}while(0)
#else
#define debug_err(msg)
#endif

#if (DEBUG_LEVEL >= DEBUG_LEVEL_WARNING)
#define debug_wrn(msg)                      do{debug_msg("WRN: ");debug_msg msg ;}while(0)
#else
#define debug_wrn(msg)
#endif

#if (DEBUG_LEVEL >= DEBUG_LEVEL_INDICATION)
#define debug_ind(msg)                      do{debug_msg("IND: ");debug_msg msg ;}while(0)
#else
#define debug_ind(msg)
#endif
extern BOOL g_bDbgTest;
#define DBG_TEST(fmtstr, args...) do{ if(g_bDbgTest) debug_msg("DBGT: " fmtstr, ##args);  }while(0)
/*
#if (defined(_CPU1_UITRON_) && defined(_CPU2_NONE_))
#if defined(_NETWORK_)
#warning Not support _NETWORK_ under this cpu config!
#define _NETWORK_ERROR_
#endif
#elif (defined(_CPU1_ECOS_) && defined(_CPU2_NONE_))
#if (defined(_NETWORK_) && !defined(_NETWORK_ON_CPU1_) && defined(_NETWORK_ON_CPU2_))
#warning Only support _NETWORK_ON_CPU1_ under this cpu config!
#define _NETWORK_ERROR_
#endif
#elif (defined(_CPU1_UITRON_) && defined(_CPU2_ECOS_))
#if (defined(_NETWORK_) && defined(_NETWORK_ON_CPU1_) && !defined(_NETWORK_ON_CPU2_))
#warning Only support _NETWORK_ON_CPU2_ under this cpu config!
#define _NETWORK_ERROR_
#endif
#elif (defined(_CPU1_ECOS_) && defined(_CPU2_ECOS_))
#if (defined(_NETWORK_) && defined(_NETWORK_ON_CPU1_) && !defined(_NETWORK_ON_CPU2_))
#warning Only support _NETWORK_ON_CPU2_ under this cpu config!
#define _NETWORK_ERROR_
#endif
#elif (defined(_CPU1_UITRON_) && defined(_CPU2_LINUX_))
#if (defined(_NETWORK_) && defined(_NETWORK_ON_CPU1_) && !defined(_NETWORK_ON_CPU2_))
#warning Only support _NETWORK_ON_CPU2_ under this cpu config!
#define _NETWORK_ERROR_
#endif
#elif (defined(_CPU1_ECOS_) && defined(_CPU2_LINUX_))
#if (defined(_NETWORK_) && defined(_NETWORK_ON_CPU1_) && !defined(_NETWORK_ON_CPU2_))
#warning Only support _NETWORK_ON_CPU2_ under this cpu config!
#define _NETWORK_ERROR_
#endif
#endif

#if defined(_NETWORK_ERROR_)
#warning Please check your "NETWORK" config in "MakeCommon\MakeOption.txt"
#warning Please check your "CPU1", "CPU2" config in "ModelConfig_[MODEL].txt"
#error (see above)
#endif
*/
#include "PrjCfg_Option.h"

#if (_FPGA_EMULATION_ == ENABLE)
#include "PrjCfg_FPGA.h"
#elif defined(_MODEL_IPCAM1_EVB_)
#include "PrjCfg_IPCam.h"
#elif defined(_MODEL_IPCAM2_EVB_)
#include "PrjCfg_IPCam.h"
#elif defined(_MODEL_IPCAM3_EVB_)
#include "PrjCfg_IPCam.h"
#elif defined(_MODEL_DVCAM1_EVB_)
#include "PrjCfg_DVCam.h"
#elif defined(_MODEL_DVCAM2_EVB_)
#include "PrjCfg_DVCam.h"
#elif defined(_MODEL_DVCAM_64M_EVB_)
#include "PrjCfg_DVCam_64M.h"
#elif defined(_MODEL_P2PCAM1_EVB_)
#include "PrjCfg_P2PCam.h"
#elif defined(_MODEL_P2PCAM2_EVB_)
#include "PrjCfg_P2PCam.h"
#elif defined(_MODEL_J511S_)
#include "PrjCfg_J511S.h"
#elif defined(_MODEL_X1_)
#include "PrjCfg_X1.h"
#elif defined(_MODEL_JXW518_)
#include "PrjCfg_JXW518.h"
#elif defined(_MODEL_Q09_2_)
#include "PrjCfg_Q09_2.h"
#elif defined(_MODEL_A7_N75_)
#include "PrjCfg_A7_N75.h"
#elif defined(_MODEL_R2_)
#include "PrjCfg_R2.h"
#elif defined(_MODEL_G600_)
#include "PrjCfg_G600.h"
#elif defined(_MODEL_WH675_64M_)
#include "PrjCfg_WH675_64M.h"
#elif defined(_MODEL_WH675_)
#include "PrjCfg_WH675.h"
#elif defined(_MODEL_WH850_672_)
#include "PrjCfg_WH850_672.h"
#elif defined(_MODEL_PW672_)
#include "PrjCfg_PW672.h"
#elif defined(_MODEL_WH950_672_)
#include "PrjCfg_WH950_672.h"
#else //Unknown MODEL
#warning Unknown MODEL?
#warning Please assign your "MODEL" in include "ModelConfig.txt"
#error (see above)
#endif

#include "PrjCfg_Default.h"

#endif //_PRJCFG_H_
