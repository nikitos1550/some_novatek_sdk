#ifndef CYGONCE_NET_DHCPD_H
#define CYGONCE_NET_DHCPD_H

#include <pkgconf/eliosdhcp.h>

#define NET_IP_MAX_LEN              16
#define NET_INTERFACE_MAX_LEN       16   //"etho","eth1","wlan0"
#define NET_LEASE_SIZE              24

typedef struct _dhcp_lease_conf
{
    char ip_pool_start[NET_IP_MAX_LEN];
    char ip_pool_end[NET_IP_MAX_LEN];
    unsigned int lease_time;
    unsigned int max_lease_cnt;
    char ip_server[NET_IP_MAX_LEN];
    char ip_gateway[NET_IP_MAX_LEN];
    char ip_subnetmask[NET_IP_MAX_LEN];
    char ip_broadcast[NET_IP_MAX_LEN];
    char ip_dns[NET_IP_MAX_LEN];
    char ip_wins[NET_IP_MAX_LEN];
}dhcp_lease_conf;

typedef struct _dhcp_assign_ip_info
{
    unsigned char  macaddr[NET_IP_MAX_LEN];    // MAC adress
    unsigned int ipaddr;                     // IP
}dhcp_assign_ip_info;

typedef struct _dhcpd_lease_tbl_loc
{
    unsigned char *pStartAddr;
    unsigned int size;
}dhcpd_lease_tbl_loc;

typedef enum{
    DHCPD_CLI_STS_RELEASE_IP = 0,          //release an ip
    DHCPD_CLI_STS_REQUEST_IP,              //request an ip
    DHCPD_CLI_STS_DECLINE_IP,              //decline an ip
    DHCPD_CLI_STS_SRVSTART,                //dhcp server open
    DHCPD_CLI_STS_SRVREADY,                //dhcp server is ready
    DHCPD_CLI_STS_MAX
}DHCPD_CLI_STS;

/**
     Prototype of registering a callback function to notify that a dhcp client connected.

     @param[in] cliSts          0=relese ip,1=request ip,2=decline ip
     @param[in] ipInfo          point to a space with structure,dhcp_assign_ip_info.
*/
typedef void (*dhcpd_cli_attach_cb_func)(DHCPD_CLI_STS cliSts, dhcp_assign_ip_info *ipInfo, dhcpd_lease_tbl_loc *pTbl);

/**
     Register a dhcp client attached notification function.

     Callback if there is a client connected to this dhcp server.

     @param[in] pFunc       points to a function with type, dhcpd_cli_attach_cb_func.
     @return
         - @b 1:   pFunc is not NULL
         - @b 0:   pFunc is NULL
*/
__externC int dhcpd_reg_cli_attach_cb_func(dhcpd_cli_attach_cb_func pFunc);

__externC int dhcpd_startup(void);
__externC int dhcpd_stop(void);
__externC void dhcpd_suspend(void);
__externC void dhcpd_exit(void);
__externC void dhcpd_set_lease_conf(dhcp_lease_conf *pdhcpLeaseConf);
__externC bool dhcpd_set_interface(char *pIntf);
__externC unsigned int dhcpd_get_assigned_ip_info(dhcp_assign_ip_info * pIpInfoAry, unsigned int aryCnt);

__externC char* dhcpd_GetVerInfo(void);   // get version info
__externC char* dhcpd_GetBuildDate(void); // get build date

#endif
