/**

    This header file of NvtEVENT APIs.

    This header file of NvtEVENT APIs for query event status

    @file       nvteventd.h
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

*/
#ifndef _NVTEVENTD_H
#define _NVTEVENTD_H

#ifdef __cplusplus
extern "C" {
#endif

extern int nvteventd_execute(void);
extern int nvteventd_get_handle(NVTEVENT_HANDLE *p_hdl); //internal used by nvtevent
extern int nvteventd_get_msg_buf(void **pp_msg); //internal used by nvtevent

#ifdef __cplusplus
}
#endif

#endif
