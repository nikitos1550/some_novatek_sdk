#ifndef _WPS_API_H
#define _WPS_API_H

#include "wps_def.h"

// register callback function
extern void register_wsc_event_cb(wsc_event_cb_func_t *pfunc);
extern void register_wsc_flash_param_cb(wsc_flash_param_cb_func_t *pfunc);

extern void apply_wps_config_to_wscd(struct wps_config *wpscfg);

extern void wsc_sig_pbc(void);
extern void wsc_sig_pin(void);
extern void wsc_sig_stop(void);
extern void wsc_unlock(void);   //unlock "auto lock down"
extern int get_wscd_lock_stat(void);

//for debug
extern void wsc_reinit(void);
extern void wsc_stop_service(void);
extern int get_wscd_state(void);
extern int get_wscd_status(void);

extern void create_wscd(void);
extern void dump_wsc_context(void *ptr);
extern void dump_wsc_flash_param(void);
//extern int validate_pin_code(unsigned long code);
extern int compute_pin_checksum(unsigned long int PIN);
extern void convert_hex_to_ascii(unsigned long code, char *out);

#endif
