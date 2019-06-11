#ifndef _WPS_DEF_H_
#define _WPS_DEF_H_

#define MAX_MANUFACT_LEN			64
#define MAX_MODEL_NAME_LEN			32
#define MAX_MODEL_NUM_LEN			32
#define MAX_SERIAL_NUM_LEN			32
#define MAX_DEVICE_NAME_LEN			32
#define WSC_MAX_SSID_LEN			32
#define MAX_WEP_KEY_LEN				26
#define MAX_NETWORK_KEY_LEN			64
#define PIN_LEN					8

enum { 
		MODE_AP_UNCONFIG=1, 			// AP unconfigured (enrollee)
		MODE_CLIENT_UNCONFIG=2, 		// client unconfigured (enrollee) 
		MODE_CLIENT_CONFIG=3,			// client configured (registrar) 
		MODE_AP_PROXY=4, 				// AP configured (proxy)
		MODE_AP_PROXY_REGISTRAR=5,		// AP configured (proxy and registrar)
		MODE_CLIENT_UNCONFIG_REGISTRAR=6		// client unconfigured (registrar)
};

#define WSC_WPA_TKIP		1
#define WSC_WPA_AES		2
#define WSC_WPA2_TKIP		4
#define WSC_WPA2_AES		8

enum {  NOT_USED=-1, 
		PROTOCOL_START=0, PROTOCOL_PBC_OVERLAPPING=1,
		PROTOCOL_TIMEOUT=2, PROTOCOL_SUCCESS=3 ,

		SEND_EAPOL_START=4, RECV_EAPOL_START=5, SEND_EAP_IDREQ=6, RECV_EAP_IDRSP=7, 
        SEND_EAP_START=8, SEND_M1=9, RECV_M1=10, SEND_M2=11, 

        RECV_M2=12, RECV_M2D=13, SEND_M3=14, RECV_M3=15,
        SEND_M4=16, RECV_M4=17, SEND_M5=18, RECV_M5=19,

        SEND_M6=20, RECV_M6=21, SEND_M7=22, RECV_M7=23,
        SEND_M8=24, RECV_M8=25, PROC_EAP_ACK=26, WSC_EAP_FAIL=27,

        HASH_FAIL=28, HMAC_FAIL=29, PWD_AUTH_FAIL=30, PROTOCOL_PIN_NUM_ERR=31,
        PROC_EAP_DONE=32,PROTOCOL_ERR=33
}; 

//---------------------------------------------------------------------------------
//apmib.h
#define MAX_SSID_LEN				33

//enum { WEP_DISABLED=0, WEP64=1, WEP128=2 };	voild compile error
//enum { KEY_ASCII=0, KEY_HEX=1 };
enum { WSC_AUTH_OPEN=1, WSC_AUTH_WPAPSK=2, WSC_AUTH_SHARED=4, WSC_AUTH_WPA=8, WSC_AUTH_WPA2=0x10, WSC_AUTH_WPA2PSK=0x20, WSC_AUTH_WPA2PSKMIXED=0x22 };
enum { WSC_ENCRYPT_NONE=1, WSC_ENCRYPT_WEP=2, WSC_ENCRYPT_TKIP=4, WSC_ENCRYPT_AES=8, WSC_ENCRYPT_TKIPAES=12 };
enum { CONFIG_BY_INTERNAL_REGISTRAR=1, CONFIG_BY_EXTERNAL_REGISTRAR=2, MANUAL_SETTING_TO_ENROLLEE=3};
typedef enum { BAND_11B=1, BAND_11G=2, BAND_11BG=3, BAND_11A=4, BAND_11N=8, BAND_5G_11AN=12, 
	BAND_5G_11AC=64,BAND_5G_11AAC=68,BAND_5G_11NAC=72,BAND_5G_11ANAC=76} BAND_TYPE_T;
enum { 
	CONFIG_METHOD_ETH=0x2, 
	CONFIG_METHOD_PIN=0x4, 
	CONFIG_METHOD_DISPLAY=0x8  ,		
	CONFIG_METHOD_PBC=0x80, 
	CONFIG_METHOD_KEYPAD=0x100,
	CONFIG_METHOD_VIRTUAL_PBC=0x280	,
	CONFIG_METHOD_PHYSICAL_PBC=0x480,
	CONFIG_METHOD_VIRTUAL_PIN=0x2008,
	CONFIG_METHOD_PHYSICAL_PIN=0x4008
};


//typedef enum { ENCRYPT_DISABLED=0, ENCRYPT_WEP=1, ENCRYPT_WPA=2, ENCRYPT_WPA2=4, ENCRYPT_WPA2_MIXED=6 ,ENCRYPT_WAPI=7} ENCRYPT_T;
//typedef enum { WDS_ENCRYPT_DISABLED=0, WDS_ENCRYPT_WEP64=1, WDS_ENCRYPT_WEP128=2, WDS_ENCRYPT_TKIP=3, WDS_ENCRYPT_AES=4} WDS_ENCRYPT_T;
//typedef enum { SUPP_NONWPA_NONE=0,SUPP_NONWPA_WEP=1,SUPP_NONWPA_1X=2} SUPP_NONWAP_T;
//typedef enum { WPA_AUTH_AUTO=1, WPA_AUTH_PSK=2 } WPA_AUTH_T;
//typedef enum { WAPI_AUTH_AUTO=1, WAPI_AUTH_PSK=2 } WAPI_AUTH_T;
//typedef enum { WPA_CIPHER_TKIP=1, WPA_CIPHER_AES=2, WPA_CIPHER_MIXED=3 } WPA_CIPHER_T;
typedef enum { WEP_DISABLED=0, WEP64=1, WEP128=2 } WEP_T;
typedef enum { KEY_ASCII=0, KEY_HEX } KEY_TYPE_T;
//typedef enum { LONG_PREAMBLE=0, SHORT_PREAMBLE=1 } PREAMBLE_T;
//typedef enum { DHCP_DISABLED=0, DHCP_CLIENT=1, DHCP_SERVER=2, PPPOE=3, PPTP=4, L2TP=6, DHCP_AUTO=15 , USB3G=16 } DHCP_T; /* # keith: add l2tp support. 20080515 */
//typedef enum { DHCP_LAN_NONE=0, DHCP_LAN_CLIENT=1, DHCP_LAN_SERVER=2, DHCP_LAN_RELAY=3 } DHCP_TYPE_T; //keith add. LAN SIDE DHCP TYPE
//typedef enum { GATEWAY_MODE=0, BRIDGE_MODE=1, WISP_MODE=2 } OPMODE_T;
//typedef enum { DISABLE_MODE=0, RIP1_MODE=1, RIP2_MODE=2 } RIP_OPMODE_T; 
//typedef enum { FCC=1, IC, ETSI, SPAIN, FRANCE, MKK } REG_DOMAIN_T;
//typedef enum { AUTH_OPEN=0, AUTH_SHARED, AUTH_BOTH } AUTH_TYPE_T;
//typedef enum { DNS_AUTO=0, DNS_MANUAL } DNS_TYPE_T;
//typedef enum { CONTINUOUS=0, CONNECT_ON_DEMAND, MANUAL } PPP_CONNECT_TYPE_T;
//typedef enum { RF_INTERSIL=1, RF_RFMD=2, RF_PHILIP=3, RF_MAXIM=4, RF_GCT=5,
 //		RF_MAXIM_AG=6, RF_ZEBRA=7, RF_8255=8 } RF_TYPE_T;
//typedef enum { LED_TX_RX=0, LED_LINK_TXRX=1, LED_LINKTXRX=2 } LED_TYPE_T;

//---------------------------------------------------------------------------------

struct wps_config
{
	char wlan_interface_name[40];
	unsigned char is_ap;	
	int start;
	int mode;
	int upnp;
	int config_method;
	char SSID[WSC_MAX_SSID_LEN+1];	
	int auth_type_flags;	
	int auth_type_flash;
	int mixedmode;
	int rf_band;
	int encrypt_type_flags;	
	int encrypt_type_flash;
	int connect_type;
	int manual_config;
	char lan_interface_name[40];
	char pin_code[PIN_LEN+1];

	char network_key[MAX_NETWORK_KEY_LEN+1];
	char wep_key2[MAX_NETWORK_KEY_LEN+1];
	char wep_key3[MAX_NETWORK_KEY_LEN+1];
	char wep_key4[MAX_NETWORK_KEY_LEN+1];	
	unsigned char wep_transmit_key;
	int network_key_len;	
	int config_by_ext_reg;	// configured by external registrar

	//wscd.conf
	char manufacturer[MAX_MANUFACT_LEN+1];
	char model_name[MAX_MODEL_NAME_LEN+1];
	char model_num[MAX_MODEL_NUM_LEN+1];	
	char serial_num[MAX_SERIAL_NUM_LEN+1];
	char manufacturerURL[MAX_MANUFACT_LEN+1];
	char model_URL[MAX_MANUFACT_LEN+1];
	char modelDescription[MAX_MANUFACT_LEN+1];//manufacturerDesc
	char device_name[MAX_DEVICE_NAME_LEN+1];
	char ssid_prefix[WSC_MAX_SSID_LEN+1];
};

struct wsc_flash_param {
	int type;
	char SSID[WSC_MAX_SSID_LEN+1];
	int AUTH_TYPE;    /* 0 ¡V open system, 1 ¡V shared key, 2 ¡V auto */
	int ENCRYPT;	  /* 0 ¡V disabled, 1 ¡V WEP64, 2 ¡V TKIP, 4 ¡V AES(CCMP), 5 ¡V WEP128 */
	int WSC_AUTH;     /* WSC_AUTH_OPEN=1, WSC_AUTH_WPAPSK=2, WSC_AUTH_SHARED=4, WSC_AUTH_WPA=8, WSC_AUTH_WPA2=0x10, WSC_AUTH_WPA2PSK=0x20, WSC_AUTH_WPA2PSKMIXED=0x22 */
	int WPA_AUTH;
	char WPA_PSK[MAX_NETWORK_KEY_LEN+1];
	int PSK_FORMAT;
	char WSC_PSK[MAX_NETWORK_KEY_LEN+1];
	int WPA_CIPHER_SUITE;
	int WPA2_CIPHER_SUITE;
	int WEP; /* WEP_DISABLED=0, WEP64=1, WEP128=2*/
	char WEP_KEY1[MAX_WEP_KEY_LEN+1];
	char WEP_KEY2[MAX_WEP_KEY_LEN+1];
	char WEP_KEY3[MAX_WEP_KEY_LEN+1];
	char WEP_KEY4[MAX_WEP_KEY_LEN+1];
	int WEP_DEFAULT_KEY;
	int WEP_KEY_TYPE;
	int WSC_ENC; /* WSC_ENCRYPT_NONE=1, WSC_ENCRYPT_WEP=2, WSC_ENCRYPT_TKIP=4, WSC_ENCRYPT_AES=8, WSC_ENCRYPT_TKIPAES=12 */
	int WSC_CONFIGBYEXTREG; /* CONFIG_BY_INTERNAL_REGISTRAR=1, CONFIG_BY_EXTERNAL_REGISTRAR=2, MANUAL_SETTING_TO_ENROLLEE=3 */
	int WSC_CONFIGURED;
	int WLAN0_P2P_TYPE;
};

#define WPS_REINIT_EVENT		1
#define WPS_STATUS_CHANGE_EVENT		2

typedef void (wsc_event_cb_func_t)(int);
typedef int (wsc_flash_param_cb_func_t)(struct wsc_flash_param *);

#endif // _WPS_DEF_H_

