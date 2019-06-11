/**
 * Copyright(c) 2013 Novatek Microelectronics Corp., Ltd. All rights reserved.\n
 * Project: NT18202
 *
 * @file wlan_api.h
 *
 * @brief Novatek Wireless Device Driver interface
 *
 * $Author: NVT03012 $
 * $Revision: 1758 $
 * $Id: wpa_supplicant_api.h 1758 2015-09-23 07:26:04Z NVT03012 $
 */

#ifndef _WPA_SUPPPLICANT_API_H    
#define _WPA_SUPPPLICANT_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*/
/* Include Files                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Constant Definitions                                                       */
/*----------------------------------------------------------------------------*/
// for client mode
#define WLAN_LINK_STATUS_DISCONNECTED	0
#define WLAN_LINK_STATUS_CONNECTED	1

// for AP mode
#define WLAN_STA_STATUS_ASSOCIATED	1
#define WLAN_STA_STATUS_REASSOCIATED	2
#define WLAN_STA_STATUS_DISASSOCIATED	3
#define WLAN_STA_STATUS_DEAUTHENTICATED	4
#define WLAN_STA_STATUS_PORT_AUTHORIZED	5

/*----------------------------------------------------------------------------*/
/* Types Declarations                                                         */
/*----------------------------------------------------------------------------*/
// callback function prototype for link status report
//typedef void (wlan_link_status_cb_func_t)(char *pIntfName, int status);

// callback function prototype for station status report
//typedef void (wlan_sta_status_cb_func_t)(char *pIntfName, char *pMacAddr, int status);

#define NVT_MAX_BSS_DESC            (64)
#define NVT_WIFIIPC_MAC_ADDR_LEN    (6)
#define NVT_MESH_ID_LEN             32

/*----------------------------------------------------------------------------*/
/* Extern Global Variables                                                    */
/*----------------------------------------------------------------------------*/
struct ntk_bss_desc {
    unsigned char   bssid[NVT_WIFIIPC_MAC_ADDR_LEN];
    unsigned char   ssid[32];
    unsigned char   *ssidptr;           // unused, for backward compatible
    unsigned short  ssidlen;
    unsigned char   meshid[NVT_MESH_ID_LEN];
    unsigned char   *meshidptr;         // unused, for backward compatible
    unsigned short  meshidlen;
    unsigned int    bsstype;
    unsigned short  beacon_prd;
    unsigned char   dtim_prd;
#if 0	
    unsigned int    t_stamp[2];
#else 
	unsigned long long tsf;
#endif	
    unsigned int    ibss_par;
    unsigned short  capability;
    //unsigned char   channel;
    int             channel;
    unsigned int    basicrate;
    unsigned int    supportrate;
    unsigned char   bdsa[NVT_WIFIIPC_MAC_ADDR_LEN];
    unsigned char   rssi;
    //unsigned char   sq;
    int             sq;    // signal quality  
    unsigned char   network;
    /*add for P2P_SUPPORT ; for sync; it exist no matter p2p enabled or not*/
    unsigned char   p2pdevname[33];
    unsigned char   p2prole;
    unsigned short  p2pwscconfig;
    unsigned char   p2paddress[NVT_WIFIIPC_MAC_ADDR_LEN];
};

typedef struct ntk_bss_info {
    //unsigned char number;
    size_t number;        
    //unsigned char pad[3];
    //BssDscr bssdb[MAX_BSS_DESC];
    struct ntk_bss_desc sta[NVT_MAX_BSS_DESC];
} NTK_BSS_INFO_T; 

extern enum scan_status{
   scan_triggered=0,
   scan_completed,	
   scan_aborted
};

/*----------------------------------------------------------------------------*/
/* Interface Function Prototype                                               */
/*----------------------------------------------------------------------------*/
//extern void test_lib();
//extern struct wpa_global * wpa_supplicant_init(struct wpa_params *params);


extern void send_msgq();
extern void wpa_supplicant_main(void);  
extern void *wpa_supplicant_get_notification();
//extern void nvt_wpa_supplicant_ctrl_iface_process(char *buf, size_t *resp_len);
extern char * wpa_supplicant_ctrl_iface_process(struct wpa_supplicant *wpa_s,
					 char *buf, size_t *resp_len);

extern struct wpa_supplicant *get_global_wpasupplicant(void); 
extern void show_monitor_tick();
/**
 * \brief Novatek wpa_supplicant initialization. \n
 *        user call this API to create and start wpa_supplicant tasks
 *
 * \return 
 *      -# 0: successed
 */
extern int nvt_wpa_supplicant_init(void);

extern enum scan_status get_scan_status(void);

extern int nvt_getWlSiteSurveyResult(NTK_BSS_INFO_T *pSSStatus);

// for client mode
//#define WIFI_LINK_STATUS_DISCONNECTED	0
//#define WIFI_LINK_STATUS_CONNECTED		1
//#define WIFI_LINK_STATUS_AUTH_FAIL		2
//#define WIFI_LINK_STATUS_DISASSOCIATED	3
//#define WIFI_LINK_STATUS_DEAUTHENTICATED	4
//#define WIFI_LINK_STATUS_HANDSHAKE_TIMEOUT	5
/*when indicate this event means once scan done and no any result*/
//#define WIFI_LINK_STATUS_SACN_ZREO_RESULT	6

// callback function prototype for link status report
//typedef void (wifi_wpa_link_status_cb_func_t)(char *pIntfName, int status);

//return 1: register success, 0: register fail.
extern int register_wifi_wpa_link_status_cb(char *pIntfName, void(*cb_func)(char *pIntfName, int s));

// for AP mode
//#define WIFI_STA_STATUS_ASSOCIATED	1
//#define WIFI_STA_STATUS_REASSOCIATED	2
//#define WIFI_STA_STATUS_DISASSOCIATED	3
//#define WIFI_STA_STATUS_DEAUTHENTICATED	4
//#define WIFI_STA_STATUS_PORT_AUTHORIZED	5
//#define WIFI_STA_STATUS_AUTH_FAIL		6
//#define WIFI_STA_STATUS_EXPIRED		7
//#define WIFI_AP_READY				8
// callback function prototype for station status report
//typedef void (wifi_wpa_sta_status_cb_func_t)(char *pIntfName, char *pMacAddr, int status);

//return 1: register success, 0: register fail.
extern int register_wifi_wpa_sta_status_cb(char *pIntfName, void(*cb_func)(char *pIntfName, char *pMacAddr, int s));

//get mac mac address form efuse
//return 0: success, 1: fail.
//extern int wlan_get_efuse_mac(char *pMacAddr); //0 success, -1 fail

//for STA mode, link status change callback
//return 1: register success, 0: register fail.
//extern int register_wlan_link_status_cb(char *pIntfName, wlan_link_status_cb_func_t *pFunc);

//for AP mode, STA status change callback
//return 1: register success, 0: register fail.
//extern int register_wlan_sta_status_cb(char *pIntfName, wlan_sta_status_cb_func_t *pFunc);

#ifdef __cplusplus
}
#endif

#endif   //End _WPA_SUPPPLICANT_API_H

/*end wlan_api.h*/
