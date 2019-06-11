#ifndef _CMDSYSAPI_H
#define _CMDSYSAPI_H
/*
    Copyright (c) 2014~  Novatek Microelectronics Corporation

    @file CmdSysAPI.h

    @version

    @date
*/

extern void nvt_hal_plf_serial_putc(void* __ch_data, cyg_uint8 __ch) __attribute__ ((weak));
extern void cyg_hal_plf_serial_user_putc(void* __ch_data, cyg_uint8 __ch) __attribute__ ((weak));
extern cyg_uint8 nvt_hal_plf_serial_getc(void* __ch_data)__attribute__ ((weak));
extern int runmain_as_pthread(char *cmdstr);
#endif /* _CMDSYSAPI_H  */
