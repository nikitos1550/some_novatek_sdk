/**
 * Copyright(c) 2013 Novatek Microelectronics Corp., Ltd. All rights reserved.\n
 * Project: NT18202
 *
 * @file wlan_mgr.h
 *
 * @brief header for WiFi main task
 *
 * $Author: NVT03012 $
 * $Revision: 2261 $
 * $Id: wlan_mgr.h 2261 2015-12-23 11:39:24Z NVT03012 $
 */

#ifndef __NTK_ES_MAIN_H__
#define __NTK_ES_MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*/
/* Include Files                                                              */
/*----------------------------------------------------------------------------*/
//#include "es_typedef.h"
//#include "es_os.h"
#include <cyg/kernel/kapi.h>

/*----------------------------------------------------------------------------*/
/* Constant Definitions                                                       */
/*----------------------------------------------------------------------------*/
//#define WLAN_MGR_FLAG_DEVICE_PLUGED    0x00000001
//#define WLAN_MGR_FLAG_DEVICE_UNPLUG    0x00000002
//#define WLAN_MGR_FLAG_MBOX             0x00000004
//#define WLAN_MGR_FLAG_EVENT_RSP        0x00000008
//#define WLAN_MGR_FLAG_PKT_DROP         0x00000010
//#define WLAN_MGR_FLAG_NLMSG_recv       0x00000020
//#define WLAN_MGR_FLAG_TICK             0x80000000
#define NTK_ES_FLAG_SWITCH_CHAN     0x00000001  
#define NTK_ES_FLAG_RECV_DATA       0x00000002  
#define NTK_ES_FLAG_DATA_PARSING    0x00000004
#define NTK_ES_FLAG_TICK            0x00000008  

#define NTK_ES_FLAG_SCAN_DONE       0x00000001  

typedef enum {
	Qos_DS_status,
    Qos_More_fragment=2,
    Qos_Retry,
    Qos_PWR_MGT,
    Qos_More_data, 
    Qos_Protected, 
    Qos_order_flag
}ntkes_Qos_flag;


typedef enum{
    w_ssid_tag,
	w_ssid_len,	
	w_auth_tag,         
	w_auth_data,
	w_pwd_tag,	
	w_pwd_len,
	w_ip_tag,         
	w_data,
	w_separator_short,
	w_separator_long,
}expected_pkt;

typedef enum
{
    Test_SEC_INVALID              =0,
    Test_SEC_NONE                 =0x01,
    Test_SEC_WPA2_AES             =0x02,
    Test_SEC_WPA2_TKIP            =0x03,
    Test_SEC_WPA_AES              =0x04,
    Test_SEC_WPA_TKIP             =0x05,
    Test_SEC_WEP                  =0x06,
}Test_SEC_SETTING;

typedef enum
{
    monitor_mode, 
	managed_mode,	
}ES_mode;

/*----------------------------------------------------------------------------*/
/* Types Declarations                                                         */
/*----------------------------------------------------------------------------*/
struct es_func_cb {
   void (*es_ssid_cb)(unsigned char *ssid);      
   void (*es_pwd_cb)(unsigned char *pwd);     
   void (*es_sec_cb)(unsigned int sec);
};

extern int ntkes_enable;

extern int ssid_decode_finished;

extern int pwd_decode_finished;

extern int auth_decode_finished;   

extern int ip_decode_finished;

extern int ntk_SmartConfgi_initialized;

extern int ntkes_timeout;

extern unsigned char ntkes_ssid[15];

extern unsigned char ntkes_pwd[30];

extern unsigned char ntkes_ip[4];

extern int ntkes_delay;

int ntk_samrt_config_init(void);

void ntkes_init(void);

int ntkes_send_msg(void *msg);

int register_wifi_ES_ssid_cb(void(*cb_func)(unsigned char *ssid));

int register_wifi_ES_pwd_cb(void(*cb_func)(unsigned char *pwd));

int register_wifi_ES_sec_cb(void(*cb_func)(unsigned int sec));

void ntkes_scan_done_cb(void);

void ntkes_wait_scan_done(void);


#ifdef __cplusplus
}
#endif

#endif /*__NTK_ES_MAIN_H__*/

/*end ntk_es_main.h*/
