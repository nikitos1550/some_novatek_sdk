#include <cyg/infra/diag.h>
#include <network.h>


#define SETIP_TEST_DBG              0
#if SETIP_TEST_DBG
#define DbgMsg_SetipTestIO          diag_printf
#define DbgMsg_SetipTestErr         diag_printf
#else
#define DbgMsg_SetipTestIO(...)
#define DbgMsg_SetipTestErr         diag_printf
#endif

#define USE_MAINFUNC_ENTRY      1

#define TEST_FIX_IP_ETH0            "192.168.0.3"
#define TEST_FIX_NETMASK_ETH0       "255.255.255.0"
#define TEST_FIX_BRAODCAST_ETH0     "192.168.0.255"
#define TEST_FIX_GATEWAY_ETH0       "192.168.0.1"
#define TEST_FIX_SRVIP_ETH0         "192.168.0.1"

#define TEST_FIX_IP_ETH1            "192.168.1.3"
#define TEST_FIX_NETMASK_ETH1       "255.255.255.0"
#define TEST_FIX_BRAODCAST_ETH1     "192.168.1.255"
#define TEST_FIX_GATEWAY_ETH1       "192.168.1.1"
#define TEST_FIX_SRVIP_ETH1         "192.168.1.1"

static fix_ip_pair_t gTestNetFixIP[2] =
{
    //eth0
    {TEST_FIX_IP_ETH0,TEST_FIX_NETMASK_ETH0,TEST_FIX_BRAODCAST_ETH0,TEST_FIX_GATEWAY_ETH0,TEST_FIX_SRVIP_ETH0},
    //eth1
    {TEST_FIX_IP_ETH1,TEST_FIX_NETMASK_ETH1,TEST_FIX_BRAODCAST_ETH1,TEST_FIX_GATEWAY_ETH1,TEST_FIX_SRVIP_ETH1}
};
static int fill_ip(char *pdest, char *psrc)
{
    int i = strlen(psrc);
    if (i < FIX_IP_MAX_LEN)
    {
        memcpy(pdest, psrc, i);
        pdest[i] = 0;
        return 0;
    }
    else
    {
        return 1;
    }
}


#if USE_MAINFUNC_ENTRY
#include <cyg/infra/mainfunc.h>
MAINFUNC_ENTRY(setip,argc,argv)
#else
int main( int argc, char **argv )
#endif
{
    DbgMsg_SetipTestIO("+%s: %d\r\n",__func__,argc);
    if (argc < 3)
    {
        DbgMsg_SetipTestErr("Syntax: net common command\r\n");
        DbgMsg_SetipTestErr("command:\r\n");
        DbgMsg_SetipTestErr("    setip -eth0 IP,MaskIP,BraodcastIP,GatewayIP,ServerIP\r\n");
        DbgMsg_SetipTestErr("    setip -eth1 IP,MaskIP,BraodcastIP,GatewayIP,ServerIP\r\n");
        return -1;
    }
    #if SETIP_TEST_DBG //dump input command string
    {
        int i = 0;
        for (i = 0; i < argc; i++)
        {
            DbgMsg_SetipTestIO("%s ", argv[i]);
        }
    }
    DbgMsg_SetipTestIO("\r\n");
    #endif

    if (strncmp("-eth0",argv[1],5)==0)
    {
        DbgMsg_SetipTestIO("[set eth0 ip] => %s %s %s %s %s\r\n",argv[2],argv[3],argv[4],argv[5],argv[6]);
        if (fill_ip(&(gTestNetFixIP[0].ip[0]), argv[2]))
        {
            DbgMsg_SetipTestIO("ip=%s Too Long\r\n",argv[2]);
            return 1;
        }
        if (fill_ip(&(gTestNetFixIP[0].netmask_ip[0]), argv[3]))
        {
            DbgMsg_SetipTestIO("netmask_ip=%s Too Long\r\n",argv[3]);
            return 1;
        }
        if (fill_ip(&(gTestNetFixIP[0].broadcast_ip[0]), argv[4]))
        {
            DbgMsg_SetipTestIO("broadcast_ip=%s Too Long\r\n",argv[4]);
            return 1;
        }
        if (fill_ip(&(gTestNetFixIP[0].gateway_ip[0]), argv[5]))
        {
            DbgMsg_SetipTestIO("gateway_ip=%s Too Long\r\n",argv[5]);
            return 1;
        }
        if (fill_ip(&(gTestNetFixIP[0].server_ip[0]), argv[6]))
        {
            DbgMsg_SetipTestIO("server-ip=%s Too Long\r\n",argv[6]);
            return 1;
        }
        dhcp_set_fix_ip_pair(&gTestNetFixIP[0], (1 << 0));
    }
    else if (strncmp("-eth1",argv[1],5)==0)
    {
        DbgMsg_SetipTestIO("[set eth1 ip] => %s %s\r\n",argv[2],argv[3],argv[4],argv[5],argv[6]);
        if (fill_ip(&(gTestNetFixIP[1].ip[0]), argv[2]))
        {
            DbgMsg_SetipTestIO("ip=%s Too Long\r\n",argv[2]);
            return 1;
        }
        if (fill_ip(&(gTestNetFixIP[1].netmask_ip[0]), argv[3]))
        {
            DbgMsg_SetipTestIO("netmask_ip=%s Too Long\r\n",argv[3]);
            return 1;
        }
        if (fill_ip(&(gTestNetFixIP[1].broadcast_ip[0]), argv[4]))
        {
            DbgMsg_SetipTestIO("broadcast_ip=%s Too Long\r\n",argv[4]);
            return 1;
        }
        if (fill_ip(&(gTestNetFixIP[1].gateway_ip[0]), argv[5]))
        {
            DbgMsg_SetipTestIO("gateway_ip=%s Too Long\r\n",argv[5]);
            return 1;
        }
        if (fill_ip(&(gTestNetFixIP[1].server_ip[0]), argv[6]))
        {
            DbgMsg_SetipTestIO("server-ip=%s Too Long\r\n",argv[6]);
            return 1;
        }
        dhcp_set_fix_ip_pair(&gTestNetFixIP[0], (1 << 1));
    }
    else if (strncmp("-reset0",argv[1],7)==0)
    {
        DbgMsg_SetipTestIO("[reset eth0]\r\n");
        dhcp_reset_fix_ip_pair(1 << 0);
    }
    else if (strncmp("-reset1",argv[1],7)==0)
    {
        DbgMsg_SetipTestIO("[reset eth1]\r\n");
        dhcp_reset_fix_ip_pair(1 << 1);
    }
    else if (strncmp("-help",argv[1],5)==0)
    {
        DbgMsg_SetipTestErr("Syntax: net common command\r\n");
        DbgMsg_SetipTestErr("command:\r\n");
        DbgMsg_SetipTestErr("    setip -eth0 IP,MaskIP,BraodcastIP,GatewayIP,ServerIP\r\n");
        DbgMsg_SetipTestErr("    setip -eth1 IP,MaskIP,BraodcastIP,GatewayIP,ServerIP\r\n");
    }
    else
    {
        DbgMsg_SetipTestErr("Invalid command!\r\n");
        return 1;
    }

    return 0;
}


