/**
 * Copyright(c) 2013 Novatek Microelectronics Corp., Ltd. All rights reserved.\n
 * Project: NT18211
 *
 * @file wlan_api.h
 *
 * @brief Novatek Wireless Device Driver interface
 *
 * $Author: NVT02569 $
 * $Revision: 894 $
 * $Id: wlan_api.h 894 2014-12-29 02:51:32Z NVT02569 $
 */

#ifndef _WLAN_API_H
#define _WLAN_API_H

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
typedef void (wlan_link_status_cb_func_t)(char *pIntfName, int status);

// callback function prototype for station status report
typedef void (wlan_sta_status_cb_func_t)(char *pIntfName, char *pMacAddr, int status);


// 20170327 caspar: for keepalive API usage
typedef struct _NVTWIFI_API_PARAM_KEEP_ALIVE {
	unsigned char smac[6];
	unsigned char dmac[6];
        unsigned char sip[4];
	unsigned char dip[4];
	unsigned int  sport;
	unsigned int  dport;
	unsigned char payload[1518];
	unsigned int  plen;
	unsigned int  interval;
} NVTWIFI_API_PARAM_KEEP_ALIVE;
/*----------------------------------------------------------------------------*/
/* Extern Global Variables                                                    */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Interface Function Prototype                                               */
/*----------------------------------------------------------------------------*/

/**
 * \brief Novatek nvt18211 wlan device driver initialization. \n
 *        user call this API to create and start wlan driver tasks
 *
 * \param [in]- pri: user specific task priority. \n
 *                   if pri=0, means used default priority
 * \param [in]- is_wow: indicate if device is under WOW mode. \n
 *
 * \param [in]- is_apd: enable SDIO auto packet done or not. \n
 *
 * \param [in]- is_hut: Download HUT mode as default or not. \n
 *
 * \return
 *      -# 0: successed
 */
extern int wlan_init_netdev(int pri, bool is_wow, bool is_sdio_apd, bool is_hut, bool is_streaming);

//get mac mac address form efuse
//return 0: success, 1: fail.
extern int wlan_get_efuse_mac(char *pMacAddr); //0 success, -1 fail

//for STA mode, link status change callback
//return 1: register success, 0: register fail.
extern int register_wlan_link_status_cb(char *pIntfName, wlan_link_status_cb_func_t *pFunc);

//for AP mode, STA status change callback
//return 1: register success, 0: register fail.
extern int register_wlan_sta_status_cb(char *pIntfName, wlan_sta_status_cb_func_t *pFunc);

extern void wlan_set_keep_alive_msg(NVTWIFI_API_PARAM_KEEP_ALIVE ka, int index);

extern bool wlan_check_resumed(void);

extern bool wlan_check_connected(void);

extern bool wlan_check_interface_up(void);

extern bool wlan_check_wow_reveived_wakeup_info(void);

extern int wlan_get_wow_wakeup_reason(void);

extern unsigned char* wlan_api_alloc_wsupp_storage(void);

extern unsigned char* wlan_api_get_wsupp_storage(void);

extern void wlan_api_free_wsupp_storage(void);

//20170523 nash: for RA
extern int wlan_init_for_ra(void);

#if 1  // for sigma case 5.2.32  
extern int ACI_5_2_32;
#endif

#ifdef __cplusplus
}
#endif

#endif//_WLAN_API_H

/*end wlan_api.h*/
