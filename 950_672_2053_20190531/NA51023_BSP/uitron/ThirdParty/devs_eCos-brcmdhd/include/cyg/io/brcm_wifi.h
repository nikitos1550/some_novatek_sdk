#ifndef _BRCM_WIFI_H
#define _BRCM_WIFI_H

#define BRCMF_TRACE_VAL         0x00000002
#define BRCMF_INFO_VAL          0x00000004
#define BRCMF_DATA_VAL          0x00000008
#define BRCMF_CTL_VAL           0x00000010
#define BRCMF_INTR_VAL          0x00000100
#define BRCMF_EVENT_VAL         0x00000400
#define BRCMF_BCDC_VAL          0x00010000
#define BRCMF_SDIO_VAL          0x00020000

int brcm_set_dbg_level(int);
void brcm_set_console_interval(unsigned int interval);
void brcm_set_hwoob_enable(bool enable);
int dhd_ecos_dbg(int argc, char **argv);
void brcm_test(void);
void wlu_main(int, char**);
int wifi_start_wlan0_netdev(void);
void wifi_stop_wlan0_netdev(void);
bool wifi_check_wlan0_resumed(void);
void wifi_suspend_wlan0_netdev(void);
bool wifi_check_wlan0_associated(void);
int wifi_get_wlan0_wakeup_info(char* buf, int buflen);
const char * wifi_get_wlan0_event_name(int code);
void wifi_set_wlan0_ap_mode(bool is_ap);
bool wifi_get_wlan0_ap_mode(void);
void set_to_wps(char *ssid, char *key);
void set_pwd_flag(int sel);
void set_role_flag(int sel);
void wps_start(void);
void init_wps(int, char**);
void dhd_send_wps(unsigned char *,int);
void set_wps_pin(char *pin);

void get_eth_addr(char *ea);
void get_ap_bssid(char *ea);
void get_simple_scan_result(char *t, int *num);
void get_simple_status(char *t);
int wifi_get_dhcpc_ip_addr(char *ip);
extern int wl_format_ssid(char* buf, unsigned char *ssid, int ssid_len);
void wifi_ignore_down_up(bool ignore);
bool wifi_set_backcomp(bool val);

typedef struct _simple_scan_result{
    char ssid[33];
    int auth;
    int sec;
    int channel;
    char band[3];
    int SNR;
    int rssi;
}simple_scan_result;

typedef struct _simple_status {
    char ssid[33];
    int channel;
    int associated;
} simple_status;

typedef struct _dhcpc_param {
    unsigned int own_ip;
    unsigned int server_ip;
    unsigned int lease_time;
} dhcpc_param;

int wifi_set_dhcpc_param(dhcpc_param *param);
int wifi_set_dtim_skip(unsigned int msec);
int wifi_get_dtim_skip(void);
int wifi_set_bcn_li_dtim(unsigned int bcn_li_dtim);
int wifi_get_bcn_li_dtim(void);

typedef void (*wifi_wps_rx_callback)(unsigned char *data, unsigned short len);
void dhd_register_wifi_wps_rx_callback(wifi_wps_rx_callback callback);

typedef void (*wifi_status_callback)(unsigned int status);
void dhd_register_wifi_status_callback(wifi_status_callback callback);

typedef void (*wifi_ap_status_callback)(unsigned int status, unsigned char *addr);
void dhd_register_wifi_ap_status_callback(wifi_ap_status_callback callback);

typedef void (*wifi_wps_sta_callback)(char *, char *, char *, char *);
void dhd_register_wps_sta_callback(wifi_wps_sta_callback callback);

typedef void (*wifi_pm_status_callback)(unsigned int status);
void dhd_register_wifi_pm_status_callback(wifi_pm_status_callback callback);

typedef void (*wifi_es_password_callback)(unsigned char *pwd);
void dhd_register_wifi_es_password_callback(wifi_es_password_callback callback);

typedef void (*wifi_es_ssid_callback)(unsigned char *ssid);
void dhd_register_wifi_es_ssid_callback(wifi_es_ssid_callback callback);

typedef void (*wifi_es_sec_callback)(unsigned int sec);
void dhd_register_wifi_es_sec_callback(wifi_es_sec_callback callback);

typedef void (*wifi_es_sender_callback)(unsigned char *ip);
void dhd_register_wifi_es_sender_callback(wifi_es_sender_callback callback);

typedef void (*wifi_scan_callback)(unsigned int status, void *data, int len);
void dhd_register_wifi_escanresult_callback(wifi_scan_callback callback);

#endif
