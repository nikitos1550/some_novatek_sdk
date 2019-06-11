#ifndef _NVTISP_COMMON_H
#define _NVTISP_COMMON_H
/*
    Copyright (c) 2015~  Novatek Microelectronics Corporation

    @file nvtisp_common.h

    @version

    @date
*/
#include <stdio.h>
#include "nvt_type.h"
#include "nvtisp.h"
#include "nvtuctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CONFIG_OPERATION_GET,
    CONFIG_OPERATION_SET,
    CONFIG_OPERATION_MAX,
} ConfigOperation;

NVTRET NvtISP_Uctrl_Command(char *cmd, void *configData, int dataSize, ConfigOperation configOP, char *retString, int retBufSize);

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTISP_COMMON_H  */
