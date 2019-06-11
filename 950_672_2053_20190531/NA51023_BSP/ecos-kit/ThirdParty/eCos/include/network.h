
//==========================================================================
//
//      include/network.h
//
//      Misc network support
//
//==========================================================================
//####BSDCOPYRIGHTBEGIN####
//
// -------------------------------------------
//
// Portions of this software may have been derived from OpenBSD or other sources,
// and are covered by the appropriate copyright disclaimers included herein.
//
// -------------------------------------------
//
//####BSDCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    gthomas
// Contributors: gthomas, andrew.lunn@ascom.ch
// Date:         2000-01-10
// Purpose:
// Description:
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifndef _NETWORK_H_
#define _NETWORK_H_

#define NO_LIBKERN_INLINE  // Avoid kernel inline functions

#include <pkgconf/system.h>
#include <pkgconf/net.h>
#include <pkgconf/io_eth_drivers.h>
#undef _KERNEL
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <net/route.h>

#include <cyg/infra/diag.h>
#include <cyg/kernel/kapi.h>

#include <netdb.h>
#include <bootp.h>

/* dhcp config */
#define DHCP_CLI_GETEWAY   0

#ifdef CYGHWR_NET_DRIVER_ETH0
extern struct bootp eth0_bootp_data;
extern cyg_bool_t   eth0_up;
extern const char  *eth0_name;
#endif
#ifdef CYGHWR_NET_DRIVER_ETH1
extern struct bootp eth1_bootp_data;
extern cyg_bool_t   eth1_up;
extern const char  *eth1_name;
#endif
#ifdef CYGHWR_NET_DRIVER_WLAN0
extern struct bootp wlan0_bootp_data;
extern cyg_bool_t   wlan0_up;
extern const char  *wlan0_name;
#endif


__externC void init_all_network_interfaces(void);
//initialize the ethernet 0 or 1 for fixed IP
__externC void init_all_network_interfaces_statically(void);
//Uninitialize the status of ethernet 0 or 1
__externC void uninit_all_network_interfaces_statically(void);
//Close the dhcp client mode
__externC void uninit_all_network_interfaces(void);
// Release the lease that DHCP client got and still valid.
__externC int dhcp_release_interfaces(unsigned int ethIdxBitMap);
//Renew the lease that DHCP client got before.
__externC int dhcp_renew_interface(unsigned int ethIdxBitMap);
__externC int dhcp_reboot_interface(unsigned int ethIdxBitMap, struct bootp *wlan0_bootp_last);
//move ping from uITRON to another CPU(Linux, eCos)
__externC bool dhcp_net_ping(char *addr_str, int packSize, unsigned int queryCnt);


//Set a fix-ip for eth0 or eth1
#define FIX_IP_MAX_LEN              16
typedef struct _fix_ip_pair
{
    char ip[FIX_IP_MAX_LEN];
    char netmask_ip[FIX_IP_MAX_LEN];
    char broadcast_ip[FIX_IP_MAX_LEN];
    char gateway_ip[FIX_IP_MAX_LEN];
    char server_ip[FIX_IP_MAX_LEN];
    unsigned int intrfIdx;
}fix_ip_pair_t, *pfix_ip_pair_t;

//Set a fix-ip for eth0 or eth1
__externC void dhcp_set_fix_ip_pair(pfix_ip_pair_t pFixIpPair,unsigned int ethIdxBitMap);
__externC void dhcp_dmp_fix_ip_pair(unsigned int ethIdxBitMap);
__externC void dhcp_reset_fix_ip_pair(unsigned int ethIdxBitMap);
__externC unsigned int dhcp_cli_get_ip(void);
__externC unsigned int dhcp_cli_get_server_ip(void);
__externC int dhcp_cli_set_config(int config,int value);
__externC int dhcp_cli_get_config(int config);
__externC void     cyg_route_reinit(void);
__externC void     perror(const char *) __THROW;
__externC int      close(int);
__externC ssize_t  read(int, void *, size_t);
__externC ssize_t  write(int, const void *, size_t);
__externC int      select(int, fd_set *, fd_set *, fd_set *, struct timeval *tv);

// This API is for our own automated network tests.
// It's not at all supported.
#define CYG_NET_GET_MEM_STATS_MISC  0 // Misc mpool
#define CYG_NET_GET_MEM_STATS_MBUFS 1 // Mbufs pool
#define CYG_NET_GET_MEM_STATS_CLUST 2 // Clust pool
int cyg_net_get_mem_stats( int which, cyg_mempool_info *p );

#ifdef CYGPKG_NET_INET6
#ifdef CYGOPT_NET_IPV6_ROUTING_THREAD
__externC void ipv6_start_routing_thread(void);
__externC int cyg_net_get_ipv6_advrouter(struct sockaddr_in6 * addr);
#endif
#endif

#endif // _NETWORK_H_
