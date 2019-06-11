#ifndef _NVTISP_DEBUG_H
#define _NVTISP_DEBUG_H
/*
    Copyright (c) 2015~  Novatek Microelectronics Corporation

    @file nvtisp_debug.h

    @version

    @date
*/
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

//#define DEBUG

#define PRINT_OUTFILE stderr

#define CHKPNT                   fprintf(PRINT_OUTFILE, "\033[37mCHK: %d, %s\033[0m\r\n",__LINE__,__func__) ///< Show a color sting of line count and function name in your insert codes
#define DBG_WRN(fmtstr, args...) fprintf(PRINT_OUTFILE, "\033[33m%s(): \033[0m" fmtstr,__func__, ##args)
#define DBG_ERR(fmtstr, args...) fprintf(PRINT_OUTFILE, "\033[31m%s(): \033[0m" fmtstr,__func__, ##args)


#ifdef DEBUG
#define DBG_IND(fmtstr, args...) fprintf(PRINT_OUTFILE, "%s(): " fmtstr, \
    __func__, ##args)
#else
#define DBG_IND(fmtstr, args...)
#endif

#define DBG_DUMP(fmtstr, args...) fprintf(PRINT_OUTFILE, fmtstr, ##args)

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTISP_DEBUG_H  */
